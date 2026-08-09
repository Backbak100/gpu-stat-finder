#ifndef IOReport_H
#define IOReport_H

#include <stdint.h>
#include <CoreFoundation/CoreFoundation.h>
#include <DriverKit/IOReportTypes.h>

/*
compile with
-lIOReport (to access to function definitions from libIOReport.tbd)
-fblocks (because of IOReportIterate())
-framework CoreFoundation (for the CF types)

General Workflow:
find channels
subscribe to interesting channel
take 2 samples from the subscription (with a wait time between them)
find delta between the samples
iterate through the sample/delta channels (the samples and deltas are stored in CFDictionary objects)
extract those values
*/

/*TYPES*/

//An opaque type, it js points to a subscription ig
//cannot be destroyed
typedef struct IOReportSubscriptionRef *IOReportSubscriptionRef;

//IOReportChannels are represented as CFDictionaries
//destroy with CFRelease()
typedef CFDictionaryRef IOReportChannelRef;

//IOReportSamples are represented as CFDictionaries
//destroy with CFRelease()
typedef CFDictionaryRef IOReportSampleRef;

/*CHANNELS*/

//copies the different channels in a group
//every channel is given a channel_id, a subgroup, and a group
//a channel is a data stream exposed by a driver or IOKit
//flags can all be put as 0 for default execution- idk what other flags exist
//destroy its return value with CFRelease()
extern CFMutableDictionaryRef IOReportCopyChannelsInGroup(
    CFStringRef group, CFStringRef subgroup, uint64_t flags1, uint64_t flags2, uint64_t flags3);

//gets the channel name and returns it as a CFString
//do not need to destroy its return value with CFRelease()
extern CFStringRef IOReportChannelGetChannelName(IOReportChannelRef channel);

//gets the channel unit label and returns it as a CFString
//do not need to destroy its return value with CFRelease()
extern CFStringRef IOReportChannelGetUnitLabel(IOReportChannelRef channel);

/*gets the channel format and returns it as:
    kIOReportInvalidFormat = 0,
	kIOReportFormatSimple = 1,
	kIOReportFormatState = 2,
	kIOReportFormatHistogram = 3,
	kIOReportFormatSimpleArray = 4
(found in IOReportTypes.h) */
extern int IOReportChannelGetFormat(CFDictionaryRef samples);

/*SUBSCRIPTIONS*/

//A subscription allows you to easily poll an interesting channel
//(for a use NULL, and b use 0 or NULL, im not sure what their acc purpose is)
extern IOReportSubscriptionRef IOReportCreateSubscription(
    void* a, CFMutableDictionaryRef desiredChannels, CFMutableDictionaryRef* subbedChannels, uint64_t channel_id, CFTypeRef b);

/*SAMPLES*/

//returns a CFDictionary with the data in the subscribed channels at that point in time
//Put a as NULL
extern CFDictionaryRef IOReportCreateSamples(IOReportSubscriptionRef iorsub, CFMutableDictionaryRef subbedChannels, CFTypeRef a);

//put a as NULL again
//Isn't necessary for basic arithmetic
extern CFDictionaryRef IOReportCreateSamplesDelta(CFDictionaryRef prev, CFDictionaryRef current, CFTypeRef a);

/*ITERATOR*/

//Apple/Clang block not in ISO C, compile with -fblocks
typedef int (^ioreportiterateblock)(IOReportChannelRef channel);

//Apple/Clang block not in ISO C, compile with -fblocks
//Iterate allows you to iterate through the samples/deltas in the CFDictionary and run code on it (defined in block) that returns an int
extern void IOReportIterate(IOReportSampleRef samples, ioreportiterateblock block);

/*MISC*/

//put a as 0
//gets an integer val from a 'simple' IO report channel
extern long IOReportSimpleGetIntegerValue(IOReportChannelRef channel, int a);

#endif
