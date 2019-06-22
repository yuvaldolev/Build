#!/bin/bash

CommonFlags="-DDEBUG -g -Weverything -Wall -Werror -fdiagnostics-absolute-paths -std=c++11 -fno-rtti -fno-exceptions"
CommonFlags+=" -Wno-unsequenced -Wno-comment -Wno-unused-variable -Wno-unused-function -Wno-unused-result -Wno-switch -Wno-old-style-cast -Wno-zero-as-null-pointer-constant -Wno-string-conversion  -Wno-newline-eof -Wno-c++98-compat-pedantic -Wno-gnu-anonymous-struct -Wno-nested-anon-types -Wno-unused-parameter -Wno-padded -Wno-missing-prototypes -Wno-cast-align -Wno-sign-conversion -Wno-switch-enum -Wno-double-promotion -Wno-gnu-zero-variadic-macro-arguments -Wno-missing-noreturn -Wno-class-varargs -Wno-deprecated-declarations"
CommonFlags+=" -DMETA_TOOL_INTERNAL=1 -DMETA_TOOL_SLOW=1"

# NOTE(yuval): Setup compiler
if [ -n "$(command -v clang++)" ]
then
  CXX=clang++
  CommonFlags+=" -Wno-missing-braces -Wno-null-dereference -Wno-self-assign"
else
  CXX=c++
  CommonFlags+=" -Wno-unused-but-set-variable"
fi

# TODO(yuval): Only darwin is supported for now
PathFlags="-Wl,-rpath,@loader_path"

mkdir -p "../build"
pushd "../build" > /dev/null

echo "Compiling Using: $CXX"

# Mac Build
CommonFlags+=" -DMETA_TOOL_MAC=1"
MacFlags="-framework Cocoa" #-framework OpenGL -framework AudioToolbox -framework IOKit"
$CXX $CommonFlags ../code/mac_meta_tool.mm -o mac_meta_tool -ldl $MacFlags $PathFlags
popd > /dev/null

# Tests
RunTests=0
if [ $RunTests -eq 1 ]
then
pushd "../test" > /dev/null
echo
echo "Running MetaTool Tests:"
../build/mac_meta_tool
popd > /dev/null
fi

