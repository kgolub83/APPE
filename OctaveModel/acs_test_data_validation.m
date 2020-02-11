clear all; 

inputVector = csvread("acs_output_data.csv");

vectorsNo = rows(inputVector);

close all;
hold on;

for i = 1:vectorsNo
  plot(inputVector(i, :));
endfor;

title("output data");
xlabel("value");
ylabel("sample no");
legend("Input A", "Input B", "APP output");

hold off;