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

rem --- LOCATE SDL_IMAGE EXECUTABLE AND SPECIFY INSTALLATION DIRECTORY ---
set premake_url=https://github.com/libsdl-org/SDL_image/releases/download/release-2.6.3/SDL2_image-devel-2.6.3-VC.zip
set curr_dir=%~dp0
set dest_dir=%curr_dir%..\Engine\External

rem --- CREATE SDL_IMAGE 2.6.3 DIRECTORY (IF NOT EXISTS) ---
if not exist %dest_dir% mkdir %dest_dir%

rem --- DOWNLOAD AND FORCE EXTRACT SDL_IMAGE 2.6.3 ZIP ---
curl -L %premake_url% > %dest_dir%\SDL_Image.zip
echo SDL_Image ZIP file download completed.
powershell -command "Expand-Archive -Path '%dest_dir%\SDL_Image.zip' -DestinationPath '%dest_dir%' -Force"
echo Extraction completed.

rem --- REMOVE ZIP FILE ---
del %dest_dir%\SDL_Image.zip
echo SDL_Image ZIP file removal completed.

rem --- COPY DLL FILE TO ENGINE DIRECTORY ---
set copy_src=%curr_dir%..\Engine\External\SDL2_image-2.6.3\lib\x64

set copy_dst_debug=%curr_dir%..\bin\windows_x86_64_Debug\Sandbox
if not exist %copy_dst_debug% mkdir %copy_dst_debug%
copy %copy_src%\SDL2_image.dll %copy_dst_debug%
echo Copied SDL2_image.dll to %copy_dst_debug% 

set copy_dst_release=%curr_dir%..\bin\windows_x86_64_Release\Sandbox
if not exist %copy_dst_release% mkdir %copy_dst_release%
copy %copy_src%\SDL2_image.dll %copy_dst_release%
echo Copied SDL2_image.dll to %copy_dst_release%

echo SDL_Image installation completed.
pause
