#ifndef OBJCSELECTOR_H
#define OBJCSELECTOR_H

#include <Python.h>

#include <objc/runtime.h>

// ObjCSelector

typedef struct {
    PyObject_HEAD
    SEL value;
} ObjCSelectorObject;

extern PyType_Spec ObjCSelector_spec;

PyObject *
ObjCSelector_FromSEL(PyObject *module, SEL sel);

#endif // OBJCSELECTOR_H
