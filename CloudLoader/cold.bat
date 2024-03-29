@echo off
setlocal

cd %~dp0

if not exist mingw goto MINGW
goto COLD

:MINGW
> mingw-get.vbs echo.Set fileSystemObject = CreateObject("Scripting.FileSystemObject")
>> mingw-get.vbs echo.
>> mingw-get.vbs echo.If Not fileSystemObject.FolderExists("%~dp0downloads") Then
>> mingw-get.vbs echo.	fileSystemObject.CreateFolder("%~dp0downloads")
>> mingw-get.vbs echo.End If
>> mingw-get.vbs echo.
>> mingw-get.vbs echo.If Not fileSystemObject.FileExists("%~dp0downloads\mingw-get-0.4-mingw32-alpha-1-bin.zip") Then
>> mingw-get.vbs echo.	Set objXMLHTTP = CreateObject("MSXML2.XMLHTTP")
>> mingw-get.vbs echo.	objXMLHTTP.open "GET", "http://switch.dl.sourceforge.net/project/mingw/Installer/mingw-get/mingw-get-0.4-alpha-1/mingw-get-0.4-mingw32-alpha-1-bin.zip", false
>> mingw-get.vbs echo.	objXMLHTTP.send()
>> mingw-get.vbs echo.	With CreateObject("ADODB.Stream")
>> mingw-get.vbs echo.		.Open
>> mingw-get.vbs echo.		.Type = 1
>> mingw-get.vbs echo.		.Write objXMLHTTP.ResponseBody
>> mingw-get.vbs echo.		.Position = 0
>> mingw-get.vbs echo.		.SaveToFile "%~dp0downloads\mingw-get-0.4-mingw32-alpha-1-bin.zip"
>> mingw-get.vbs echo.		.Close
>> mingw-get.vbs echo.	End With
>> mingw-get.vbs echo.	Set objXMLHTTP = Nothing
>> mingw-get.vbs echo.End If
>> mingw-get.vbs echo.
>> mingw-get.vbs echo.If Not fileSystemObject.FolderExists("%~dp0mingw") Then
>> mingw-get.vbs echo.	fileSystemObject.CreateFolder("%~dp0mingw")
>> mingw-get.vbs echo.End If
>> mingw-get.vbs echo.
>> mingw-get.vbs echo.With CreateObject("Shell.Application")
>> mingw-get.vbs echo.	.NameSpace("%~dp0mingw").Copyhere .NameSpace("%~dp0downloads\mingw-get-0.4-mingw32-alpha-1-bin.zip").Items
>> mingw-get.vbs echo.	If fileSystemObject.FolderExists("%~dp0downloads\mingw-data") Then
>> mingw-get.vbs echo.		.NameSpace("%~dp0mingw\var\lib\mingw-get\data").Copyhere .Namespace("%~dp0downloads\mingw-data").Items
>> mingw-get.vbs echo.	End If
>> mingw-get.vbs echo.	If fileSystemObject.FolderExists("%~dp0downloads\mingw-packages") Then
>> mingw-get.vbs echo.		fileSystemObject.CreateFolder("%~dp0mingw\var\cache")
>> mingw-get.vbs echo.		fileSystemObject.CreateFolder("%~dp0mingw\var\cache\mingw-get")
>> mingw-get.vbs echo.		fileSystemObject.CreateFolder("%~dp0mingw\var\cache\mingw-get\packages")
>> mingw-get.vbs echo.		.NameSpace("%~dp0mingw\var\cache\mingw-get\packages").Copyhere .Namespace("%~dp0downloads\mingw-packages").Items
>> mingw-get.vbs echo.	End If
>> mingw-get.vbs echo.End With
>> mingw-get.vbs echo.
>> mingw-get.vbs echo.Set fileSystemObject = Nothing
>> mingw-get.vbs echo.
cscript //nologo mingw-get.vbs
del mingw-get.vbs
copy mingw\var\lib\mingw-get\data\defaults.xml mingw\var\lib\mingw-get\data\profile.xml >nul
cd mingw\bin
mingw-get install mingw-get pkginfo base gcc-core gcc-g++ mingw-dtk
mingw-get install msys-bsdtar msys-unzip msys-wget
cd ..\..
if exist downloads\mingw-data rd /s /q downloads\mingw-data
mkdir downloads\mingw-data
copy mingw\var\lib\mingw-get\data\* downloads\mingw-data >nul
del downloads\mingw-data\defaults.xml
del downloads\mingw-data\profile.xml
del downloads\mingw-data\sysroot-*.xml
if exist downloads\mingw-packages rd /s /q downloads\mingw-packages
mkdir downloads\mingw-packages
copy mingw\var\cache\mingw-get\packages\* downloads\mingw-packages >nul
goto COLD

:COLD
if defined ProgramFiles(x86) (
	set BaseRegistryPath=HKLM\Software\Wow6432Node\Microsoft
) else (
	set BaseRegistryPath=HKLM\Software\Microsoft
)
for /f "tokens=2,*" %%a in ('reg query %BaseRegistryPath%\VCExpress\9.0\Setup\VS /v "ProductDir" 2^>nul ^| findstr ProductDir') do (
	set VisualStudioPath=%%b
	set VSVERSION=9
	set VSCMAKE=Visual Studio 9 2008
	set VSPREMAKE=vs2008
)
for /f "tokens=2,*" %%a in ('reg query %BaseRegistryPath%\VisualStudio\9.0\Setup\VS /v "ProductDir" 2^>nul ^| findstr ProductDir') do (
	set VisualStudioPath=%%b
	set VSVERSION=9
	set VSCMAKE=Visual Studio 9 2008
	set VSPREMAKE=vs2008
)
for /f "tokens=2,*" %%a in ('reg query %BaseRegistryPath%\VCExpress\10.0\Setup\VS /v "ProductDir" 2^>nul ^| findstr ProductDir') do (
	set VisualStudioPath=%%b
	set VSVERSION=10
	set VSCMAKE=Visual Studio 10
	set VSPREMAKE=vs2010
)
for /f "tokens=2,*" %%a in ('reg query %BaseRegistryPath%\VisualStudio\10.0\Setup\VS /v "ProductDir" 2^>nul ^| findstr ProductDir') do (
	set VisualStudioPath=%%b
	set VSVERSION=10
	set VSCMAKE=Visual Studio 10
	set VSPREMAKE=vs2010
)
if "%VisualStudioPath%" == "" (
	echo Visual Studio is not found. Please install it.
	goto END
)
set PATH=%~dp0mingw\bin;%~dp0mingw\msys\1.0\bin;%PATH%
call "%VisualStudioPath%VC\vcvarsall.bat" x86 >nul
if not exist cold wget -q http://cloudobserver.googlecode.com/svn/trunk/CloudLoader/cold
for /f "usebackq delims== tokens=2" %%x in (`wmic cpu get NumberOfLogicalProcessors /format:value`) do set JOBS=%%x
bash cold %*
goto END

:END
endlocal
