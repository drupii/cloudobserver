Начальную информацию о формировании HTTP запросов можно почерпнуть здесь:

http://www.cyberguru.ru/dotnet/web-services-wcf/wcf-http-page2.html

Просто отправить HTTP-запрос можно с помощью сокетов. Надо просто сделать подключение и отправить с помощью имеющегося метода работы с сокетами сформированный "в строке" запрос соответствующий спецификации HTTP, которая имеется здесь:

http://lib.misto.kiev.ua/WEBMASTER/rfc2068/

Для нашего сервиса:
address="http://localhost:2101/CloudObserverWebsite/CloudObserverService.svc"

В первой статье стоит обратить внимание на пример:
то что написано ниже лучше смотреть в режиме редактирования страницы.
[ServiceContract](ServiceContract.md)
public interface IPictureService
{
> [OperationContract](OperationContract.md)
> [WebGet(UriTemplate = "picture/{pictureId}")]
> Stream GetPicture(String pictureId);

> [OperationContract](OperationContract.md)
> [WebGet(UriTemplate = "picture/t/{pictureId}")]
> Stream GetPictureThumbnail(String pictureId);
}
Здесь, если к base-адресу добавлен такой шаблон, то происходит автоматическое считывание данных с преобразованием к строке(по сути разборка - парсинг). Из этого примера становится легко сделать задачу по сохранению изображения на стороне сервиса.