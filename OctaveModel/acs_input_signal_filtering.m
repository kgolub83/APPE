clear all;
pkg load signal;

inputData = csvread("acs_test_data.csv");

sps = inputData(1,1);                 #input signal sample rate 
ns = inputData(1,2);                  #get number of samples
verticalResolution = inputData(1,3);  #get vertical resolution
guard = inputData(1,4);               #get guard region
decoderRange = inputData(1,5);        

time = ns/sps;                        #input signal time period in seconds
              
########################## User filter settings  ############################### 
cutof_freq = 30;                      #low pass filter frequency
move_avg_magic_x = 0.196202;          #cutof frequency calculation factor
iir_tune_factor = 0.98;               #time domain tune factor

#move average filter parameters
average_window = sqrt(1 + sps*sps*move_avg_magic_x/(cutof_freq*cutof_freq)); #number of samples for move average filter
average_window = floor(average_window);
move_average_time = average_window/sps;

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
x_avg_filt(1:average_window) = recoursive_filtered(1:average_window);     %iir2_filtered(1:average_window);

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
fft_iir_avg = abs(fft(iir2_filtered));   %negative frequencyes to positive 
fft_iir_avg = fft_iir_avg(1:ns/2);          %discard freqs beyound fs/2

%move average wave spectrum
fft_move_avg = abs(fft(x_avg_response));  %negative frequencyes to positive 
fft_move_avg = fft_move_avg(1:ns/2);      %discard freqs beyound fs/2

f_axis(1) = 0.0001;       %nonzeros for log scale

######################### plot results ################################
close all;
figure(1, 'position', [0,50,1900,900]);
subplot(2,1,1)
hold on;
  plot(t, signal); 
  plot(t, x_avg_filt); 
  plot(t, recoursive_filtered); 
  legend("input", "moving average   ", "recoursive");
  title("Time domain response");
  xlabel("time [ms]");
  ylabel("amplitude");
hold off;

subplot(2,1,2)
hold on;
  semilogx(f_axis, fft_move_avg, ";move avg;", f_axis, fft_iir_avg, ";recoursive average   ;");
  axis([1,1000]);
  title("Frequency response");
  xlabel("frequency [Hz]");
  ylabel("amplitude");  
hold off;

print("Figs/filterResponse.svg", "-dsvg", "-S1600, 900");