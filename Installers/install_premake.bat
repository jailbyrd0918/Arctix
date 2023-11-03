@echo off

:: BatchGotAdmin
:-------------------------------------
rem  --- CHECK FOR PERMISSIONS ---
if "%PROCESSOR_ARCHITECTURE%" EQU "amd64" (
	>nul 2>&1 "%SYSTEMROOT%\SysWOW64\cacls.exe" "%SYSTEMROOT%\SysWOW64\config\system"
) 
else (
	>nul 2>&1 "%SYSTEMROOT%\system32\cacls.exe" "%SYSTEMROOT%\system32\config\system"
)

rem --- IF ERROR FLAG SET, WE DO NOT HAVE ADMIN ---
if '%errorlevel%' NEQ '0' (
	echo Requesting administrative privileges...
	goto UACPrompt
) 
else ( 
	goto gotAdmin 
)

:UACPrompt
    echo Set UAC = CreateObject^("Shell.Application"^) > "%temp%\getadmin.vbs"
    echo UAC.ShellExecute "%~s0", "", "", "runas", 1 >> "%temp%\getadmin.vbs"
    "%temp%\getadmin.vbs"
    exit /B

:gotAdmin
    if exist "%temp%\getadmin.vbs" ( del "%temp%\getadmin.vbs" )
    
:-------------------------------------

rem --- LOCATE PREMAKE 5.0.0 (ALPHA) EXECUTABLE AND SPECIFY INSTALLATION DIRECTORY ---
set premake_url=https://github.com/premake/premake-core/releases/download/v5.0.0-alpha16/premake-5.0.0-alpha16-windows.zip
set curr_dir=%~dp0
set dest_dir=%curr_dir%..\External\premake

rem --- CREATE PREMAKE DIRECTORY (IF NOT EXISTS) ---
if not exist %dest_dir% mkdir %dest_dir%

rem --- DOWNLOAD AND FORCE EXTRACT PREMAKE EXECUTABLE ---
curl -L %premake_url% > %dest_dir%\premake.zip
echo Premake ZIP file download completed.
powershell -command "Expand-Archive -Path '%dest_dir%\premake.zip' -DestinationPath '%dest_dir%' -Force"
echo Extraction completed.

rem --- CLEANUP ---
del %dest_dir%\premake.zip
echo Cleanup completed.

echo Premake installation completed.
pause
