#ifndef livestatsh
#define livestatsh

#include "IOReport.h"

//gets input to get_gpu_residency_percent
//free the return val and input to this function with CFRelease()
IOReportSubscriptionRef get_input(CFMutableDictionaryRef *);

//gets percentage of gpu active residency as a float
float get_gpu_residency_percent(IOReportSubscriptionRef subscription, CFMutableDictionaryRef subbedChannels); //gets gpu active residency as a percentage

#endif
