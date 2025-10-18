#include <Python.h>

#include "objcmethod.h"

#include "objctypes.h"
#include "objctypes_cache.h"

// Destruct an ObjCMethod.
static void
ObjCMethod_dealloc(ObjCMethodObject *self)
{
    ObjCMethod_cache_del(self->value);
    Py_TYPE(self)->tp_free((PyObject *)self);
}

// ObjCMethod.__repr__()
static PyObject *
ObjCMethod_repr(ObjCMethodObject *self)
{
    return PyUnicode_FromFormat("<ObjCMethod %s at %p>",
                                sel_getName(method_getName(self->value)),
                                self->value);
}

// ObjCMethod.__str__()
static PyObject *
ObjCMethod_str(ObjCMethodObject *self)
{
    return PyUnicode_FromString(sel_getName(method_getName(self->value)));
}

// ObjCMethod.name
static PyObject *
ObjCMethod_name(ObjCMethodObject *self, void *Py_UNUSED(closure))
{
    return PyUnicode_FromString(sel_getName(method_getName(self->value)));
}

// ObjCMethod.address
static PyObject *
ObjCMethod_address(ObjCMethodObject *self, void *Py_UNUSED(closure))
{
    return PyLong_FromVoidPtr(self->value);
}

// Get an ObjCMethod from a Python type and an Objective-C Method.
static ObjCMethodObject *
_ObjCMethod_FromMethod(PyTypeObject *type, Method method)
{
    ObjCMethodObject *self = ObjCMethod_cache_get(method);

    if (self == NULL) {
        self = (ObjCMethodObject *)type->tp_alloc(type, 0);
        if (self != NULL) {
            self->value = method;
            ObjCMethod_cache_set(method, self);
        }
    }

    return self;
}

// ObjCMethod.from_address()
static PyObject *
ObjCMethod_from_address(PyTypeObject *type, PyObject *address)
{
    if (!PyLong_Check(address)) {
        PyErr_Format(
            PyExc_TypeError,
            "ObjCMethod.from_address() argument 1 must be int, not %T",
            address);
        return NULL;
    }

    return (PyObject *)_ObjCMethod_FromMethod(type, PyLong_AsVoidPtr(address));
}

// ObjCMethod.__new__()
static PyObject *
ObjCMethod_new(PyTypeObject *Py_UNUSED(type), PyObject *Py_UNUSED(args),
               PyObject *Py_UNUSED(kwds))
{
    PyErr_SetNone(PyExc_NotImplementedError);
    return NULL;
}

static PyMethodDef ObjCMethod_methods[] = {
    {
        "from_address",
        (PyCFunction)ObjCMethod_from_address,
        METH_O | METH_CLASS,
        PyDoc_STR("Get an ObjCMethod from the memory address."),
    },
    {.ml_name = NULL},
};

static PyGetSetDef ObjCMethod_getset[] = {
    {
        "address",
        (getter)ObjCMethod_address,
        NULL,
        PyDoc_STR("The address of the Objective-C method."),
        NULL,
    },
    {
        "name",
        (getter)ObjCMethod_name,
        NULL,
        PyDoc_STR("The name of the Objective-C method."),
        NULL,
    },
    {.name = NULL},
};

PyTypeObject ObjCMethodType = {
    .ob_base = PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "objctypes.ObjCMethod",
    .tp_basicsize = sizeof(ObjCMethodObject),
    .tp_itemsize = 0,
    .tp_dealloc = (destructor)ObjCMethod_dealloc,
    .tp_repr = (reprfunc)ObjCMethod_repr,
    .tp_str = (reprfunc)ObjCMethod_str,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = PyDoc_STR("Python wrapper for Objective-C Method."),
    .tp_methods = ObjCMethod_methods,
    .tp_getset = ObjCMethod_getset,
    .tp_new = ObjCMethod_new,
};

PyObject *
ObjCMethod_FromMethod(Method method)
{
    return (PyObject *)_ObjCMethod_FromMethod(&ObjCMethodType, method);
}
