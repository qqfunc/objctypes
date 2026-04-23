#include <Python.h>

#include "objcobject.h"

#include "objctypes.h"
#include "objctypes_cache.h"

#include "objctypes_module.h"

// Destruct an ObjCObject.
static void
ObjCObject_dealloc(ObjCObjectObject *self)
{
    PyObject *module = PyType_GetModuleByDef(Py_TYPE(self), &objctypes_module);
    if (module != NULL) {
        ObjCObject_cache_del(module, self->value);
    }
    Py_TYPE(self)->tp_free((PyObject *)self);
}

// ObjCObject.__repr__()
static PyObject *
ObjCObject_repr(ObjCObjectObject *self)
{
    return PyUnicode_FromFormat("<ObjCObject %s at %p>",
                                object_getClassName(self->value), self->value);
}

// ObjCObject.address
static PyObject *
ObjCObject_address(ObjCObjectObject *self, void *Py_UNUSED(closure))
{
    return PyLong_FromVoidPtr(self->value);
}

// Get an ObjCObject from a Python type and an Objective-C id.
static ObjCObjectObject *
_ObjCObject_FromId(PyTypeObject *type, id obj)
{
    PyObject *module = PyType_GetModuleByDef(type, &objctypes_module);
    if (module == NULL) {
        return NULL;
    }

    ObjCObjectObject *self = ObjCObject_cache_get(module, obj);

    if (self == NULL) {
        self = (ObjCObjectObject *)type->tp_alloc(type, 0);
        if (self != NULL) {
            self->value = obj;
            ObjCObject_cache_set(module, obj, self);
        }
    }

    return self;
}

// ObjCObject.from_address()
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
        }
        else {
            PyErr_Format(PyExc_TypeError,
                         "The Objective-C object at %p is a class. Use "
                         "ObjCClass.from_address() instead.",
                         obj);
        }
    }

    return (PyObject *)_ObjCObject_FromId(type, obj);
}

// ObjCObject.__new__()
static PyObject *
ObjCObject_new(PyTypeObject *type, PyObject *Py_UNUSED(args),
               PyObject *Py_UNUSED(kwds))
{
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
        (getter)ObjCObject_address,
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
    .basicsize = sizeof(ObjCObjectObject),
    .itemsize = 0,
    .flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .slots = ObjCObject_slots,
};

PyObject *
ObjCObject_FromId(PyObject *module, id obj)
{
    objctypes_state *state = PyModule_GetState(module);
    if (state->ObjCObject_Type == NULL) {
        return NULL;
    }

    return (PyObject *)_ObjCObject_FromId(state->ObjCObject_Type, obj);
}
