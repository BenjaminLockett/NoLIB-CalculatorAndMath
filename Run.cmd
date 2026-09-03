@echo off
setlocal
cd /d "%~dp0"
if exist "bin\x64\Release\NoLibMath.exe" (
    "bin\x64\Release\NoLibMath.exe" %*
    exit /b
)
if exist "bin\x64\Debug\NoLibMath.exe" (
    "bin\x64\Debug\NoLibMath.exe" %*
    exit /b
)
echo Open NoLibMath.sln in Visual Studio and build x64 Release first.
pause
