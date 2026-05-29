@echo off
set "DIR=%~dp0"
if exist "%DIR%..\dist\bin\note_permutations.exe" (
  "%DIR%..\dist\bin\note_permutations.exe"
) else if exist "%DIR%..\build-win\note_permutations.exe" (
  "%DIR%..\build-win\note_permutations.exe"
) else (
  echo Could not find note_permutations.exe
)
pause
