# Boost IO streams GZIP Tutorials #
First you will need to compile boost with IO streams with encoding into Gzip support. How to build Boost with IO streams encoding support is described [here](http://www.boost.org/doc/libs/1_48_0/libs/iostreams/doc/installation.html)

## How to Gzip encode a string or a file with Boost IO streams ##
Based on [official tutorial](http://www.boost.org/doc/libs/1_41_0/libs/iostreams/doc/classes/gzip.html), shows how to encode into gzip.
### Code ###
```

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>

#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/stream.hpp>


int main(  )
{
	std::cout << "create data." << std::endl;
	std::string data_part_1 = "rock in clouds";
	std::string data_part_2 = "!";

	std::cout << "encode data:" << std::endl;
	{
		std::ofstream file( std::string( "hello world.gz" ).c_str(),  std::ofstream::binary);
		boost::iostreams::filtering_streambuf< boost::iostreams::input> in;
		in.push( boost::iostreams::gzip_compressor());
		std::stringstream data;
		data << data_part_1 << data_part_2;
		in.push(data);
		boost::iostreams::copy(in, file);
	}
		std::cout << "data encoded." << std::endl;
	std::cin.get();
	return 0;
}

```

### Output ###

```

create data.
encode data:
data encoded.

```
And here is [generated file](link.md)

## How to Gzip encode entire folder with Boost IO streams ##
You will need a [cross platform header only hpp class `lindenb::io::Tar` which you can download from here](http://code.google.com/p/cloudobserver/source/browse/trunk/CloudServer/3rdparty/header-only/lindenb/io/tarball.hpp?spec=svn1913&r=1913) to put multiple files into tar archive and then gzip it.

### Code ###
```
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <string>
#include <algorithm>

#include <boost/filesystem.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/stream.hpp>

#include <lindenb/io/tarball.hpp>

void is_file( boost::filesystem::path p, lindenb::io::Tar & archive )
{
	std::string file_name_in_archive = p.relative_path().normalize().string();
	std::replace(file_name_in_archive.begin(), file_name_in_archive.end(), '\\', '/');
	file_name_in_archive = file_name_in_archive.substr(2, file_name_in_archive.size());
	archive.putFile(p.string().c_str(), file_name_in_archive.c_str());
}

void is_dir( boost::filesystem::path dir, lindenb::io::Tar & archive )
{
	if(!boost::filesystem::exists(dir))
	{
		return;
	}
	//create_file(dir, old_fs, new_fs);
	if (boost::filesystem::is_directory(dir))
	{
		boost::filesystem::directory_iterator dirIter( dir );
		boost::filesystem::directory_iterator dirIterEnd;

		while ( dirIter != dirIterEnd )
		{
			if ( boost::filesystem::exists( *dirIter ) && !boost::filesystem::is_directory( *dirIter ) )
			{
				try
				{
					is_file((*dirIter), archive);
				}
				catch(std::exception){}
			}
			else
			{
				try
				{
					is_dir((*dirIter), archive);
				}
				catch(std::exception){}
			}
			++dirIter;
		}
	}
}

int main(int argc,char** argv)
{
	std::stringstream out("pseudofile");
	lindenb::io::Tar tarball(out);
	is_dir("./myfiles", tarball);
	tarball.finish();
	{
		std::ofstream file( std::string( "hello folder.tar.gz" ).c_str(),  std::ofstream::binary);
		boost::iostreams::filtering_streambuf< boost::iostreams::input> in;
		in.push( boost::iostreams::gzip_compressor());
		in.push(out);
		boost::iostreams::copy(in, file);
	}
	std::cin.get();
	return 0;
}

```