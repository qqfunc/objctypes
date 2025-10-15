#include <Python.h>

#include "objctypes.h"

#include "objctypes_module.h"

static int
objctypes_module_exec(PyObject *module)
{
    // Get the module state

    // Add ObjCClass
    ObjCClassType.tp_base = &PyType_Type;
    if (PyModule_AddType(module, &ObjCClassType) < 0) {
        return -1;
    }

    // Add ObjCObject
    Py_SET_TYPE(&ObjCObjectType, &ObjCClassType);
    if (PyModule_AddType(module, (PyTypeObject *)&ObjCObjectType) < 0) {
        return -1;
    }

    // Add ObjCMethod
    if (PyModule_AddType(module, &ObjCMethodType) < 0) {
        return -1;
    }

    // Add ObjCSelector
    if (PyModule_AddType(module, &ObjCSelectorType) < 0) {
        return -1;
    }

    // Add ObjCBool
    if (PyModule_AddType(module, &ObjCBoolType) < 0) {
        return -1;
    }

    // Add YES
    PyObject *objc_yes = ObjCBool_FromLong(1);
    if (PyModule_Add(module, "YES", objc_yes) < 0) {
        return -1;
    }

    // Add NO
    PyObject *objc_no = ObjCBool_FromLong(0);
    if (PyModule_Add(module, "NO", objc_no) < 0) {
        return -1;
    }

    return 0;
}

static int
objctypes_module_traverse(PyObject *Py_UNUSED(module),
                          visitproc Py_UNUSED(visit), void *Py_UNUSED(arg))
{
    // objctypes_state *state = PyModule_GetState(module);
    return 0;
}

static int
objctypes_module_clear(PyObject *Py_UNUSED(module))
{
    // objctypes_state *state = PyModule_GetState(module);
    return 0;
}

static void
objctypes_module_free(void *module)
{
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
