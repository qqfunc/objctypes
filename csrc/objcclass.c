#include <Python.h>
#include <objc/objc.h>
#include <objc/runtime.h>

#include "objctypes.h"

// Destruct an ObjCClass.
static void
ObjCClass_dealloc(ObjCClassObject *self)
{
    if (self->value != NULL) {
        cache_delete_ObjCClass(self->value);
    }
    Py_TYPE(self)->tp_free((PyObject *)self);
}

// ObjCClass.__repr__()
static PyObject *
ObjCClass_repr(ObjCClassObject *self)
{
    if (self->value == NULL) {
        return PyUnicode_FromString("<class 'ObjCObject'>");
    }
    return PyUnicode_FromFormat("<ObjCClass '%s'>",
                                class_getName(self->value));
}

// ObjCClass.address
static PyObject *
ObjCClass_address(ObjCClassObject *self, void *Py_UNUSED(closure))
{
    return PyLong_FromVoidPtr(self->value);
}

// ObjCClass.name
static PyObject *
ObjCClass_name(ObjCClassObject *self, PyObject *Py_UNUSED(closure))
{
    if (self->value == NULL) {
        return Py_GetConstant(Py_CONSTANT_EMPTY_STR);
    }
    return PyUnicode_FromString(class_getName(self->value));
}

// ObjCClass.load_methods
static PyObject *
ObjCClass_load_methods(ObjCClassObject *self, PyObject *Py_UNUSED(args))
{
    unsigned int outCount, num;
    Method *methods;

    methods = class_copyMethodList(self->value, &outCount);
    for (num = 0; num < outCount; num++) {
        printf("Method %s\n", sel_getName(method_getName(methods[num])));
    }
    free(methods);

    return Py_None;
}

// Get an ObjCClass from a Python type and an Objective-C Class.
static ObjCClassObject *
_ObjCClass_FromClass(PyTypeObject *type, Class cls)
{
    Class super_cls;
    ObjCClassObject *self, *super;

    self = cache_get_ObjCClass(cls);
    if (self == NULL) {
        super_cls = class_getSuperclass(cls);
        if (super_cls == NULL) {
            // The class is a root class.
            self = (ObjCClassObject *)PyType_Type.tp_new(
                type, Py_BuildValue("(s(){})", class_getName(cls)),
                PyDict_New());
        }
        else {
            super = _ObjCClass_FromClass(type, super_cls);
            self = (ObjCClassObject *)PyType_Type.tp_new(
                type, Py_BuildValue("(s(O){})", class_getName(cls), super),
                PyDict_New());
        }
        if (self != NULL) {
            self->value = cls;
            cache_add_ObjCClass(cls, self);
        }
    }

    return self;
}

// ObjCClass.from_address()
static PyObject *
ObjCClass_from_address(PyTypeObject *type, PyObject *args)
{
    PyObject *address;
    Class cls;
    if (!PyArg_ParseTuple(args, "O!:ObjCClass.from_address", &PyLong_Type,
                          &address)) {
        return NULL;
    }
    cls = PyLong_AsVoidPtr(address);

    // Make sure the class is not Nil.
    if (cls == NULL) {
        PyErr_SetString(PyExc_TypeError,
                        "the specified Objective-C class is Nil");
        return NULL;
    }

    // Make sure the class is an Objective-C class.
    if (!object_isClass((id)cls)) {
        PyErr_Format(PyExc_TypeError,
                     "The Objective-C object at %p is not a class. Use "
                     "ObjCObject.from_address() instead.",
                     cls);
        return NULL;
    }

    // Make sure the class is not a metaclass.
    if (class_isMetaClass(cls)) {
        PyErr_Format(PyExc_TypeError,
                     "The Objective-C class at %p is a metaclass. Use "
                     "ObjCMetaClass.from_address() instead.",
                     cls);
    }

    return (PyObject *)_ObjCClass_FromClass(type, cls);
}

// ObjCClass.__new__()
static PyObject *
ObjCClass_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    static char *kwlist[] = {"", "", "", NULL};
    char *name;
    PyObject *bases = NULL, *dict = NULL;
    Class cls;
    ObjCClassObject *self;

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "s|O!O!:ObjCClass", kwlist,
                                     &name, &PyTuple_Type, &bases,
                                     &PyDict_Type, &dict)) {
        return NULL;
    }

    if (bases == NULL) {
        // ObjCClass(name, /)
        cls = objc_getClass(name);
        if (cls == NULL) {
            PyErr_Format(PyExc_NameError,
                         "Objective-C class '%s' is not defined", name);
            return NULL;
        }

        self = _ObjCClass_FromClass(type, cls);
    }
    else if (dict == NULL) {
        PyErr_SetString(
            PyExc_TypeError,
            "ObjCClass() arguments 2 and 3 must be given together");
        return NULL;
    }
    else {
        // ObjCClass(name, bases, dict, /)
        self = (ObjCClassObject *)PyType_Type.tp_new(
            type, Py_BuildValue("(s(){})", name), PyDict_New());
        if (self != NULL) {
            self->value = NULL;
        }
    }

    return (PyObject *)self;
}

static PyMethodDef ObjCClass_methods[] = {
    {
        "from_address",
        (PyCFunction)ObjCClass_from_address,
        METH_VARARGS | METH_CLASS,
        PyDoc_STR("Get an ObjCClass from the memory address."),
    },
    {
        "load_methods",
        (PyCFunction)ObjCClass_load_methods,
        METH_NOARGS,
        PyDoc_STR("Load or reload all methods of the class."),
    },
    {.ml_name = NULL},
};

static PyGetSetDef ObjCClass_getset[] = {
    {
        "address",
        (getter)ObjCClass_address,
        NULL,
        PyDoc_STR("The address of the Objective-C class."),
        NULL,
    },
    {
        "name",
        (getter)ObjCClass_name,
        NULL,
        PyDoc_STR("The name of the Objective-C class."),
        NULL,
    },
    {.name = NULL},
};

PyTypeObject ObjCClassType = {
    .ob_base = PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "objctypes.ObjCClass",
    .tp_basicsize = sizeof(ObjCClassObject),
    .tp_itemsize = 0,
    .tp_dealloc = (destructor)ObjCClass_dealloc,
    .tp_repr = (reprfunc)ObjCClass_repr,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = PyDoc_STR("Python wrapper for Objective-C Class."),
    .tp_methods = ObjCClass_methods,
    .tp_getset = ObjCClass_getset,
    .tp_new = ObjCClass_new,
};

PyObject *
ObjCClass_FromClass(Class cls)
{
    return (PyObject *)_ObjCClass_FromClass(&ObjCClassType, cls);
}
