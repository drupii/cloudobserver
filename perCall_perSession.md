> # Per-call services #

4.2. Per-Call Services
When the service type is configured for per-call activation, a service instance (the CLR object) exists only while a client call is in progress. Every client request (that is, a method call on the WCF contract) gets a new dedicated service instance. The following list details how per-call activation works, and the steps are illustrated in Figure 4-1:

The client calls the proxy and the proxy forwards the call to the service.

WCF creates a service instance and calls the method on it.

When the method call returns, if the object implements IDisposable, WCF calls IDisposable.Dispose( ) on it.

The client calls the proxy and the proxy forwards the call to the service.

WCF creates an object and calls the method on it.


Disposing of the service instance is an interesting point. As I just mentioned, if the service supports the IDisposable interface, WCF will automatically call the Dispose( ) method, allowing the service to perform any required cleanup. Note that Dispose( ) is called on the same thread that dispatched the original method call, and that Dispose( ) has an operation context (presented later). Once Dispose( ) is called, WCF disconnects the instance from the rest of the WCF infrastructure, making it a candidate for garbage collection.

4.2.1. Benefits of Per-Call Services
In the classic client/server programming model, using languages such as C++ or C#, every client gets its own dedicated server object. The fundamental problem with this approach is that it doesn't scale well. Imagine an application that has to serve many clients. Typically, these clients create the objects they need when the client application starts and dispose of them when the client application shuts down. What impedes scalability with the client/server model is that the client applications can hold onto objects for long periods of time, while actually using them for only a fraction of that time. Those objects may hold expensive or scarce resources, such as database connections, communication ports, or files. If you allocate an object for each client, you will tie up such crucial and/or limited resources for long periods, and you will eventually run out of resources.

A better activation model is to allocate an object for a client only while a call is in progress from the client to the service. That way, you have to create and maintain in memory only as many objects as there are concurrent calls, not as many objects as there are outstanding clients. My personal experience indicates that in a typical Enterprise system, especially one that involves users, only 1 percent of all clients make concurrent calls (in a high-load Enterprise system, that figure rises to 3 percent). Thus, if your system can concurrently sustain 100 expensive service instances, it can still typically serve as many as 10,000 outstanding clients. This is precisely the benefit the per-call instance activation mode offers. In between calls, the client holds a reference on a proxy that doesn't have an actual object at the end of the wire. This means that you can dispose of the expensive resources the service instance occupies long before the client closes the proxy. By that same token, acquiring the resources is postponed until they are actually needed by a client.

Keep in mind that creating and destroying a service instance repeatedly on the service side without tearing down the connection to the client (with its client-side proxy) is a lot cheaper than repeatedly creating an instance and a connection. The second benefit is that forcing the service instance to reallocate or connect to its resources on every call caters very well to transactional resources and transactional programming (discussed in Chapter 7), because it eases the task of enforcing consistency with the instance state. The third benefit of per-call services is that they can be used in conjunction with queued disconnected calls (described in Chapter 9), because they allow easy mapping of service instances to discrete queued messages.

4.2.2. Configuring Per-Call Services
To configure a service type as a per-call service, you apply the ServiceBehavior attribute with the InstanceContextMode property set to InstanceContextMode.PerCall:

[ServiceContract](ServiceContract.md)
interface IMyContract
{...}

[ServiceBehavior(InstanceContextMode = InstanceContextMode.PerCall)]
class MyService : IMyContract
{...}


Example 4-2 lists a simple per-call service and its client. As you can see from the program output, for each client method call a new service instance is constructed.

Example 4-2. Per-call service and client
///////////////////////// Service Code /////////////////////
[ServiceContract](ServiceContract.md)
interface IMyContract
{
> [OperationContract](OperationContract.md)
> void MyMethod(  );
}
[ServiceBehavior(InstanceContextMode = InstanceContextMode.PerCall
)]
class MyService : IMyContract,IDisposable
{
> int m\_Counter = 0;

> MyService(  )
> {
> > Trace.WriteLine("MyService.MyService(  )");

> }
> public void MyMethod(  )
> {
> > m\_Counter++;
> > Trace.WriteLine("Counter = " + m\_Counter);

> }
> public void Dispose(  )
> {
> > Trace.WriteLine("MyService.Dispose(  )");

> }
}
///////////////////////// Client Code /////////////////////
MyContractClient proxy = new MyContractClient(  );

proxy.MyMethod(  );
proxy.MyMethod(  );

