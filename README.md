# gpu stat finder

Only works on mac, and reads the GPU Active Residency from macOS command-line tool `powermetrics`, and gets the chip model and core count through IOKit and Corefoundation

It gets the active residency by launching ```sudo powermetrics --samplers gpu_power -i 1000``` in a child process using `popen()`, reads the GPU Active Residency percentage from its output, and displays it in the terminal

Compile it alongside the frameworks IOKit and Corefoundation  
Ex. ```gcc .../main.c -framework IOKit -framework Corefoundation```
