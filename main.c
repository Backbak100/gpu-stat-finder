#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NUM "1234567890"

int main (void) {
    FILE *fp = popen("sudo powermetrics --samplers gpu_power -i 1000", "r");

    char line[1024];

    while (fgets(line, sizeof(line), fp) != NULL) {
        if (strstr(line, "GPU HW active residency:")){
            printf("\rGPU Active Residency: ");
            fflush(stdout);
            int i;
            for (i = 0; !strchr(NUM, line[i]); i++);
            for (int j = 0; j <= 4; j++) {
                printf("%c", line[i]);
                i++;
            }
        }
        fflush(stdout);
    }

    return 0;
}
