/**
 * @file objcmetaclass.c
 * @brief Source declarations and definitions for objcmetaclass.c.
 */

#include <Python.h>

#include "objcmetaclass.h"

#include "objctypes.h"
#include "objctypes_cache.h"
#include "objctypes_module.h"

/// @brief Destruct an ObjCMetaClass.
static void
ObjCMetaClass_dealloc(PyObject *self)
{
    PyObject *module = PyType_GetModuleByDef(Py_TYPE(self), &objctypes_module);
    if (module != NULL) {
        objctypes_state *state = PyModule_GetState(module);
        ObjCMetaClassState *cls_state =
            PyObject_GetTypeData(self, state->ObjCMetaClass_Type);
        if (cls_state != NULL) {
            PyMutex_Lock(&state->ObjCMetaClass_cache_mutex);
            ObjCMetaClass_cache_del(module, cls_state->value);
            PyMutex_Unlock(&state->ObjCMetaClass_cache_mutex);
        }
    }
    Py_TYPE(self)->tp_free((PyObject *)self);
}

/// @brief `ObjCMetaClass.__repr__()`
static PyObject *
ObjCMetaClass_repr(PyObject *self)
{
    PyObject *module = PyType_GetModuleByDef(Py_TYPE(self), &objctypes_module);
    if (module == NULL) {
        return NULL;
    }

    objctypes_state *state = PyModule_GetState(module);
    ObjCMetaClassState *cls_state =
        PyObject_GetTypeData(self, state->ObjCMetaClass_Type);

    if (cls_state->value == NULL) {
        return PyUnicode_FromString("<ObjCMetaClass>");
    }
    return PyUnicode_FromFormat("<ObjCMetaClass '%s'>",
                                class_getName(cls_state->value));
}

/// @brief `ObjCMetaClass.address`
static PyObject *
ObjCMetaClass_address(PyObject *self, void *Py_UNUSED(closure))
{
    PyObject *module = PyType_GetModuleByDef(Py_TYPE(self), &objctypes_module);
    if (module == NULL) {
        return NULL;
    }

    objctypes_state *state = PyModule_GetState(module);
    ObjCMetaClassState *cls_state =
        PyObject_GetTypeData(self, state->ObjCMetaClass_Type);

    return PyLong_FromVoidPtr(cls_state->value);
}

/// @brief `ObjCMetaClass.name`
static PyObject *
ObjCMetaClass_name(PyObject *self, PyObject *Py_UNUSED(closure))
{
    PyObject *module = PyType_GetModuleByDef(Py_TYPE(self), &objctypes_module);
    if (module == NULL) {
        return NULL;
    }

    objctypes_state *state = PyModule_GetState(module);
    ObjCMetaClassState *cls_state =
        PyObject_GetTypeData(self, state->ObjCMetaClass_Type);

    if (cls_state->value == NULL) {
        return Py_GetConstant(Py_CONSTANT_EMPTY_STR);
    }
    return PyUnicode_FromString(class_getName(cls_state->value));
}

/// @brief Get an ObjCMetaClass from a Python type and an Objective-C
/// metaclass.
static PyObject *
_ObjCMetaClass_FromClass(PyTypeObject *type, Class cls, int lock_cache)
{
    PyObject *module = PyType_GetModuleByDef(type, &objctypes_module);
    if (module == NULL) {
        return NULL;
    }

    objctypes_state *state = PyModule_GetState(module);

    if (lock_cache) {
        PyMutex_Lock(&state->ObjCMetaClass_cache_mutex);
    }

    PyObject *self = ObjCMetaClass_cache_get(module, cls);

    if (self == NULL) {
        PyObject *args =
            Py_BuildValue("(s(O){})", class_getName(cls), &PyBaseObject_Type);
        PyObject *kwds = PyDict_New();
        self = PyType_Type.tp_new(type, args, kwds);
        Py_XDECREF(args);
        Py_XDECREF(kwds);

        if (self != NULL) {
            ObjCMetaClassState *cls_state =
                PyObject_GetTypeData(self, state->ObjCMetaClass_Type);
            cls_state->value = cls;
            ObjCMetaClass_cache_set(module, cls, self);
        }
    }

    if (lock_cache) {
        PyMutex_Unlock(&state->ObjCMetaClass_cache_mutex);
    }

    return self;
}

/// @brief `ObjCMetaClass.from_address()`
static PyObject *
ObjCMetaClass_from_address(PyTypeObject *type, PyObject *address)
{
    if (!PyLong_Check(address)) {
        PyErr_Format(
            PyExc_TypeError,
            "ObjCMetaClass.from_address() argument 1 must be int, not %T",
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
                     "The Objective-C object at %p is not a class.", cls);
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

    return _ObjCMetaClass_FromClass(type, cls, 1);
}

/// @brief `ObjCMetaClass.from_name()`
static PyObject *
ObjCMetaClass_from_name(PyTypeObject *type, PyObject *name)
{
    if (!PyUnicode_Check(name)) {
        PyErr_Format(
            PyExc_TypeError,
            "ObjCMetaClass.from_name() argument 1 must be str, not %T", name);
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

    return _ObjCMetaClass_FromClass(type, cls, 1);
}

static PyMethodDef ObjCMetaClass_methods[] = {
    {
        "from_address",
        (PyCFunction)ObjCMetaClass_from_address,
        METH_O | METH_CLASS,
        PyDoc_STR("Get an ObjCMetaClass from the memory address."),
    },
    {
        "from_name",
        (PyCFunction)ObjCMetaClass_from_name,
        METH_O | METH_CLASS,
        PyDoc_STR("Get an ObjCMetaClass from the class name."),
    },
    {.ml_name = NULL},
};

static PyGetSetDef ObjCMetaClass_getset[] = {
    {
        "address",
        (getter)ObjCMetaClass_address,
        NULL,
        PyDoc_STR("The address of the Objective-C metaclass."),
        NULL,
    },
    {
        "name",
        (getter)ObjCMetaClass_name,
        NULL,
        PyDoc_STR("The name of the Objective-C metaclass."),
        NULL,
    },
    {.name = NULL},
};

static PyType_Slot ObjCMetaClass_slots[] = {
    {Py_tp_dealloc, ObjCMetaClass_dealloc},
    {Py_tp_repr, ObjCMetaClass_repr},
    {Py_tp_doc, "Python wrapper for Objective-C metaclass."},
    {Py_tp_methods, ObjCMetaClass_methods},
    {Py_tp_getset, ObjCMetaClass_getset},
    {0, NULL},
};

PyType_Spec ObjCMetaClass_spec = {
    .name = "objctypes.ObjCMetaClass",
    .basicsize = -(long)sizeof(ObjCMetaClassState),
    .itemsize = 0,
    .flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_TYPE_SUBCLASS,
    .slots = ObjCMetaClass_slots,
};

PyObject *
ObjCMetaClass_FromClass(PyObject *module, Class cls)
{
    objctypes_state *state = PyModule_GetState(module);
    if (state->ObjCMetaClass_Type == NULL) {
        return NULL;
    }

    return _ObjCMetaClass_FromClass((PyTypeObject *)state->ObjCMetaClass_Type,
                                    cls, 1);
}
