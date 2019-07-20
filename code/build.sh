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
CommonFlags+=" -DBUILD_MACOS=1"
MacFlags="-framework Cocoa -framework IOKit -framework Security" #-framework OpenGL -framework AudioToolbox"
CrashpadIncludeFlags="-I../crashpad/crashpad -I../crashpad/crashpad/third_party/mini_chromium/mini_chromium" CrashpadLinkerFlags="-L../crashpad/crashpad/out/Default -lclient -lhandler -lutil -lbase -lbsm"
$CXX $CommonFlags $CrashpadIncludeFlags ../code/mac_build.mm -o mac_build -ldl $MacFlags $CrashpadLinkerFlags $PathFlags

# Getting the compilation exit code
CompilationExitCode=$?

# Checking if the compilation was successful
if [ $CompilationExitCode -eq 0 ]
then
  # Crashpad Debug Symbols Upload
  echo
  zip -r mac_build.dSYM.zip mac_build.dSYM
  morgue put Build mac_build.dSYM.zip --format=symbols

  # Tests
  RunTests=0
  if [ $RunTests -eq 1 ]
  then
    pushd "../test" > /dev/null
    echo
    echo "Running Build Tests:"
    ../build/mac_build
    popd > /dev/null
  fi
fi

popd > /dev/null
