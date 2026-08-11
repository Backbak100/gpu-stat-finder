#include "IOReport.h"
#include <unistd.h>
#include <time.h>

//gets the gpu's active residency as a float
//subscription should be a parameter- not constantly reestablished (ex. cluttered in a forever while loop)
float get_gpu_residency_percent(void) {

    __block long value, value2, off, off2;
    CFMutableDictionaryRef channels = IOReportCopyChannelsInGroup(CFSTR("GPU Stats"), CFSTR("GPU Performance States"), 0, 0, 0);

        CFMutableDictionaryRef subbedChannels = NULL;
        IOReportSubscriptionRef subscription = IOReportCreateSubscription(NULL, channels, &subbedChannels, 0, 0);
        while (1){
            IOReportSampleRef samples = IOReportCreateSamples(subscription, subbedChannels, NULL);
            sleep(1);
            IOReportSampleRef samples2 = IOReportCreateSamples(subscription, subbedChannels, NULL);

            IOReportIterate(samples,
                ^int(IOReportChannelRef channel) {
                    int format = IOReportChannelGetFormat(channel);

                    value = 0;

                    if (format == kIOReportFormatState) {
                        off = IOReportStateGetResidency(channel, 0);
                        for (int i = 0; i <16; i++)
                            value += IOReportStateGetResidency(channel, i);
                    }

                    return kIOReportIterOk; 
                }
            );

            IOReportIterate(samples2,
                ^int(IOReportChannelRef channel) {
                    int format = IOReportChannelGetFormat(channel);

                    value2=0;

                    if (format == kIOReportFormatState) {
                        off2 = IOReportStateGetResidency(channel, 0);
                        for (int i = 0; i <16; i++)
                            value2 += IOReportStateGetResidency(channel, i);
                    }

                    return kIOReportIterOk; 
                }
            );
            
            long doff = off2-off;
            long dval = value2-value;
            float percentage = ((float)doff/(float)dval) *100.0f;
            percentage = 100.0f-percentage;
        
            CFRelease(samples);
            CFRelease(samples2);
            CFRelease(channels);
            CFRelease(subbedChannels);
            return percentage;
        }


}
