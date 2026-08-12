#include "IOReport.h"
#include <unistd.h>
#include <time.h>
#include "livestats.h"

IOReportSubscriptionRef get_input(CFMutableDictionaryRef *subbedChannels) {
    CFMutableDictionaryRef channels = IOReportCopyChannelsInGroup(CFSTR("GPU Stats"), CFSTR("GPU Performance States"), 0, 0, 0);

    IOReportSubscriptionRef subscription = IOReportCreateSubscription(NULL, channels, subbedChannels, 0, 0);

    CFRelease(channels);

    return subscription;
}

float get_gpu_residency_percent(IOReportSubscriptionRef subscription, CFMutableDictionaryRef subbedChannels) {

    __block long value, value2, off, off2;

    IOReportSampleRef samples = IOReportCreateSamples(subscription, subbedChannels, NULL);
    sleep(1);
    IOReportSampleRef samples2 = IOReportCreateSamples(subscription, subbedChannels, NULL);

    IOReportIterate(samples,
        ^int(IOReportChannelRef channel) {
            int format = IOReportChannelGetFormat(channel);

            value = 0;

            if (format == kIOReportFormatState) {
                off = IOReportStateGetResidency(channel, 0);
                for (int i = 0; i < IOReportStateGetCount(channel); i++)
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
                for (int i = 0; i < IOReportStateGetCount(channel); i++)
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
    return percentage;

}
