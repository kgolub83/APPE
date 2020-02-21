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

#calculate time and construct time axis
time = 1000*(ns-1)/sampleRate;        %calculate time in miliseconds
timeAxis = linspace(0, time, ns); 

close all;

figure(1, 'position', [0,50,1900,900]);

#plot input data
subplot(2,2,1)
hold on;
  plot(timeAxis, resultVector(inputDataA,:));
  plot(timeAxis, resultVector(inputDataB,:));
  title("Input data");
  xlabel("time [ms]");
  ylabel("amplitude");
  legend("Input A  ", "Input B  ");
hold off;

#plot filtered data
subplot(2,2,3)
hold on;
  plot(timeAxis, resultVector(filteredFIR_A,:));
  plot(timeAxis, resultVector(filteredFIR_B,:));
  plot(timeAxis, resultVector(filteredIIR_A,:));
  plot(timeAxis, resultVector(filteredIIR_B,:));
  plot(timeAxis, resultVector(outputData, :));
  title("Processed data");
  xlabel("time [ms]");
  ylabel("amplitude");
  legend("FIR A", "FIR B", "IIR A", "IIR B", "Output  ");
hold off;

#plot system flags
subplot(2,2,2)
hold on;
  plot(timeAxis, resultVector(systemFlags,:));
  title("Output state");
  xlabel("time [ms]");
  ylabel("amplitude");
hold off;

#plot resulting system state
figure(1, 'position', [0,50,1900,900]);
subplot(2,2,4)
hold on;
  plot(timeAxis, resultVector(outputState,:));
  title("System flags");
  xlabel("time [ms]");
  ylabel("amplitude");
hold off;

print("Figs/output.svg", "-dsvg", "-S1600, 900");