# macgpu

Reads the GPU Active Residency from macOS command-line tool `powermetrics`

It launches ```sudo powermetrics --samplers gpu_power -i 1000``` in a child process using `popen()`, reads the GPU Active Residency percentage from its output, and displays it in the terminal
