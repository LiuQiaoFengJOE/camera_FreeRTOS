@echo off

@rem 確保當前文件夾是批處理所在文件夾
cd /d %~dp0

del %~dp0\lib\drv\usb_driver\*.c
del %~dp0\lib\drv\usb_driver\usb_dev\*.c
del %~dp0\lib\drv\usb_driver\usb_host\*.c
del %~dp0\lib\drv\usb_driver\usb_class\hub\*.c
del %~dp0\lib\drv\usb_driver\usb_class\uvc\*.c
del %~dp0\lib\drv\diskio\*.c
del %~dp0\lib\drv\mmc\core\*.c
del %~dp0\lib\drv\mmc\host\*.c
del %~dp0\net\ssv6158\host\platform\jianrong\*.c
del %~dp0\net\ssv6158\host\*.c
del %~dp0\net\ssv6158\host\os_wrapper\FreeRTOS\*.c
del %~dp0\lib\mediaFile\wav\*.c
del %~dp0\lib\fatfs\*.c
del %~dp0\lib\mediaFile\*.c
del %~dp0\lib\mediaFile\avi\*.c
del %~dp0\lib\mediaFile\mp3\src\*.c
del %~dp0\lib\rtsp\*.c
del %~dp0\lib\stream\*.c
del %~dp0\os\freeRTOS\taskMsg.c

del %~dp0\sdk\build\_build-appoplat2-sdk-6030*
del %~dp0\sdk\build\_build-appoplat2-sdk-6152*
del %~dp0\sdk\build\_build-appoplat-atbm*
del %~dp0\sdk\AX3292\AX3292_6011*
del %~dp0\sdk\AX3292\AX3292-lib.*
del %~dp0\sdk\AX3292\AX3292-lib14.*
del %~dp0\sdk\AX3292\AX3292-lib20.*
del %~dp0\sdk\AX3292\AX3292-os*
del %~dp0\sdk\AX3292\AX3292-rtl8189ftv*
del %~dp0\sdk\AX3292\AX3292-ssv6158*
del %~dp0\sdk\AX3292\AX3292-sv6030*
del %~dp0\sdk\AX3292\AX3292-sv6152*
del %~dp0\sdk\AX3292\AX3292-sys-sv6030*
del %~dp0\sdk\AX3292\AX3292-sys-sv6152*
del %~dp0\sdk\AX3292\AX3292-unzip*

del %~dp0\demo\helloftv\AX3292-hello-atbm*
del %~dp0\demo\helloftv\AX3292-hello-sv6030*
del %~dp0\demo\helloftv\AX3292-hello-sv6152*

rd net\atbm  /s /q
rd net\sv6030  /s /q
rd net\sv6152  /s /q
rd os\lwip\1.4.1-ssv  /s /q

rd demo\helloftv\Debug  /s /q
rd demo\helloftv\.codelite  /s /q
rd demo\helloftv\.clang  /s /q

rd sdk\AX3292\Debug  /s /q
rd sdk\build\.codelite  /s /q
rd sdk\build\.clang  /s /q
rd sdk\build\.build-debug  /s /q

pause
