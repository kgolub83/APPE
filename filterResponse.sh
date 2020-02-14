#!/bin/bash

echo "Plot ACS filter frequency response..."

#run octave script
cd OctaveModel/
octave acs_input_signal_filtering.m

#plot data
eog --fullscreen Figs/filterResponse.svg
