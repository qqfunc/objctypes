#include <Python.h>

#include "objctypes_cache.h"

#include "objcmethod.h"
#include "objcobject.h"
#include "objcselector.h"
#include "objctypes_module.h"

#include <map>

typedef std::map<void *, PyObject *> cache_map;

// Cache ObjCClass

void *
ObjCClass_cache_alloc(void)
{
    return new (std::nothrow) cache_map();
}

void
ObjCClass_cache_dealloc(void *cache)
{
    delete (cache_map *)cache;
}

PyObject *
ObjCClass_cache_get(PyObject *module, Class cls)
{
    objctypes_state *state = (objctypes_state *)PyModule_GetState(module);
    cache_map *cache = (cache_map *)state->ObjCClass_cache;

    const auto it = cache->find(cls);
    if (it != cache->end()) {
        return Py_NewRef(it->second);
    }
    return NULL;
}

void
ObjCClass_cache_set(PyObject *module, Class cls, PyObject *obj)
{
    objctypes_state *state = (objctypes_state *)PyModule_GetState(module);
    cache_map *cache = (cache_map *)state->ObjCClass_cache;
    (*cache)[cls] = (PyObject *)obj;
}

void
ObjCClass_cache_del(PyObject *module, Class cls)
{
    objctypes_state *state = (objctypes_state *)PyModule_GetState(module);
    cache_map *cache = (cache_map *)state->ObjCClass_cache;
    cache->erase(cls);
}

// Cache ObjCObject

void *
ObjCObject_cache_alloc(void)
{
    return new (std::nothrow) cache_map();
}

void
ObjCObject_cache_dealloc(void *cache)
{
    delete (cache_map *)cache;
}

ObjCObjectObject *
ObjCObject_cache_get(PyObject *module, id obj)
{
    objctypes_state *state = (objctypes_state *)PyModule_GetState(module);
    cache_map *cache = (cache_map *)state->ObjCObject_cache;

    const auto it = cache->find(obj);
    if (it != cache->end()) {
        return (ObjCObjectObject *)Py_NewRef(it->second);
    }
    return NULL;
}

void
ObjCObject_cache_set(PyObject *module, id obj, ObjCObjectObject *pyobj)
{
    objctypes_state *state = (objctypes_state *)PyModule_GetState(module);
    cache_map *cache = (cache_map *)state->ObjCObject_cache;
    (*cache)[obj] = (PyObject *)pyobj;
}

void
ObjCObject_cache_del(PyObject *module, id obj)
{
    objctypes_state *state = (objctypes_state *)PyModule_GetState(module);
    cache_map *cache = (cache_map *)state->ObjCObject_cache;
    cache->erase(obj);
}

// Cache ObjCMethod

void *
ObjCMethod_cache_alloc(void)
{
    return new (std::nothrow) cache_map();
}

void
ObjCMethod_cache_dealloc(void *cache)
{
    delete (cache_map *)cache;
}

ObjCMethodObject *
ObjCMethod_cache_get(PyObject *module, Method method)
{
    objctypes_state *state = (objctypes_state *)PyModule_GetState(module);
    cache_map *cache = (cache_map *)state->ObjCMethod_cache;

    const auto it = cache->find(method);
    if (it != cache->end()) {
        return (ObjCMethodObject *)Py_NewRef(it->second);
    }
    return NULL;
}

void
ObjCMethod_cache_set(PyObject *module, Method method, ObjCMethodObject *obj)
{
    objctypes_state *state = (objctypes_state *)PyModule_GetState(module);
    cache_map *cache = (cache_map *)state->ObjCMethod_cache;
    (*cache)[method] = (PyObject *)obj;
}

void
ObjCMethod_cache_del(PyObject *module, Method method)
{
    objctypes_state *state = (objctypes_state *)PyModule_GetState(module);
    cache_map *cache = (cache_map *)state->ObjCMethod_cache;
    cache->erase(method);
}

// Cache ObjCSelector

void *
ObjCSelector_cache_alloc(void)
{
    return new (std::nothrow) cache_map();
}

void
ObjCSelector_cache_dealloc(void *cache)
{
    delete (cache_map *)cache;
}

ObjCSelectorObject *
ObjCSelector_cache_get(PyObject *module, SEL sel)
{
    objctypes_state *state = (objctypes_state *)PyModule_GetState(module);
    cache_map *cache = (cache_map *)state->ObjCSelector_cache;

    const auto it = cache->find(sel);
    if (it != cache->end()) {
        return (ObjCSelectorObject *)Py_NewRef(it->second);
    }
    return NULL;
}

void
ObjCSelector_cache_set(PyObject *module, SEL sel, ObjCSelectorObject *obj)
{
    objctypes_state *state = (objctypes_state *)PyModule_GetState(module);
    cache_map *cache = (cache_map *)state->ObjCSelector_cache;
    (*cache)[sel] = (PyObject *)obj;
}

void
ObjCSelector_cache_del(PyObject *module, SEL sel)
{
    objctypes_state *state = (objctypes_state *)PyModule_GetState(module);
    cache_map *cache = (cache_map *)state->ObjCSelector_cache;
    cache->erase(sel);
}
