#! /usr/bin/python
# -*- coding: utf-8 -*-

import os

if os.path.exists("build/Maze"):
    cmd = "valgrind --leak-check=full build/Maze"
    os.system(cmd)
else:
    print("No executable found to analyse.\n")
