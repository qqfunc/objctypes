/**
 * @file objcmetaclass.h
 * @brief Source declarations and definitions for objcmetaclass.h.
 */

#ifndef OBJCMETACLASS_H
#define OBJCMETACLASS_H

#include <Python.h>

#include <objc/objc.h>

/// ObjCMetaclass

typedef struct {
    Class value;
} ObjCMetaclassState;

extern PyType_Spec ObjCMetaclass_spec;

#endif // OBJCMETACLASS_H
