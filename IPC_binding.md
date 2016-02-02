> # IPC binding #

Offered by the NetNamedPipeBinding class, this uses named pipes as a transport for same-machine communication. It is the most secure binding, since it cannot accept calls from outside the machine. The IPC binding supports a variety of features, similar to the TCP binding. It is also the most performant binding, since IPC is a lighter protocol than TCP.


> # Note #

The NetNamedPipeBinding class is named incorrectly, since the binding naming convention is to refer to the protocol, not the communication mechanism (thus, we have NetTcpBinding rather than NetSocketBinding). The correct name for this binding should have been NetIpcBinding. Throughout this book, I will refer to the NetNamedPipeBinding as the IPC binding.

> # Classification #

Name : NetNamedPipeBinding

Transport : IPC

Encoding : Binary

Interoperable : No

Chosing : Wcf-to-wcf => not disconnected calls => Non-cross-machine => IPC

Full description : http://blogs.msdn.com/drnick/archive/2006/06/06/618445.aspx

> # Using #
IPC Addresses
IPC addresses use net.pipe for transport, to indicate the use of the Windows named pipe mechanism. In WCF, services that use IPC can only accept calls from the same machine. Consequently, you must specify either the explicit local machine name or localhost for the machine name, followed by a unique string for the pipe name:

net.pipe://localhost/MyPipe


You can open a named pipe only once per machine, so it is not possible for two named pipe addresses to share a pipe name on the same machine.

Note: The IPC address format is incorrect, indicating the mechanism instead of the protocol. The correct schema format should have been net.ipc instead of net.pipe, much like the TCP address uses net.tcp rather than net.socket.

IPC binding has no need for the reliable messaging protocol (all calls will be on the same machine anyway), and it is considered an inherently reliable transport.

With the IPC binding, the client can access the SessionId property before making the first call, but the client will always get a session ID different from that of the service. When using this binding, it is therefore better to ignore the session ID altogether.

In VS2008 System.ServiceModel.NetNamedPipeBinding.

Important note: net.pipe://localhost/MyPipe without port and necessarily localhost. Without address after domain, because there is just a name of pipe.