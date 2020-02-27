clear all;
pkg load signal;

inputData = csvread("appe_test_data.csv");

sps = inputData(1,1);                 #input signal sample rate 
ns = inputData(1,2);                  #get number of samples
verticalResolution = inputData(1,3);  #get vertical resolution
guard = inputData(1,4);               #get guard region
decoderRange = inputData(1,5);        

time = ns/sps;                        #input signal time period in seconds
              
########################## User filter settings ################################ 
cutof_freq = 15;                      #low pass filter frequency
move_avg_magic_x = 0.196202;          #cutof frequency calculation factor
iir_tune_factor = 1.1;                #time domain tune factor

#move average filter parameters
average_window = sqrt(1 + sps*sps*move_avg_magic_x/(cutof_freq*cutof_freq)); #number of samples for move average filter
average_window = floor(average_window);
move_average_time = average_window/sps;

moveAverageBuffer = linspace (guard, guard, average_window);

t = linspace(0,time,ns);         %time axis
signal = inputData (2,:);        %get test data

############################### signal filtering ###############################

#recoursive filter
avg_win = 2*average_window/pi();
#recoursive average
iir2_coef = iir_tune_factor*1/avg_win;
recoursive_filtered = guard;
iir2_filtered(1) = recoursive_filtered;

for i=2:(ns)
  recoursive_filtered(i) = iir2_coef*(signal(i) - recoursive_filtered(i-1)) + recoursive_filtered(i-1);
endfor 

#move average
for i = 1:(ns-average_window)
  move_avg_sum = sum(signal(i:(i+average_window-1)));
  x_avg_filt(i+average_window) = move_avg_sum/average_window;
endfor

for i = 1:average_window
  moveAverageBuffer = shift(moveAverageBuffer, -1);
  moveAverageBuffer(average_window) = signal(i);
  x_avg_filt(i+1) = sum(moveAverageBuffer)/average_window;
endfor

x_avg_filt(1) = guard;

#filtering error between FIR and IIR implementation

filtering_error = x_avg_filt - recoursive_filtered;

max(filtering_error)
min(filtering_error)

############################# frequency response ###############################

f_axis = sps*(0:ns/2 - 1)/ns;      %calculate freq axis

%generate dirac impuls
dirac = zeros(1,ns);
dirac(average_window+1) = ones(1,1);

#recoursive average
iir2_filtered(1) = 0;

for i=2:(ns)
  iir2_filtered(i) = iir2_coef*(dirac(i) - iir2_filtered(i-1)) + iir2_filtered(i-1);
endfor 

#clasical move average
for i = 1:(ns-average_window)
  move_avg_sum = sum(dirac(i:(i+average_window-1)));
  x_avg_response(i+average_window) = move_avg_sum/average_window;
endfor

x_avg_response(1:average_window) = 0;

%recoursive average wave spectrum
fft_result = fft(iir2_filtered);
fft_iir_avg = abs(fft_result);            %negative frequencyes to positive 
fft_iir_avg = fft_iir_avg(1:ns/2);        %discard freqs beyound fs/2

%recoursive phase response
iir_avg_phase = atan2(imag(fft_result), real(fft_result)); %calculate phase
#iir_avg_phase = unwrap(iir_avg_phase);
iir_avg_phase = iir_avg_phase(1:ns/2)*180/pi;

%move average wave spectrum
fft_result = fft(x_avg_response);
fft_move_avg = abs(fft_result);           %negative frequencyes to positive 
fft_move_avg = fft_move_avg(1:ns/2);      %discard freqs beyound fs/2

move_avg_phase = atan2(imag(fft_result), real(fft_result)); %calculate phase
#move_avg_phase = unwrap(move_avg_phase);
move_avg_phase = move_avg_phase(1:ns/2)*180/pi;

f_axis(1) = 0.1;       %nonzeros for log scale

################## integrate impulse response #########################

sum(x_avg_response)
sum(iir2_filtered)

######################### plot results ################################
close all;
figure(1, 'position', [0,50,1900,900]);
subplot(2,2,1)
hold on;
  plot(t, signal); 
  plot(t, x_avg_filt); 
  plot(t, recoursive_filtered); 
  legend("input", "moving average   ", "recoursive average");
  title("Time domain response");
  xlabel("time [ms]");
  ylabel("amplitude");
hold off;

subplot(2,2,3)
hold on;
  semilogx(f_axis, fft_move_avg); 
  semilogx(f_axis, fft_iir_avg);
  axis([0.1,1000]);
  legend("moving average", "recoursive average");
  title("Frequency response");
  xlabel("frequency [Hz]");
  ylabel("amplitude");  
hold off;

subplot(2,2,2)
hold on;
  plot(t, x_avg_response);
  plot(t, iir2_filtered);
  legend("moving average", "recoursive average");  
  title("Impulse response");
  xlabel("time [ms]");
  ylabel("amplitude");  
hold off;

subplot(2,2,4)
hold on;
  semilogx(f_axis, move_avg_phase);
  semilogx(f_axis, iir_avg_phase);
  axis([0.1, 1000]);
  legend("moving average", "recoursive average");    
  title("Phase response");
  xlabel("frequency [Hz]");
  ylabel("angle [deg]");  
hold off;

print("Figs/filterResponse.svg", "-dsvg", "-S1600, 900");