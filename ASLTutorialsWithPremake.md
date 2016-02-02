# Introduction #
We, at CloudForewer, team plan to use ASL for creation of some minimalistic graphical user interfaces which are sometimes required in our projects. We work across Mac OS X, Windows and lots of Linux flavors. We have taken a small look at Qt GUI and wxWidgets and found out they are way too much for our current needs. We desired something more minimalistic and more boost oriented. We found such lib - Adobe Source Libraries (ASL). ASL is not only UI library but we plan to concentrate on its GUI features.

## What are main ASL concepts ##
  * We found [grate video](http://www.youtube.com/v/4moyKUHApq4&hl=en&fs=1) where core ASL developer gives a speech on concepts of this library;
  * Also concepts are described on [ASL site](http://stlab.adobe.com/).

## How to compile ASL ##
We faced some problems in Adobe Source Libraries compilation so we asked for detailed instructions, which were provided to us [here at stackoverflow](http://stackoverflow.com/questions/6397501/how-to-compile-asl-boost-based-adobe-c-gui-library-on-windows-7). Instructions are for Windows OS but concept of working with [bjam](http://www.boost.org/boost-build2/) are same on every platform. It compiles into static `libasl` and static `libadobe_widgets`.

## What is Adobe Source Libraries tutorials and demos base? ##
  * You can find interesting precompiled adobe demos for Windows in  asl\_begin\_win archives [here](http://sourceforge.net/projects/adobe-source/files/adobe-source/);
  * There is `test` folder is archives with adobe source libraries it contains some samples.

## What materials we plan to produce for ASL community ##

We plan to port and join some of [official ASL tutorials](http://sourceforge.net/projects/adobe-source/files/adobe-source/) so that C++ projects/solutions for next environments could be created:

  * Microsoft Visual Studio 2005-2010, including the Express editions;
  * GNU Make, including Cygwin and MinGW;
  * Apple Xcode;
  * Code::Blocks;
  * CodeLite;

So no BJAM would be required to get projects/solutions for your favorite environments. To achieve such expansion we plan to modify some of the source files a bit and use [Premake build configuration generator](http://industriousone.com/what-premake) for projects/solutions creation.

# Details #
## Project compilation ##
Now all you'll need to do to see some of Adobe-Source-Libraries-Tutorials as projects/solutions compilable from your favorite IDE is call something like `premake4 --os=YOUR-OS  --platform=x32 IDE-NAME ` (of course you would have built ASL and Boost libraries inside your Program files folder)

Where YOUR-OS could be:

  * linux
  * macosx
  * windows

Where IDE-NAME could be:

  * codeblocks
  * codelite
  * gmake
  * vs2002
  * vs2003
  * vs2005
  * vs2008
  * vs2010
  * xcode3

## What is Adam ##
Adam is Lua/JSON like language meant for creation of interaction/data-binding rules for ASL

## What is Eve ##
Eve is layout description language. It has Lua/JSON alike structures

## What tutorials we plan to create ##

### Alert ###
Simple cross platform “Alert” application based on ASL alert source. It will show Adam and Eve based window with ok button
### Adam and Eve ###
Cross platform application whose behavior could be modified via both GUI (Described in EVE file) and console input (all controlled by Adam file)

## Tips ##
It is not the best idea to hardcore Adam and Eve values into your classes because they can be changed dynamically. But in case of need there Is an [small web 2.0 application](http://jsfiddle.net/t9sgA/10/show/)  for turning normally formatted text into  `static char*` formatted text.

# For More Details #
Please read `ReadMe.txt` file inside folder with project.

## On Windows ##
If you do not have Boost (current 1.46.1) and ASL libraries installed into Program Files/Boost-1.46.1/ please read `ReadMe.txt` file.