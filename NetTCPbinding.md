> # TCP binding #

Description could be find: http://blogs.msdn.com/drnick/archive/2006/06/05/617703.aspx.

And Here: http://msdn.microsoft.com/en-us/library/ms752250.aspx.

Offered by the NetTcpBinding class, this uses TCP for cross-machine communication on the intranet. It supports a variety of features, including reliability, transactions, and security, and is optimized for WCF-to-WCF communication. As a result, it requires both the client and the service to use WCF.

When trying to use netTCpbinding with WinForms related to threading. That is strange that there was no problem with it earlier. http://blogs.msdn.com/mahjayar/archive/2006/11/07/winfx-usesynchronizationcontext-usage.aspx
You have to set usesynchronizationcontext = false in description of service behavior. otherwise you will never receive an answer. There is no such problem with console application because of the only thread.


Is the service hosted in IIS? If so, IIS 5.0 cannot host net.tcp binding services. NetTcp in IIS are only supported in IIS7 with WAS.

http://www.neovolve.com/post/2007/09/18/wcf-nettcpbinding-service-in-ws2008-beta-2.aspx

adding written in the link: http://msdn.microsoft.com/en-us/library/ms731053.aspx.