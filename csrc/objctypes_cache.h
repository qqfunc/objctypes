#ifndef OBJC_CACHE_H
#define OBJC_CACHE_H

#include <Python.h>

#include "objcclass.h"
#include "objcmethod.h"
#include "objcobject.h"
#include "objcselector.h"

#include <objc/objc.h>

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

// Create a new ObjCSelector cache.
void *
ObjCSelector_cache_alloc(void);

// Delete an ObjCSelector cache.
void
ObjCSelector_cache_dealloc(void *cache);

// Get an ObjCSelector cache if it exists. Returns a new reference or NULL.
ObjCSelectorObject *
ObjCSelector_cache_get(PyObject *module, SEL selector);

// Cache an ObjCSelector.
void
ObjCSelector_cache_set(PyObject *module, SEL sel, ObjCSelectorObject *obj);

// Delete an ObjCSelector cache.
void
ObjCSelector_cache_del(PyObject *module, SEL selector);

#ifdef __cplusplus
}
#endif

#endif // OBJC_CACHE_H
