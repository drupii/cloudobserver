> # Некоторый troubleshooting #

> # Часть 1. #

> ## Проблема: ##

> ## Решение: ##

> # Часть 2. #

> ==Проблема:== Ограничен максимальный размер передаваемых данных: строк, структур данных, массивов

> ## Решение: ##
Опытные люди сказали мне, что решения нет. WCF имеет неснимаемое ограничение. Поэтому например в другом проекте написали пересылку по сегментам. Хотя можно попробовать и это:
http://sql.ru/forum/actualthread.aspx?tid=518271
решение состоит в том что на стороне сервиса(следовательно при добавлении ServiceReference и у клиента) прописать новые параметры взамен тех что по умолчанию в описание биндинга:

> 

&lt;bindings&gt;


> > 

&lt;basicHttpBinding&gt;


> > > 

&lt;binding name="basicHttpBinding\_IPhotoManagerService" transferMode="Streamed" messageEncoding="Mtom"  closeTimeout="00:01:00" openTimeout="00:01:00" receiveTimeout="00:10:00" sendTimeout="00:01:00" bypassProxyOnLocal="false" hostNameComparisonMode="StrongWildcard" maxBufferPoolSize="524288" maxReceivedMessageSize="2097151" textEncoding="utf-8" useDefaultWebProxy="true" allowCookies="false"&gt;


> > > > 

&lt;readerQuotas maxDepth="32" maxStringContentLength="8192" maxArrayLength="16384" maxBytesPerRead="4096" maxNameTableCharCount="16384"/&gt;




> 

&lt;security mode="TransportCredentialOnly"&gt;


> > 

&lt;transport clientCredentialType="None" proxyCredentialType="None" realm=""/&gt;


> > 

&lt;message  algorithmSuite="Default" /&gt;



> 

&lt;/security&gt;


> 

&lt;/binding&gt;


> 

&lt;/basicHttpBinding&gt;


> 

&lt;/bindings&gt;



Важно, что указаны параметры: Streamed, Mtom, 65536000.
А еще важно, что :
  1. При отдельно стоящем WCF-сервисе стриминг идет отлично в обе стороны.
  1. При попытке запихать сервис под IIS, существует такая описанная проблема: стриминг НА сервер работает. Стриминг С сервера - не работает. IIS упорно буфферизует поток и заставить его стриминговать - нереально... При передаче файликов метров по двести очень хорошо видно, как это всё лезет через память :)
У вас есть два варианта:
  1. Забить на IIS и использовать WCF-сервис под своей виндослужбой.
  1. Забить на WCF и использовать IIS с веб-сервисами.

Еще предлагается попытаться сделать то, что написано здесь:
http://sites.google.com/site/phenixpinkbookmarks/iis-wcf-maxclientrequestbuffer-instruction

Basic binding

Offered by the BasicHttpBinding class, this is designed to expose a WCF service as a legacy ASMX web service, so that old clients can work with new services. The basic binding makes your service look, on the wire, like a legacy web service that communicates over the basic web service profile. When used by clients, this binding enables new WCF clients to work with old ASMX services.

Web Service (WS) binding

Offered by the WSHttpBinding class, this uses HTTP or HTTPS for transport, and is designed to offer a variety of features (such as reliability, transactions, and security) over the Internet, all using the WS-**standards. This binding is designed to interoperate with any party that supports the WS-** standards.


> # Часть 3. #

> ==Проблема:== Происходит ошибка при попытке выполнить создание прокси-объекта для работы с сервисом. На самом деле не формируется xml файл c 

&lt;config&gt;

.

> ==Решение:== Со стороны сервиса в настройках биндинга в описании EndPoint указать basicHttpBinding вместо указанного по умолчанию wsHttpBinding.