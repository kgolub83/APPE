#********************************Copyright (c)*********************************
#                                GlobalLogic
#
# @author Kristijan Golub - kristijan.golub@globallogic.com
#
# @date 2020-02-25
# 
# @brief  filter response in frequency and time domain
#
# @version 0.1
#
# @section REVISION HISTORY
#  - KG 2020-02-25 Initial implementation 
#
#******************************************************************************/

clear all;

ns = 1024;      %number of samples
fs = 48000;     %samplig frequency in Hz
f_span = 20000;  %frequency span for filter test
f_step = 1000;

period = 1/fs;
duration = period*ns;
time = linspace(0, duration, ns);

%test biquad filter
wave = zeros(1,ns);
wave(1) = ones(1,1);
%{
for i = 1:f_span
  wave = wave + (sin(time*2*pi*i));
  i = i + f_step;
endfor
%}

%wave spectrum
fft_synth = abs(fft(wave));  %negative frequencyes to positive 
fft_synth = fft_synth(1:ns/2);    %discard freqs beyound fs/2
f_axis = fs*(0:ns/2 - 1)/ns;      %calculate freq axis

%----------------Biquad Filter-------------------

%BAND PASS

a0 = 0.5883841179682276;
a1 = -1.1674890669124613;
a2 = 0.5883841179682276;
b1 = -1.9413458309760059;
b2 = 0.9506249999999999;


z1 = 0;
z2 = 0;

for i = 1:ns
  x = wave(i);
  y = x*a0 + z1;
  z1 =x*a1 + z2 - b1* y;
  z2 = x * a2-b2 * y;
  biquad_filt(i) = y;
endfor

%LOW PASS
c0 = 0.0010907885845168783;
c1 = 0.0021815771690337567;
c2 = 0.0010907885845168783;
d1 = -1.9543654156704593;
d2 = 0.958728570008527;

z1 = 0;
z2 = 0;

for i = 1:ns
  x = biquad_filt(i);
  y = x*c0 + z1;
  z1 =x*c1 + z2 - d1* y;
  z2 = x * c2-d2 * y;
  biquad_filt(i) = y;
endfor

%filtered wave spectrum
fft_filt = abs(fft(biquad_filt));  %negative frequencyes to positive 
fft_filt = fft_filt(1:ns/2);    %discard freqs beyound fs/2

subplot(2,2,1)
plot(time, wave);

subplot(2,2,2)
plot(f_axis, fft_synth);
semilogy(f_axis, fft_synth);    %plot spectrum of the synthesised wave
axis([1,2000]);                %frequency axis range

subplot(2,2,3)
plot(time, biquad_filt);


subplot(2,2,4)
plot(f_axis, fft_filt);
%semilogy(f_axis, fft_filt);    %plot spectrum of the synthesised wave
axis([1,2000]);                %frequency axis range