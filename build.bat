@echo off
set BUILD_PATH=%~dp0build
set INSTALL_PATH=%~dp0install
set PKG_CONFIG_PATH=%INSTALL_PATH%\lib\pkgconfig

:: install all libraries
set ARG_BUILD_CONFIG=%~1
IF "%ARG_BUILD_CONFIG%" == "" (set ARG_BUILD_CONFIG=Debug)

cmake -G "Visual Studio 16 2019" -DFETCHCONTENT_QUIET=OFF -DCMAKE_INSTALL_PREFIX=%INSTALL_PATH% -DCMAKE_BUILD_TYPE=%ARG_BUILD_CONFIG% -S %~dp0 -B %BUILD_PATH%
cmake --build %BUILD_PATH% --target INSTALL --config %ARG_BUILD_CONFIG%
