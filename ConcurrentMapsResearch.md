# Introduction #

We need to select most fast concurrent map container that would work with our thread pool. Also we want to see how to tween and tweak our thread pool for best performance.


# Details #

Result of Concurrent-Maps project run is a “log.txt” file with stats that can be easily brought into excel.

# Research results #

## 4 cores, Windows, Core 2 Quad ##
[![](http://cloudobserver.googlecode.com/svn/wiki/Assets/tp_performance_4_2.jpg)](http://code.google.com/p/cloudobserver/wiki/ConcurrentMapsResearch)

## 8 cores, Windows, Core i7 ##
[![](http://cloudobserver.googlecode.com/svn/wiki/Assets/tp_performance_8_2.jpg)](http://code.google.com/p/cloudobserver/wiki/ConcurrentMapsResearch)

As you can see optimal thread pool performance is not gained on some constant/relative number of cores, while we can see that unordered boost maps beat STL maps.