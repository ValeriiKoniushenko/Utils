@echo off

rem Check and update git submodules
set count=0
for %%x in (dependencies/*) do set /a count+=1
if %ERRORLEVEL% NEQ 0 echo "[31mError:[0m Was met some error while trying to calculate dependecies" && exit 1

if %count%==0 (
    git submodule update --init --force --remote
    if %ERRORLEVEL% NEQ 0 echo "[31mError:[0m Was met some error while trying to updating git submodules\" && exit 1
)

where python
if %ERRORLEVEL% NEQ 0 echo "[31mError:[0m python wasn't found. Install it(https://www.python.org/downloads/) and try again." && exit 1

where pip3
if %ERRORLEVEL% NEQ 0 echo "[31mError:[0m pip3 wasn't found. Install it(https://pypi.org/project/pip/) and try again." && exit 1

for /F "delims=" %%i in ('pip3 list ^| findstr packaging') do set "isExistsPipPackaging=%%i"
if "%isExistsPipPackaging%"=="" ( 
    echo "[31mError:[0m pip3's package: 'packaging' wasn't found. Install it(https://pypi.org/project/packaging/) and try again."
    exit 1
)

where cppcheck
if %ERRORLEVEL% NEQ 0 echo "[31mError:[0m cppcheck wasn't found. Install it(https://sourceforge.net/projects/cppcheck/#download) and try again." && exit 1

where cmake
if %ERRORLEVEL% NEQ 0 echo "[31mError:[0m cmake wasn't found. Install it(https://cmake.org/download/) and try again." && exit 1

where git
if %ERRORLEVEL% NEQ 0 echo "[31mError:[0m git wasn't found. Install it(https://git-scm.com/downloads) and try again."  && exit 1

where curl
if %ERRORLEVEL% NEQ 0 echo "[31mError:[0m curl wasn't found. Install it(https://curl.se/download.html) and try again." && exit 1

git config --global --add safe.directory %~dp0

python ./scripts/check.py
if NOT "%ERRORLEVEL%"=="0" (
    echo [31mError:[0m Was met some errors while executing of scripts/check.py. Read errors, fix errors and relaunch.
    exit 1
)


echo [32mInstall is success![0m