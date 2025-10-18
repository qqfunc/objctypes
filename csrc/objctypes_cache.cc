#include <Python.h>

#include "objcclass.h"
#include "objcmethod.h"
#include "objcobject.h"
#include "objcselector.h"

#include <map>

typedef std::map<void *, PyObject *> cache_map;

// Cache ObjCClass

static cache_map ObjCClass_cache;

ObjCClassObject *
ObjCClass_cache_get(Class cls)
{
    const auto it = ObjCClass_cache.find(cls);
    if (it != ObjCClass_cache.end()) {
        return (ObjCClassObject *)Py_NewRef(it->second);
    }
    return NULL;
}

void
ObjCClass_cache_set(Class cls, ObjCClassObject *obj)
{
    ObjCClass_cache[cls] = (PyObject *)obj;
}

void
ObjCClass_cache_del(Class cls)
{
    ObjCClass_cache.erase(cls);
}

// Cache ObjCObject

static cache_map ObjCObject_cache;

ObjCObjectObject *
ObjCObject_cache_get(id obj)
{
    const auto it = ObjCObject_cache.find(obj);
    if (it != ObjCObject_cache.end()) {
        return (ObjCObjectObject *)Py_NewRef(it->second);
    }
    return NULL;
}

void
ObjCObject_cache_set(id obj, ObjCObjectObject *pyobj)
{
    ObjCObject_cache[obj] = (PyObject *)pyobj;
}

void
ObjCObject_cache_del(id obj)
{
    ObjCObject_cache.erase(obj);
}

// Cache ObjCMethod

static cache_map ObjCMethod_cache;

ObjCMethodObject *
ObjCMethod_cache_get(Method method)
{
    const auto it = ObjCMethod_cache.find(method);
    if (it != ObjCMethod_cache.end()) {
        return (ObjCMethodObject *)Py_NewRef(it->second);
    }
    return NULL;
}

void
ObjCMethod_cache_set(Method method, ObjCMethodObject *obj)
{
    ObjCMethod_cache[method] = (PyObject *)obj;
}

void
ObjCMethod_cache_del(Method method)
{
    ObjCMethod_cache.erase(method);
}

// Cache ObjCSelector

static cache_map ObjCSelector_cache;

ObjCSelectorObject *
ObjCSelector_cache_get(SEL sel)
{
    const auto it = ObjCSelector_cache.find(sel);
    if (it != ObjCSelector_cache.end()) {
        return (ObjCSelectorObject *)Py_NewRef(it->second);
    }
    return NULL;
}

void
ObjCSelector_cache_set(SEL sel, ObjCSelectorObject *obj)
{
    ObjCSelector_cache[sel] = (PyObject *)obj;
}

void
ObjCSelector_cache_del(SEL sel)
{
    ObjCSelector_cache.erase(sel);
}
