#include <stdio.h>
#include "include/staticstats.h"

//gets gpu core count and chip model- returns NULL if failed
gpu_info_t *get_gpu_info() {
    gpu_info_t *head = NULL;
    gpu_info_t *tail = NULL;
    int gpu_num = 0;

    io_iterator_t iterator = get_iterator_for_gpus();

    io_service_t service;

    while ((service = IOIteratorNext(iterator)) != IO_OBJECT_NULL) {
        gpu_num++;
        
        CFMutableDictionaryRef properties = NULL;
        kern_return_t result = IORegistryEntryCreateCFProperties(service, &properties, kCFAllocatorDefault, 0);

        if (result != KERN_SUCCESS) {
            fprintf(stderr, "Error: Unable to create CF properties for \"IOAccelerator\"\n");

            MCFRelease(properties);
            IOObjectRelease(service);
            break;
        }

        gpu_info_t *gpu = malloc(sizeof(gpu));

        if(!gpu) {
            fprintf(stderr, "Error: malloc() could not allocate correctly\n");

            MCFRelease(properties);
            IOObjectRelease(service);
            break;
        }

        gpu->gpu_num = gpu_num;

        gpu->core_count = get_gpu_core_count(properties);
    
        get_chip_model(properties, gpu->chip_model);

        gpu->next_gpu = NULL;

        if (!head)
            head = gpu;
        else
            tail->next_gpu = gpu;
        
        tail = gpu;

        IOObjectRelease(service); //discussion All objects returned by IOKitLib should be released with this function
        MCFRelease(properties);
    }

    IOObjectRelease(iterator);
    return head;
}

//gets the CFDictionary of IOAccelerator (Graphics), returns NULL if failed
io_iterator_t get_iterator_for_gpus() {
    CFMutableDictionaryRef matching = IOServiceMatching("IOAccelerator");

    if (!matching) {
        fprintf(stderr, "Error: No matching dictionary for \"IOAccelerator\"\n");
        return IO_OBJECT_NULL;
    }

    io_iterator_t iterator;
    kern_return_t result = IOServiceGetMatchingServices(kIOMainPortDefault, matching, &iterator);

    if (result != KERN_SUCCESS) {
        fprintf(stderr, "Error: Unable to create CF properties for \"IOAccelerator\"\n");
        return IO_OBJECT_NULL;
    }

    if (iterator == IO_OBJECT_NULL) {
        fprintf(stderr, "Error: No GPU iterator\n");
        return IO_OBJECT_NULL;
    }

    return iterator;
}

CFMutableDictionaryRef get_properties() {
    CFMutableDictionaryRef matching = IOServiceMatching("IOAccelerator");

    if (!matching) {
        fprintf(stderr, "Error: No matching dictionary for \"IOAccelerator\"\n");
        return NULL;
    }

    io_service_t service = IOServiceGetMatchingService(kIOMainPortDefault, matching);

    CFMutableDictionaryRef properties = NULL;

    kern_return_t result = IORegistryEntryCreateCFProperties(service, &properties, kCFAllocatorDefault, 0);

    if (result != KERN_SUCCESS) {
        fprintf(stderr, "Error: Unable to create CF properties for \"IOAccelerator\"\n");

        MCFRelease(properties);
        IOObjectRelease(service);
        return NULL;
    }

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
