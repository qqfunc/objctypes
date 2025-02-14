#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <objc/objc.h>
#include <objc/runtime.h>

#include "objctypes.h"

// Destruct an ObjCObject.
static void
ObjCObject_dealloc(ObjCObjectObject *self)
{
    cache_delete_ObjCObject(self->value);
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
_ObjCObject_FromId(ObjCClassObject *type, id obj)
{
    PyTypeObject *pytype;
    ObjCObjectObject *self;

    pytype = &(type->type);

    self = cache_get_ObjCObject(obj);
    if (self == NULL) {
        self = (ObjCObjectObject *)pytype->tp_alloc(pytype, 0);
        if (self != NULL) {
            self->value = obj;
            cache_add_ObjCObject(obj, self);
        }
    }

    return self;
}

// ObjCObject.from_address()
static PyObject *
ObjCObject_from_address(ObjCClassObject *type, PyObject *args)
{
    PyObject *address;
    id obj;
    if (!PyArg_ParseTuple(args, "O!:ObjCObject.from_address", &PyLong_Type,
                          &address)) {
        return NULL;
    }
    obj = PyLong_AsVoidPtr(address);

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
ObjCObject_new(ObjCClassObject *type, PyObject *args, PyObject *kwds)
{
    if (type == &ObjCObjectType) {
        PyErr_SetString(PyExc_TypeError,
                        "ObjCObject cannot be initialized directly");
        return NULL;
    }

    PyErr_SetNone(PyExc_NotImplementedError);
    return NULL;
}

static PyMethodDef ObjCObject_methods[] = {
    {
        "from_address",
        (PyCFunction)ObjCObject_from_address,
        METH_VARARGS | METH_CLASS,
        PyDoc_STR("Get an ObjCObject from the memory address."),
    },
    {NULL},
};

static PyGetSetDef ObjCObject_getset[] = {
    {
        "address",
        (getter)ObjCObject_address,
        NULL,
        PyDoc_STR("The address of the Objective-C object."),
        NULL,
    },
    {NULL},
};

ObjCClassObject ObjCObjectType = {
    .type =
        {
            .ob_base = PyVarObject_HEAD_INIT(NULL, 0).tp_name =
                "objctypes.ObjCObject",
            .tp_basicsize = sizeof(ObjCObjectObject),
            .tp_itemsize = 0,
            .tp_dealloc = (destructor)ObjCObject_dealloc,
            .tp_repr = (reprfunc)ObjCObject_repr,
            .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
            .tp_doc = PyDoc_STR("Python wrapper for Objective-C Class."),
            .tp_methods = ObjCObject_methods,
            .tp_getset = ObjCObject_getset,
            .tp_new = (newfunc)ObjCObject_new,
        },
    .value = NULL,
};

PyObject *
ObjCObject_FromId(id obj)
{
    return (PyObject *)_ObjCObject_FromId(&ObjCObjectType, obj);
}
