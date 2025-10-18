#ifndef OBJCOBJECT_H
#define OBJCOBJECT_H

#include <Python.h>

#include "objcclass.h"

#include <objc/objc.h>

// ObjCObject

typedef struct {
    PyObject_HEAD
    id value;
} ObjCObjectObject;

extern ObjCClassObject ObjCObjectType;

#endif // OBJCOBJECT_H
