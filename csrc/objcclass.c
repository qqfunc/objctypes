/**
 * @file objcclass.c
 * @brief Source declarations and definitions for objcclass.c.
 */

#include <Python.h>

#include "objcclass.h"

#include "objctypes.h"
#include "objctypes_cache.h"
#include "objctypes_module.h"

/// @brief Destruct an ObjCClass.
static void
ObjCClass_dealloc(PyObject *self)
{
    PyObject *module = PyType_GetModuleByDef(Py_TYPE(self), &objctypes_module);
    if (module != NULL) {
        objctypes_state *state = PyModule_GetState(module);
        ObjCClassState *cls_state =
            PyObject_GetTypeData(self, state->ObjCClass_Type);
        if (cls_state != NULL) {
            PyMutex_Lock(&state->ObjCClass_cache_mutex);
            ObjCClass_cache_del(module, cls_state->value);
            PyMutex_Unlock(&state->ObjCClass_cache_mutex);
        }
    }
    Py_TYPE(self)->tp_free((PyObject *)self);
}

/// @brief `ObjCClass.__repr__()`
static PyObject *
ObjCClass_repr(PyObject *self)
{
    PyObject *module = PyType_GetModuleByDef(Py_TYPE(self), &objctypes_module);
    if (module == NULL) {
        return NULL;
    }

    objctypes_state *state = PyModule_GetState(module);
    ObjCClassState *cls_state =
        PyObject_GetTypeData(self, state->ObjCClass_Type);

    if (cls_state->value == NULL) {
        return PyUnicode_FromString("<class 'objctypes.ObjCObject'>");
    }
    return PyUnicode_FromFormat("<ObjCClass '%s'>",
                                class_getName(cls_state->value));
}

/// @brief `ObjCClass.address`
static PyObject *
ObjCClass_address(PyObject *self, void *Py_UNUSED(closure))
{
    PyObject *module = PyType_GetModuleByDef(Py_TYPE(self), &objctypes_module);
    if (module == NULL) {
        return NULL;
    }

    objctypes_state *state = PyModule_GetState(module);
    ObjCClassState *cls_state =
        PyObject_GetTypeData(self, state->ObjCClass_Type);

    return PyLong_FromVoidPtr(cls_state->value);
}

/// @brief `ObjCClass.name`
static PyObject *
ObjCClass_name(PyObject *self, PyObject *Py_UNUSED(closure))
{
    PyObject *module = PyType_GetModuleByDef(Py_TYPE(self), &objctypes_module);
    if (module == NULL) {
        return NULL;
    }

    objctypes_state *state = PyModule_GetState(module);
    ObjCClassState *cls_state =
        PyObject_GetTypeData(self, state->ObjCClass_Type);

    if (cls_state->value == NULL) {
        return Py_GetConstant(Py_CONSTANT_EMPTY_STR);
    }
    return PyUnicode_FromString(class_getName(cls_state->value));
}

/// @brief `ObjCClass.load_methods`
static PyObject *
ObjCClass_load_methods(PyObject *self, PyObject *Py_UNUSED(args))
{
    PyObject *module = PyType_GetModuleByDef(Py_TYPE(self), &objctypes_module);
    if (module == NULL) {
        return NULL;
    }

    objctypes_state *state = PyModule_GetState(module);
    ObjCClassState *cls_state =
        PyObject_GetTypeData(self, state->ObjCClass_Type);

    unsigned int outCount;
    Method *methods = class_copyMethodList(cls_state->value, &outCount);
    for (unsigned int num = 0; num < outCount; num++) {
        printf("Method %s\n", sel_getName(method_getName(methods[num])));
    }
    free(methods);

    return Py_None;
}

/// @brief Get an ObjCClass from a Python type and an Objective-C Class.
static PyObject *
_ObjCClass_FromClass(PyTypeObject *type, Class cls, int lock_cache)
{
    PyObject *module = PyType_GetModuleByDef(type, &objctypes_module);
    if (module == NULL) {
        return NULL;
    }

    objctypes_state *state = PyModule_GetState(module);

    if (lock_cache) {
        PyMutex_Lock(&state->ObjCClass_cache_mutex);
    }

    PyObject *self = ObjCClass_cache_get(module, cls);

    if (self == NULL) {
        objctypes_state *state = PyModule_GetState(module);

        PyObject *base;

        Class super_cls = class_getSuperclass(cls);
        if (super_cls == NULL) {
            // The class is a root class
            base = (PyObject *)state->ObjCObject_Type;
        }
        else {
            // Retrieve the superclass of the Objective-C class
            base = _ObjCClass_FromClass(type, super_cls, 0);
            if (base == NULL) {
                if (lock_cache) {
                    PyMutex_Unlock(&state->ObjCClass_cache_mutex);
                }
                return NULL;
            }
        }

        PyObject *args = Py_BuildValue("(s(O){})", class_getName(cls), base);
        PyObject *kwds = PyDict_New();
        self = PyType_Type.tp_new(type, args, kwds);
        Py_XDECREF(args);
        Py_XDECREF(kwds);

        if (self != NULL) {
            ObjCClassState *cls_state =
                PyObject_GetTypeData(self, state->ObjCClass_Type);
            cls_state->value = cls;
            ObjCClass_cache_set(module, cls, self);
        }
    }

    if (lock_cache) {
        PyMutex_Unlock(&state->ObjCClass_cache_mutex);
    }

    return self;
}

