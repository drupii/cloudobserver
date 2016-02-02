> # Introduction #
In another page on wiki was described a problem with limited size of transferring data. In the article was written two possible decisions:
  1. refuse using wcf and using iss + winservces
  1. refuse using iss and use only wcf
Some another decision is below.

> # Solving #

Really, if we don't use iis the trouble is solved easily. We have to describe service endpoint and binding config in such way:
serverSide:


&lt;endpoint address ="" binding="wsHttpBinding" bindingConfiguration="WsStreaming"  contract="CloudObserverBroadcastServiceLibrary.ICloudObserverBroadcastService"&gt;



> 

&lt;bindings&gt;


> > 

&lt;wsHttpBinding&gt;


> > > 

&lt;binding name="WsStreaming" maxReceivedMessageSize="65536000" closeTimeout="00:05:00" bypassProxyOnLocal="true" openTimeout="00:05:00" receiveTimeout="00:30:00" sendTimeout="00:30:00" messageEncoding="Mtom"&gt;


> > > > 

&lt;readerQuotas maxArrayLength="1000000000" /&gt;



> > > 

&lt;/binding&gt;



> > 

&lt;/wsHttpBinding&gt;



> 

&lt;/bindings&gt;



In some place must be added TransferMode="streaming"(defaultly it is buffered: we wait until all the message is downloaded and only then work it up. if it is streamed(seems, impossible with IIS), the message is represented as stream and we start work when received first parts of message) but I yet haven't known where.
It is important that on the client size the config could be not generated if it is wsHttpBinding. Or on the client side you will have to add some strings to app.config by youself. Seems, VS2008 have some problems with correct description upload.
In Client Side:


&lt;binding name="WSHttpBinding\_ICloudObserverBroadcastService" closeTimeout="00:01:00" openTimeout="00:01:00" receiveTimeout="00:10:00" sendTimeout="00:01:00" bypassProxyOnLocal="false" transactionFlow="false" hostNameComparisonMode="StrongWildcard" maxBufferPoolSize="524288" maxReceivedMessageSize="65536000" messageEncoding="Mtom" textEncoding="utf-8" useDefaultWebProxy="true" allowCookies="false"&gt;




&lt;readerQuotas maxDepth="32" maxStringContentLength="8192" maxArrayLength="1000000000" maxBytesPerRead="4096" maxNameTableCharCount="16384" /&gt;



> 

&lt;endpoint address="http://localhost:8731/Design\_Time\_Addresses/CloudObserverBroadcastServiceLibrary/Service1/" binding="wsHttpBinding" bindingConfiguration="WSHttpBinding\_ICloudObserverBroadcastService" contract="CloudObserverBroadcastService.ICloudObserverBroadcastService" name="WSHttpBinding\_ICloudObserverBroadcastService"&gt;