proxy.Close(  );

//Possible output
MyService.MyService(  )
Counter = 1
MyService.Dispose(  )
MyService.MyService(  )
Counter = 1
MyService.Dispose(  )




4.2.3. Per-Call Services and Transport Sessions
The use of a per-call service is independent from the presence of a transport session (described in Chapter 1). A transport session correlates all messages from a particular client to a particular channel. If the service is configured for per-call instantiation, there can still be a transport session, but for every call WCF will create a new context used just for that call. If transport-level sessions are not used, as you will see later, the service always behaves as a per-call service, regardless of its configuration.

If the per-call service has a transport session, communication from the client is subjected to the inactivity timeout of the transport session (which defaults to 10 minutes). Once the timeout has expired, the client can no longer use the proxy to invoke operations on the per-call service, since the transport session has ended.

The biggest effect transport sessions have on per-call services is that when the service is configured for single-threaded access (the WCF default, explained in Chapter 8), the transport session enforces a lock-step execution, where calls to the per-call service from the same proxy are serialized. That is, even if the client issues the calls concurrently, they are executed against different instances, one at a time, in order. This has particular implications for disposing of the instance. WCF does not block the client while it disposes of the service instance. However, if during the call to Dispose( ) the client has issued a second call, that call will be allowed to access a new instance only after Dispose( ) has returned. For example, the output at the end of Example 4-2 represents a case where there is a transport session, since the second call can only execute once Dispose( ) has returned. If Example 4-2 had no transport session, you might end up with the same output but also an out-of-order invocation where Dispose( ) is nonblocking, such as:

MyService.MyService(  )
Counter = 1
MyService.MyService(  )
Counter = 1
MyService.Dispose(  )
MyService.Dispose(  )


4.2.4. Designing Per-Call Services
Although in theory you can use the per-call instance activation mode on any service type, in practice you need to design the service and its contracts to support this mode from the ground up. The main problem is that the client doesn't know it's getting a new instance each time it makes a call. Per-call services must be state-aware; that is, they must proactively manage their state, giving the client the illusion of a continuous session. A state-aware service isn't the same as a stateless service. In fact, if the per-call service were truly stateless, there would be no need for per-call activation in the first place. It is precisely because it has state, and an expensive state at that, that you need the per-call mode. An instance of a per-call service is created just before every method call and is destroyed immediately after each call. Therefore, at the beginning of each call, the object should initialize its state from values saved in some storage, and at the end of the call it should return its state to the storage. Such storage is typically either a database or the filesystem, but volatile storage (e.g., static variables) may be used instead.

Not all of the object's state can be saved as-is, however. For example, if the state contains a database connection, the object must reacquire the connection at construction or at the beginning of every call and dispose of the connection at the end of the call or in its implementation of IDisposable.Dispose( ).

Using the per-call instance mode has one important implication for operation design: every operation must include a parameter to identify the service instance whose state needs to be retrieved. The instance uses that parameter to get its state from the storage, and not the state of another instance of the same type. Consequently, state storage is typically keyed (for example, as a static dictionary in memory or a database table). Examples of such state parameters are the account number for a bank account service, the order number for an order-processing service, and so on.

Example 4-3 shows a template for implementing a per-call service.

Example 4-3. Implementing a per-call service
[DataContract](DataContract.md)
class Param
{...}

[ServiceContract](ServiceContract.md)
interface IMyContract
{
> [OperationContract](OperationContract.md)
> void MyMethod(Param stateIdentifier);
}
[ServiceBehavior(InstanceContextMode = InstanceContextMode.PerCall)]
class MyPerCallService : IMyContract,IDisposable
{
> public void MyMethod(Param stateIdentifier)
> {
> > GetState(stateIdentifier);
> > DoWork(  );
> > SaveState(stateIdentifier);

> }
> void GetState(Param stateIdentifier)
> {...}
> void DoWork(  )
> {...}
> void SaveState(Param stateIdentifier)
> {...}
> public void Dispose(  )
> {...}
}




The class implements the MyMethod( ) operation, which accepts a parameter of type Param (a pseudotype invented for this example) that identifies the instance:

public void MyMethod(Param stateIdentifier);


The instance then uses the identifier to retrieve its state and to save the state back at the end of the method call. Any piece of state that is common to all clients can be allocated at the constructor and disposed of in Dispose( ).

The per-call activation mode works best when the amount of work to be done in each method call is finite, and there are no more activities to complete in the background once a method returns. Because the object will be discarded once the method returns, you should not spin off background threads or dispatch asynchronous calls back into the instance.

