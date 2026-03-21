@echo off
setlocal

for %%I in ("%~dp0.") do set ROOT=%%~fI
set BUILD_DIR=%ROOT%\build

cmake -S "%ROOT%" -B "%BUILD_DIR%" -G "Visual Studio 18 2026" -A x64
if errorlevel 1 exit /b %errorlevel%

cmake --build "%BUILD_DIR%" --config Release
exit /b %errorlevel%
