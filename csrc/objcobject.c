/**
 * @file objcobject.c
 * @brief Source declarations and definitions for objcobject.c.
 */

#include <Python.h>

#include "objctypes.h"
#include "objctypes_cache.h"
#include "objctypes_module.h"

/// @brief Destruct an ObjCObject.
static void
ObjCObject_dealloc(PyObject *self)
{
    PyObject *module = PyType_GetModuleByDef(Py_TYPE(self), &objctypes_module);
    if (module != NULL) {
        objctypes_state *state = PyModule_GetState(module);
        ObjCObjectData *data =
            PyObject_GetTypeData(self, state->ObjCObject_Type);
        if (data != NULL) {
            PyMutex_Lock(&state->ObjCObject_cache_mutex);
            ObjCObject_cache_del(module, data->value);
            PyMutex_Unlock(&state->ObjCObject_cache_mutex);
        }
    }
    Py_TYPE(self)->tp_free(self);
}

/// @brief `ObjCObject.__repr__()`
static PyObject *
ObjCObject_repr(PyObject *self)
{
    // Get the type data of the ObjCObject object
    PyObject *module = PyType_GetModuleByDef(Py_TYPE(self), &objctypes_module);
    if (module == NULL) {
        return NULL;
    }
    objctypes_state *state = PyModule_GetState(module);
    ObjCObjectData *data = PyObject_GetTypeData(self, state->ObjCObject_Type);
    if (data == NULL) {
        return NULL;
    }

    return PyUnicode_FromFormat("<ObjCObject %s at %p>",
                                object_getClassName(data->value), data->value);
}

/// @brief `ObjCObject.address`
static PyObject *
ObjCObject_address(PyObject *self, void *Py_UNUSED(closure))
{
    PyObject *module = PyType_GetModuleByDef(Py_TYPE(self), &objctypes_module);
    if (module == NULL) {
        return NULL;
    }
    objctypes_state *state = PyModule_GetState(module);
    ObjCObjectData *data = PyObject_GetTypeData(self, state->ObjCObject_Type);
    if (data == NULL) {
        return NULL;
    }

    return PyLong_FromVoidPtr(data->value);
}

/// @brief Get an ObjCObject from a Python type and an Objective-C id.
static PyObject *
_ObjCObject_FromId(PyTypeObject *type, id obj)
{
    // Get the module state
    PyObject *module = PyType_GetModuleByDef(type, &objctypes_module);
    if (module == NULL) {
        return NULL;
    }
    objctypes_state *state = PyModule_GetState(module);

    PyMutex_Lock(&state->ObjCObject_cache_mutex);

    PyObject *self = ObjCObject_cache_get(module, obj);
    if (self == NULL) {
        self = type->tp_alloc(type, 0);
        if (self != NULL) {
            ObjCObjectData *data =
                PyObject_GetTypeData(self, state->ObjCObject_Type);
            if (data == NULL) {
                Py_DECREF(self);
                self = NULL;
            }
            else {
                data->value = obj;
                ObjCObject_cache_set(module, obj, self);
            }
        }
    }

    PyMutex_Unlock(&state->ObjCObject_cache_mutex);

    return self;
}

/// @brief `ObjCObject.from_address()`
static PyObject *
ObjCObject_from_address(PyTypeObject *type, PyObject *address)
{
    if (!PyLong_Check(address)) {
        PyErr_Format(
            PyExc_TypeError,
            "ObjCObject.from_address() argument 1 must be int, not %T",
            address);
        return NULL;
    }

    id obj = PyLong_AsVoidPtr(address);

    // Make sure the object is not nil.
    if (obj == NULL) {
        PyErr_SetString(PyExc_TypeError,
                        "the specified Objective-C object is nil");
        return NULL;
    }

    // Make sure the object is not Objective-C class.
    if (object_isClass(obj)) {
        // Make sure the object is not a metaclass.
        if (class_isMetaClass((Class)obj)) {
            PyErr_Format(PyExc_TypeError,
                         "The Objective-C object at %p is a metaclass. Use "
                         "ObjCMetaClass.from_address() instead.",
                         obj);
            return NULL;
        }
        else {
            PyErr_Format(PyExc_TypeError,
                         "The Objective-C object at %p is a class. Use "
                         "ObjCClass.from_address() instead.",
                         obj);
            return NULL;
        }
    }

    return _ObjCObject_FromId(type, obj);
}

/// @brief `ObjCObject.__new__()`
static PyObject *
ObjCObject_new(PyTypeObject *type, PyObject *Py_UNUSED(args),
               PyObject *Py_UNUSED(kwds))
{
    // Get the module state
    PyObject *module = PyType_GetModuleByDef(type, &objctypes_module);
    if (module == NULL) {
        return NULL;
    }
    objctypes_state *state = PyModule_GetState(module);

    if (type == state->ObjCObject_Type) {
        PyErr_SetString(PyExc_TypeError,
                        "ObjCObject cannot be initialized directly");
        return NULL;
    }

    PyErr_SetNone(PyExc_NotImplementedError);
    return NULL;
}

static PyMethodDef ObjCObject_methods[] = {
    {
        "__class_getitem__",
        Py_GenericAlias,
        METH_O | METH_CLASS,
        PyDoc_STR("Python wrapper for Objective-C Class."),
    },
    {
        "from_address",
        (PyCFunction)ObjCObject_from_address,
        METH_O | METH_CLASS,
        PyDoc_STR("Get an ObjCObject from the memory address."),
    },
    {.ml_name = NULL},
};

static PyGetSetDef ObjCObject_getset[] = {
    {
        "address",
        ObjCObject_address,
        NULL,
        PyDoc_STR("The address of the Objective-C object."),
        NULL,
    },
    {.name = NULL},
};

static PyType_Slot ObjCObject_slots[] = {
    {Py_tp_dealloc, ObjCObject_dealloc},
    {Py_tp_repr, ObjCObject_repr},
    {Py_tp_doc, "Python wrapper for Objective-C Class."},
    {Py_tp_methods, ObjCObject_methods},
    {Py_tp_getset, ObjCObject_getset},
    {Py_tp_new, ObjCObject_new},
    {0, NULL},
};

PyType_Spec ObjCObject_spec = {
    .name = "objctypes.ObjCObject",
    .basicsize = -(long)sizeof(ObjCObjectData),
    .itemsize = 0,
    .flags = Py_TPFLAGS_BASETYPE | Py_TPFLAGS_DEFAULT,
    .slots = ObjCObject_slots,
};

PyObject *
ObjCObject_FromId(PyObject *module, id obj)
{
    // Get the module state
    objctypes_state *state = PyModule_GetState(module);
    if (state->ObjCObject_Type == NULL) {
        return NULL;
    }

    return _ObjCObject_FromId(state->ObjCObject_Type, obj);
}
