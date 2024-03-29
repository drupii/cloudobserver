#include "http_request.h"

http_request::http_request()
{
	this->reset();
}

http_request::http_request(boost::asio::ip::tcp::socket& socket)
{
	this->receive(socket);
}

http_request::~http_request()
{
}

void http_request::clear()
{
	this->method = "";
	this->url = "";
	this->version = "";
	this->arguments.clear();
	this->headers.clear();
	this->body_size = 0;
	this->body = "";
}

void http_request::reset()
{
	this->method = "GET";
	this->url = "/";
	this->version = "HTTP/1.1";
	this->arguments.clear();
	this->headers.clear();
	this->body_size = 0;
	this->body = "";
}

void http_request::receive(boost::asio::ip::tcp::socket& socket)
{
	this->clear();

	http_request_parser_state parser_state = METHOD;

	int buffer_size = 1024;
	char* buffer = new char[buffer_size];
	std::string key = "";
	std::string value = "";

	try
	{
		do
		{
			int bytes_read = socket.read_some(boost::asio::buffer(buffer, buffer_size));
			parse_buffer(buffer, parser_state, key, value, bytes_read);

		} while (parser_state != OK);
	}
	catch (...)
	{
		delete buffer;
		throw;
	}
	delete buffer;
}

bool http_request::timed_receive(boost::asio::ip::tcp::socket &socket, int& seconds_to_wait)
{
	size_t buffer_size = 1024;
	return this->timed_receive_base( socket, buffer_size, seconds_to_wait);
}

int http_request::read_some( boost::asio::ip::tcp::socket *sock, char* buffer, size_t& buffer_size )
{
	return sock->read_some(boost::asio::buffer(buffer, buffer_size));
}

bool http_request::timed_receive_base(boost::asio::ip::tcp::socket& socket, size_t& buffer_size, int& seconds_to_wait)
{
	this->clear();

	http_request_parser_state parser_state = METHOD;

	char* buffer = new char[buffer_size];
	std::string key = "";
	std::string value = "";

	try
	{
		do
		{

			boost::shared_ptr<boost::packaged_task<int> >  pt( new boost::packaged_task<int>(boost::bind(&http_request::read_some , this,  &socket, buffer, buffer_size)));

			boost::unique_future<int> fi=pt->get_future();

			boost::thread task(boost::bind(&http_request::run_item<int>, this, pt)); // launch task on a thread
			size_t as = 1;
			fi.timed_wait_until(boost::get_system_time()+boost::posix_time::seconds(seconds_to_wait));

			if (fi.has_value())
			{
				int bytes_read = fi.get();
				parse_buffer(buffer, parser_state, key, value, bytes_read);
			}
			else
			{
				task.interrupt();
				return false;
			}

		} while (parser_state != OK);
	}
	catch (...)
	{
		delete buffer;
		throw;
	}
	delete buffer;
	return true;
}

void http_request::parse_buffer(char* buffer, http_request_parser_state &parser_state, std::string &key, std::string &value, int bytes_read)
{
	char* position = buffer;
	do
	{
		switch (parser_state)
		{
		case METHOD:
			if (this->method == "<policy-file-request/>")
				throw policy_file_request_exception();
			if (*position != ' ')
				this->method += *position++;
			else
			{
				position++;
				parser_state = URL;
			}
			break;
		case URL:
			if (*position == '?')
			{
				position++;
				key = "";
				parser_state = URL_PARAM;
			}
			else if (*position != ' ')
				this->url += *position++;
			else
			{
				position++;
				parser_state = VERSION;
			}
			break;
		case URL_PARAM:
			if (*position == '=')
			{
				position++;
				value = "";
				parser_state = URL_VALUE;
			}
			else if (*position == ' ')
			{
				position++;
				parser_state = VERSION;
			}
			else
				key += *position++;
			break;
		case URL_VALUE:
			if (*position == '&')
			{
				position++;
				this->arguments[key] += value;
				key = "";
				parser_state = URL_PARAM;
			}
			else if (*position == ' ')
			{
				position++;
				this->arguments[key] += value;
				parser_state = VERSION;
			}
			else
				value += *position++;
			break;
		case VERSION:
			if (*position == '\r')
				position++;
			else if (*position != '\n')
				this->version += *position++;
			else
			{
				position++;
				key = "";
				parser_state = HEADER_KEY;
			}
			break;
		case HEADER_KEY:
			if (*position == '\r')
				position++;
			else if (*position == '\n')
			{
				position++;
				std::map<std::string, std::string>::iterator iterator = this->headers.find("Content-Length");
				if (iterator != this->headers.end())
					this->body_size = boost::lexical_cast<int>(iterator->second);
				else
					this->body_size = 0;
				parser_state = (this->body_size == 0) ? OK : BODY;
			}
			else if (*position == ':')
				position++;
			else if (*position != ' ')
				key += *position++;
			else
			{
				position++;
				value = "";
				parser_state = HEADER_VALUE;
			}
			break;
		case HEADER_VALUE:
			if (*position == '\r')
				position++;
			else if (*position != '\n')
				value += *position++;
			else
			{
				position++;
				this->headers.insert(std::pair<std::string, std::string>(key, value));
				key = "";
				parser_state = HEADER_KEY;
			}
			break;
		case BODY:
			this->body += *position++;
			if (this->body.length() == this->body_size)
				parser_state = OK;
			break;
		case OK:
			position = buffer + bytes_read;
			break;
		}
	} while (position < buffer + bytes_read);

	typedef std::map<std::string, std::string> map_ss;
	map_ss::iterator has_cookie = this->headers.find("Cookie");

	if (has_cookie != this->headers.end()){
		cookies = parse_cookie(has_cookie->second);
		this->headers.erase(has_cookie);
	}
}


