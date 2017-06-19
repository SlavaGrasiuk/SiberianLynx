:: 1st arg 	- toolchain direcory
:: 2nd arg	- output directory
:: 3rd arg	- output file name
:: 4rd arg	- output file name without extension

%1\bin\arm-none-eabi-objdump.exe -DC %2\%3 > %2\%4.lss