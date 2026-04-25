/**
 * @file objctypes_module.h
 * @brief Source declarations and definitions for objctypes_module.h.
 */

#ifndef OBJCTYPES_MODULE_H
#define OBJCTYPES_MODULE_H

#include <Python.h>

#include <objc/runtime.h>

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
     * @details This is a pointer to a C++ `std::map` that maps Objective-C
     * class pointers to their corresponding `ObjCClass` Python objects.
     * @warning Do not manipulate this field outside of the `ObjCClass` type.
     */
    void *ObjCClass_cache;

    /**
     * @brief Mutex for synchronizing access to the `ObjCClass` cache.
     * @warning Do not manipulate this field outside of the `ObjCClass` type.
     */
    PyMutex ObjCClass_cache_mutex;

    /// @brief The `ObjCMetaClass` type.
    PyTypeObject *ObjCMetaClass_Type;

    /**
     * @brief Cache for `ObjCMetaClass` instances.
     * @details This is a pointer to a C++ `std::map` that maps Objective-C
     * metaclass pointers to their corresponding `ObjCMetaClass` Python
     * objects.
     * @warning Do not manipulate this field outside of the `ObjCMetaClass`
     * type.
     */
    void *ObjCMetaClass_cache;

    /**
     * @brief Mutex for synchronizing access to the `ObjCMetaClass` cache.
     * @warning Do not manipulate this field outside of the `ObjCMetaClass`
     * type.
     */
    PyMutex ObjCMetaClass_cache_mutex;

    /// @brief The `ObjCMethod` type.
    PyTypeObject *ObjCMethod_Type;

    /**
     * @brief Cache for `ObjCMethod` instances.
     * @details This is a pointer to a C++ `std::map` that maps Objective-C
     * method pointers to their corresponding `ObjCMethod` Python objects.
     * @warning Do not manipulate this field outside of the `ObjCMethod` type.
     */
    void *ObjCMethod_cache;

    /**
     * @brief Mutex for synchronizing access to the `ObjCMethod` cache.
     * @warning Do not manipulate this field outside of the `ObjCMethod` type.
     */
    PyMutex ObjCMethod_cache_mutex;

    /// @brief The `ObjCObject` type.
    PyTypeObject *ObjCObject_Type;

    /**
     * @brief Cache for `ObjCObject` instances.
     * @details This is a pointer to a C++ `std::map` that maps Objective-C
     * object pointers to their corresponding `ObjCObject` Python objects.
     * @warning Do not manipulate this field outside of the `ObjCObject` type.
     */
    void *ObjCObject_cache;

    /**
     * @brief Mutex for synchronizing access to the `ObjCObject` cache.
     * @warning Do not manipulate this field outside of the `ObjCObject` type.
     */
    PyMutex ObjCObject_cache_mutex;

    /// @brief The `ObjCSelector` type.
    PyTypeObject *ObjCSelector_Type;

    /**
     * @brief Cache for `ObjCSelector` instances.
     * @details This is a pointer to a C++ `std::map` that maps Objective-C
     * selector pointers to their corresponding `ObjCSelector` Python objects.
     * @warning Do not manipulate this field outside of the `ObjCSelector`
     * type.
     */
    void *ObjCSelector_cache;

    /**
     * @brief Mutex for synchronizing access to the `ObjCSelector` cache.
     * @warning Do not manipulate this field outside of the `ObjCSelector`
     * type.
     */
    PyMutex ObjCSelector_cache_mutex;

} objctypes_state;

typedef struct {
    BOOL value;
} ObjCBoolData;

extern PyType_Spec ObjCBool_spec;

typedef struct {
    Class value;
} ObjCClassData;

extern PyType_Spec ObjCClass_spec;

typedef struct {
    Class value;
} ObjCMetaClassData;

extern PyType_Spec ObjCMetaClass_spec;

typedef struct {
    Method value;
} ObjCMethodData;

extern PyType_Spec ObjCMethod_spec;

typedef struct {
    id value;
} ObjCObjectData;

extern PyType_Spec ObjCObject_spec;

typedef struct {
    SEL value;
} ObjCSelectorData;

extern PyType_Spec ObjCSelector_spec;

#endif // OBJCTYPES_MODULE_H
