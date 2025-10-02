#include <Python.h>

#include "objctypes.h"

#import <Foundation/Foundation.h>

const char *
objc_get_debug_description(id obj)
{
    if (class_conformsToProtocol(object_getClass(obj), @protocol(NSObject))) {
        return [[obj debugDescription] UTF8String];
    }
    else {
        return NULL;
    }
}
