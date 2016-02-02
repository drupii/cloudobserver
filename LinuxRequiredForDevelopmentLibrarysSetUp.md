# Introduction #

Here we describe what is needed to be capable of compiling writer client application on Linux. Details on how to compile an application is not topic of this page.


# Details #

First install throe your packet manager (or from developers site):
  * [Boost](http://www.boost.org/) C++ library's
  * [OpenCV](http://opencv.willowgarage.com/wiki/)
  * [OpenAL](http://connect.creativelabs.com/openal/default.aspx)

Secondly get latest svn of [FFmpeg](http://www.ffmpeg.org/) and open folder with its truck contents in terminal. type:
  * ./configure --enable-pthreads --enable-shared
  * make
  * sudo make install
This will take some time.

Follow this instruction and you will install all required library's into your system.