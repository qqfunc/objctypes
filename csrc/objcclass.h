#ifndef OBJCCLASS_H
#define OBJCCLASS_H

#include <Python.h>

#include <objc/objc.h>

// ObjCClass

typedef struct {
    Class value;
} ObjCClassState;

extern PyType_Spec ObjCClass_spec;

#endif // OBJCCLASS_H
