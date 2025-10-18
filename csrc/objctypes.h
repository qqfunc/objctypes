#ifndef OBJCTYPES_H
#define OBJCTYPES_H

#include <Python.h>

#include <objc/objc.h>
#include <objc/runtime.h>

// Get an ObjCClass from an Objective-C Class.
PyObject *
ObjCClass_FromClass(Class cls);

// Get an ObjCObject from an Objective-C id.
PyObject *
ObjCObject_FromId(id obj);

// Get an ObjCMethod from an Objective-C Method.
PyObject *
ObjCMethod_FromMethod(Method method);

// Get an ObjCSelector from an Objective-C SEL.
PyObject *
ObjCSelector_FromSEL(SEL sel);

// Convert a Python object to an Objective-C selector.
// A converter function for `O&` format unit.
// See https://docs.python.org/3.13/c-api/arg.html#other-objects.
int
ObjCSelector_SELConverter(PyObject *obj, void *ptr);

// ObjCBool

// Get an ObjCBool from an int.
PyObject *
ObjCBool_FromLong(PyObject *module, long v);

#endif // OBJCTYPES_H
