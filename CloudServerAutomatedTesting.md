# Introduction #

Cloud Server the core component in our infrastructure. Once we got bored with recompiling it by hand and created [Cloud Server network build/setup scripts](http://code.google.com/p/cloudobserver/wiki/BuildingCloudServer). But we had to call them on each of our testing servers by hand. Our servers tend to work with out reset 24/7/365 so we created simple script that can be launched by user, would look for updates in our SVN and rebuild server to keep up with current source code base.

# Cloud Server Build Automation #

## With out administration privileges ##

### Linux way ###
For Linux distributions you could use our [Automated Cloud Server Testing build/setup script](http://cloudobserver.googlecode.com/svn/trunk/CloudLoader/cloud-updater-linux.sh).

Here is how you can use it:
```
	wget http://cloudobserver.googlecode.com/svn/trunk/CloudLoader/cloud-updater-linux.sh 
	chmod u+x cloud-updater-linux.sh 
	nohup ./cloud-updater-linux.sh  >& /dev/null &
```

when you'll get tired of it just get scripts process ID:
```
	echo `ps -fe | grep -F 'cloud-updater-linux' | grep -v -F 'grep' | awk '{ print $2 }'`
```
and `kill` it, then find CloudServer's process ID:
```
	 echo `ps aux | grep -F 'CloudServer' | grep -v -F 'grep' | awk '{ print $2 }'`
```
and `kill` CloudServer.


### Warning ###
Scripts have problems with paths containing either non-ASCII characters or spaces. (you can use `pwd` command to find your current path) so we recommend first of all to try and run [Cloud Server network build/setup script](http://code.google.com/p/cloudobserver/wiki/BuildingCloudServer) in the directory you want automated builds test script to work.

# Warranty Disclaimer #
CO CF PROVIDES THE CF SERVER AND ITS COMPONENTS TO YOU "AS IS." CO MEMBERS AND THERE SUPPLIERS DISCLAIM ANY EXPRESS, IMPLIED, STATUTORY OR OTHER WARRANTY OF ANY KIND WITH RESPECT TO THE CF SERVER AND ITS COMPONENTS INCLUDING ANY WARRANTY WITH REGARD TO PERFORMANCE, MERCHANTABILITY, INTEGRATION, SATISFACTORY QUALITY, NONINFRINGEMENT OR FITNESS FOR ANY PARTICULAR PURPOSE. YOU BEAR THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF OR RESULTS OBTAINED BY USING THE CF SERVER AND ITS COMPONENTS.

The foregoing exclusions and limitations will apply to the maximum extent permitted by applicable law, even if any remedy fails its essential purpose.