/**
 * @file objcmetaclass.h
 * @brief Source declarations and definitions for objcmetaclass.c.
 */

#ifndef OBJCMETACLASS_H
#define OBJCMETACLASS_H

#include <Python.h>

#include <objc/objc.h>

/// ObjCMetaClass

typedef struct {
    Class value;
} ObjCMetaClassState;

extern PyType_Spec ObjCMetaClass_spec;

#endif // OBJCMETACLASS_H
