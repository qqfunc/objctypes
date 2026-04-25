/**
 * @file objcbool.c
 * @brief Source declarations and definitions for objcbool.c.
 */

#include <Python.h>

#include "objctypes.h"
#include "objctypes_module.h"

/// @brief Destruct an ObjCBool.
static void
ObjCBool_dealloc(PyObject *self)
{
    Py_TYPE(self)->tp_free(self);
}

/// @brief `ObjCBool.__repr__()`
static PyObject *
ObjCBool_repr(PyObject *self)
{
    // Get the type data of the ObjCBool object
    PyObject *module = PyType_GetModuleByDef(Py_TYPE(self), &objctypes_module);
    if (module == NULL) {
        return NULL;
    }
    objctypes_state *state = PyModule_GetState(module);
    ObjCBoolData *data = PyObject_GetTypeData(self, state->ObjCBool_Type);
    if (data == NULL) {
        return NULL;
    }

    return PyUnicode_FromFormat("ObjCBool(%s)",
                                data->value ? "True" : "False");
}

/// @brief `ObjCBool.__str__()`
static PyObject *
ObjCBool_str(PyObject *self)
{
    // Get the type data of the ObjCBool object
    PyObject *module = PyType_GetModuleByDef(Py_TYPE(self), &objctypes_module);
    if (module == NULL) {
        return NULL;
    }
    objctypes_state *state = PyModule_GetState(module);
    ObjCBoolData *data = PyObject_GetTypeData(self, state->ObjCBool_Type);
    if (data == NULL) {
        return NULL;
    }

    return PyUnicode_FromString(data->value ? "YES" : "NO");
}

/// @brief Get an ObjCBool from a Python type and an long.
static PyObject *
_ObjCBool_FromLong(PyTypeObject *type, long v)
{
    // Get the module state
    PyObject *module = PyType_GetModuleByDef(type, &objctypes_module);
    if (module == NULL) {
        return NULL;
    }
    objctypes_state *state = PyModule_GetState(module);

    if (v) {
        // Cache the YES value if it hasn't been cached yet
        if (state->ObjCBool_YES == NULL) {
            state->ObjCBool_YES = type->tp_alloc(type, 0);
            if (state->ObjCBool_YES != NULL) {
                ObjCBoolData *data = PyObject_GetTypeData(
                    state->ObjCBool_YES, state->ObjCBool_Type);
                if (data != NULL) {
                    data->value = YES;
                }
                else {
                    Py_DECREF(state->ObjCBool_YES);
                    state->ObjCBool_YES = NULL;
                }
            }
        }
        return Py_XNewRef(state->ObjCBool_YES);
    }

    // Cache the NO value if it hasn't been cached yet
    if (state->ObjCBool_NO == NULL) {
        state->ObjCBool_NO = type->tp_alloc(type, 0);
        if (state->ObjCBool_NO != NULL) {
            ObjCBoolData *data =
                PyObject_GetTypeData(state->ObjCBool_NO, state->ObjCBool_Type);
            if (data == NULL) {
                Py_DECREF(state->ObjCBool_NO);
                state->ObjCBool_NO = NULL;
            }
            else {
                data->value = NO;
            }
        }
    }
    return Py_XNewRef(state->ObjCBool_NO);
}

/// @brief `ObjCBool.__new__()`
static PyObject *
ObjCBool_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    static char *kwlist[] = {"", NULL};
    int v = 0;

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|p:ObjCBool", kwlist, &v)) {
        return NULL;
    }

    return _ObjCBool_FromLong(type, v);
}

/// @brief `ObjCBool.__bool__()`
static int
ObjCBool_bool(PyObject *self)
{
    // Get the type data of the ObjCBool object
    PyObject *module = PyType_GetModuleByDef(Py_TYPE(self), &objctypes_module);
    if (module == NULL) {
        return -1;
    }
    objctypes_state *state = PyModule_GetState(module);
    ObjCBoolData *data = PyObject_GetTypeData(self, state->ObjCBool_Type);
    if (data == NULL) {
        return -1;
    }

    return data->value ? 1 : 0;
}

/// @brief `ObjCBool.__invert__()`
static PyObject *
ObjCBool_invert(PyObject *self)
{
    // Get the type data of the ObjCBool object
    PyObject *module = PyType_GetModuleByDef(Py_TYPE(self), &objctypes_module);
    if (module == NULL) {
        return NULL;
    }
    objctypes_state *state = PyModule_GetState(module);
    ObjCBoolData *data = PyObject_GetTypeData(self, state->ObjCBool_Type);
    if (data == NULL) {
        return NULL;
    }

    return _ObjCBool_FromLong(Py_TYPE(self), !(data->value));
}

/// @brief `ObjCBool.__int__()`
static PyObject *
ObjCBool_int(PyObject *self)
{
    // Get the type data of the ObjCBool object
    PyObject *module = PyType_GetModuleByDef(Py_TYPE(self), &objctypes_module);
    if (module == NULL) {
        return NULL;
    }
    objctypes_state *state = PyModule_GetState(module);
    ObjCBoolData *data = PyObject_GetTypeData(self, state->ObjCBool_Type);
    if (data == NULL) {
        return NULL;
    }

    return PyLong_FromLong(data->value ? 1 : 0);
}

/// @brief `ObjCBool.__float__()`
static PyObject *
ObjCBool_float(PyObject *self)
{
    // Get the type data of the ObjCBool object
    PyObject *module = PyType_GetModuleByDef(Py_TYPE(self), &objctypes_module);
    if (module == NULL) {
        return NULL;
    }
    objctypes_state *state = PyModule_GetState(module);
    ObjCBoolData *data = PyObject_GetTypeData(self, state->ObjCBool_Type);
    if (data == NULL) {
        return NULL;
    }

    return PyFloat_FromDouble(data->value ? 1 : 0);
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
    .basicsize = -(long)sizeof(ObjCBoolData),
    .itemsize = 0,
    .flags = Py_TPFLAGS_DEFAULT,
    .slots = ObjCBool_slots,
};

PyObject *
ObjCBool_FromLong(PyObject *module, long v)
{
    // Get the module state
    objctypes_state *state = PyModule_GetState(module);
    if (state->ObjCBool_Type == NULL) {
        return NULL;
    }

    return (PyObject *)_ObjCBool_FromLong((PyTypeObject *)state->ObjCBool_Type,
                                          v);
}
