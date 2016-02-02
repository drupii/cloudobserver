# Развёртка среды разработки. #

## Используемое программное обеспечение ##
  * [Программный Эмулятор Камеры  VirtualCamera (1,4 Мб)](http://www.soundmorning.com/download.php)
  * Microsoft Visual Studio 2008 Team System (3,67 Гб)
  * [Microsoft Visual Studio 2008 Service Pack 1 (831,3 Мб)](http://www.microsoft.com/downloads/details.aspx?FamilyID=27673c47-b3b5-4c67-bd99-84e525b5ce61&DisplayLang=en)
  * [Microsoft Silverlight 3 Tools for Visual Studio 2008 SP1 (32,2 Мб)](http://www.microsoft.com/downloads/details.aspx?FamilyID=9442b0f2-7465-417a-88f3-5e7b5409e9dd&DisplayLang=en)
  * [Microsoft Expression Blend 3 + SketchFlow (71,2 Мб)](http://www.microsoft.com/downloads/details.aspx?FamilyID=92e1db7a-5d36-449b-8c6b-d25f078f3609&DisplayLang=en)
  * [Deep Zoom Composer (5,6 Мб)](http://www.microsoft.com/downloads/details.aspx?FamilyID=457b17b7-52bf-4bda-87a3-fa8a4673f8bf&DisplayLang=en)
  * [Microsoft Silverlight 3 Toolkit July 2009 (13,1 Мб)](http://silverlight.codeplex.com/Release/ProjectReleases.aspx?ReleaseId=24246#DownloadId=73527)
  * [Adobe Air (15,0 Мб)](http://get.adobe.com/ru/air/thankyou/?installer=Adobe_AIR_1.5.1_for_Win32)
  * [Adobe Flash Builder 4 Beta (285,4 Мб)](http://labs.adobe.com/technologies/flashbuilder4/)
  * [TortoiseSVN 1.6.3 (18,9 Мб)](http://downloads.sourceforge.net/tortoisesvn/TortoiseSVN-1.6.3.16613-win32-svn-1.6.3.msi?download)

## Установка программного обеспечения ##
  1. Установите VirtualCamera.  Перезагрузите компьютер.
  1. Установите Microsoft Visual Studio 2008 Team System. Убедитесь, что также установился Microsoft SQL Server 2005.
  1. Установите Microsoft Visual Studio 2008 Service Pack 1.
  1. Удалите все приложения Microsoft Expression и Microsoft Silverlight. Перезагрузите компьютер.
  1. Установите Microsoft Silverlight 3 Tools for Visual Studio 2008 SP1 (также установятся Microsoft Silverlight и Microsoft Silverlight 3 SDK). Перезагрузите компьютер.
  1. Установите Microsoft Expression Blend 3 + SketchFlow (установятся Microsoft Expression Blend 3 и Microsoft Expression Blend 3 SDK). Перезагрузите компьютер.
  1. Установите Deep Zoom Composer.
  1. Установите Microsoft Silverlight 3 Toolkit July 2009.
  1. Установите Adobe AIR.
  1. Установите Adobe Flash Builder 4 Beta.
  1. Установите TortoiseSVN 1.6.3.

## Загрузка исходных кодов программ ##
  1. В проводнике Windows создайте папку для хранения рабочей копии.
  1. Нажмите правой кнопкой мыши по созданной папке и выберите в контекстном меню проводника пункт SVN Checkout...
  1. Укажите в поле URL of repository путь к главной ветви разработки системы CloudObserver: https://cloudobserver.googlecode.com/svn/trunk
  1. Нажмите OK и дождитесь загрузки рабочей копии. По запросу введите логин и пароль. В качестве логина используйте логин для авторизации в системе Google Code со страницы [Source Checkout](http://code.google.com/p/cloudobserver/source/checkout). Пароль вы можете узнать на вкладке [Settings](http://code.google.com/hosting/settings) настроек своего профайла Google Code.

## Сборка проектов в Microsoft Visual Studio 2008 ##
  1. Откройте файл решения cloudobserver.sln в Microsoft Visual Studio 2008 из папки src\cloudobserver.
  1. Запустите сборку всех проектов решения, выбрав пункт меню Build -> Build Solution (или нажав сочетание клавиш Ctrl+Shift+B).

## Создание базы данных ##
  1. Откройте файл решения cloudobserver.sln в Microsoft Visual Studio 2008 из папки src\cloudobserver.
  1. Запустите отладку проекта DatabaseManagementTool, выбрав пункт контекстного меню Debug -> Start new instance или непосредственно запустите исполняемый файл DatabaseManagementTool.exe (**внимание:** для корректной работы с базой данных, в папке с исполняемым файлом также должна находится библиотека CloudObserverDatabaseLibrary.dll).
  1. Нажмите на кнопку Connect для подключения к базе (если базы не существует, она будет создана).
  1. Нажмите на кнопку Setup Default Values для загрузки в базу тестовых значений.
  1. Изучите выведенный отчёт относительно записанных в базу значений.

## Подключение базы данных ##
  1. Запустите Microsoft Visual Studio 2008.
  1. Откройте окно Server Explorer, выбрав пункт меню View -> Server Explorer (или нажав сочетание клавиш Ctrl+Alt+S).
  1. Нажмите правой кнопкой мыши по папке Data Connections и выберите в контекстном меню пункт Add Connection... В поле Server name введите '.\sqlexpress' (без кавычек), в поле Select or enter a database name выберите CloudObserverDatabase.
  1. Нажмите OK.
  1. Для просмотра значений таблиц подключённой базы выделите таблицу в окне Server Explorer и выберите в контекстном меню пункт Show Table Data.

## Запуск сервисов ##
### С использованием утилиты Services Hoster ###
  1. Откройте файл решения cloudobserver.sln в Microsoft Visual Studio 2008 из папки src\cloudobserver.
  1. Запустите отладку проекта CloudObserverServicesHoster, выбрав пункт контекстного меню Debug -> Start new instance или непосредственно запустите исполняемый файл CloudObserverServicesHoster.exe (**внимание:** для корректной работы сервисов, взаимодействующих с базой данных, в папке с исполняемым файлом также должна находится библиотека CloudObserverDatabaseLibrary.dll).
  1. Нажмите на кнопку Install Service.
  1. Нажмите на кнопку Load Additional Services.
  1. Выберите файл динамической библиотеки (.dll) нужного сервиса из папки lib (при запуске приложения из среды Visual Studio возможно возникновение уведомления об отключенном режиме отладки библиотеки).
  1. Нажмите на кнопку Load Service (автоматически будет произведено тестирование подключаемого сервиса).
  1. Выберите загруженный сервис, установите желаемый порт и нажмите на кнопку Install.
  1. Установите флажок рядом с установленным сервисом для его запуска.
  1. Повторите шаги 3-8 для запуска всех необходимых сервисов.

### С использованием утилиты Simple Service Hoster ###
  1. Откройте файл решения cloudobserver.sln в Microsoft Visual Studio 2008 из папки src\cloudobserver.
  1. Запустите отладку тестового проекта SimpleServiceHoster, выбрав пункт контекстного меню Debug -> Start new instance или непосредственно запустите исполняемый файл SimpleServiceHoster.exe (**внимание:** для корректной работы сервисов, взаимодействующих с базой данных, в папке с исполняемым файлом также должна находится библиотека CloudObserverDatabaseLibrary.dll).
  1. В диалоговом окне выбора файла выберите файл динамической библиотеки (.dll) нужного сервиса из папки lib.
  1. Введите номер порта для запуска сервиса и нажмите клавишу Enter.
  1. Повторите шаги 2-4 для запуска всех необходимых сервисов.

## Запуск веб-сайта ##
  1. Откройте файл решения cloudobserver.sln в Microsoft Visual Studio 2008 из папки src\cloudobserver.
  1. В окне Solution Explorer установите CloudObserverWebsite проектом по-умолчанию, выбрав в контекстном меню проекта пункт Set as StartUp Project.
  1. Убедитесь, что сервис авторизации установлен и работает.
  1. Запустите отладку, выбрав пункт меню Debug -> Start Debugging (или нажмите клавишу F5).