#include <Python.h>
#include <objc/objc.h>
#include <unordered_map>

#include "objctypes.h"

typedef std::unordered_map<void *, PyObject *> cache_map;

// Cache ObjCClass

static cache_map ObjCClass_cache;

ObjCClassObject *
cache_get_ObjCClass(Class cls)
{
    const auto it = ObjCClass_cache.find(cls);
    if (it != ObjCClass_cache.end()) {
        Py_INCREF(it->second);
        return (ObjCClassObject *)it->second;
    }
    return NULL;
}

void
cache_add_ObjCClass(Class cls, ObjCClassObject *obj)
{
    ObjCClass_cache[cls] = (PyObject *)obj;
}

void
cache_delete_ObjCClass(Class cls)
{
    ObjCClass_cache.erase(cls);
}

// Cache ObjCObject

static cache_map ObjCObject_cache;

ObjCObjectObject *
cache_get_ObjCObject(id obj)
{
    const auto it = ObjCObject_cache.find(obj);
    if (it != ObjCObject_cache.end()) {
        Py_INCREF(it->second);
        return (ObjCObjectObject *)it->second;
    }
    return NULL;
}

void
cache_add_ObjCObject(id obj, ObjCObjectObject *pyobj)
{
    ObjCObject_cache[obj] = (PyObject *)pyobj;
}

void
cache_delete_ObjCObject(id obj)
{
    ObjCObject_cache.erase(obj);
}

// Cache ObjCMethod

static cache_map ObjCMethod_cache;

ObjCMethodObject *
cache_get_ObjCMethod(Method method)
{
    const auto it = ObjCMethod_cache.find(method);
    if (it != ObjCMethod_cache.end()) {
        Py_INCREF(it->second);
        return (ObjCMethodObject *)it->second;
    }
    return NULL;
}

void
cache_add_ObjCMethod(Method method, ObjCMethodObject *obj)
{
    ObjCMethod_cache[method] = (PyObject *)obj;
}

void
cache_delete_ObjCMethod(Method method)
{
    ObjCMethod_cache.erase(method);
}

// Cache ObjCSelector

static cache_map ObjCSelector_cache;

ObjCSelectorObject *
cache_get_ObjCSelector(SEL sel)
{
    const auto it = ObjCSelector_cache.find(sel);
    if (it != ObjCSelector_cache.end()) {
        Py_INCREF(it->second);
        return (ObjCSelectorObject *)it->second;
    }
    return NULL;
}

void
cache_add_ObjCSelector(SEL sel, ObjCSelectorObject *obj)
{
    ObjCSelector_cache[sel] = (PyObject *)obj;
}

void
cache_delete_ObjCSelector(SEL sel)
{
    ObjCSelector_cache.erase(sel);
}
