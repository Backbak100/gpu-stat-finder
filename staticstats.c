#include <IOKit/IOKitLib.h>
#include <CoreFoundation/CoreFoundation.h>
#include <stdio.h>
#include <stdlib.h>

CFMutableDictionaryRef get_properties_for_gpu(); //get CFDictionary of gpu stats returns NULL if failed
int get_gpu_core_count(CFMutableDictionaryRef); //get gpu core count as an int returns negative number if failed
char *get_chip_model(CFMutableDictionaryRef); //get chip model as string (Apple M3, Apple M2...) returns NULL if failed

//get CFDictionary of gpu stats returns NULL if failed
CFMutableDictionaryRef get_properties_for_gpu() {
    char *find = "IOAccelerator";
    io_service_t service = IOServiceGetMatchingService(kIOMainPortDefault, IOServiceMatching(find));
   
    if (service == IO_OBJECT_NULL) {
        printf("no matching service\n");
        return NULL;
    }
    
    CFMutableDictionaryRef properties = NULL;
    kern_return_t result = IORegistryEntryCreateCFProperties(service, &properties, kCFAllocatorDefault, 0);

    if (result != KERN_SUCCESS) {
        printf("Unable to create CF properties\n");
        return NULL;
    }

    IOObjectRelease(service); //discussion All objects returned by IOKitLib should be released with this function

    return properties;
}

//get gpu core count as an int returns negative number if failed
int get_gpu_core_count(CFMutableDictionaryRef properties) {
    int gpu_cores;

    if (!properties) {
        printf("properties is not defined properly\n");
        return -1;
    }

    CFTypeRef stat = CFDictionaryGetValue(properties, CFSTR("gpu-core-count"));
    if (stat != NULL && CFGetTypeID(stat) == CFNumberGetTypeID())
        CFNumberGetValue((CFNumberRef)stat, kCFNumberIntType, &gpu_cores);
    else {
        printf("gpu-core-count not displayed as an CFNumber\n");
        return -1;
    }

    return gpu_cores;
}

//get chip model as string (Apple M3, Apple M2...) returns NULL if failed
char *get_chip_model(CFMutableDictionaryRef properties) {
    char *chip_model = malloc(1024); //user hasto free the return value in main()
    if (!properties) {
        printf("properties is not defined properly\n");
        return NULL;
    }

    CFTypeRef stat = CFDictionaryGetValue(properties, CFSTR("model"));
    if (stat != NULL && CFGetTypeID(stat) == CFStringGetTypeID()) {
        if (!CFStringGetCString((CFStringRef)stat, chip_model, 1024, kCFStringEncodingUTF8)) {
            printf("CFStringGetCString failed\n");
            return NULL;
        }
    }
    else {
        printf("chip_model not displayed as a CFString\n");
        return NULL;
    }
    
    return chip_model;
}
