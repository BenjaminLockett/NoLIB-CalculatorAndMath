@echo off
setlocal
cd /d "%~dp0"
if not exist "bin\x64\Release\NoLibMathTests.exe" (
    echo Build NoLibMath.sln with x64 Release first.
    pause
    exit /b 1
)
"bin\x64\Release\NoLibMathTests.exe"
set "test_result=%errorlevel%"
pause
exit /b %test_result%
