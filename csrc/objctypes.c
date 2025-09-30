#include <Python.h>
#include <objc/objc.h>

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
    PyObject *module;
    PyObject *objc_yes, *objc_no;

    module = PyModule_Create(&objctypesmodule);
    if (module == NULL) {
        return NULL;
    }

    // Add ObjCClass
    ObjCClassType.tp_base = &PyType_Type;
    PyModule_AddType(module, &ObjCClassType);
    Py_XDECREF(&ObjCClassType);

    // Add ObjCObject
    Py_SET_TYPE(&ObjCObjectType, &ObjCClassType);
    PyModule_AddType(module, (PyTypeObject *)&ObjCObjectType);
    Py_XDECREF(&ObjCObjectType);

    // Add ObjCMethod
    PyModule_AddType(module, &ObjCMethodType);
    Py_XDECREF(&ObjCMethodType);

    // Add ObjCSelector
    PyModule_AddType(module, &ObjCSelectorType);
    Py_XDECREF(&ObjCSelectorType);

    // Add ObjCBool
    PyModule_AddType(module, &ObjCBoolType);
    Py_XDECREF(&ObjCBoolType);

    // Add YES
    objc_yes = ObjCBoolType.tp_new(&ObjCBoolType, PyTuple_Pack(1, Py_True),
                                   PyDict_New());
    PyModule_AddObjectRef(module, "YES", objc_yes);
    Py_XDECREF(objc_yes);

    // Add NO
    objc_no = ObjCBoolType.tp_new(&ObjCBoolType, PyTuple_Pack(1, Py_False),
                                  PyDict_New());
    PyModule_AddObjectRef(module, "NO", objc_no);
    Py_XDECREF(objc_no);

    return module;
}
