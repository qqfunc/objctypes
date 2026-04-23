/**
 * @file objcselector.h
 * @brief Source declarations and definitions for objcselector.h.
 */

#ifndef OBJCSELECTOR_H
#define OBJCSELECTOR_H

#include <Python.h>

#include <objc/runtime.h>

/// ObjCSelector

typedef struct {
    PyObject_HEAD
    SEL value;
} ObjCSelectorObject;

extern PyType_Spec ObjCSelector_spec;

#endif // OBJCSELECTOR_H
