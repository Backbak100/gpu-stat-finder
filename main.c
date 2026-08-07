#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NUM "1234567890"
#define TRUE 1
#define FALSE 0

int applesilicon_mchip = TRUE;

int main (void) {

    if (applesilicon_mchip) {
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
