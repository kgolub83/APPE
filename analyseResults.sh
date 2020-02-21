#!/bin/bash

echo "APPE model output data analysis..."

#run octave script
cp Data/appe_output_data.csv OctaveModel/
cd OctaveModel/
octave appe_test_data_validation.m

#plot data
eog --fullscreen Figs/output.svg
cd ..