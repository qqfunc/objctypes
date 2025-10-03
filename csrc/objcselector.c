#include <Python.h>

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

// ObjCSelector.is_mapped
static PyObject *
ObjCSelector_is_mapped(ObjCSelectorObject *self, void *Py_UNUSED(closure))
{
    if (sel_isMapped(self->value)) {
        return Py_True;
    }
    else {
        return Py_False;
    }
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
    {.ml_name = NULL},
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
    {
        "is_mapped",
        (getter)ObjCSelector_is_mapped,
        NULL,
        PyDoc_STR("Whether the Objective-C selector is mapped."),
        NULL,
    },
    {.name = NULL},
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

int
ObjCSelector_SELConverter(PyObject *obj, void *ptr)
{
    SEL sel;
    const char *name;

    if (PyObject_TypeCheck(obj, &ObjCSelectorType)) {
        sel = ((ObjCSelectorObject *)obj)->value;
    }
    else if (PyUnicode_Check(obj)) {
        name = PyUnicode_AsUTF8(obj);
        if (name == NULL) {
            return 0; // Failure
        }
        sel = sel_registerName(name);
    }
    else if (PyBytes_Check(obj)) {
        sel = sel_registerName(PyBytes_AS_STRING(obj));
    }
    else {
        PyErr_Format(PyExc_TypeError,
                     "Expected an Objective-C selector, got '%s'",
                     Py_TYPE(obj)->tp_name);
        return 0; // Failure
    }

    *(SEL *)ptr = sel;
    return 1; // Success
}
