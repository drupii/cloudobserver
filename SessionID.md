> # Preview #

Our sample project CloudObserverBroadcastServiceLibrary is done without correct users and cameras checking. The service is a singleton instance object that suppose that we have to have a metod of identifying who we work with at the moment. When a user or camera want to become a client of our service we identify them by SessionID. It is a property which can be received here: OperationContext/current/SessionID. When we have a binding which support a transport sessions we can get SessionID. If it doesn't support transport session than SessionId is null

> # Trouble Description #

When we use basicHttpBinding we can't correctly get a SessionID

> # Troubleshoot #

Really, basicHttpBinding doesn't support transport sessions. We have to use wsHttpBinding. It simulates transport session. So we get metod to identify which client we work with at the moment.
Another solving is ti refuse using SessionID property and use information which client will send to us.
Some another information could be get in Wiki/WCFServices /book1 in the list of links.
and here: http://silverlight.net/forums/t/14175.aspx

also look this: http://weblogs.asp.net/brucezhang/archive/2009/06/01/stream-operation-in-wcf.aspx
When TransferMode is stream than we have not sessionID

> # Afternote #

that is not enought useful for our tasks