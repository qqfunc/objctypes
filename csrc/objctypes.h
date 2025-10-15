#ifndef OBJCTYPES_H
#define OBJCTYPES_H

#include <Python.h>

#include <objc/objc.h>
#include <objc/runtime.h>

// ObjCClass

typedef struct {
    PyTypeObject type;
    Class value;
} ObjCClassObject;

extern PyTypeObject ObjCClassType;

// Get an ObjCClass from an Objective-C Class.
PyObject *
ObjCClass_FromClass(Class cls);

// ObjCObject

typedef struct {
    PyObject_HEAD
    id value;
} ObjCObjectObject;

extern ObjCClassObject ObjCObjectType;

// Get an ObjCObject from an Objective-C id.
PyObject *
ObjCObject_FromId(id obj);

// ObjCMethod

typedef struct {
    PyObject_HEAD
    Method value;
} ObjCMethodObject;

extern PyTypeObject ObjCMethodType;

// Get an ObjCMethod from an Objective-C Method.
PyObject *
ObjCMethod_FromMethod(Method method);

// ObjCSelector

typedef struct {
    PyObject_HEAD
    SEL value;
} ObjCSelectorObject;

extern PyTypeObject ObjCSelectorType;

// Get an ObjCSelector from an Objective-C SEL.
PyObject *
ObjCSelector_FromSEL(SEL sel);

// Convert a Python object to an Objective-C selector.
// A converter function for `O&` format unit.
// See https://docs.python.org/3.13/c-api/arg.html#other-objects.
int
ObjCSelector_SELConverter(PyObject *obj, void *ptr);

// ObjCBool

typedef struct {
    PyObject_HEAD
    BOOL value;
} ObjCBoolObject;

extern PyTypeObject ObjCBoolType;

// Get an ObjCBool from an int.
PyObject *
ObjCBool_FromLong(long v);

// Cache functions

#ifdef __cplusplus
extern "C" {
#endif

// Get an ObjCClass cache if it exists. Returns a new reference or NULL.
ObjCClassObject *
ObjCClass_cache_get(Class cls);

// Cache an ObjCClass.
void
ObjCClass_cache_set(Class cls, ObjCClassObject *obj);

// Delete an ObjCClass cache.
void
ObjCClass_cache_del(Class cls);

// Get an ObjCObject cache if it exists. Returns a new reference or NULL.
ObjCObjectObject *
ObjCObject_cache_get(id obj);

// Cache an ObjCObject.
void
ObjCObject_cache_set(id obj, ObjCObjectObject *pyobj);

// Delete an ObjCObject cache.
void
ObjCObject_cache_del(id obj);

// Get an ObjCMethod cache if it exists. Returns a new reference or NULL.
ObjCMethodObject *
ObjCMethod_cache_get(Method method);

// Cache an ObjCMethod.
void
ObjCMethod_cache_set(Method method, ObjCMethodObject *obj);

// Delete an ObjCMethod cache.
void
ObjCMethod_cache_del(Method method);

// Get an ObjCSelector cache if it exists. Returns a new reference or NULL.
ObjCSelectorObject *
ObjCSelector_cache_get(SEL selector);

// Cache an ObjCSelector.
void
ObjCSelector_cache_set(SEL sel, ObjCSelectorObject *obj);

// Delete an ObjCSelector cache.
void
ObjCSelector_cache_del(SEL selector);

#ifdef __cplusplus
}
#endif

#endif // OBJCTYPES_H
