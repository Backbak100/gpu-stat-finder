#ifndef staticstatsh
#define staticstatsh

#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOKitLib.h>

typedef struct gpu_info {
    int gpu_num;
    int core_count;
    char chip_model[1024];
    struct gpu_info *next_gpu;
} gpu_info_t;

extern CFMutableDictionaryRef get_properties(void); //gets properties (assumes 1 GPU)

extern gpu_info_t *get_gpu_info(void); //gets gpu core count and chip model- returns whatever gpu_info was gathered if failed

extern io_iterator_t get_iterator_for_gpus(void); /*gets the IOKit iterator for IOKit services under "IOAccelerator", returns NULL if failed*/

extern int get_gpu_core_count(CFMutableDictionaryRef); //gets gpu core count as an int, returns 0 if failed

extern char *get_chip_model(CFMutableDictionaryRef, char *); //gets chip model as a string (char *), returns NULL if failed

#endif
