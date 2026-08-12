#include "include/staticstats.h"
#include "include/livestats.h"

//compile as gcc main.c livestats.c staticstats.c -framework CoreFoundation -framework IOKit -lIOReport -fblocks

int applesilicon_mchip = 1;

int main (void) {
    if (applesilicon_mchip) {
            struct timespec start, end;
            gpu_info_t *gpu_info = get_gpu_info();
            
            while(gpu_info) {
                gpu_info_t *next = gpu_info->next_gpu;
                printf("GPU %d\nGPU core count: %d\nChip model: %s\n\n", gpu_info->gpu_num, gpu_info->core_count, gpu_info->chip_model);
                free(gpu_info);
                gpu_info = next;
            }
    
            CFMutableDictionaryRef subbedChannels = NULL;
            IOReportSubscriptionRef subscription = get_input(&subbedChannels);
    
            while (1) {
                printf("\r\033[KGPU Active Residency = %.2f%%", get_gpu_residency_percent(subscription, subbedChannels));
                fflush(stdout);
            }
    
            CFRelease(subscription);
            CFRelease(subbedChannels);
    }

    return 0;
}
