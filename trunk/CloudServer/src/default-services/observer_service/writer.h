#ifndef WRITER_H
#define WRITER_H

#include <exception>
#include <fstream>
#include <iostream>
#include <set>

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "flv_tag.h"
#include "reader.h"

#define HEADER_LENGTH 13
#define SIGNATURE1 0x46
#define SIGNATURE2 0x4C
#define SIGNATURE3 0x56
#define VERSION 1
#define TAG_HEADER_LENGTH 11
#define TAGTYPE_AUDIO 8
#define TAGTYPE_VIDEO 9
#define TAGTYPE_DATA 18

class writer
{
public:
	writer(boost::shared_ptr<boost::asio::ip::tcp::socket> socket, std::ofstream* dump);
	~writer();
	void connect_reader(boost::shared_ptr<boost::asio::ip::tcp::socket> socket, std::ofstream* dump);
	void process();
	int get_width() { return this->width; }
	int get_height() { return this->height; }

	class flv_format_violation_exception: public std::exception { };
private:
	double get_double_variable_from_flv_script_tag(char* script_tag_data, int data_size, std::string variable_name);
	unsigned short to_ui16(unsigned char* value, int start_index);
	unsigned int to_ui24(unsigned char* value, int start_index);
	unsigned int to_ui32(unsigned char* value, int start_index);

	boost::shared_ptr<boost::asio::ip::tcp::socket> socket;
	std::ofstream* dump;

	int width;
	int height;

	std::set<reader*> readers;

	char* header;
	std::vector<flv_tag> script_data, tags_buffer;
	unsigned int buffered_timestamp;
	bool key_frames;

	boost::mutex mutex;
};

#endif // WRITER_H