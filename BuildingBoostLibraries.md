# Introduction #

We use [Boost](http://www.boost.org/) a lot in our C++ code. Mostly because it is easy to use, production level tested opensource library. Boost is one of 4 cross platform libs we use to achieve applications scale over **Windows**, **Mac OS X** and **Linux** distributions. Boost is mainly header only library but there are some parts of Boost that require compilation.  Here we show how we compile that parts of Boost. To compile our C++ projects from source you will need build Boost libraries.


# Details #

Libraries that are **official parts of Boost** (current release `1.46.1` ) that require compilation are:

  * date\_time
  * filesystem
  * graph
  * graph\_parallel
  * iostreams
  * math
  * mpi
  * program\_options
  * python
  * random
  * regex
  * serialization
  * signals
  * system
  * test
  * thread
  * wave

<a href='Hidden comment: 
There some *unofficial parts of Boost* we use which also require compilation:
'></a>

# Official  Boost libraries compilation #

## With out administration privileges ##

### Manually ###

  1. [Download](http://www.boost.org/users/download/) latest Boost release (currently [1.46.1](http://sourceforge.net/projects/boost/files/boost/1.46.1/)).
  1. Unpack, extract downloaded content into some folder.
  1. Open that folder in terminal (for example you ca open 'cmd' or 'terminal' an use command like `cd absolute/path/to/directory/with/extracted/Boost` than use command to show folder contents `ls` on Unix like OS (Mac, Linux) and `dir` on Windows - in output you should be capable to find `boost-build.jam` )
  1. Create two folders (using command like `mkdir` _foldername_ ) `build-dir` and `install-dir`
  1. run command `./booststrap` (`booststrap.sh` or `booststrap.bat` on windows)
  1. now we can compile Boost and install it with `./bjam -j4 link=static  threading=single,multi --builddir=./build-dir install  --without-mpi --prefix=./install-dir`
    * Such call will compile static libraries of Boost into `build-dir`. (you probably will not be capable to use Boost out from there - there will be no includes headers and a really big directories hierarchy )
    * Than **bjam** will create for us "installation" of Boost into `install-dir` - there will appear folder with all boost libs (static version) and includes folder with all of boost headers.
    * We set -j4 flag to compile on 4 cores. use -j`N` where `N` is number of desired cores to be used during compilation process.
    * We set `--without-mpi` flag because we currently do not use it. (on installing boost with mpi please refer to [this official Boost.MPI page](http://www.boost.org/doc/libs/1_46_1/doc/html/mpi/getting_started.html#mpi.config))

### Automated ways ###
For Unix distributions (Mac OS X or Linux) you could use our [Boost network build/setup script](http://cloudobserver.googlecode.com/svn/trunk/CloudLoader/boost_net_setup.sh).

Here is how you can use it:
```
	wget http://cloudobserver.googlecode.com/svn/trunk/CloudLoader/boost_net_setup.sh
	chmod u+x boost_net_setup.sh
	./boost_net_setup.sh
```

<a href='Hidden comment: 
./bjam -j4 link=static  threading=multi toolset=msvc-9.0 --builddir=./build-dir --without-mpi  install --prefix=./install-dir

bjam -j4 link=static  threading=single,multi --build-dir=./build-dir --without-mpi  install --prefix=./install-dir
'></a>