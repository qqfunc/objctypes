#include <Python.h>

#include "objcmethod.h"

#include "objctypes.h"
#include "objctypes_cache.h"

#include "objctypes_module.h"

// Destruct an ObjCMethod.
static void
ObjCMethod_dealloc(ObjCMethodObject *self)
{
    PyObject *module = PyType_GetModuleByDef(Py_TYPE(self), &objctypes_module);
    if (module != NULL) {
        ObjCMethod_cache_del(module, self->value);
    }
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
    PyObject *module = PyType_GetModuleByDef(type, &objctypes_module);
    if (module == NULL) {
        return NULL;
    }

    ObjCMethodObject *self = ObjCMethod_cache_get(module, method);

    if (self == NULL) {
        self = (ObjCMethodObject *)type->tp_alloc(type, 0);
        if (self != NULL) {
            self->value = method;
            ObjCMethod_cache_set(module, method, self);
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

static PyType_Slot ObjCMethod_slots[] = {
    {Py_tp_dealloc, ObjCMethod_dealloc},
    {Py_tp_repr, ObjCMethod_repr},
    {Py_tp_str, ObjCMethod_str},
    {Py_tp_doc, "Python wrapper for Objective-C Method."},
    {Py_tp_methods, ObjCMethod_methods},
    {Py_tp_getset, ObjCMethod_getset},
    {Py_tp_new, ObjCMethod_new},
    {0, NULL},
};

PyType_Spec ObjCMethod_spec = {
    .name = "objctypes.ObjCMethod",
    .basicsize = sizeof(ObjCMethodObject),
    .itemsize = 0,
    .flags = Py_TPFLAGS_DEFAULT,
    .slots = ObjCMethod_slots,
};

PyObject *
ObjCMethod_FromMethod(PyObject *module, Method method)
{
    objctypes_state *state = PyModule_GetState(module);
    if (state->ObjCMethod_Type == NULL) {
        return NULL;
    }

    return (PyObject *)_ObjCMethod_FromMethod(
        (PyTypeObject *)state->ObjCMethod_Type, method);
}
