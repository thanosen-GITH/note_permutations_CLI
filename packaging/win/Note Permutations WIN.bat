@echo off
setlocal

set "SCRIPT_DIR=%~dp0"
set "EXE=%SCRIPT_DIR%build-win\note_permutations.exe"

if not exist "%EXE%" (
    echo Error: could not find executable:
    echo %EXE%
    echo.
    pause
    exit /b 1
)

cd /d "%SCRIPT_DIR%"
"%EXE%"

endlocal