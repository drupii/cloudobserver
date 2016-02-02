# Introduction #

We [ported](http://code.google.com/p/cloudobserver/source/browse/#svn%2Fbranches%2Fv0.4%2FBoost.Extension.Tutorial) some of [official Boost-Extension tutorials](http://svn.boost.org/svn/boost/sandbox/libs/extension/examples/) so that C++ projects/solutions for next environments could be created:

  * Microsoft Visual Studio 2005-2010, including the Express editions
  * GNU Make, including Cygwin and MinGW
  * Apple Xcode
  * Code::Blocks
  * CodeLite

To achieve such expansion we had to modify some of the source files a bit and use [premake build configuration generator](http://industriousone.com/what-premake) for projects/solutions creation.

# Details #
Now all you need to do to see some of Boost-Extension-Tutorials as projects/solutions compilable from your favorite IDE is call something like `premake4 --os=YOUR-OS --BoostIncludesPath="ABSOLUTE-PATH/TO/EXTENSION-SVN-SOURCE/"  --platform=x32 IDE-NAME `

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

# For More Detailes #
Please read `ReadMe.txt` file inside folder with project.

## On windows ##
If you do not have Boost (current 1.46.1) libraries installed into Program Files/Boost-1.46.1/ please read `ReadMe.txt` file inside folder with project.