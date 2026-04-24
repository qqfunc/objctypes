/**
 * @file objcmetaclass.c
 * @brief Source declarations and definitions for objcmetaclass.c.
 */

#include <Python.h>

#include "objcmetaclass.h"

#include "objctypes.h"
#include "objctypes_cache.h"
#include "objctypes_module.h"

/// @brief Destruct an ObjCMetaclass.
static void
ObjCMetaclass_dealloc(PyObject *self)
{
    PyObject *module = PyType_GetModuleByDef(Py_TYPE(self), &objctypes_module);
    if (module != NULL) {
        objctypes_state *state = PyModule_GetState(module);
        ObjCMetaclassState *cls_state =
            PyObject_GetTypeData(self, state->ObjCMetaclass_Type);
        if (cls_state != NULL) {
            PyMutex_Lock(&state->ObjCMetaclass_cache_mutex);
            ObjCMetaclass_cache_del(module, cls_state->value);
            PyMutex_Unlock(&state->ObjCMetaclass_cache_mutex);
        }
    }
    Py_TYPE(self)->tp_free((PyObject *)self);
}

/// @brief `ObjCMetaclass.__repr__()`
static PyObject *
ObjCMetaclass_repr(PyObject *self)
{
    PyObject *module = PyType_GetModuleByDef(Py_TYPE(self), &objctypes_module);
    if (module == NULL) {
        return NULL;
    }

    objctypes_state *state = PyModule_GetState(module);
    ObjCMetaclassState *cls_state =
        PyObject_GetTypeData(self, state->ObjCMetaclass_Type);

    if (cls_state->value == NULL) {
        return PyUnicode_FromString("<ObjCMetaclass>");
    }
    return PyUnicode_FromFormat("<ObjCMetaclass '%s'>",
                                class_getName(cls_state->value));
}

/// @brief `ObjCMetaclass.address`
static PyObject *
ObjCMetaclass_address(PyObject *self, void *Py_UNUSED(closure))
{
    PyObject *module = PyType_GetModuleByDef(Py_TYPE(self), &objctypes_module);
    if (module == NULL) {
        return NULL;
    }

    objctypes_state *state = PyModule_GetState(module);
    ObjCMetaclassState *cls_state =
        PyObject_GetTypeData(self, state->ObjCMetaclass_Type);

    return PyLong_FromVoidPtr(cls_state->value);
}

/// @brief `ObjCMetaclass.name`
static PyObject *
ObjCMetaclass_name(PyObject *self, PyObject *Py_UNUSED(closure))
{
    PyObject *module = PyType_GetModuleByDef(Py_TYPE(self), &objctypes_module);
    if (module == NULL) {
        return NULL;
    }

    objctypes_state *state = PyModule_GetState(module);
    ObjCMetaclassState *cls_state =
        PyObject_GetTypeData(self, state->ObjCMetaclass_Type);

    if (cls_state->value == NULL) {
        return Py_GetConstant(Py_CONSTANT_EMPTY_STR);
    }
    return PyUnicode_FromString(class_getName(cls_state->value));
}

/// @brief Get an ObjCMetaclass from a Python type and an Objective-C
/// metaclass.
static PyObject *
_ObjCMetaclass_FromClass(PyTypeObject *type, Class cls, int lock_cache)
{
    PyObject *module = PyType_GetModuleByDef(type, &objctypes_module);
    if (module == NULL) {
        return NULL;
    }

    objctypes_state *state = PyModule_GetState(module);

    if (lock_cache) {
        PyMutex_Lock(&state->ObjCMetaclass_cache_mutex);
    }

    PyObject *self = ObjCMetaclass_cache_get(module, cls);

    if (self == NULL) {
        PyObject *args = Py_BuildValue("(s(O){})", class_getName(cls),
                                       &PyBaseObject_Type);
        PyObject *kwds = PyDict_New();
        self = PyType_Type.tp_new(type, args, kwds);
        Py_XDECREF(args);
        Py_XDECREF(kwds);

        if (self != NULL) {
            ObjCMetaclassState *cls_state =
                PyObject_GetTypeData(self, state->ObjCMetaclass_Type);
            cls_state->value = cls;
            ObjCMetaclass_cache_set(module, cls, self);
        }
    }

    if (lock_cache) {
        PyMutex_Unlock(&state->ObjCMetaclass_cache_mutex);
    }

    return self;
}

