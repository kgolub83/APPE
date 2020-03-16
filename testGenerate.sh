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
# @args -e -> execute only dont plot data
#
# @version 0.1
#
# @section REVISION HISTORY
#  - KG 2019-12-20 Initial implementation 
#
#*****************************************************************************/

echo "APPE Test data generator..."

#run octave script
mkdir -p Data
cd OctaveModel
mkdir -p Figs
octave appe_test_data_generator.m

#copy data to destination
cp -p appe_test_data.csv ../Data

#remove files
rm appe_decoder.c
rm appe_decoder.h

#plot data
if [ $# -eq 1 ]
then
	if [ $1 = '-e' ]
	then
		echo "Test data execute only..." 
	   	cd ..	
	else
		echo "Invalid argument $1"
	fi
else
	eog --fullscreen Figs/input.svg
	cd ..	
fi