void http_request::send(boost::asio::ip::tcp::socket& socket)
{
	headers["Content-Length"] = boost::lexical_cast<std::string>(body.length());

	std::string request = this->method + ' ' + this->url;
	if (arguments.begin() != arguments.end())
	{
		request += '?';
		bool first = true;
		for (std::map<std::string, std::string>::iterator argument = this->arguments.begin(); argument != this->arguments.end(); ++argument)
		{
			std::vector<std::string> values;
			boost::split(values, argument->second, boost::is_any_of(", "));
			for (std::vector<std::string>::iterator value = values.begin(); value != values.end(); ++value)
			{
				if (!first)
					request += '&';
				else
					first = false;
				request += argument->first + '=' + *value;
			}
		}
	}
	request += ' ' + this->version + "\r\n";
	for (std::map<std::string, std::string>::iterator header = this->headers.begin(); header != this->headers.end(); ++header)
		request += header->first + ": " + header->second + "\r\n";
	request += "\r\n" + this->body;
	socket.send(boost::asio::buffer(request.c_str(), request.length()));
}

boost::asio::ip::tcp::socket& http_request::send(std::string absolute_url, boost::asio::ip::tcp::socket& socket)
{
	// Parse the URL.
	std::vector<std::string> url_parts;
	boost::regex url_expression(
		// protocol            host               port
		"^(\?:([^:/\?#]+)://)\?(\\w+[^/\?#:]*)(\?::(\\d+))\?"
		// path                file       parameters
		"(/\?(\?:[^\?#/]*/)*)\?([^\?#]*)\?(\\\?(.*))\?"
		);
	boost::regex_split(std::back_inserter(url_parts), absolute_url, url_expression);
	std::string host = url_parts[1];
	std::string port = url_parts[2];
	this->url = url_parts[3] + url_parts[4];

	// Add the 'Host' header to the request. Not doing this is treated as bad request by many servers.
	this->headers["Host"] = host;

	// Use the default port if no port is specified.
	if (port.empty())
		port = "80";

	// Use the empty path if no path is specified.
	if (this->url.empty())
		this->url = "/";

	// Resolve the hostname.
	boost::asio::io_service io_service;
	boost::asio::ip::tcp::resolver resolver(io_service);
	boost::asio::ip::tcp::resolver::query query(boost::asio::ip::tcp::v4(), host, port);
	boost::asio::ip::tcp::resolver::iterator iterator;
	try
	{
		iterator = resolver.resolve(query);
	}
	catch (boost::system::system_error&)
	{
		throw connection_exception();
	}

	// Try to connect to the server using one of the endpoints.
	bool connected = false;
	for (iterator; iterator != boost::asio::ip::tcp::resolver::iterator(); ++iterator)
	{
		boost::asio::ip::tcp::endpoint endpoint = iterator->endpoint();
		try
		{
			socket.connect(endpoint);
			connected = true;
			break;
		}
		catch (boost::system::system_error&)
		{
		}
	}

	// Check if the connection is successful.
	if (!connected)
		throw connection_exception();

	// Send the request.
	this->send(socket);

	// Return the socket.
	return socket;
}

boost::shared_ptr<std::string> http_request::serialize()
{
	return boost::shared_ptr<std::string>(new std::string(serialize_base()),boost::bind(&pointer_utils::delete_ptr<std::string>, _1) );
}

void http_request::deserialize( boost::shared_ptr<std::string> request_string)
{
	deserialize_base(*request_string); 
}

std::string http_request::serialize_base()
{
	std::stringstream oa_ss_req;
	boost::archive::text_oarchive  oa_request(oa_ss_req);
	oa_request << *this;
	return oa_ss_req.str();
}

void http_request::deserialize_base( std::string request_string)
{
	std::stringstream ia_ss_req;
	ia_ss_req << request_string;
	boost::archive::text_iarchive ia_req(ia_ss_req);
	ia_req >> *this;	 
}

std::map<std::string, std::string> http_request::parse_cookie( std::string cookie_data )
{
	std::map<std::string, std::string> parsed_cookie;
	std::string token, token2;
	std::istringstream iss(cookie_data);
	while ( getline(iss, token, ' ') )
	{
		std::string name, val;
		std::istringstream iss2(token);
		int num = 0 ;
		while ( getline(iss2, token2, '=') )
		{
			if ( num == 0)
			{
				name = token2;
				num++;
			}
			else
			{
				val = token2;
				std::string::iterator it = val.end() - 1;
				if (*it == ';')
					val.erase(it);

			}
		}
		parsed_cookie.insert(std::pair<std::string, std::string>(name, val));
	}
	return parsed_cookie;
}