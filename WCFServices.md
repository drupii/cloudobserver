# WCF Services #

WCF(Windows Communication Foundation) - реализация SOAP протокола от Microsoft (http://ru.wikipedia.org/wiki/SOAP). В нашем случае с его помощью мы будем реализовывать удаленный вызов методов (сервисов) и обращеные к данным через них, которые выполняются на сервере.
Для нас важно, что WCF сервисы бывают двух видов - IIS hosting и Self-hosting. Первые осуществляют хостинг (т.е. располагаются) на IIS сервере, вторые там, где им будет сказано при обращении к ним.

# Using #

Важно понимать, что работа с WCF осуществляется под VS2008. Не столько как удобной средой для работы с WCF, сколько как интерфейс для редактирования непосредственно конфигурации WCF сервиса.

Для пользователей OC Vista VisualStudio необходимо запускать от имени администратора(соответствующий пункт в сниспадающем с ярлыка меню)
Для создания сервиса в рамках проекта (File -> new -> project -> c# -> Web -> WCF Service Application), для создания библиотеки сервисов (ile -> new -> project -> c# -> WCF -> WCF Service Library)

В получившемся проекте для нас ключевыми являются файлы Web.config для IIS(App.Config для self-hostingg), Service.svc, IService.cs. Service.cs. Имена последних двух зависят от имени проекта. Важное **замечание** состоит в том, что при изменении имен этих файлов необходимо отражать эти изменения в файле .config. Файл Service.svc хранит информацию о хосте и ссылку на файл с метаданными. метаданные это Интерфейс сервиса через который осуществляется доступ к самому сервису.

Сейчас советую прочитать (6) разделы "Хостинг и экземпляр службы", "Инстанциация", "А что клиент?" для получения общего представления оп роисходящем. Правда, то что там написано, будет повторено мною здесь.

На счет файлов .config:
Длинное автосгенерированное студией описание в начале нас не интересует. В конце есть тег 

&lt;service&gt;

 с описанием сервиса.

В нем важно: заголовок сервиса, описания endpoint'ов, поведений, baseaddress. Разбор на примере:


&lt;service name="CloudObserverWCFService.Service1" behaviorConfiguration="CloudObserverWCFService.Service1Behavior"&gt;


Имя сервиса - совпадает с файлом где реализована бизнес-логика сервиса с учетом пространства имен. Подлежит изменению при переименовании файла. Поведение - описано ниже - характеризует такие свойства сервиса, как раскрытие мета-информации, шифроние передач и т.п.


&lt;endpoint address="" binding="wsHttpBinding" contract="CloudObserverWCFService.IService1"&gt;


адрес - конечный адрес, по которому можно достучаться до сервиса. Если есть тег 

&lt;basesddress&gt;

, то адреса конкатенируются. Тип биндинга (биндинг - читать это: "there are many possible transport protocols for the messages, such as HTTP (or HTTPS), TCP, IPC, and MSMQ. There are also a few possible message encoding options: you can choose plain text to enable interoperability, binary encoding to optimize performance, or the Message Transport Optimization Mechanism (MTOM) for large payloads. Finally, there are multiple options for securing messages: you can choose not to secure them at all, to provide transport-level security only, or to provide message-level privacy and security, and of course there are numerous ways to authenticate and authorize the clients. Message delivery might be unreliable or reliable end-to-end across intermediaries and dropped connections, and the messages might be processed in the order they were sent or in the order they were received. Your service might need to interoperate with other services or clients that are aware of only the basic web service protocol, or with clients and services capable of using the score of WS-modern protocols, such as WS-Security and WS-Atomic Transactions. To simplify these choices and make them manageable, WCF groups together sets of such communication aspects in bindings") Стандартные Биндинги: BasicHttpBinding, NetTcpBinding, NetNamedPipeBinding, WSHttpBinding, подробнее см. книгу 1. Контракт - описание интерфейса сервеса - хранится в файле с описанием интрефейса, подлежит изменению при переименовании соответствующего файла.


&lt;dns value="localhost"/&gt;

 - кто занимается выдачей имен.


&lt;endpoint address="mex" binding="mexHttpBinding" contract="IMetadataExchange"/&gt;

 - ендпоинт, с которого клиенты получают мета-информацию.
Ниже идут описания поведений. Суть происходящего состоит в том, чтобы сократить до минимума нагрузку по хранению и поточной обработки информаци со стороны клиента, переложив его на сервер. Попутно можно осуществлять ограничения на доступ, шифрование передаваемых данных, сокрытие бизнес-логики проекта. Этого можно добиться путем описания поведений.
Ендпоинтов и поведений может быть несколько - читай книги =)

Далее, рассмотрим вопрос реализации сервисов:
Есть два файла - один с интерфейсом, второй с реализацией. Второй мы не трогаем - там просто реализуется интерфейс. Над именем интерфейса пишем [ServiceContract](ServiceContract.md)(service контракт это то что реализует функциональность сервиса). В описании интерфейса - если мы хотим какой то метод сделать как предоставляемый сервисом(т.е. чтобы клиенты могли его использовать) прямо над ним пишем [OperationContract](OperationContract.md). Если мы хотим чтобы какие то поля были доступны и видны пользователю над полем пишем атрибут [DataContract](DataContract.md), а если тип - перечисление, то [Enumcontract](Enumcontract.md) над каждым элементом перечисления.
Подробные примеры см здесь: (4). Об атрибутах см (5).

**Замечание** Для того чтобы передавать объекты по WCF можно разобраться с примером по ссылке (7).

Теперь, кажется, работа с сервисом закончена и мы хотим получить доступ к WCF Service со стороны клиента.
Создаем какой-нибудь тестовый проект или имеем уже готовый. Нажимаем кнопочку "AddServiceReference" в сниспадающем меню иконки клиент-проекта. В поле для адреса вводим адрес где искать сервисы. Если сервис в нашем же Solution, то просто нажимаем Discover. В левом поле появится список доступных сервисов. Выбираем нужный и в поле NameSpace пишем понравившееся имя пространства имен. Именно в нем будут лежать нужные нам методы и поля. Его надо добавить проект и обращаться к методам через него. То есть поступаем с ним соответственно обращению с любым пространством имен.

**Замечание** для использования сервисов в своем проекте надо добавить:
using System.ServiceModel; //там лежит описание сервис подхода. Т.е. реализация WCF
using System.ServiceModel.Description;
using System.Runtime.Serialisation;
//для знатоков c# поэтому не комментирую
Если некоторые из них он не находит, то AddReference и ищем  их там.

Для того, чтобы начать работу с нашим WCF сервисом - надо создать объект через который мы будем с методами работать(proxy - объект). Для self-hosting сервисов это делается примерно так:
ServiceHost <имя> = new ServiceHost(<тип сервиса>,<адрес>);
<имя>.open();
//работаем
<имя>.close();

Для сервисов с хостингом на IIS или с помощью встроенной утилиты от VS2008, пишем
<имя интерфейса сервиса> + "clien" -  создаем такой вот новый объект и через него обращаемся к функциональности сервиса.

# TroubleShooting #

Основная проблема состоит в том, при добавлении в проект сервисов и библиотек сервисов появятся проблемы с получением мета-информации от сервиса. Это связано с тем что Vista предъявляет большие требования к защите, не пропускает запросы на порты кроме 80, и занимается какой то ерундой по переадресации запросов туда, где их никто не ждет.
Для пользователей операционной системы Windows Vista, прежде чем начать работу с сервисами необходимо сделать 9 следующих пунктов(для XP это кажется необязательным, но лучше тоже сделать):
  1. Go to c:\Windows\System32\drivers\etc\ and open the file named hosts with a text editor (для Vista - запуск в режиме администратора) Search for the line containing "::1" mapped to "localhost" Change "::1" to ":::1" by adding an extra ":" Как вариант можно заменить на "#::1"
  1. Установить "Microsoft Silverlight Tools Beta 2 for Visual Studio 2008"
  1. Установить Microsoft Expression Blend 2.5 June 2008 Preview (для того, чтобы можно было создавать дизайн, т.к. Visual Studio не работает как WYSIWYG-редактор с сильверлайтом)
  1. Установить в висте IIS: Панель управления –> Программы –> Программы и компоненты –> Включение и отключение компонентов Windows -> Службы IIS -> отмечаем “Службы Интернета” (галочки по дефолту), а также отмечаем в разделе «Средства управления Веб-узлом» ВСЁ (кроме консоли управления IIS 6 в сегменте «Совместимость управления IIS 6»), жмем «Ok» и дожидаемся, пока все поставится.
  1. Доставляем компоненты Microsoft .NET Framework 3.0 (там же, в «Компонентах Windows»): отмечаем в сегменте «Microsoft .NET Framework 3.0» галочками «Windows Communication Foundation HTTP Activation» и «Windows Communication Foundation Non-HTTP Activation»
  1. Далее необходимо запустить с правами администратора консоль (CMD) и установить ASP.NET в IIS 7 командной "c:\Windows\Microsoft.NET\Framework64\v2.0.50727\aspnet\_regiis.exe -i" Версия 2-го framework'а (2.0.50727 или другой) и платформа (x86 или x64) может отличаться.
  1. С правами администратора из консоли выполнить регистрацию WCF в IIS 7 командой "c:\Windows\Microsoft.NET\Framework64\v3.0\Windows Communication Foundation\ServiceModelReg.exe -i".Платформа (x86, x64) может отличаться. [вобщем-то, для того, чтобы все хостилось на IIS'e, достаточно вышеописанных действий и запуске VS2008 от имени администратора, если включена служба UAC]
  1. Для запуска и отладки ASP.NET и WCF-приложений в Internet Explorer 7 из Visual Studio через «ASP.NET Development Server» необходимо отключить поддержку IP v6. Если вы используете Firefox в качестве браузера по умолчанию, это сделать просто. Для отключения IPv6 в Firefox надо: Написать «about:config» в строку адреса и нажать «Enter». Внизу найти «network.dns.disableIPv6». Дважды кликнув, изменить значение на «true». Перезагрузить Firefox. А с IE дела обстоят сложнее, т.к. такой возможности (отключить поддержку версии протокола нет). Поэтому пришлось сделать следующее. В реестре создать/изменить ключ (типа REG\_DWORD) по пути HKEY\_LOCAL\_MACHINE\SYSTEM\CurrentControlSet\Services\tcpip6\Parameters\DisabledComponents, установив его равным 0xff (255 в десятеричном представлении) и _перезагрузить систему_. Отлично, но при нажатии по F5 вновьсозданного проекта "ASP.NET Development Server" открывал порт, отличный от того, что у URL-e браузера. Причем стабильно отличный на 2 (например, в строке браузера http://localhost:1654/Service1/ ,a сервер открывает отладку на http://localhost:1652/Service1/). При попытке выключить динамический выбор порта, прописав статический, студия выдавала ошибку, что данный порт занят (какой бы я не вводил). Погуглив еще, я нашел, что с подобными затруднениями встречались многие. Так вот... Если на компьютере установлен «ESET NOD32» или «ESET Smart Security», то необходимо сделать настройку фильтрации HTTP-трафика: в расширенном режиме настройки в пути «Защита от вирусов и шпионских программ» -> «Фильтрация протоколов» -> «Перенаправить проверяемый трафик для» -> указать «порты HTTP и POP3», и нажимаем «ОК». Иначе открываемый порт «Development Server»-ом, отличается от порта, к которому идет обращение из билда, при этом создавать порт автоматически не удается.
  1. Создаем тестовый проект, конфигурируем – “IIS” или “DevelopmentServer”, нажимаем “F5” и проверяем, работает ли!

Дополнительную информацию можно поискать по ссылкам:
http://www.sql.ru/forum/actualthread.aspx?tid=570157
http://bvencel.blogspot.com/2008/05/aspnet-development-server-problems.html

# Details #
Книги и ссылки на тему:
  1. Programming WCF Services, 2nd Edition, Print ISBN-13: 978-0-596-52130-1
  1. Inside Windows Communication Foundation, Microsoft Press
  1. http://msdn.microsoft.com/en-us/library/ms733766.aspx про то как хостить сервис на IIS.
  1. http://www.dinalt.ru/Blog/WCF1
  1. Шилдт Г. Полный Справочник по C#
  1. http://www.aist-agent.ru/?WCF0
  1. http://www.gotdotnet.ru/Forums/DotNet3.0/452331.aspx