#include <IOKit/IOKitLib.h>
#include <CoreFoundation/CoreFoundation.h>
#include <stdio.h>

typedef struct {
    int core_count;
    char chip_model[1024];
} gpu_info_t;

gpu_info_t get_gpu_info(); //gets gpu core count and chip model- returns NULL if failed
CFMutableDictionaryRef get_properties_for_gpu(); //gets the CFDictionary of IOAccelerator (Graphics), returns NULL if failed
int get_gpu_core_count(CFMutableDictionaryRef); //gets gpu core count as an int, returns 0 if failed 
char *get_chip_model(CFMutableDictionaryRef, char *); //gets chip model as a string (char *), returns NULL if failed

//gets gpu core count and chip model- returns NULL if failed
gpu_info_t get_gpu_info() {
    gpu_info_t gpu_info;

    CFMutableDictionaryRef properties = get_properties_for_gpu();

    gpu_info.core_count = get_gpu_core_count(properties);

    get_chip_model(properties, gpu_info.chip_model);

    CFRelease(properties);
    return gpu_info;
}

//gets the CFDictionary of IOAccelerator (Graphics), returns NULL if failed
CFMutableDictionaryRef get_properties_for_gpu() {
    char *find = "IOAccelerator";
    io_service_t service = IOServiceGetMatchingService(kIOMainPortDefault, IOServiceMatching(find));
   
    if (service == IO_OBJECT_NULL) {
        fprintf(stderr, "Error: No matching service for \"%s\"\n", find);
        return NULL;
    }
    
    CFMutableDictionaryRef properties = NULL;
    kern_return_t result = IORegistryEntryCreateCFProperties(service, &properties, kCFAllocatorDefault, 0);

    if (result != KERN_SUCCESS) {
        fprintf(stderr, "Error: Unable to create CF properties for \"%s\"\n", find);
        return NULL;
    }

    IOObjectRelease(service); //discussion All objects returned by IOKitLib should be released with this function

    return properties;
}

//get gpu core count as an int returns 0 if failed
int get_gpu_core_count(CFMutableDictionaryRef properties) {
    int gpu_cores;

    if (!properties) {
        fprintf(stderr, "Error: Properties is not defined properly\n");
        return 0;
    }

    CFTypeRef stat = CFDictionaryGetValue(properties, CFSTR("gpu-core-count"));
    if (stat != NULL && CFGetTypeID(stat) == CFNumberGetTypeID())
        CFNumberGetValue((CFNumberRef)stat, kCFNumberIntType, &gpu_cores);
    else {
        fprintf(stderr, "Error: \"gpu-core-count\" is unavailable or is not displayed as an CFNumber\n");
        return 0;
    }

    return gpu_cores;
}

//get chip model as string (Apple M3, Apple M2...) returns NULL if failed
char *get_chip_model(CFMutableDictionaryRef properties, char *chip_model) {
    if (!properties) {
        fprintf(stderr, "Error: Properties is not defined properly\n");
        return NULL;
    }

    CFTypeRef stat = CFDictionaryGetValue(properties, CFSTR("model"));
    if (stat != NULL && CFGetTypeID(stat) == CFStringGetTypeID()) {
        if (!CFStringGetCString((CFStringRef)stat, chip_model, 1024, kCFStringEncodingUTF8)) {
            fprintf(stderr, "Error: CFStringGetCString failed to convert to char *\n");
            return NULL;
        }
    }
    else {
        fprintf(stderr, "Error: \"model\" is not displayed as a CFString\n");
        return NULL;
    }
    
    return chip_model;
}
