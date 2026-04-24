/**
 * @file objctypes_cache.h
 * @brief Source declarations and definitions for objctypes_cache.h.
 */

#ifndef OBJC_CACHE_H
#define OBJC_CACHE_H

#include <Python.h>

#include "objcmethod.h"
#include "objcobject.h"
#include "objcselector.h"

#include <objc/objc.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize a cache map for `ObjCClass`.
 * @param module The Python module object.
 */
void
ObjCClass_cache_init(PyObject *module);

/**
 * @brief Deinitialize the cache map for `ObjCClass`.
 * @param module The Python module object.
 */
void
ObjCClass_cache_deinit(PyObject *module);

/**
 * @brief Get an `ObjCClass` from the cache if it exists, otherwise return `NULL`.
 * @param module The Python module object.
 * @param cls The Objective-C class to look up.
 * @return A new reference to the cached `ObjCClass` object, or `NULL` if not found.
 */
PyObject *
ObjCClass_cache_get(PyObject *module, Class cls);

/**
 * @brief Set an `ObjCClass` in the cache.
 * @param module The Python module object.
 * @param cls The Objective-C class to cache.
 * @param obj The `ObjCClass` to associate with the class.
 */
void
ObjCClass_cache_set(PyObject *module, Class cls, PyObject *obj);

/**
 * @brief Delete an `ObjCClass` from the cache.
 * @param module The Python module object.
 * @param cls The Objective-C class to delete from the cache.
 */
void
ObjCClass_cache_del(PyObject *module, Class cls);

/**
 * @brief Initialize a cache map for `ObjCObject`.
 * @param module The Python module object.
 */
void
ObjCObject_cache_init(PyObject *module);

/**
 * @brief Deinitialize the cache map for `ObjCObject`.
 * @param module The Python module object.
 */
void
ObjCObject_cache_deinit(PyObject *module);

/**
 * @brief Get an `ObjCObject` from the cache if it exists, otherwise return `NULL`.
 * @param module The Python module object.
 * @param obj The Objective-C object to look up.
 * @return A new reference to the cached `ObjCObject` object, or `NULL` if not found.
 */
ObjCObjectObject *
ObjCObject_cache_get(PyObject *module, id obj);

/**
 * @brief Set an `ObjCObject` in the cache.
 * @param module The Python module object.
 * @param obj The Objective-C object to cache.
 * @param pyobj The `ObjCObject` to associate with the object.
 */
void
ObjCObject_cache_set(PyObject *module, id obj, ObjCObjectObject *pyobj);

/**
 * @brief Delete an `ObjCObject` from the cache.
 * @param module The Python module object.
 * @param obj The Objective-C object to delete from the cache.
 */
void
ObjCObject_cache_del(PyObject *module, id obj);

/**
 * @brief Initialize a cache map for `ObjCMethod`.
 * @param module The Python module object.
 */
void
ObjCMethod_cache_init(PyObject *module);

/**
 * @brief Deinitialize the cache map for `ObjCMethod`.
 * @param module The Python module object.
 */
void
ObjCMethod_cache_deinit(PyObject *module);

/**
 * @brief Get an `ObjCMethod` from the cache if it exists, otherwise return `NULL`.
 * @param module The Python module object.
 * @param method The Objective-C method to look up.
 * @return A new reference to the cached `ObjCMethod` object, or `NULL` if not found.
 */
ObjCMethodObject *
ObjCMethod_cache_get(PyObject *module, Method method);

/**
 * @brief Set an `ObjCMethod` in the cache.
 * @param module The Python module object.
 * @param method The Objective-C method to cache.
 * @param obj The `ObjCMethod` to associate with the method.
 */
void
ObjCMethod_cache_set(PyObject *module, Method method, ObjCMethodObject *obj);

/**
 * @brief Delete an `ObjCMethod` from the cache.
 * @param module The Python module object.
 * @param method The Objective-C method to delete from the cache.
 */
void
ObjCMethod_cache_del(PyObject *module, Method method);

/**
 * @brief Initialize a cache map for `ObjCSelector`.
 * @param module The Python module object.
 */
void
ObjCSelector_cache_init(PyObject *module);

/**
 * @brief Deinitialize the cache map for `ObjCSelector`.
 * @param module The Python module object.
 */
void
ObjCSelector_cache_deinit(PyObject *module);

/**
 * @brief Get an `ObjCSelector` from the cache if it exists, otherwise return `NULL`.
 * @param module The Python module object.
 * @param sel The Objective-C selector to look up.
 * @return A new reference to the cached `ObjCSelector` object, or `NULL` if not found.
 */
void
ObjCSelector_cache_deinit(PyObject *module);

/**
 * @brief Get an `ObjCSelector` from the cache if it exists, otherwise return `NULL`.
 * @param module The Python module object.
 * @param sel The Objective-C selector to look up.
 * @return A new reference to the cached `ObjCSelector` object, or `NULL` if not found.
 */
ObjCSelectorObject *
ObjCSelector_cache_get(PyObject *module, SEL sel);

/**
 * @brief Set an `ObjCSelector` in the cache.
 * @param module The Python module object.
 * @param sel The Objective-C selector to cache.
 * @param obj The `ObjCSelector` to associate with the selector.
 */
void
ObjCSelector_cache_set(PyObject *module, SEL sel, ObjCSelectorObject *obj);

/**
 * @brief Delete an `ObjCSelector` from the cache.
 * @param module The Python module object.
 * @param sel The Objective-C selector to delete from the cache.
 */
void
ObjCSelector_cache_del(PyObject *module, SEL sel);

#ifdef __cplusplus
}
#endif

#endif // OBJC_CACHE_H
