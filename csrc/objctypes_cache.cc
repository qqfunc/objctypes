/**
 * @file objctypes_cache.cc
 * @brief Source declarations and definitions for objctypes_cache.cc.
 */

#include <Python.h>

#include "objctypes_cache.h"

#include "objctypes_module.h"

#include <map>

typedef std::map<void *, PyObject *> cache_map;

void
ObjCMetaClass_cache_init(PyObject *module)
{
    objctypes_state *state = (objctypes_state *)PyModule_GetState(module);
    state->ObjCMetaClass_cache = new (std::nothrow) cache_map();
}

void
ObjCMetaClass_cache_deinit(PyObject *module)
{
    objctypes_state *state = (objctypes_state *)PyModule_GetState(module);
    delete (cache_map *)state->ObjCMetaClass_cache;
}

PyObject *
ObjCMetaClass_cache_get(PyObject *module, Class cls)
{
    objctypes_state *state = (objctypes_state *)PyModule_GetState(module);
    cache_map *cache = (cache_map *)state->ObjCMetaClass_cache;

    const auto it = cache->find(cls);
    if (it != cache->end()) {
        return Py_NewRef(it->second);
    }
    return NULL;
}

void
ObjCMetaClass_cache_set(PyObject *module, Class cls, PyObject *obj)
{
    objctypes_state *state = (objctypes_state *)PyModule_GetState(module);
    cache_map *cache = (cache_map *)state->ObjCMetaClass_cache;
    (*cache)[cls] = obj;
}

void
ObjCMetaClass_cache_del(PyObject *module, Class cls)
{
    objctypes_state *state = (objctypes_state *)PyModule_GetState(module);
    cache_map *cache = (cache_map *)state->ObjCMetaClass_cache;
    cache->erase(cls);
}

void
ObjCClass_cache_init(PyObject *module)
{
    objctypes_state *state = (objctypes_state *)PyModule_GetState(module);
    state->ObjCClass_cache = new (std::nothrow) cache_map();
}

void
ObjCClass_cache_deinit(PyObject *module)
{
    objctypes_state *state = (objctypes_state *)PyModule_GetState(module);
    delete (cache_map *)state->ObjCClass_cache;
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
    (*cache)[cls] = obj;
}

void
ObjCClass_cache_del(PyObject *module, Class cls)
{
    objctypes_state *state = (objctypes_state *)PyModule_GetState(module);
    cache_map *cache = (cache_map *)state->ObjCClass_cache;
    cache->erase(cls);
}

void
ObjCObject_cache_init(PyObject *module)
{
    objctypes_state *state = (objctypes_state *)PyModule_GetState(module);
    state->ObjCObject_cache = new (std::nothrow) cache_map();
}

void
ObjCObject_cache_deinit(PyObject *module)
{
    objctypes_state *state = (objctypes_state *)PyModule_GetState(module);
    delete (cache_map *)state->ObjCObject_cache;
}

PyObject *
ObjCObject_cache_get(PyObject *module, id obj)
{
    objctypes_state *state = (objctypes_state *)PyModule_GetState(module);
    cache_map *cache = (cache_map *)state->ObjCObject_cache;

    const auto it = cache->find(obj);
    if (it != cache->end()) {
        return Py_NewRef(it->second);
    }
    return NULL;
}

void
ObjCObject_cache_set(PyObject *module, id obj, PyObject *pyobj)
{
    objctypes_state *state = (objctypes_state *)PyModule_GetState(module);
    cache_map *cache = (cache_map *)state->ObjCObject_cache;
    (*cache)[obj] = pyobj;
}

void
ObjCObject_cache_del(PyObject *module, id obj)
{
    objctypes_state *state = (objctypes_state *)PyModule_GetState(module);
    cache_map *cache = (cache_map *)state->ObjCObject_cache;
    cache->erase(obj);
}

void
ObjCMethod_cache_init(PyObject *module)
{
    objctypes_state *state = (objctypes_state *)PyModule_GetState(module);
    state->ObjCMethod_cache = new (std::nothrow) cache_map();
}

void
ObjCMethod_cache_deinit(PyObject *module)
{
    objctypes_state *state = (objctypes_state *)PyModule_GetState(module);
    delete (cache_map *)state->ObjCMethod_cache;
}

PyObject *
ObjCMethod_cache_get(PyObject *module, Method method)
{
    objctypes_state *state = (objctypes_state *)PyModule_GetState(module);
    cache_map *cache = (cache_map *)state->ObjCMethod_cache;

    const auto it = cache->find(method);
    if (it != cache->end()) {
        return Py_NewRef(it->second);
    }
    return NULL;
}

void
ObjCMethod_cache_set(PyObject *module, Method method, PyObject *obj)
{
    objctypes_state *state = (objctypes_state *)PyModule_GetState(module);
    cache_map *cache = (cache_map *)state->ObjCMethod_cache;
    (*cache)[method] = obj;
}

void
ObjCMethod_cache_del(PyObject *module, Method method)
{
    objctypes_state *state = (objctypes_state *)PyModule_GetState(module);
    cache_map *cache = (cache_map *)state->ObjCMethod_cache;
    cache->erase(method);
}

void
ObjCSelector_cache_init(PyObject *module)
{
    objctypes_state *state = (objctypes_state *)PyModule_GetState(module);
    state->ObjCSelector_cache = new (std::nothrow) cache_map();
}

void
ObjCSelector_cache_deinit(PyObject *module)
{
    objctypes_state *state = (objctypes_state *)PyModule_GetState(module);
    delete (cache_map *)state->ObjCSelector_cache;
}

PyObject *
ObjCSelector_cache_get(PyObject *module, SEL sel)
{
    objctypes_state *state = (objctypes_state *)PyModule_GetState(module);
    cache_map *cache = (cache_map *)state->ObjCSelector_cache;

    const auto it = cache->find(sel);
    if (it != cache->end()) {
        return Py_NewRef(it->second);
    }
    return NULL;
}

void
ObjCSelector_cache_set(PyObject *module, SEL sel, PyObject *obj)
{
    objctypes_state *state = (objctypes_state *)PyModule_GetState(module);
    cache_map *cache = (cache_map *)state->ObjCSelector_cache;
    (*cache)[sel] = obj;
}

void
ObjCSelector_cache_del(PyObject *module, SEL sel)
{
    objctypes_state *state = (objctypes_state *)PyModule_GetState(module);
    cache_map *cache = (cache_map *)state->ObjCSelector_cache;
    cache->erase(sel);
}
