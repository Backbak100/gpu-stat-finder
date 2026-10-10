# gpu stat finder

Only works on apple silicon (?), and reads the GPU Active Residency from through Apples private API IOReport, and gets the chip model and core count through IOKit

run by copying the repository, and then 'make' followed by 'make run' (or ./a.out), then 'make clean' to get rid of all the compiled files
