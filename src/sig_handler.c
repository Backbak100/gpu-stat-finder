#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include "livestats.h"
#define DELETE 11
#define KEEP 10

extern int applesilicon_mchip;
void charwrite(char *, int);
void big_sig_handler(struct sigaction);
void my_sig_handler(int);

//with ctrl c escape

void charwrite(char *string, int status) {
    if (status == DELETE)
        write(1, "\r\e[2K", 6);

    unsigned long len = strlen(string);
    unsigned long bytes = write(1, string, len);

    if (bytes == -1) 
        write(2, "Error writing to stdout\n", 25);

}

void big_sig_handler(struct sigaction sa) {
    for (int i = 0; i < NSIG; i++)
        sigaction(i, &sa, NULL);
}

void my_sig_handler(int sig) {
    switch (sig) {
        case SIGINT: //ctrl c
            applesilicon_mchip = 0;
            return;
        case SIGTSTP: //ctrl z
            applesilicon_mchip = 0;
            return;
        case SIGQUIT: //ctrl + \ */
            applesilicon_mchip = 0;
            return;
    }
}
