#!/bin/bash

echo "ACS model output data analysis..."

#run octave script
cp Data/acs_output_data.csv OctaveModel/
cd OctaveModel/
octave acs_test_data_validation.m

#plot data
eog --fullscreen Figs/output.svg
cd ..