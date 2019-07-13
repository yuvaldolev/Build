#!/bin/bash

CommonFlags="-g -Weverything -Wall -Werror -fdiagnostics-absolute-paths -std=c++11 -fno-rtti -fno-exceptions"
CommonFlags+=" -Wno-unsequenced -Wno-comment -Wno-unused-variable -Wno-unused-function -Wno-unused-result -Wno-switch -Wno-old-style-cast -Wno-zero-as-null-pointer-constant -Wno-string-conversion  -Wno-newline-eof -Wno-c++98-compat-pedantic -Wno-gnu-anonymous-struct -Wno-nested-anon-types -Wno-unused-parameter -Wno-padded -Wno-missing-prototypes -Wno-cast-align -Wno-sign-conversion -Wno-switch-enum -Wno-double-promotion -Wno-gnu-zero-variadic-macro-arguments -Wno-missing-noreturn -Wno-class-varargs -Wno-deprecated-declarations -Wno-documentation-unknown-command -Wno-weak-vtables -Wno-cast-qual"
CommonFlags+=" -DBUILD_INTERNAL=1 -DBUILD_SLOW=1"

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
CommonFlags+=" -DBUILD_MAC=1 -DBUILD_TRAVIS=1"
MacFlags="-framework Cocoa -framework IOKit -framework Security" #-framework OpenGL -framework AudioToolbox"
$CXX $CommonFlags ../code/mac_build.mm -o mac_build -ldl $MacFlags $PathFlags