Since the per-call service retrieves its state from some storage in every method call, per-call services work very well in conjunction with a load-balancing machine, as long as the state repository is some global resource accessible to all machines. The load balancer can redirect calls to different machines at will, knowing that each per-call service can execute the call after retrieving its state.

4.2.4.1. Per-call services and performance
Per-call services clearly offer a trade-off between performance (the overhead of retrieving and saving the instance state on each method call) and scalability (holding onto the state and the resources it ties in). There are no hard-and-fast rules as to when and to what extent you should trade some performance for a lot of scalability. You may need to profile your system and ultimately design some services to use per-call activation and others not to use it.

4.2.4.2. Cleanup operations
Whether or not the service type supports IDisposable is an implementation detail and is of no relevance to the client. In fact, the client has no way of calling the Dispose( ) method anyway. When you design a contract for a per-call service, avoid defining operations that are dedicated for state or resource cleanup, like this:

//Avoid
[ServiceContract](ServiceContract.md)
interface IMyContract
{
> void DoSomething(  );
> void Cleanup(  );
}
[ServiceBehavior(InstanceContextMode = InstanceContextMode.PerCall)]
class MyPerCallService : IMyContract,IDisposable
{
> public void DoSomething(  )
> {...}
> public void Cleanup(  )
> {...}
> public void Dispose(  )
> {
> > Cleanup(  );

> }
}


The folly of such a design is obvious: if the client does call the cleanup method, it has the detrimental effect of creating an object just so the client can call Cleanup( ) on it, followed by a call to IDisposable.Dispose( ) (if present) by WCF to do the cleanup again.

4.2.5. Choosing Per-Call Services
While the programming model of per-call services may look somewhat alien to client/server developers, per-call services are actually the preferred instance management mode for many WCF services. This is simply because per-call services scale better, or at least are scale-invariant. When designing a service, my golden rule for scalability is 10X. That is, every service should be designed to handle a load at least an order of magnitude greater than what its requirements call for. In every other engineering discipline, engineers never design a system to handle its exact nominal specified load. You would not want to enter a building whose beams can support only the exact load they were required to handle, ride in an elevator whose cable can handle only the exact number of passengers it's rated for, and so on. Software systems are no different—why design a system for the specific current load while every other person in the company is working to increase business and the implied load? You should design software systems to last years and to sustain current and future loads. As a result, when using the 10X golden rule, you very quickly end up needing the scalability of the per-call service.

> # Per-session services #

4.3. Per-Session Services
WCF can maintain a logical session between a client and a particular service instance. When the client creates a new proxy to a service configured as a sessionful service, the client gets a new dedicated service instance that is independent of all other instances of the same service. That instance will typically remain in service until the client no longer needs it. This activation mode (sometimes also referred to as the private-session mode) is very much like the classic client/server model: each private session uniquely binds a proxy and its set of client- and service-side channels to a particular service instance, or, more specifically, to its channel. It follows that a transport session is required for the private-session instantiation mode, as discussed later in this section.

Because the service instance remains in memory throughout the session, it can maintain state in memory, and the programming model is very much like that of the classic client/server model. Consequently, it suffers from the same scalability and transaction issues as the classic client/server model. A service configured for private sessions cannot typically support more than a few dozen (or perhaps up to one or two hundred) outstanding clients, due to the cost associated with each such dedicated service instance.

> The client session is per service endpoint per proxy. If the client creates another proxy to the same or a different endpoint, that second proxy will be associated with a new instance and session.






4.3.1. Configuring Private Sessions
There are three elements to supporting a session: behavior, binding, and contract. The behavior part is required so that WCF will keep the service instance context alive throughout the session, and to direct the client messages to it. This local behavior facet is achieved by setting the InstanceContextMode property of the ServiceBehavior attribute to InstanceContextMode.PerSession:

[ServiceBehavior(InstanceContextMode = InstanceContextMode.PerSession)]
class MyService : IMyContract
{...}


Since InstanceContextMode.PerSession is the default value of the InstanceContextMode property, these definitions are equivalent:

class MyService : IMyContract
{...}

[ServiceBehavior](ServiceBehavior.md)
class MyService : IMyContract
{...}

[ServiceBehavior(InstanceContextMode = InstanceContextMode.PerSession)]
class MyService : IMyContract
{...}


