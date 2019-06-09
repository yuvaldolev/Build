@echo off

IF NOT EXIST build mkdir build

pushd build
cl -nologo -Zi -FC ..\test.cpp user32.lib
popd

