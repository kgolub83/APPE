#!/bin/bash

echo "ACS Test data generator..."

#run octave script
cd OctaveModel
octave acs_test_data_generator.m

#copy data to destination
cp -p acs_decoder.c ../Framework
cp -p acs_decoder.h ../Framework

#plot data

eog --fullscreen Figs/input.svg

cd ..
