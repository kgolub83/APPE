#!/bin/bash
#********************************Copyright (c)********************************
#                                  GlobalLogic
#
# @author Kristijan Golub - kristijan.golub@globallogic.com
#
# @date 2020-02-12
# 
# @brief plot filter data 
#
# @version 0.1
#
# @section REVISION HISTORY
#  - KG 2020-02-12 Initial implementation 
#
#*****************************************************************************/

echo "Plot APPE filter frequency response..."

#run octave script
cd OctaveModel/
octave appe_input_signal_filtering.m

#plot data
eog --fullscreen Figs/filterResponse.svg
