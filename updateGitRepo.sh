#!/bin/bash

#clean binary output files
make clean
rm /home/fsuser/CProjects/ACS/OctaveModel/Figs/*

#copy project to git repository
echo "Update GIT repository..."
cp -Rp ./*  /home/fsuser/GitRepo/GLProjects/APPE 