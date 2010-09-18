@echo off
cls
echo.

rem In Vista, if install.bat is running elevated, its initial working
rem directory will not be where it resides, so set it to be so.
rem In earlier versions of Windows this is a null operation.
cd /d "%~dp0"

rem Check if the files have been uncompressed correctly
if not exist lib\*.* goto bad_unzip
if not exist include\*.* goto bad_unzip

rem Check if Visual Studio 2003/2005/2008/2010 is installed and set environment variables
set vstoolspath=
if not "%vs70comntools%" == "" set vstoolspath=%vs70comntools%
if not "%vs80comntools%" == "" set vstoolspath=%vs80comntools%
if not "%vs90comntools%" == "" set vstoolspath=%vs90comntools%
if not "%vs100comntools%" == "" set vstoolspath=%vs100comntools%
if not exist "%vstoolspath%vsvars32.bat" goto no_vs
call "%vstoolspath%vsvars32.bat"

rem Check if VC++ 2003/2005/2008/2010 is installed
if "%VCINSTALLDIR%" == "" goto no_vc

echo.
echo Copying libraries...
xcopy /s /y lib "%VCINSTALLDIR%" > NUL
if errorlevel 1 goto copy_error
echo Copying header files...
xcopy /s /y include "%VCINSTALLDIR%" > NUL
if errorlevel 1 goto copy_error
echo All files copied!
goto done

:copy_error
echo Error while copying the files. Make sure you have enough disk space available,
echo and that you have appropriate permissions. If you're using Windows Vista,
echo Windows 7 or newer with UAC turned on (which is the default setting), you need
echo to right-click on this file and select "Run as an administrator".
goto done

:bad_unzip
echo You haven't uncompressed the compressed file maintaining directory structure.
echo Installation will now stop.
echo Uncompress it again, maintaining directory structure.
goto done

:no_vs
echo Sorry, it appears that Visual Studio 2003/2005/2008/2010 is not properly installed on your system.
echo Installation will now stop.
echo Make sure that variable "vs70comntools" / "vs80comntools" / "vs90comntools" / "vs100comntools"
echo (VS2003/VS2005/VS2008) is properly set for your system.
goto done

:no_vc
echo Sorry, it appears that Visual C++ 2003/2005/2008/2010 is not properly installed on your system.
echo Installation will now stop.
echo make sure that variable "VCINSTALLDIR" is properly set in %vstoolspath%vsvars32.bat.
goto done

:done
echo.
rem Pause, in case the command window is set to dissappear after execution is complete
pause
