function checksum = fletcher32(inData)

  %init parameters
  uint32(inData);
  words = columns(inData);
  sum1 = sum2 = 0;
  k = 1;

  %calculate data
  while words > 0
    blockSize = min(words, 359);
    
    for i = 1:blockSize
      sum1 += inData(k);
      sum2 += sum1;
      k++;
    endfor
  %reduce sums to 16 bit  
    sum1 = (bitand(sum1,0xFFFF) + bitshift(sum1, -16));
    sum2 = (bitand(sum2,0xFFFF) + bitshift(sum2, -16));
    
    words -= blockSize;
  endwhile

  sum1 = (bitand(sum1,0xFFFF) + bitshift(sum1, -16));
  sum2 = (bitand(sum2,0xFFFF) + bitshift(sum2, -16));

  %compose 32bit value checksum
  msb = bitshift(sum2, 16);
  lsb = sum1;
  
  checksum = uint32(bitor(msb,lsb));

endfunction