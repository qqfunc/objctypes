/**
 * @file objctypes_module.h
 * @brief Source declarations and definitions for objctypes_module.h.
 */

#ifndef OBJCTYPES_MODULE_H
#define OBJCTYPES_MODULE_H

#include <Python.h>

extern PyModuleDef objctypes_module;

typedef struct {

    /// @brief The `ObjCBool` type.
    PyTypeObject *ObjCBool_Type;

    /**
     * @brief Cache for the `YES` value of the `ObjCBool` type.
     * @warning Do not manipulate this field outside of the `ObjCBool` type.
     */
    PyObject *ObjCBool_YES;

    /**
     * @brief Cache for the `NO` value of the `ObjCBool` type.
     * @warning Do not manipulate this field outside of the `ObjCBool` type.
     */
    PyObject *ObjCBool_NO;

    /// @brief The `ObjCClass` type.
    PyTypeObject *ObjCClass_Type;

    /**
     * @brief Cache for `ObjCClass` instances.
     * @details This is a pointer to a C++ `std::map` that maps Objective-C class pointers to their corresponding `ObjCClass` Python objects.
     * @warning Do not manipulate this field outside of the `ObjCClass` type.
     */
    void *ObjCClass_cache;

    /// @brief The `ObjCMethod` type.
    PyTypeObject *ObjCMethod_Type;

    /**
     * @brief Cache for `ObjCMethod` instances.
     * @details This is a pointer to a C++ `std::map` that maps Objective-C method pointers to their corresponding `ObjCMethod` Python objects.
     * @warning Do not manipulate this field outside of the `ObjCMethod` type.
     */
    void *ObjCMethod_cache;

    /// @brief The `ObjCObject` type.
    PyTypeObject *ObjCObject_Type;

    /**
     * @brief Cache for `ObjCObject` instances.
     * @details This is a pointer to a C++ `std::map` that maps Objective-C object pointers to their corresponding `ObjCObject` Python objects.
     * @warning Do not manipulate this field outside of the `ObjCObject` type.
     */
    void *ObjCObject_cache;

    /// @brief The `ObjCSelector` type.
    PyTypeObject *ObjCSelector_Type;

    /**
     * @brief Cache for `ObjCSelector` instances.
     * @details This is a pointer to a C++ `std::map` that maps Objective-C selector pointers to their corresponding `ObjCSelector` Python objects.
     * @warning Do not manipulate this field outside of the `ObjCSelector` type.
     */
    void *ObjCSelector_cache;

} objctypes_state;

#endif // OBJCTYPES_MODULE_H
