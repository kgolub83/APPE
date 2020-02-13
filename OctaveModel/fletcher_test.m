clear all;

#define test string
testDataString = "abcdefgh";
result = 0xEBE19591;

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

resultFletcher = fletcher32(test);

printf("%x", resultFletcher);
