#!/bin/bash

echo "GClient Setup"
gclient root
gclient config --spec 'solutions = [
  {
    "url": "https://chromium.googlesource.com/crashpad/crashpad.git",
    "managed": False,
    "name": "crashpad",
  },
]'

echo
echo "Cloning Crashpad Git Repository"
git clone https://github.com/yuvaldolev/crashpad.git

echo
echo "GClient Sync"
gclient sync

echo
echo "Building Crashpad"
cd crashpad
gn gen out/Default
ninja -C out/Default


echo
echo "Crashpad Libraries Fix"
cp out/Default/obj/client/libclient.a out/Default
cp out/Default/obj/handler/libhandler.a out/Default
cp out/Default/obj/util/libutil.a out/Default
cp out/Default/obj/third_party/mini_chromium/mini_chromium/base/libbase.a out/Default

echo
echo "Crashpad Database Configuration"
cd ..
./crashpad/out/Default/crashpad_database_util -d meta_crashpad_db --create
./crashpad/out/Default/crashpad_database_util -d meta_crashpad_db --set-uploads-enabled true
