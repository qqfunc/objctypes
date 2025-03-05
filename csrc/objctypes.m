#include <Python.h>
#include <objc/objc.h>
#include <objc/runtime.h>

#include "objctypes.h"

char *
objc_get_debug_description(id obj)
{
    if (class_comformsToProtocol(object_getClass(obj), @protocol(NSObject))) {
        return [[obj debugDescription] UTF8String];
    }
    else {
        return NULL;
    }
}
