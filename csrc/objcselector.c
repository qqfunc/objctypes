#include <Python.h>

#include "objcselector.h"

#include "objctypes.h"
#include "objctypes_cache.h"
#include "objctypes_module.h"

// Destruct an ObjCSelector.
static void
ObjCSelector_dealloc(ObjCSelectorObject *self)
{
    PyObject *module = PyType_GetModuleByDef(Py_TYPE(self), &objctypes_module);
    if (module != NULL) {
        ObjCSelector_cache_del(module, self->value);
    }
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
    return sel_isMapped(self->value) ? Py_True : Py_False;
}

// Get an ObjCSelector from a Python type and an Objective-C SEL.
static ObjCSelectorObject *
_ObjCSelector_FromSEL(PyTypeObject *type, SEL sel)
{
    PyObject *module = PyType_GetModuleByDef(type, &objctypes_module);
    if (module == NULL) {
        return NULL;
    }

    ObjCSelectorObject *self = ObjCSelector_cache_get(module, sel);

    if (self == NULL) {
        self = (ObjCSelectorObject *)type->tp_alloc(type, 0);
        if (self != NULL) {
            self->value = sel;
            ObjCSelector_cache_set(module, sel, self);
        }
    }

    return self;
}

// ObjCSelector.from_address()
static PyObject *
ObjCSelector_from_address(PyTypeObject *type, PyObject *address)
{
    if (!PyLong_Check(address)) {
        PyErr_Format(
            PyExc_TypeError,
            "ObjCSelector.from_address() argument 1 must be int, not %T",
            address);
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

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "s:ObjCSelector", kwlist,
                                     &name)) {
        return NULL;
    }

    ObjCSelectorObject *self =
        _ObjCSelector_FromSEL(type, sel_registerName(name));
    return (PyObject *)self;
}

static PyMethodDef ObjCSelector_methods[] = {
    {
        "from_address",
        (PyCFunction)ObjCSelector_from_address,
        METH_O | METH_CLASS,
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

static PyType_Slot ObjCSelector_slots[] = {
    {Py_tp_dealloc, ObjCSelector_dealloc},
    {Py_tp_repr, ObjCSelector_repr},
    {Py_tp_str, ObjCSelector_str},
    {Py_tp_doc, "Python wrapper for Objective-C SEL."},
    {Py_tp_methods, ObjCSelector_methods},
    {Py_tp_getset, ObjCSelector_getset},
    {Py_tp_new, ObjCSelector_new},
    {0, NULL},
};

PyType_Spec ObjCSelector_spec = {
    .name = "objctypes.ObjCSelector",
    .basicsize = sizeof(ObjCSelectorObject),
    .itemsize = 0,
    .flags = Py_TPFLAGS_DEFAULT,
    .slots = ObjCSelector_slots,
};

PyObject *
ObjCSelector_FromSEL(PyObject *module, SEL sel)
{
    objctypes_state *state = PyModule_GetState(module);
    if (state->ObjCSelector_Type == NULL) {
        return NULL;
    }

    return (PyObject *)_ObjCSelector_FromSEL(
        (PyTypeObject *)state->ObjCSelector_Type, sel);
}
