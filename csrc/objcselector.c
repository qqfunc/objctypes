/**
 * @file objcselector.c
 * @brief Source declarations and definitions for objcselector.c.
 */

#include <Python.h>

#include "objctypes.h"
#include "objctypes_cache.h"
#include "objctypes_module.h"

/// @brief Destruct an ObjCSelector.
static void
ObjCSelector_dealloc(PyObject *self)
{
    PyObject *module = PyType_GetModuleByDef(Py_TYPE(self), &objctypes_module);
    if (module != NULL) {
        objctypes_state *state = PyModule_GetState(module);
        ObjCSelectorData *data =
            PyObject_GetTypeData(self, state->ObjCSelector_Type);
        if (data != NULL) {
            PyMutex_Lock(&state->ObjCSelector_cache_mutex);
            ObjCSelector_cache_del(module, data->value);
            PyMutex_Unlock(&state->ObjCSelector_cache_mutex);
        }
    }
    Py_TYPE(self)->tp_free(self);
}

/// @brief `ObjCSelector.__repr__()`
static PyObject *
ObjCSelector_repr(PyObject *self)
{
    // Get the type data of the ObjCSelector object
    PyObject *module = PyType_GetModuleByDef(Py_TYPE(self), &objctypes_module);
    if (module == NULL) {
        return NULL;
    }
    objctypes_state *state = PyModule_GetState(module);
    ObjCSelectorData *data =
        PyObject_GetTypeData(self, state->ObjCSelector_Type);

    return PyUnicode_FromFormat("ObjCSelector('%s')",
                                sel_getName(data->value));
}

/// @brief `ObjCSelector.__str__()`
static PyObject *
ObjCSelector_str(PyObject *self)
{
    // Get the type data of the ObjCSelector object
    PyObject *module = PyType_GetModuleByDef(Py_TYPE(self), &objctypes_module);
    if (module == NULL) {
        return NULL;
    }
    objctypes_state *state = PyModule_GetState(module);
    ObjCSelectorData *data =
        PyObject_GetTypeData(self, state->ObjCSelector_Type);

    return PyUnicode_FromString(sel_getName(data->value));
}

/// @brief `ObjCSelector.address`
static PyObject *
ObjCSelector_address(PyObject *self, void *Py_UNUSED(closure))
{
    // Get the type data of the ObjCSelector object
    PyObject *module = PyType_GetModuleByDef(Py_TYPE(self), &objctypes_module);
    if (module == NULL) {
        return NULL;
    }
    objctypes_state *state = PyModule_GetState(module);
    ObjCSelectorData *data =
        PyObject_GetTypeData(self, state->ObjCSelector_Type);

    return PyLong_FromVoidPtr(data->value);
}

/// @brief `ObjCSelector.name`
static PyObject *
ObjCSelector_name(PyObject *self, void *Py_UNUSED(closure))
{
    // Get the type data of the ObjCSelector object
    PyObject *module = PyType_GetModuleByDef(Py_TYPE(self), &objctypes_module);
    if (module == NULL) {
        return NULL;
    }
    objctypes_state *state = PyModule_GetState(module);
    ObjCSelectorData *data =
        PyObject_GetTypeData(self, state->ObjCSelector_Type);

    return PyUnicode_FromString(sel_getName(data->value));
}

/// @brief `ObjCSelector.is_mapped`
static PyObject *
ObjCSelector_is_mapped(PyObject *self, void *Py_UNUSED(closure))
{
    // Get the type data of the ObjCSelector object
    PyObject *module = PyType_GetModuleByDef(Py_TYPE(self), &objctypes_module);
    if (module == NULL) {
        return NULL;
    }
    objctypes_state *state = PyModule_GetState(module);
    ObjCSelectorData *data =
        PyObject_GetTypeData(self, state->ObjCSelector_Type);

    return sel_isMapped(data->value) ? Py_True : Py_False;
}

/// @brief Get an ObjCSelector from a Python type and an Objective-C SEL.
static PyObject *
_ObjCSelector_FromSEL(PyTypeObject *type, SEL sel)
{
    // Get the module state
    PyObject *module = PyType_GetModuleByDef(type, &objctypes_module);
    if (module == NULL) {
        return NULL;
    }
    objctypes_state *state = PyModule_GetState(module);

    PyMutex_Lock(&state->ObjCSelector_cache_mutex);

    PyObject *self = ObjCSelector_cache_get(module, sel);
    if (self == NULL) {
        self = type->tp_alloc(type, 0);
        if (self != NULL) {
            ObjCSelectorData *data =
                PyObject_GetTypeData(self, state->ObjCSelector_Type);
            data->value = sel;
            ObjCSelector_cache_set(module, sel, self);
        }
    }

    PyMutex_Unlock(&state->ObjCSelector_cache_mutex);

    return self;
}

/// @brief `ObjCSelector.from_address()`
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

    SEL sel = PyLong_AsVoidPtr(address);
    if (sel == NULL) {
        PyErr_Format(PyExc_TypeError,
                     "The specified address is a null selector", sel);
        return NULL;
    }

    return _ObjCSelector_FromSEL(type, sel);
}

/// @brief `ObjCSelector.__new__()`
static PyObject *
ObjCSelector_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    static char *kwlist[] = {"", NULL};
    char *name;

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "s:ObjCSelector", kwlist,
                                     &name)) {
        return NULL;
    }

    return _ObjCSelector_FromSEL(type, sel_registerName(name));
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
        ObjCSelector_address,
        NULL,
        PyDoc_STR("The address of the Objective-C selector."),
        NULL,
    },
    {
        "name",
        ObjCSelector_name,
        NULL,
        PyDoc_STR("The name of the Objective-C selector."),
        NULL,
    },
    {
        "is_mapped",
        ObjCSelector_is_mapped,
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
    .basicsize = -(long)sizeof(ObjCSelectorData),
    .itemsize = 0,
    .flags = Py_TPFLAGS_DEFAULT,
    .slots = ObjCSelector_slots,
};

PyObject *
ObjCSelector_FromSEL(PyObject *module, SEL sel)
{
    // Get the module state
    objctypes_state *state = PyModule_GetState(module);
    if (state->ObjCSelector_Type == NULL) {
        return NULL;
    }

    return _ObjCSelector_FromSEL((PyTypeObject *)state->ObjCSelector_Type,
                                 sel);
}