/// @brief `ObjCClass.__init__()`
static int
ObjCClass_init(PyObject *self, PyObject *args, PyObject *kwds)
{
    static char *kwlist[] = {"", "", "", NULL};
    char *name;
    PyObject *bases = NULL, *dict = NULL;

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "s|O!O!:ObjCClass", kwlist,
                                     &name, &PyTuple_Type, &bases,
                                     &PyDict_Type, &dict)) {
        return -1;
    }

    PyObject *module = PyType_GetModuleByDef(Py_TYPE(self), &objctypes_module);
    objctypes_state *state = PyModule_GetState(module);
    ObjCClassState *cls_state =
        PyObject_GetTypeData(self, state->ObjCClass_Type);
    cls_state->value = NULL;

    return 0;
}

/// @brief `ObjCClass.from_address()`
static PyObject *
ObjCClass_from_address(PyTypeObject *type, PyObject *address)
{
    if (!PyLong_Check(address)) {
        PyErr_Format(PyExc_TypeError,
                     "ObjCClass.from_address() argument 1 must be int, not %T",
                     address);
        return NULL;
    }

    Class cls = PyLong_AsVoidPtr(address);

    // Make sure the class is not Nil.
    if (cls == NULL) {
        PyErr_SetString(PyExc_TypeError,
                        "the specified Objective-C class is Nil");
        return NULL;
    }

    // Make sure the class is an Objective-C class.
    if (!object_isClass((id)cls)) {
        PyErr_Format(PyExc_TypeError,
                     "The Objective-C object at %p is not a class. Use "
                     "ObjCObject.from_address() instead.",
                     cls);
        return NULL;
    }

    // Make sure the class is not a metaclass.
    if (class_isMetaClass(cls)) {
        PyErr_Format(PyExc_TypeError,
                     "The Objective-C class at %p is a metaclass. Use "
                     "ObjCMetaClass.from_address() instead.",
                     cls);
        return NULL;
    }

    return (PyObject *)_ObjCClass_FromClass(type, cls, 1);
}

/// @brief `ObjCClass.from_name()`
static PyObject *
ObjCClass_from_name(PyTypeObject *type, PyObject *name)
{
    PyObject *self;

    if (!PyUnicode_Check(name)) {
        PyErr_Format(PyExc_TypeError,
                     "ObjCClass.from_name() argument 1 must be str, not %T",
                     name);
        return NULL;
    }

    PyObject *module = PyType_GetModuleByDef(type, &objctypes_module);
    if (module == NULL) {
        return NULL;
    }

    const char *cls_name = PyUnicode_AsUTF8(name);

    Class cls = objc_getClass(cls_name);
    if (cls == NULL) {
        PyErr_Format(PyExc_NameError, "Objective-C class '%s' is not defined",
                     cls_name);
        return NULL;
    }

    self = _ObjCClass_FromClass(type, cls, 1);

    return (PyObject *)self;
}

static PyMethodDef ObjCClass_methods[] = {
    {
        "from_address",
        (PyCFunction)ObjCClass_from_address,
        METH_O | METH_CLASS,
        PyDoc_STR("Get an ObjCClass from the memory address."),
    },
    {
        "from_name",
        (PyCFunction)ObjCClass_from_name,
        METH_O | METH_CLASS,
        PyDoc_STR("Get an ObjCClass from the class name."),
    },
    {
        "load_methods",
        (PyCFunction)ObjCClass_load_methods,
        METH_NOARGS,
        PyDoc_STR("Load or reload all methods of the class."),
    },
    {.ml_name = NULL},
};

static PyGetSetDef ObjCClass_getset[] = {
    {
        "address",
        (getter)ObjCClass_address,
        NULL,
        PyDoc_STR("The address of the Objective-C class."),
        NULL,
    },
    {
        "name",
        (getter)ObjCClass_name,
        NULL,
        PyDoc_STR("The name of the Objective-C class."),
        NULL,
    },
    {.name = NULL},
};

static PyType_Slot ObjCClass_slots[] = {
    {Py_tp_dealloc, ObjCClass_dealloc},
    {Py_tp_repr, ObjCClass_repr},
    {Py_tp_doc, "Python wrapper for Objective-C Class."},
    {Py_tp_methods, ObjCClass_methods},
    {Py_tp_getset, ObjCClass_getset},
    {Py_tp_init, ObjCClass_init},
    {0, NULL},
};

PyType_Spec ObjCClass_spec = {
    .name = "objctypes.ObjCClass",
    .basicsize = -(long)sizeof(ObjCClassState),
    .itemsize = 0,
    .flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_TYPE_SUBCLASS,
    .slots = ObjCClass_slots,
};

PyObject *
ObjCClass_FromClass(PyObject *module, Class cls)
{
    objctypes_state *state = PyModule_GetState(module);
    if (state->ObjCClass_Type == NULL) {
        return NULL;
    }

    return (PyObject *)_ObjCClass_FromClass(
        (PyTypeObject *)state->ObjCClass_Type, cls, 1);
}
