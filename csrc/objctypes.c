#include <Python.h>

#include "objctypes.h"

#include "objcbool.h"
#include "objcclass.h"
#include "objcmethod.h"
#include "objcobject.h"
#include "objcselector.h"
#include "objctypes_cache.h"
#include "objctypes_module.h"

static int
objctypes_module_exec(PyObject *module)
{
    // Get the module state
    objctypes_state *state = PyModule_GetState(module);

    state->ObjCBool_Type =
        (PyTypeObject *)PyType_FromModuleAndSpec(module, &ObjCBool_spec, NULL);
    if (state->ObjCBool_Type == NULL) {
        return -1;
    }

    state->ObjCSelector_Type = (PyTypeObject *)PyType_FromModuleAndSpec(
        module, &ObjCSelector_spec, NULL);
    if (state->ObjCSelector_Type == NULL) {
        return -1;
    }

    state->ObjCSelector_cache = ObjCSelector_cache_alloc();
    if (state->ObjCSelector_cache == NULL) {
        return -1;
    }

    state->ObjCClass_Type = (PyTypeObject *)PyType_FromModuleAndSpec(
        module, &ObjCClass_spec, (PyObject *)&PyType_Type);
    if (state->ObjCClass_Type == NULL) {
        return -1;
    }

    state->ObjCClass_cache = ObjCClass_cache_alloc();
    if (state->ObjCClass_cache == NULL) {
        return -1;
    }

    state->ObjCObject_Type = (PyTypeObject *)PyType_FromMetaclass(
        state->ObjCClass_Type, module, &ObjCObject_spec, NULL);
    if (state->ObjCObject_Type == NULL) {
        return -1;
    }

    state->ObjCObject_cache = ObjCObject_cache_alloc();
    if (state->ObjCObject_cache == NULL) {
        return -1;
    }

    state->ObjCMethod_Type = (PyTypeObject *)PyType_FromModuleAndSpec(
        module, &ObjCMethod_spec, NULL);
    if (state->ObjCMethod_Type == NULL) {
        return -1;
    }

    state->ObjCMethod_cache = ObjCMethod_cache_alloc();
    if (state->ObjCMethod_cache == NULL) {
        return -1;
    }

    // Add ObjCObject
    if (PyModule_AddType(module, (PyTypeObject *)state->ObjCObject_Type) < 0) {
        return -1;
    }

    // Add ObjCClass
    if (PyModule_AddType(module, (PyTypeObject *)state->ObjCClass_Type) < 0) {
        return -1;
    }

    // Add ObjCMethod
    if (PyModule_AddType(module, (PyTypeObject *)state->ObjCMethod_Type) < 0) {
        return -1;
    }

    // Add ObjCSelector
    if (PyModule_AddType(module, (PyTypeObject *)state->ObjCSelector_Type)
        < 0) {
        return -1;
    }

    // Add ObjCBool
    if (PyModule_AddType(module, state->ObjCBool_Type) < 0) {
        return -1;
    }

    // Add YES
    PyObject *ObjCBool_YES = ObjCBool_FromLong(module, 1);
    if (PyModule_Add(module, "YES", ObjCBool_YES) < 0) {
        return -1;
    }

    // Add NO
    PyObject *ObjCBool_NO = ObjCBool_FromLong(module, 0);
    if (PyModule_Add(module, "NO", ObjCBool_NO) < 0) {
        return -1;
    }

    return 0;
}

static int
objctypes_module_traverse(PyObject *module, visitproc visit, void *arg)
{
    objctypes_state *state = PyModule_GetState(module);
    Py_VISIT(state->ObjCBool_Type);
    Py_VISIT(state->ObjCBool_YES);
    Py_VISIT(state->ObjCBool_NO);
    Py_VISIT(state->ObjCClass_Type);
    Py_VISIT(state->ObjCMethod_Type);
    Py_VISIT(state->ObjCObject_Type);
    Py_VISIT(state->ObjCSelector_Type);
    return 0;
}

static int
objctypes_module_clear(PyObject *module)
{
    objctypes_state *state = PyModule_GetState(module);
    Py_CLEAR(state->ObjCBool_Type);
    Py_CLEAR(state->ObjCBool_YES);
    Py_CLEAR(state->ObjCBool_NO);
    Py_CLEAR(state->ObjCClass_Type);
    Py_CLEAR(state->ObjCMethod_Type);
    Py_CLEAR(state->ObjCObject_Type);
    Py_CLEAR(state->ObjCSelector_Type);
    return 0;
}

static void
objctypes_module_free(void *module)
{
    objctypes_state *state = (objctypes_state *)PyModule_GetState(module);

    ObjCClass_cache_dealloc(state->ObjCClass_cache);

    ObjCMethod_cache_dealloc(state->ObjCMethod_cache);

    ObjCObject_cache_dealloc(state->ObjCObject_cache);

    ObjCSelector_cache_dealloc(state->ObjCSelector_cache);

    objctypes_module.m_clear(module);
}

static PyModuleDef_Slot objctypes_module_slots[] = {
    {Py_mod_exec, objctypes_module_exec},
    {Py_mod_multiple_interpreters, Py_MOD_MULTIPLE_INTERPRETERS_NOT_SUPPORTED},
    {0, NULL},
};

PyModuleDef objctypes_module = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "objctypes",
    .m_doc = "A bridge between Objective-C and Python.",
    .m_size = sizeof(objctypes_state),
    .m_slots = objctypes_module_slots,
    .m_traverse = objctypes_module_traverse,
    .m_clear = objctypes_module_clear,
    .m_free = (freefunc)objctypes_module_free,
};

PyMODINIT_FUNC
PyInit_objctypes(void)
{
    return PyModuleDef_Init(&objctypes_module);
}