The session typically terminates when the client closes the proxy, which causes the proxy to notify the service that the session has ended. If the service supports IDisposable, the Dispose( ) method will be called asynchronously to the client. However, Disposed( ) will be called on a worker thread without an operation context.

In order to correlate all messages from a particular client to a particular instance, WCF needs to be able to identify the client. As explained in Chapter 1, this is exactly what the transport session achieves. If your service is designed to be used as a sessionful service, there has to be some contract-level way for you to express that expectation. The contractual element is required across the service boundary, because the client-side WCF runtime needs to know it should use a session. To that end, the ServiceContract attribute offers the property SessionMode, of the enum type SessionMode:

public enum SessionMode
{
> Allowed,
> Required,
> NotAllowed
}
[AttributeUsage(AttributeTargets.Interface|AttributeTargets.Class,
> > Inherited=false)]
public sealed class ServiceContractAttribute : Attribute
{

> public SessionMode SessionMode
> {get;set;}
> //More members
}


SessionMode defaults to SessionMode.Allowed. The configured SessionMode value is included in the service metadata and is reflected correctly when the client imports the contract metadata. The enum value of SessionMode has nothing to do with the service session; in fact, its proper name should have been transportSessionMode since it pertains to the transport session, not to the logical session maintained between the client and the instance.

4.3.1.1. SessionMode.Allowed
SessionMode.Allowed is the default value of the SessionMode property, so these definitions are equivalent:

[ServiceContract](ServiceContract.md)
interface IMyContract
{...}

[ServiceContract(SessionMode = SessionMode.Allowed)]
interface IMyContract
{...}


All bindings support configuring the contract on the endpoint with SessionMode.Allowed. When the SessionMode property is configured with this value, transport sessions are allowed, but not enforced. The exact resulting behavior is a product of the service configuration and the binding used. If the service is configured for per-call activation, it still behaves as per-call service, as is the case in Example 4-2. When the service is configured for per-session activation, it will behave as a per-session service only if the binding used maintains a transport-level session. For example, the BasicHttpBinding can never have a transport-level session, due to the connectionless nature of the HTTP protocol. The WSHttpBinding without security and without reliable messaging will also not maintain a transport-level session. In both of these cases, even though the service is configured with InstanceContextMode.PerSession and the contract with SessionMode.Allowed, the service will behave as a per-call service.

However, if you use the WSHttpBinding with security (its default configuration) or with reliable messaging, or if you use the NetTcpBinding or the NetNamedPipeBinding, the service will behave as a per-session service. For example, assuming use of the NetTcpBinding, this service behaves as sessionful:

[ServiceContract](ServiceContract.md)
interface IMyContract
{...}

class MyService : IMyContract
{...}


Note that the previous code snippet simply takes the default of both the SessionMode and the InstanceContextMode properties.

4.3.1.2. SessionMode.Required
The SessionMode.Required value mandates the use of a transport-level session, but not necessarily an application-level session. You cannot have a contract configured with SessionMode.Required with a service endpoint whose binding does not maintain a transport-level session, and this constraint is verified at the service load time. However, you can still configure the service to be a per-call service, and the service instance will be created and destroyed on each client call. Only if the service is configured as a sessionful service will the service instance persist throughout the client's session:

[ServiceContract(SessionMode = SessionMode.Required)]
interface IMyContract
{...}

class MyService : IMyContract
{...}


> When designing a sessionful contract, I recommend explicitly using SessionMode.Required and not relying on the default of SessionMode.Allowed. The rest of the code samples in this book actively apply SessionMode.Required when sessionful interaction is by design.






Example 4-4 lists the same service and client as in Example 4-2, except the contract and service are configured to require a private session. As you can see from the output, the client got a dedicated instance.

Example 4-4. Per-session service and client
///////////////////////// Service Code /////////////////////
[ServiceContract(SessionMode = SessionMode.Required)]
interface IMyContract
{
> [OperationContract](OperationContract.md)
> void MyMethod(  );
}
class MyService : IMyContract,IDisposable
{
> int m\_Counter = 0;

> MyService(  )
> {
> > Trace.WriteLine("MyService.MyService(  )");

> }
> public void MyMethod(  )
> {
> > m\_Counter++;
> > Trace.WriteLine("Counter = " + m\_Counter);

> }
> public void Dispose(  )
> {
> > Trace.WriteLine("MyService.Dispose(  )");

> }
}
///////////////////////// Client Code /////////////////////
MyContractClient proxy = new MyContractClient(  );

