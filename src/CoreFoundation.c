#include "CoreFoundation.h"

void MCFRelease(CFTypeRef);

void MCFRelease(CFTypeRef cf) {
    if (cf) CFRelease(cf);
}
