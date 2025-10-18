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
ObjCSelector_FromSEL(PyObject *module, SEL sel);

// ObjCBool

// Get an ObjCBool from an int.
PyObject *
ObjCBool_FromLong(PyObject *module, long v);

#endif // OBJCTYPES_H
