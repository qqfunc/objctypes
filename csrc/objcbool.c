#include <Python.h>

#include "objcbool.h"

#include "objctypes.h"
#include "objctypes_module.h"

// Destruct an ObjCBool.
static void
ObjCBool_dealloc(ObjCBoolObject *self)
{
    Py_TYPE(self)->tp_free((PyObject *)self);
}

// ObjCBool.__repr__()
static PyObject *
ObjCBool_repr(ObjCBoolObject *self)
{
    return PyUnicode_FromFormat("ObjCBool(%s)",
                                self->value ? "True" : "False");
}

// ObjCBool.__str__()
static PyObject *
ObjCBool_str(ObjCBoolObject *self)
{
    return PyUnicode_FromString(self->value ? "YES" : "NO");
}

// Get an ObjCBool from a Python type and an long.
static ObjCBoolObject *
_ObjCBool_FromLong(PyTypeObject *type, long v)
{
    PyObject *module = PyType_GetModuleByDef(type, &objctypes_module);
    if (module == NULL) {
        return NULL;
    }

    objctypes_state *state = PyModule_GetState(module);

    if (v) {
        if (state->ObjCBool_YES == NULL) {
            state->ObjCBool_YES = type->tp_alloc(type, 0);
            if (state->ObjCBool_YES != NULL) {
                ((ObjCBoolObject *)state->ObjCBool_YES)->value = YES;
            }
        }
        return (ObjCBoolObject *)Py_NewRef(state->ObjCBool_YES);
    }

    if (state->ObjCBool_NO == NULL) {
        state->ObjCBool_NO = type->tp_alloc(type, 0);
        if (state->ObjCBool_NO != NULL) {
            ((ObjCBoolObject *)state->ObjCBool_NO)->value = NO;
        }
    }
    return (ObjCBoolObject *)Py_NewRef(state->ObjCBool_NO);
}

// ObjCBool.__new__()
static PyObject *
ObjCBool_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    static char *kwlist[] = {"", NULL};
    int v = 0;

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|p:ObjCBool", kwlist, &v)) {
        return NULL;
    }

    return (PyObject *)_ObjCBool_FromLong(type, v);
}

// ObjCBool.__bool__()
static int
ObjCBool_bool(ObjCBoolObject *self)
{
    return self->value ? 1 : 0;
}

// ObjCBool.__invert__()
static PyObject *
ObjCBool_invert(ObjCBoolObject *self)
{
    PyObject *module = PyType_GetModuleByDef(Py_TYPE(self), &objctypes_module);
    if (module == NULL) {
        return NULL;
    }

    return (PyObject *)ObjCBool_FromLong(module, !(self->value));
}

// ObjCBool.__int__()
static PyObject *
ObjCBool_int(ObjCBoolObject *self)
{
    return PyLong_FromLong(self->value ? 1 : 0);
}

// ObjCBool.__float__()
static PyObject *
ObjCBool_float(ObjCBoolObject *self)
{
    return PyFloat_FromDouble(self->value ? 1 : 0);
}

static PyType_Slot ObjCBool_slots[] = {
    {Py_tp_dealloc, ObjCBool_dealloc},
    {Py_tp_repr, ObjCBool_repr},
    {Py_nb_bool, ObjCBool_bool},
    {Py_nb_invert, ObjCBool_invert},
    {Py_nb_int, ObjCBool_int},
    {Py_nb_float, ObjCBool_float},
    {Py_tp_str, ObjCBool_str},
    {Py_tp_doc, "Python wrapper for Objective-C BOOL."},
    {Py_tp_new, ObjCBool_new},
    {0, NULL},
};

PyType_Spec ObjCBool_spec = {
    .name = "objctypes.ObjCBool",
    .basicsize = sizeof(ObjCBoolObject),
    .itemsize = 0,
    .flags = Py_TPFLAGS_DEFAULT,
    .slots = ObjCBool_slots,
};

PyObject *
ObjCBool_FromLong(PyObject *module, long v)
{
    objctypes_state *state = PyModule_GetState(module);
    if (state->ObjCBool_Type == NULL) {
        return NULL;
    }

    return (PyObject *)_ObjCBool_FromLong((PyTypeObject *)state->ObjCBool_Type, v);
}
