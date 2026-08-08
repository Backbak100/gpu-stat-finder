#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "staticstats.c"
#define NUM "1234567890"
#define TRUE 1
#define FALSE 0

//parses

int applesilicon_mchip = TRUE;

int main (void) {

    CFMutableDictionaryRef properties = get_properties_for_gpu();

    printf("GPU cores = %d\n", get_gpu_core_count(properties));

    char *model = get_chip_model(properties);
    printf("GPU model = %s\n", model);
    free(model);

    CFRelease(properties);

    if (applesilicon_mchip) {
        FILE *fp_gpures = popen("sudo powermetrics --samplers gpu_power -i 1000", "r");
        FILE *fp_chiptype = popen("system_profiler SPDisplaysDataType", "r");

        char line[1024];

        float gpu_residency;

        while (fgets(line, sizeof(line), fp_gpures) != NULL) {
            if (sscanf(line, "GPU HW active residency:   %f", &gpu_residency) == 1)
                printf("\rGPU Active Residency: %.2f%%", gpu_residency);
            fflush(stdout);
        }
    }

    return 0;
}
