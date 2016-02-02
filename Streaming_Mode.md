> # Streaming #

By default, when the client and the service exchange messages, these messages are buffered on the receiving end and delivered only once the entire message has been received. This is true whether it is the client sending a message to the service or the service returning a message to the client. As a result, when the client calls the service, the service is invoked only after the client's message has been received in its entirety; likewise, the client is unblocked only once the returned message with the results of the invocation has been received in its entirety.

For sufficiently small messages, this exchange pattern provides for a simple programming model because the latency caused by receiving the message is usually negligible compared with the message processing itself. However, when it comes to much larger messages—such as ones involving multimedia content, large files, or batches of data—blocking until the entire message has been received may be impractical. To handle such cases, WCF enables the receiving side (be it the client or the service) to start processing the data in the message while the message is still being received by the channel. This type of processing is known as streaming transfer mode. With large payloads, streaming provides improved throughput and responsiveness because neither the receiving nor the sending side is blocked while the message is being sent or received.

> ## I/O Streams ##

For message streaming, WCF requires the use of the .NET Stream class. In fact, the contract operations used for streaming look just like conventional I/O methods. The Stream class is the base class of all the I/O streams in .NET (such as the FileStream, NetworkStream, and MemoryStream classes), allowing you to stream content from any of these I/O sources. All you need to do is return or receive a Stream as an operation parameter, as shown in Example.

Example. Streaming operations
[ServiceContract](ServiceContract.md)
interface IMyContract
{
> [OperationContract](OperationContract.md)
> Stream StreamReply1( );

> [OperationContract](OperationContract.md)
> void StreamReply2(out Stream stream);

> [OperationContract](OperationContract.md)
> void StreamRequest(Stream stream);
[OperationContract(IsOneWay = true)]
> void OneWayStream(Stream stream);
}




Note that you can only define as an operation parameter the abstract class Stream or a specific serializable subclass such as MemoryStream. Subclasses such as FileStream are not serializable; you will have to use the base Stream instead.

WCF lets services stream the reply, the request, or both the request and the reply.

> ## Streaming and Binding ##

Only the TCP, IPC, and basic HTTP bindings support streaming. With all of these bindings streaming is disabled by default, and the binding will buffer the message in its entirety even when a Stream is used. You have to enable streaming by setting the transferMode property according to the desired streaming mode. For example, when using the BasicHttpBinding:

public enum TransferMode
{
> Buffered, //Default
> Streamed,
> StreamedRequest,
> StreamedResponse
}
public class BasicHttpBinding : Binding,...
{
> public TransferMode TransferMode
> {get;set;}
> //More members
}


TRansferMode.Streamed supports all streaming modes, and this is the only transfer mode that can support all the operations in Example. However, if the contract contains only a specific type of streaming, such as streamed reply:

[ServiceContract](ServiceContract.md)
interface IMyContract
{
> //Stream reply
> [OperationContract](OperationContract.md)
> Stream GetStream1( );

> [OperationContract](OperationContract.md)
> int MyMethod( );
}


you can have a buffered request and streamed reply by selecting TRansferMode.StreamedResponse.

You will need to configure the binding on the client or service side (or both) per the required stream mode:



&lt;configuration&gt;


> <system.serviceModel>
> > 

&lt;client&gt;


> > > <endpoint
> > > > binding = "basicHttpBinding"
> > > > bindingConfiguration = "StreamedHTTP"
> > > > ...

> > > />

> > 

&lt;/client&gt;


> > 

&lt;bindings&gt;


> > > 

&lt;basicHttpBinding&gt;


> > > > <binding name = "StreamedHTTP"
> > > > > transferMode = "Streamed"

> > > > />

> > > 

&lt;/basicHttpBinding&gt;



> > 

&lt;/bindings&gt;



> </system.serviceModel>


&lt;/configuration&gt;




> ## Streaming and Transport ##

It is important to realize that WCF streaming is merely a programming model nicety. The underlying transport itself is not streamed, and the default maximum message size is set to 64K. This may be a problem with the sort of data you are likely to use streaming with, because streamed messages tend to be very large (hence the motivation for streaming in the first place). If the default limit proves insufficient, you can increase the maximum message size on the receiving side by setting the MaxReceivedMessageSize property to the expected maximum message size:

public class BasicHttpBinding : Binding,...
{
> public long MaxReceivedMessageSize
> {get;set;}
> //More memebrs
}


You can even use trial-and-error at runtime to find out what the streamed message size is and set the binding accordingly.

Typically, though, you would place that piece of configuration in the config file and avoid doing it programmatically, as message size tends to be deployment-specific:



&lt;bindings&gt;


> 

&lt;basicHttpBinding&gt;


> > <binding name = "StreamedHTTP"
> > > transferMode = "Streamed"
> > > maxReceivedMessageSize = "120000"

> > />

> 

&lt;/basicHttpBinding&gt;




&lt;/bindings&gt;




> ## Stream Management ##

When a client passes a request stream to a service, the service may read from the stream long after the client is gone. The client has no way of knowing when the service is done using the stream, and consequently should not close the stream itself. WCF will automatically close the client-side stream once the service is done using it.

A similar problem exists when a client interacts with a response stream: the stream is produced on the service side, but the service should not close it because it has no way of knowing when the client is done using the stream. In this case, WCF can't help, because it has no idea what the client is doing with the stream. The client is therefore always responsible for closing reply streams.

> When you use streaming, you cannot use message-level transfer security. This is the main reason why only the TCP, IPC, and basic bindings (and their subclasses) support streaming: with these bindings, you typically do not (and, in the case of IPC, cannot) use message security. When streaming with the TCP binding, you also cannot enable reliable messaging.

Using streamed messages has a few additional implications. First, you need to synchronize access to the streamed content; for example, by opening the file stream in a read-only mode to allow other parties to access the file, or opening the stream in an exclusive mode to prevent others from accessing it if so required. In addition, you cannot use streaming when the contract is configured with SessionMode.Required.