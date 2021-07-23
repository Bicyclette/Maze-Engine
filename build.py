#! /usr/bin/python3
# -*- coding: utf-8 -*-

import os
import sys

argLength = len(sys.argv)
debug = False
release = False
cmd = ""

if argLength == 2 :
    if sys.argv[1] == "debug":
        debug = True
    elif sys.argv[1] == "release" :
        release = True
    else:
        quit("wrong arg")
else:
    quit("no arg provided")

if debug :
    cmd = "cmake -S . -B build -DCMAKE_BUILD_TYPE:STRING=Debug"
elif release :
    cmd = "cmake -S . -B build -DCMAKE_BUILD_TYPE:STRING=Release"
    #cmd = "cmake -S . -B build -DCMAKE_BUILD_TYPE:STRING=Release -DSDL2_INCLUDE_DIRS:FILEPATH=/usr/include/SDL2 -DSDL2_LIB_DIRS:FILEPATH=/usr/lib/x86_64-linux-gnu -DSDL2_LIBS:STRING=\"/usr/lib/x86_64-linux-gnu/libSDL2main.a;/usr/lib/x86_64-linux-gnu/libSDL2.so;-lpthread\""

os.system(cmd)
