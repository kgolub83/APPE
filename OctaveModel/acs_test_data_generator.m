# data generator for functional safety design example of
# hypotetical accelerator control system (ACS) 

clear all;

################################## USER INPUT ##################################
time = 1000;              %data time span: [ms]
sample_rate = 1000;       %data sample rate: [sps]
noise_gain = 15;           %noise gain: [%]
bit_depth = 10;           %vertical data resolution [bits]
idle_state = 0;           %idle signal time in miliseconds
safety_margin = 10;       %accelerator data safety guards [%]
shape_a = 10;             %logaritmic function generator shape [>0]
offset_a = 0;             %test signal offset
shape_b = 10;             %logaritmic function generator shape [>0]
offset_b = 0;             %test signal offset
decoder_range = 1000;     %linearization decoding range
data_vectors = 2;         %generated test data vectors

ns = (time/1000)*sample_rate + 1;           %calculate number of data samples
idle_ns = (idle_state/1000)*sample_rate;    %idle samples
time_vector = linspace(0, time/1000, ns);   %generate taime axis
resolution = power(2, bit_depth)-1;         %calculate vertical resolution
guard = resolution*safety_margin/100;       %calculate guard range

adc_vector = linspace(0, resolution, resolution+1);

#generate input data
sensor_a_data = log(1 + time_vector*shape_a);
range = (resolution-2*guard);  
scale = range/sensor_a_data(ns);
sensor_a_data *= scale;
sensor_a_data += guard;

sensor_b_data = log(1 + time_vector*shape_b);  
range = (resolution-2*guard);
scale = range/sensor_b_data(ns);
sensor_b_data *= scale;
sensor_b_data = range - sensor_b_data;
sensor_b_data += guard;

sensor_a_data(idle_ns+1:ns+idle_ns) = sensor_a_data;
sensor_a_data(1:idle_ns) = guard;
sensor_b_data(idle_ns+1:ns+idle_ns) = sensor_b_data;
sensor_b_data(1:idle_ns) = resolution - guard;

time_vector = linspace(0, (time+idle_state)/1000, ns+idle_ns);
         
#noise generator
noise_vector = rand(ns+idle_ns);                            %get uniform random data 
noise_amplitude = (noise_gain/100) * resolution;    %calculate noise amplitude
noise_vector = noise_vector(1,:) * noise_amplitude; %scale to noise amplitude                
noise_vector = noise_vector - noise_amplitude/2;    %simetric around zero      

lut_gen_data = sensor_a_data;
sensor_a_data += noise_vector - offset_a;
sensor_b_data += noise_vector + offset_b;

input_data (2,:) = int32(sensor_a_data);
input_data (3,:) = int32(sensor_b_data);

sum_ab = sensor_a_data + sensor_b_data;
dif_ab = (sensor_a_data - sensor_b_data + range)/2 + guard;
dif2_ab = (sensor_b_data - sensor_a_data + range)/2 + guard;

#decoding LUT generator
lut = (decoder_range/(time/1000))*(exp((lut_gen_data-guard)/scale)-1)/shape_a;

lut_adc = (decoder_range/(time/1000))*(exp((adc_vector-guard)/scale)-1)/shape_a;
lut_adc = uint32(lut_adc);

guard_element = uint32(guard);

lut_adc(1:guard_element) = 0;
lut_adc(resolution-guard_element: resolution+1) = decoder_range;

fletcherChecksum = fletcher32(lut_adc);

#generate decoder lut .c file
fp = fopen ("acs_decoder.c", 'w+');
fprintf(fp, "/*file generated with %s.m octave/matlab script %s*/\n", mfilename(), datestr(clock()));
lut_row_elements = 16;
lut_rows = uint32((resolution+1)/lut_row_elements);
lut_elements = columns(lut_adc);

fprintf(fp, "#include <stdint.h>\n\n");

#write primary table
fprintf(fp, "/* primary APPE decoding look up table */\n");
fprintf(fp, "const uint32_t acsPrimDecodingLUT[%d] =\n{\n", lut_elements+1);
for i=1:lut_rows
  for j=1:lut_row_elements
    fprintf(fp, "%4d", lut_adc(1,(i-1)*lut_row_elements + j));
    fprintf(fp, ", ");
  endfor;
  fprintf(fp, "\n");
endfor;
fprintf(fp, "0x%X};\n\n", fletcherChecksum);

#write backup table
fprintf(fp, "/* backup APPE decoding look up table */\n");
fprintf(fp, "const uint32_t acsBackDecodingLUT[%d] =\n{\n", lut_elements+1);
for i=1:lut_rows
  for j=1:lut_row_elements
    fprintf(fp, "%4d", lut_adc(1,(i-1)*lut_row_elements + j));
    fprintf(fp, ", ");
  endfor;
  fprintf(fp, "\n");
endfor;
fprintf(fp, "0x%X};\n", fletcherChecksum);

fclose(fp);

#generate decoder lut .h file
fp = fopen ("acs_decoder.h", 'w+');
fprintf(fp, "/*file generated with %s.m octave/matlab script %s*/\n\n", mfilename(), datestr(clock()));
fprintf(fp, "#include <stdint.h>\n");
fprintf(fp, "#ifndef ACS_DECODER_H\n");
fprintf(fp, "#define ACS_DECODER_H\n\n");
fprintf(fp, "#define ACS_DECODER_LUT_ELEMENTS  %d\n\n", lut_elements+1);
fprintf(fp, "extern const uint32_t acsPrimDecodingLUT[];\n");
fprintf(fp, "extern const uint32_t acsBackDecodingLUT[];\n\n");
fprintf(fp, "#endif /*ACS_DECODER_H*/\n");

fclose(fp);

#set test data properties
input_data(1,1) = sample_rate;      %data sample rate
input_data(1,2) = ns+idle_ns;       %number of data samples 
input_data(1,3) = resolution;       %vertical resolution
input_data(1,4) = guard;            %absolute value of guard regions
input_data(1,5) = decoder_range;    %absolute output decoder range
input_data(1,6) = data_vectors;     %number of data vector in file

#write data to scv file
csvwrite('acs_test_data.csv', input_data);
fp = fopen('acs_test_data.csv', 'a+');
fprintf(fp, "\n");
fclose(fp);

#invert signal
sensor_a_data_inverted = resolution - sensor_a_data;
sensor_b_data_inverted = resolution - sensor_b_data;

time_ms = time_vector*1000;
#plot results
close all;
figure(1, 'position', [0,50,1900,900]);
hold on;
  plot(time_ms, sensor_a_data); 
  plot(time_ms, sensor_b_data); 
  plot(time_ms, sum_ab); 
  plot(time_ms, lut);
  title("Input data");
  xlabel("time [ms]");
  ylabel("amplitude");
  legend("Signal A", "Signal B", "Response   ");
hold off;

print("Figs/input.svg", "-dsvg", "-S1600, 900");



