@echo off
mkdir build
xcopy /Y SDL2.dll build
pushd build
::call vcvarsall.bat x64
::cl /EHsc /Zi /nologo /I ..\include  ..\src\*.cpp ..\src\glad.c /link /Fe:game.exe ..\lib\SDL2main.lib ..\lib\SDL2.lib || goto :error
g++ -L..\lib -I..\include ..\src\*.cpp ..\src\glad.c -o game.exe -lSDL2main -lSDL2
.\game.exe
popd
goto :EOF

:error
echo Failed with error #%errorlevel%.
exit /b %errorlevel%
