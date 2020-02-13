#!/bin/bash

#clean binary output files
make clean

#copy project to git repository
echo "Update GIT repository..."
cp -Rp ./*  /home/fsuser/GitRepo/GLProjects/APPE 