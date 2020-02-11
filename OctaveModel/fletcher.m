clear all;

#define test string
testDataString = "abcdefgh";

#convert characters to ASCII values
testData = toascii(testDataString);
words = columns(testData);
#check two byte aligenment
padding = mod(words,2);
if padding
  words +=1;
  testData(words) = 0;
endif

#convert to 16 bit values
for i = 1:words
  if(mod(i,2))
    test(floor(i/2)+1) = testData(i);
  else
    test((i/2)) += bitshift(testData(i), 8);
  endif
endfor

words = columns(test);
sum1 = sum2 = 0;
k = 1;
while words > 0
  blockSize = min(words, 359);
  
  for i = 1:blockSize
    sum1 += test(k);
    sum2 += sum1;
    k++;
  endfor
#reduce sums to 16 bit  
  sum1 = (bitand(sum1,0xFFFF) + bitshift(sum1, -16));
  sum2 = (bitand(sum2,0xFFFF) + bitshift(sum2, -16));
  
  words -= blockSize;
endwhile

sum1 = (bitand(sum1,0xFFFF) + bitshift(sum1, -16));
sum2 = (bitand(sum2,0xFFFF) + bitshift(sum2, -16));

#compose 32bit value checksum
msb = bitshift(sum2, 16);
lsb = sum1;
resultFlatcher = bitor(msb,lsb);

printf("%X", resultFlatcher);
