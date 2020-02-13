clear all;
pkg load signal;

time = 3;                 #input signal time period in seconds
nominal_time = 0.5;       #nominal input signal time in seconds 
steady_state = 1;         #input signal steady state in seconcds
sps = 1000;               #sample rate 
start_value = 800;        #generated signal start value
nominal_value = 1023;     #starting nlominal value
signal_slope = -40;       #slope precent per seconds 
cutof_freq = 15;          #move average time window
noise_gain = 10;          #noise in precent
move_avg_magic_x = 0.196202;

ns = time*sps;                          #number of samples in generated signal
average_window = sqrt(1 + sps*sps*move_avg_magic_x/(cutof_freq*cutof_freq)); # number of samples for move average filter
average_window = floor(average_window);
move_average_time = average_window/sps;
ns_change = (time-steady_state-nominal_time)*sps;    #signal change number of samples
t = linspace(0,time,ns);                #timwe axis

nominal_samples = floor(nominal_time*sps);
signal_step = start_value*signal_slope/(100*sps);
signal(nominal_samples+1:ns_change+nominal_samples) = linspace(start_value, start_value+(ns_change*signal_step), ns_change);
signal (1:nominal_samples) = nominal_value;

signal(ns_change+average_window+1:ns) = start_value +((ns_change)*signal_step);

%noise generator
n_g = nominal_value*noise_gain/100;
noise = rand (1, ns);
noise = n_g.*(noise .- 0.5).*2;

signal = signal + noise;

######### signal filtering ###########

#recoursive filter
avg_win = average_window/pi();
average_acumulator = nominal_value*avg_win; 
old_average = nominal_value;
for i = 1:ns
  average_acumulator = average_acumulator-old_average+signal(i);
  filtered_signal(i) = average_acumulator/avg_win;
  old_average = filtered_signal(i);  
endfor

#move average
for i = 1:(ns-average_window)
  move_avg_sum = sum(signal(i:(i+average_window-1)));
  x_avg_filt(i+average_window) = move_avg_sum/average_window;
endfor
x_avg_filt(1:average_window) = nominal_value;

########### frequency response #############

f_axis = sps*(0:ns/2 - 1)/ns;      %calculate freq axis

%generate dirac impuls
dirac = zeros(1,ns);
dirac(average_window+1) = ones(1,1);

#recursive filter
average_acumulator = 0; 
old_average = 0;
for i = 1:ns
  average_acumulator = average_acumulator-old_average+dirac(i);
  modified_response(i) = average_acumulator/avg_win;
  old_average = modified_response(i);  
endfor

#recoursive average
iir2_coef = 1/avg_win;
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

%modified average wave spectrum
fft_modified = abs(fft(modified_response));   %negative frequencyes to positive 
fft_modified = fft_modified(1:ns/2);          %discard freqs beyound fs/2

%recoursive average wave spectrum
fft_iir_avg = abs(fft(iir2_filtered));   %negative frequencyes to positive 
fft_iir_avg = fft_iir_avg(1:ns/2);          %discard freqs beyound fs/2

%move average wave spectrum
fft_move_avg = abs(fft(x_avg_response));  %negative frequencyes to positive 
fft_move_avg = fft_move_avg(1:ns/2);      %discard freqs beyound fs/2

############# plot results ############
subplot(2,1,1)
plot(t, signal, ";input signal;", t, filtered_signal,";recoursive signal;", t, x_avg_filt, ";move average;");

subplot(2,1,2)
semilogx(f_axis, fft_modified, ";recoursive ;", f_axis, fft_move_avg, ";move avg;", f_axis, fft_iir_avg, ";recoursive average;");
axis([1,1000]);