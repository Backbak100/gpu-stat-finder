# gpu stat finder

Only works on mac, and reads the GPU Active Residency from through Apples private API IOReport, and gets the chip model and core count through IOKit

Compile it alongside the frameworks Corefoundation and IOKit, and the arguments -lIOReport and -fblocks  

Ex. ```gcc main.c livestats.c staticstats.c sig_handler.c CoreFoundation.c -framework CoreFoundation -framework IOKit -lIOReport -fblocks```