proxy.MyMethod(  );
proxy.MyMethod(  );

proxy.Close(  );

//Output
MyService.MyService(  )
Counter = 1
Counter = 2
MyService.Dispose(  )




4.3.1.3. SessionMode.NotAllowed
SessionMode.NotAllowed disallows the use of a transport-level session, which precludes an application-level session. Regardless of the service configuration, when this value is used the service will always behave as a per-call service.

Since both the TCP and IPC protocols maintain a session at the transport level, you cannot configure a service endpoint that uses the NetTcpBinding or the NetNamedPipeBinding to expose a contract marked with SessionMode.NotAllowed, and this is verified at the service load time. However, the use of the WSHttpBinding with an emulated transport session is still allowed. In the interest of readability, I recommend that when selecting SessionMode.NotAllowed, you always also configure the service as per-call:

[ServiceContract(SessionMode = SessionMode.NotAllowed)]
interface IMyContract
{...}

[ServiceBehavior(InstanceContextMode = InstanceContextMode.PerCall)]
class MyService : IMyContract
{...}


Since the BasicHttpBinding cannot have a transport-level session, endpoints that use it behave as if the contract is always configured with SessionMode.NotAllowed. I view SessionMode.NotAllowed as a setting available for the sake of completeness more than anything else, and I would not explicitly choose it.

4.3.1.4. Bindings, contracts, and service behavior
Table 4-1 summarizes the resulting instance mode as a product of the binding being used, the session mode in the contract, and the configured instance context mode in the service behavior. The table does not list invalid configurations, such as SessionMode.Required with the BasicHttpBinding.

Table 4-1. Instance mode as a product of the binding, contract configuration, and service behavior Binding
> Session mode
> Context mode
> Instance mode

Basic
> Allowed/NotAllowed
> PerCall/PerSession
> PerCall

TCP, IPC
> Allowed/Required
> PerCall
> PerCall

TCP, IPC
> Allowed/Required
> PerSession
> PerSession

WS (no security, no reliability)
> NotAllowed/Allowed
> PerCall/PerSession
> PerCall

WS (with security or reliability)
> Allowed/Required
> PerSession
> PerSession

WS (with security or reliability)
> NotAllowed
> PerCall/PerSession
> PerCall





4.3.1.5. Consistent configuration
I strongly recommend that if one contract the service implements is a sessionful contract, then all contracts should be sessionful, and that you should avoid mixing per-call and sessionful contracts on the same per-session service type (even though WCF allows it):

[ServiceContract(SessionMode = SessionMode.Required)]
interface IMyContract
{...}

[ServiceContract(SessionMode = SessionMode.NotAllowed)]
interface IMyOtherContract
{...}

//Avoid
class MyService : IMyContract,IMyOtherContract
{...}


The reason is obvious: per-call services need to proactively manage their state, while per-session services do not. While the two contracts will be exposed on two different endpoints and can be consumed independently by two different clients, this duality requires cumbersome implementation for the underlying service class.

4.3.2. Sessions and Reliability
The session between the client and the service instance is only as reliable as the underlying transport session. Consequently, a service that implements a sessionful contract should have all of its endpoints that expose that contract use bindings that enable reliable transport sessions. Make sure to always use a binding that supports reliability and to explicitly enable it at both the client and the service, either programmatically or administratively, as shown in Example 4-5.

Example 4-5. Enabling reliability for per-session services
<!—Host configuration:—>
<system.serviceModel>
> 

&lt;services&gt;


> > 

&lt;service name = "MyPerSessionService"&gt;


> > > <endpoint
> > > > address  = "net.tcp://localhost:8000/MyPerSessionService"
> > > > binding  = "netTcpBinding"
> > > > bindingConfiguration = "TCPSession"
> > > > contract = "IMyContract"

> > > />

> > 

&lt;/service&gt;



> 

&lt;/services&gt;


> 

&lt;bindings&gt;


> > 

&lt;netTcpBinding&gt;


> > > 

&lt;binding name = "TCPSession"&gt;


> > > > 

&lt;reliableSession enabled = "true"/&gt;



> > > 

&lt;/binding&gt;



> > 

&lt;/netTcpBinding&gt;



> 

&lt;/bindings&gt;


</system.serviceModel>

<!—Client configuration:—>
<system.serviceModel>
> 

&lt;client&gt;


> > <endpoint
> > > address  = "net.tcp://localhost:8000/MyPerSessionService/"
> > > binding  = "netTcpBinding"
> > > bindingConfiguration = "TCPSession"
> > > contract = "IMyContract"

