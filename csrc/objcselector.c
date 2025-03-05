#include <Python.h>
#include <objc/objc.h>

#include "objctypes.h"

// Destruct an ObjCSelector.
static void
ObjCSelector_dealloc(ObjCSelectorObject *self)
{
    cache_delete_ObjCSelector(self->value);
    Py_TYPE(self)->tp_free((PyObject *)self);
}

// ObjCSelector.__repr__()
static PyObject *
ObjCSelector_repr(ObjCSelectorObject *self)
{
    return PyUnicode_FromFormat("ObjCSelector('%s')",
                                sel_getName(self->value));
}

// ObjCSelector.__str__()
static PyObject *
ObjCSelector_str(ObjCSelectorObject *self)
{
    return PyUnicode_FromString(sel_getName(self->value));
}

// ObjCSelector.address
static PyObject *
ObjCSelector_address(ObjCSelectorObject *self, void *Py_UNUSED(closure))
{
    return PyLong_FromVoidPtr(self->value);
}

// ObjCSelector.name
static PyObject *
ObjCSelector_name(ObjCSelectorObject *self, void *Py_UNUSED(closure))
{
    return PyUnicode_FromString(sel_getName(self->value));
}

// Get an ObjCSelector from a Python type and an Objective-C SEL.
static ObjCSelectorObject *
_ObjCSelector_FromSEL(PyTypeObject *type, SEL sel)
{
    ObjCSelectorObject *self;

    self = cache_get_ObjCSelector(sel);
    if (self == NULL) {
        self = (ObjCSelectorObject *)type->tp_alloc(type, 0);
        if (self != NULL) {
            self->value = sel;
            cache_add_ObjCSelector(sel, self);
        }
    }

    return self;
}

// ObjCSelector.from_address()
static PyObject *
ObjCSelector_from_address(PyTypeObject *type, PyObject *args)
{
    PyObject *address;

    if (!PyArg_ParseTuple(args, "O!:ObjCSelector.from_address", &PyLong_Type,
                          &address)) {
        return NULL;
    }

    return (PyObject *)_ObjCSelector_FromSEL(type, PyLong_AsVoidPtr(address));
}

// ObjCSelector.__new__()
static PyObject *
ObjCSelector_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    static char *kwlist[] = {"", NULL};
    char *name;
    ObjCSelectorObject *self;

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "s:ObjCSelector", kwlist,
                                     &name)) {
        return NULL;
    }

    self = _ObjCSelector_FromSEL(type, sel_registerName(name));
    return (PyObject *)self;
}

static PyMethodDef ObjCSelector_methods[] = {
    {
        "from_address",
        (PyCFunction)ObjCSelector_from_address,
        METH_VARARGS | METH_CLASS,
        PyDoc_STR("Get an ObjCSelector from the memory address."),
    },
    {NULL},
};

static PyGetSetDef ObjCSelector_getset[] = {
    {
        "address",
        (getter)ObjCSelector_address,
        NULL,
        PyDoc_STR("The address of the Objective-C selector."),
        NULL,
    },
    {
        "name",
        (getter)ObjCSelector_name,
        NULL,
        PyDoc_STR("The name of the Objective-C selector."),
        NULL,
    },
    {NULL},
};

PyTypeObject ObjCSelectorType = {
    .ob_base = PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "objctypes.ObjCSelector",
    .tp_basicsize = sizeof(ObjCSelectorObject),
    .tp_itemsize = 0,
    .tp_dealloc = (destructor)ObjCSelector_dealloc,
    .tp_repr = (reprfunc)ObjCSelector_repr,
    .tp_str = (reprfunc)ObjCSelector_str,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = PyDoc_STR("Python wrapper for Objective-C SEL."),
    .tp_methods = ObjCSelector_methods,
    .tp_getset = ObjCSelector_getset,
    .tp_new = ObjCSelector_new,
};

PyObject *
ObjCSelector_FromSEL(SEL sel)
{
    return (PyObject *)_ObjCSelector_FromSEL(&ObjCSelectorType, sel);
}
