#ifndef OBJCTYPES_MODULE_H
#define OBJCTYPES_MODULE_H

#include <Python.h>

extern PyModuleDef objctypes_module;

typedef struct {

    PyTypeObject *ObjCBool_Type;

    // Cache of the `YES` object. Do not manipulate this field outside of the
    // `ObjCBool` type.
    PyObject *ObjCBool_YES;

    // Cache of the `NO` object. Do not manipulate this field outside of the
    // `ObjCBool` type.
    PyObject *ObjCBool_NO;

    PyTypeObject *ObjCClass_Type;

    void *ObjCClass_cache;

    PyTypeObject *ObjCMethod_Type;

    void *ObjCMethod_cache;

    PyTypeObject *ObjCObject_Type;

    void *ObjCObject_cache;

    PyTypeObject *ObjCSelector_Type;

    void *ObjCSelector_cache;

} objctypes_state;

#endif // OBJCTYPES_MODULE_H
