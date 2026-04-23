#ifndef OBJCOBJECT_H
#define OBJCOBJECT_H

#include <Python.h>

#include <objc/objc.h>

// ObjCObject

typedef struct {
    PyObject_HEAD
    id value;
} ObjCObjectObject;

extern PyType_Spec ObjCObject_spec;

#endif // OBJCOBJECT_H
