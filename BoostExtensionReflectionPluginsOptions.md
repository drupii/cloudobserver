# Introduction #

There are two ways for plug in creation:

  * **Extension** - application can find out what classes in plugin inherit some of the classes app had during its compilation.
<a href='Hidden comment: 
* *Reflection* - it is when app is provided with name of class to load, its function name to call and its function types.
'></a>

# Details #

## Connecting a plugin ##
What shall be provided to an app to correctly load plugin created with Extension or Reflection?
> Let us imagine that our app has some function that whose functionality we want extend. We have agreements on what types it should receive. In such situation we have 2 sets of data we shall provide to our app in order to get functionality from plugins we create:

  1. **Extension**:
    * Array of shared libraries names, ones that can provide class inheritance tree from base - class (like an interface) application had during compilation to the latest class that we want to load
    * class to load name
> <a href='Hidden comment: 
# *Reflection*:
*  Shared library name
*  Class name
*  Function name
'></a>

## Creating a plugin ##
On basics of creation such plugins please [review Boost Extension](http://boost-extension.redshoelace.com/docs/boost/extension/boost_extension/tutorials.html) and [Boost Reflection C++ tutorials](http://boost-extension.redshoelace.com/docs/boost/extension/boost_extension/extension_reflection.html).
> Here we assume you have read them. We will just draw your attention to main differences between creation of plugin with Boost.Extension vs creating a plugin with Boost.Reflection

  1. **Extension**:
    * each class you want to be shared across library shall have:
      1. various Boost.Extension includes in header and implementation file
      1. BOOST\_EXTENSION\_EXPORT\_DECL in its declaration
    * after class declaration in implementation file you shall use standart BOOST\_EXTENSION\_TYPE\_MAP\_FUNCTION
<a href='Hidden comment: 
# *Reflection*:
* includes to <boost/extension/extension.hpp> and <boost/reflection/reflection.hpp> in implementation file are required
* no special directives in class declarations
* you have freedom in naming a function you want to use for creating library description reflection map.
'></a>