/// @brief `ObjCMetaclass.from_address()`
static PyObject *
ObjCMetaclass_from_address(PyTypeObject *type, PyObject *address)
{
    if (!PyLong_Check(address)) {
        PyErr_Format(
            PyExc_TypeError,
            "ObjCMetaclass.from_address() argument 1 must be int, not %T",
            address);
        return NULL;
    }

    Class cls = PyLong_AsVoidPtr(address);

    // Make sure the metaclass is not Nil.
    if (cls == NULL) {
        PyErr_SetString(PyExc_TypeError,
                        "the specified Objective-C metaclass is Nil");
        return NULL;
    }

    // Make sure the pointer refers to an Objective-C class.
    if (!object_isClass((id)cls)) {
        PyErr_Format(PyExc_TypeError,
                     "The Objective-C object at %p is not a class.",
                     cls);
        return NULL;
    }

    // Make sure the class is a metaclass.
    if (!class_isMetaClass(cls)) {
        PyErr_Format(PyExc_TypeError,
                     "The Objective-C class at %p is not a metaclass. Use "
                     "ObjCClass.from_address() instead.",
                     cls);
        return NULL;
    }

    return _ObjCMetaclass_FromClass(type, cls, 1);
}

/// @brief `ObjCMetaclass.from_name()`
static PyObject *
ObjCMetaclass_from_name(PyTypeObject *type, PyObject *name)
{
    if (!PyUnicode_Check(name)) {
        PyErr_Format(
            PyExc_TypeError,
            "ObjCMetaclass.from_name() argument 1 must be str, not %T",
            name);
        return NULL;
    }

    PyObject *module = PyType_GetModuleByDef(type, &objctypes_module);
    if (module == NULL) {
        return NULL;
    }

    const char *cls_name = PyUnicode_AsUTF8(name);

    Class cls = objc_getMetaClass(cls_name);
    if (cls == NULL) {
        PyErr_Format(PyExc_NameError, "Objective-C class '%s' is not defined",
                     cls_name);
        return NULL;
    }

    return _ObjCMetaclass_FromClass(type, cls, 1);
}

static PyMethodDef ObjCMetaclass_methods[] = {
    {
        "from_address",
        (PyCFunction)ObjCMetaclass_from_address,
        METH_O | METH_CLASS,
        PyDoc_STR("Get an ObjCMetaclass from the memory address."),
    },
    {
        "from_name",
        (PyCFunction)ObjCMetaclass_from_name,
        METH_O | METH_CLASS,
        PyDoc_STR("Get an ObjCMetaclass from the class name."),
    },
    {.ml_name = NULL},
};

static PyGetSetDef ObjCMetaclass_getset[] = {
    {
        "address",
        (getter)ObjCMetaclass_address,
        NULL,
        PyDoc_STR("The address of the Objective-C metaclass."),
        NULL,
    },
    {
        "name",
        (getter)ObjCMetaclass_name,
        NULL,
        PyDoc_STR("The name of the Objective-C metaclass."),
        NULL,
    },
    {.name = NULL},
};

static PyType_Slot ObjCMetaclass_slots[] = {
    {Py_tp_dealloc, ObjCMetaclass_dealloc},
    {Py_tp_repr, ObjCMetaclass_repr},
    {Py_tp_doc, "Python wrapper for Objective-C metaclass."},
    {Py_tp_methods, ObjCMetaclass_methods},
    {Py_tp_getset, ObjCMetaclass_getset},
    {0, NULL},
};

PyType_Spec ObjCMetaclass_spec = {
    .name = "objctypes.ObjCMetaclass",
    .basicsize = -(long)sizeof(ObjCMetaclassState),
    .itemsize = 0,
    .flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_TYPE_SUBCLASS,
    .slots = ObjCMetaclass_slots,
};

PyObject *
ObjCMetaclass_FromClass(PyObject *module, Class cls)
{
    objctypes_state *state = PyModule_GetState(module);
    if (state->ObjCMetaclass_Type == NULL) {
        return NULL;
    }

    return _ObjCMetaclass_FromClass(
        (PyTypeObject *)state->ObjCMetaclass_Type, cls, 1);
}
