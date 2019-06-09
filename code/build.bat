@echo off

IF NOT EXIST ..\build mkdir ..\build

pushd ..\build
cl -nologo -MT -GR- -EHa- -Oi -WX -W4 -wd4201 -wd4100 -wd4189 -wd4505 -Z7 -FC -Fmwin32_meta_tool.map ..\code\win32_meta_tool.cpp user32.lib
popd

