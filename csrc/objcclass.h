#ifndef OBJCCLASS_H
#define OBJCCLASS_H

#include <Python.h>

#include <objc/objc.h>

// ObjCClass

typedef struct {
    PyTypeObject type;
    Class value;
} ObjCClassObject;

extern PyTypeObject ObjCClassType;

#endif // OBJCCLASS_H
