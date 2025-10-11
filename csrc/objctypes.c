#include <Python.h>

#include "objctypes.h"

static int
objctypes_module_exec(PyObject *m)
{
    // Add ObjCClass
    ObjCClassType.tp_base = &PyType_Type;
    if (PyModule_AddType(m, &ObjCClassType) < 0) {
        return -1;
    }

    // Add ObjCObject
    Py_SET_TYPE(&ObjCObjectType, &ObjCClassType);
    if (PyModule_AddType(m, (PyTypeObject *)&ObjCObjectType) < 0) {
        return -1;
    }

    // Add ObjCMethod
    if (PyModule_AddType(m, &ObjCMethodType) < 0) {
        return -1;
    }

    // Add ObjCSelector
    if (PyModule_AddType(m, &ObjCSelectorType) < 0) {
        return -1;
    }

    // Add ObjCBool
    if (PyModule_AddType(m, &ObjCBoolType) < 0) {
        return -1;
    }

    // Add YES
    PyObject *objc_yes = ObjCBool_FromLong(1);
    if (PyModule_Add(m, "YES", objc_yes) < 0) {
        return -1;
    }

    // Add NO
    PyObject *objc_no = ObjCBool_FromLong(0);
    if (PyModule_Add(m, "NO", objc_no) < 0) {
        return -1;
    }

    return 0;
}

static PyModuleDef_Slot objctypes_module_slots[] = {
    {Py_mod_exec, objctypes_module_exec},
    {Py_mod_multiple_interpreters, Py_MOD_MULTIPLE_INTERPRETERS_NOT_SUPPORTED},
    {0, NULL}
};

static PyModuleDef objctypes_module = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "objctypes",
    .m_doc = "A bridge between Objective-C and Python.",
    .m_size = 0,
    .m_slots = objctypes_module_slots,
};

PyMODINIT_FUNC
PyInit_objctypes(void)
{
    return PyModuleDef_Init(&objctypes_module);
}
