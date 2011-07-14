#ifndef SERVICE_HPP
#define SERVICE_HPP

// Boost
#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>

// Cloud Forever
#include <http.h>

class service
{
public:
	virtual void service_call(http_request, boost::shared_ptr<boost::asio::ip::tcp::socket>) = 0;
};

#endif // SERVICE_HPP
