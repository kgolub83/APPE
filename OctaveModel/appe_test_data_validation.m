#********************************Copyright (c)*********************************
#                                GlobalLogic
#
# @author Kristijan Golub - kristijan.golub@globallogic.com
#
# @date 2020-02-25
# 
# @brief  script validates processed data
#
# @version 0.1
#
# @section REVISION HISTORY
#  - KG 2020-02-25 Initial implementation 
#
#******************************************************************************/

clear all; 
inputVector = csvread("appe_test_data.csv");
resultVector = csvread("appe_output_data.csv");

vectorsNo = rows(resultVector);

#set test data properties
sampleRate = inputVector(1,1);       %data sample rate
ns = inputVector(1,2);               %number of data samples 
resolution = inputVector(1,3);       %vertical resolution
guard = inputVector(1,4);            %absolute value of guard regions
decoder_range = inputVector(1,5);    %absolute output decoder range
data_vectors = inputVector(1,6);     %number of data vector in file

#input vectors identification
inputDataA = 1;
inputDataB = 2;
outputData = 3;
outputState = 4;
systemFlags = 5;
filteredFIR_A = 6;
filteredFIR_B = 7;
filteredIIR_A = 8;
filteredIIR_B = 9;

line_width = 2;       %set line width

#calculate time and construct time axis
time = 1000*(ns-1)/sampleRate;        %calculate time in miliseconds
timeAxis = linspace(0, time, ns); 

close all;

figure(1, 'position', [0,50,1900,900]);

#plot input data
subplot(2,2,1)
hold on;
  plot(timeAxis, resultVector(inputDataA,:), "linewidth", line_width);
  plot(timeAxis, resultVector(inputDataB,:), "linewidth", line_width);
  title("Input data");
  xlabel("time [ms]");
  ylabel("amplitude");
  legend("Input A  ", "Input B  ");
hold off;

#plot filtered data
subplot(2,2,3)
hold on;
  plot(timeAxis, resultVector(filteredFIR_A,:), "linewidth", line_width);
  plot(timeAxis, resultVector(filteredFIR_B,:), "linewidth", line_width);
  plot(timeAxis, resultVector(filteredIIR_A,:), "linewidth", line_width);
  plot(timeAxis, resultVector(filteredIIR_B,:), "linewidth", line_width);
  plot(timeAxis, resultVector(outputData, :), "linewidth", line_width);
  title("Processed data");
  xlabel("time [ms]");
  ylabel("amplitude");
  legend("FIR A", "FIR B", "IIR A", "IIR B", "Output  ");
hold off;

#plot system flags
subplot(2,2,2)
hold on;
  plot(timeAxis, resultVector(systemFlags,:),"linewidth", line_width);
  title("Output state");
  xlabel("time [ms]");
  ylabel("amplitude");
hold off;

#plot resulting system state
figure(1, 'position', [0,50,1900,900]);
subplot(2,2,4)
hold on;
  plot(timeAxis, resultVector(outputState,:), "linewidth", line_width);
  title("System flags");
  xlabel("time [ms]");
  ylabel("amplitude");
hold off;

print("Figs/output.svg", "-dsvg", "-S1600, 900");