> > />

> 

&lt;/client&gt;


> 

&lt;bindings&gt;


> > 

&lt;netTcpBinding&gt;


> > > 

&lt;binding name = "TCPSession"&gt;


> > > > 

&lt;reliableSession enabled = "true"/&gt;



> > > 

&lt;/binding&gt;



> > 

&lt;/netTcpBinding&gt;



> 

&lt;/bindings&gt;


</system.serviceModel>




The one exception to this rule is the IPC binding. This binding has no need for the reliable messaging protocol (all calls will be on the same machine anyway), and it is considered an inherently reliable transport.

Just as a reliable transport session is optional, so is ordered delivery of messages, and WCF will provide for a session even when ordered delivery is disabled. However, by the very nature of an application session, a client that interacts with a sessionful service expects all messages to be delivered in the order they are sent. Luckily, ordered delivery is enabled by default when reliable transport sessions are enabled, so no additional setting is required.

4.3.3. The Session ID
Every session has a unique ID that both the client and the service can obtain. The session ID is largely in the form of a GUID, and it can be used for logging and diagnostics. The service can access the session ID via the operation call context, which is a set of properties (including the session ID) that are used for callbacks, message headers, transaction management, security, host access, and access to the object representing the execution context itself. Every service operation has an operation call context, accessible via the OperationContext class. A service can obtain a reference to the operation context of the current method via the Current static method of the OperationContext class:

public sealed class OperationContext : ...
{
> public static OperationContext Current
> {get;set;}
> public string SessionId
> {get;}
}


To access the session ID, the service needs to read the value of the SessionId property, which returns (almost) a GUID in the form of a string, followed by the ordinal number of the session from that host:

string sessionID = OperationContext.Current.SessionId;
Trace.WriteLine(sessionID);
//Traces:
//uuid:8a0480da-7ac0-423e-9f3e-b2131bcbad8d;id=1


If a per-call service without a transport session accesses the SessionId property, the session ID will be null, since there is no session and therefore no ID.

The client can access the session ID via the proxy. As introduced in Chapter 1, the class ClientBase

&lt;T&gt;

 is the base class of the proxy. ClientBase

&lt;T&gt;

 provides the read-only property InnerChannel of the type IClientChannel. IClientChannel derives from the interface IContextChannel, which provides a SessionId property that returns the session ID in the form of a string:

public interface IContextChannel : ...
{
> string SessionId
> {get;}
> //More members
}
public interface IClientChannel : IContextChannel,...
{...}
public abstract class ClientBase

&lt;T&gt;

 : ...
{
> public IClientChannel InnerChannel
> {get;}
> //More members
}


Given the definitions in Example 4-4, the client might obtain the session ID like this:

MyContractClient proxy = new MyContractClient(  );
proxy.MyMethod(  );

string sessionID = proxy.InnerChannel.SessionId;
Trace.WriteLine(sessionID);
//Traces:
//urn:uuid:c8141f66-51a6-4c66-9e03-927d5ca97153


However, the degree to which the client-side session ID matches that of the service (and even when the client is allowed to access the SessionId property) is a product of the binding used and its configuration. What correlates the client-side and service-side session IDs is the reliable session at the transport level. If the TCP binding is used, when a reliable session is enabled (as it should be) the client can obtain a valid session ID only after issuing the first method call to the service to establish the session, or after explicitly opening the proxy. In this case, the session ID obtained by the client will match that of the service. (If the client accesses the session ID before the first call, the SessionId property will be set to null.) If the TCP binding is used but reliable sessions are disabled, the client can access the session ID before making the first call, but the ID obtained will be different from that obtained by the service. With either of the WS bindings, if reliable messaging is enabled the session ID will be null until after the first call (or after the client opens the proxy), but after that the client and the service will always have the same session ID. Without reliable messaging, the client must first use the proxy (or just open it) before accessing the session ID, or risk an InvalidOperationException. After opening the proxy, the client and the service will have a correlated session ID. With the IPC binding, the client can access the SessionId property before making the first call, but the client will always get a session ID different from that of the service. When using this binding, it is therefore better to ignore the session ID altogether.

4.3.4. Session Termination
Typically, the session will end once the client closes the proxy. However, in case the client neglects to close the proxy, or when the client terminates ungracefully or there is a communication problem, the session will also terminate once the inactivity timeout of the transport session is exceeded.