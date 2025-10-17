#ifndef OBJCBOOL_H
#define OBJCBOOL_H

#include <Python.h>

#include <objc/objc.h>

typedef struct {
    PyObject_HEAD
    BOOL value;
} ObjCBoolObject;

extern PyType_Spec ObjCBool_spec;

#endif // OBJCBOOL_H
