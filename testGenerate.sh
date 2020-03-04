#!/bin/bash
#********************************Copyright (c)********************************
#                                  GlobalLogic
#
# @author Kristijan Golub - kristijan.golub@globallogic.com
#
# @date 2019-12-20
# 
# @brief generate input test data (emulating ADC values from position sensor)
#		-generated data saved in a appe_test_data.csv file
#
# @version 0.1
#
# @section REVISION HISTORY
#  - KG 2019-12-20 Initial implementation 
#
#*****************************************************************************/

echo "APPE Test data generator..."

#run octave script
cd OctaveModel
octave appe_test_data_generator.m

#copy data to destination
cp -p appe_test_data.csv ../Data

#remove files
rm appe_decoder.c
rm appe_decoder.h

#plot data
eog --fullscreen Figs/input.svg

cd ..
