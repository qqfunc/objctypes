#include <Python.h>

#include "objctypes.h"

static ObjCBoolObject *objc_yes = NULL, *objc_no = NULL;

// Destruct an ObjCBool.
static void
ObjCBool_dealloc(ObjCBoolObject *self)
{
    if (self->value) {
        objc_yes = NULL;
    }
    else {
        objc_no = NULL;
    }
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
    if (v) {
        if (objc_yes == NULL) {
            objc_yes = (ObjCBoolObject *)type->tp_alloc(type, 0);
            if (objc_yes != NULL) {
                objc_yes->value = YES;
            }
        }
        else {
            Py_INCREF(objc_yes);
        }
        return objc_yes;
    }

    if (objc_no == NULL) {
        objc_no = (ObjCBoolObject *)type->tp_alloc(type, 0);
        if (objc_no != NULL) {
            objc_no->value = NO;
        }
    }
    else {
        Py_INCREF(objc_no);
    }
    return objc_no;
}

// ObjCBool.__new__()
static PyObject *
ObjCBool_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    static char *kwlist[] = {"", NULL};
    int v = 0;

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|p:ObjCBool", kwlist,
                                     &v)) {
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
    return (PyObject *)ObjCBool_FromLong(!(self->value));
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

static PyNumberMethods ObjCBool_number_methods = {
    .nb_bool = (inquiry)ObjCBool_bool,
    .nb_invert = (unaryfunc)ObjCBool_invert,
    .nb_int = (unaryfunc)ObjCBool_int,
    .nb_float = (unaryfunc)ObjCBool_float,
};

PyTypeObject ObjCBoolType = {
    .ob_base = PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "objctypes.ObjCBool",
    .tp_basicsize = sizeof(ObjCBoolObject),
    .tp_itemsize = 0,
    .tp_dealloc = (destructor)ObjCBool_dealloc,
    .tp_repr = (reprfunc)ObjCBool_repr,
    .tp_as_number = &ObjCBool_number_methods,
    .tp_str = (reprfunc)ObjCBool_str,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = PyDoc_STR("Python wrapper for Objective-C BOOL."),
    .tp_new = ObjCBool_new,
};

PyObject *
ObjCBool_FromLong(long v)
{
    return (PyObject *)_ObjCBool_FromLong(&ObjCBoolType, v);
}
