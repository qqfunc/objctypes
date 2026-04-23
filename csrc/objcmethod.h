#ifndef OBJCMETHOD_H
#define OBJCMETHOD_H

#include <Python.h>

#include <objc/runtime.h>

// ObjCMethod

typedef struct {
    PyObject_HEAD
    Method value;
} ObjCMethodObject;

extern PyType_Spec ObjCMethod_spec;

#endif // OBJCMETHOD_H
