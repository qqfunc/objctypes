#include <Python.h>

#include "objctypes.h"

static PyModuleDef objctypesmodule = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "objctypes",
    .m_doc = "A bridge between Objective-C and Python.",
    .m_size = -1,
};

PyMODINIT_FUNC
PyInit_objctypes(void)
{
    PyObject *module = PyModule_Create(&objctypesmodule);
    if (module == NULL) {
        return NULL;
    }

    // Add ObjCClass
    ObjCClassType.tp_base = &PyType_Type;
    if (PyModule_AddType(module, &ObjCClassType) < 0) {
        return NULL;
    }
    Py_XDECREF(&ObjCClassType);

    // Add ObjCObject
    Py_SET_TYPE(&ObjCObjectType, &ObjCClassType);
    if (PyModule_AddType(module, (PyTypeObject *)&ObjCObjectType) < 0) {
        return NULL;
    }
    Py_XDECREF(&ObjCObjectType);

    // Add ObjCMethod
    if (PyModule_AddType(module, &ObjCMethodType) < 0) {
        return NULL;
    }
    Py_XDECREF(&ObjCMethodType);

    // Add ObjCSelector
    if (PyModule_AddType(module, &ObjCSelectorType) < 0) {
        return NULL;
    }
    Py_XDECREF(&ObjCSelectorType);

    // Add ObjCBool
    if (PyModule_AddType(module, &ObjCBoolType) < 0) {
        return NULL;
    }
    Py_XDECREF(&ObjCBoolType);

    // Add YES
    PyObject *objc_yes = ObjCBool_FromInt(1);
    if (PyModule_Add(module, "YES", objc_yes) < 0) {
        return NULL;
    }

    // Add NO
    PyObject *objc_no = ObjCBool_FromInt(0);
    if (PyModule_Add(module, "NO", objc_no) < 0) {
        return NULL;
    }

    return module;
}
