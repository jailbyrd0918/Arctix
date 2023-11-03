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

rem --- LOCATE VULKAN LATEST SDK INSTALLER EXECUTABLE AND SPECIFY INSTALLATION DIRECTORY ---
set vulkan_url=https://sdk.lunarg.com/sdk/download/latest/windows/vulkan-sdk.exe
set curr_dir=%~dp0
set dest_dir=%curr_dir%..\Engine\External\Vulkan
set sdk_installer=%dest_dir%\..\vulkan_installer.exe

rem --- CREATE VULKAN SDK DIRECTORY (IF NOT EXISTS) ---
if not exist %dest_dir% mkdir %dest_dir%

rem --- DOWNLOAD AND FORCE EXTRACT VULKAN EXECUTABLE ---
curl -L %vulkan_url% > %sdk_installer%
echo Vulkan SDK Installer download completed.

rem --- RUN THE INSTALLER  ---
rem --- Additionally, install Volk loader
start /b /wait "" %sdk_installer% --root %dest_dir% --accept-licenses --default-answer --confirm-command install com.lunarg.vulkan.sdl2 com.lunarg.vulkan.volk
echo Vulkan SDK installation completed.

setx /m VK_SDK_PATH %dest_dir%
setx /m VK_INSTANCE_LAYERS VK_LAYER_KHRONOS_validation
setx /m VK_LAYER_PATH %dest_dir%\Bin
echo SDK console variables configuration completed.

rem --- CLEANUP ---
del %sdk_installer%
echo Cleanup completed.

echo Vulkan SDK installation completed.
pause




