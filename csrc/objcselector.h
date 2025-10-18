#ifndef OBJCSELECTOR_H
#define OBJCSELECTOR_H

#include <Python.h>

#include <objc/runtime.h>

// ObjCSelector

typedef struct {
    PyObject_HEAD
    SEL value;
} ObjCSelectorObject;

extern PyTypeObject ObjCSelectorType;

#endif // OBJCSELECTOR_H
