# Introduction #

Cloud Server is one of the core components in our infrastructure. Here we show how you can build it.

# Cloud Server compilation #

## With out administration privileges ##

### Automated ways ###
For Unix distributions (Mac OS X or Linux) you could use our [Cloud Server network build/setup script](http://cloudobserver.googlecode.com/svn/trunk/CloudLoader/cloud-loader-linux.sh).

Here is how you can use it:
```
	wget http://cloudobserver.googlecode.com/svn/trunk/CloudLoader/cloud-loader-linux.sh
	chmod u+x cloud-loader-linux.sh
	./cloud-loader-linux.sh
```

### Warning ###
Scripts have problems with paths containing either non-ASCII characters or spaces. (you can use `pwd` command to find your current path)

# Warranty Disclaimer #
CO CF PROVIDES THE CF SERVER AND ITS COMPONENTS TO YOU "AS IS." CO MEMBERS AND THERE SUPPLIERS DISCLAIM ANY EXPRESS, IMPLIED, STATUTORY OR OTHER WARRANTY OF ANY KIND WITH RESPECT TO THE CF SERVER AND ITS COMPONENTS INCLUDING ANY WARRANTY WITH REGARD TO PERFORMANCE, MERCHANTABILITY, INTEGRATION, SATISFACTORY QUALITY, NONINFRINGEMENT OR FITNESS FOR ANY PARTICULAR PURPOSE. YOU BEAR THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF OR RESULTS OBTAINED BY USING THE CF SERVER AND ITS COMPONENTS.

The foregoing exclusions and limitations will apply to the maximum extent permitted by applicable law, even if any remedy fails its essential purpose.