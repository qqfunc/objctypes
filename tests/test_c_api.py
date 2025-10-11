"""Test functions for objctypes C API."""

import sys
from ctypes import PyDLL, c_void_p, py_object
from pathlib import Path

from objctypes import NO, YES, ObjCClass, ObjCSelector

LIB_NAME = (
    f"objctypes.cpython-{sys.version_info.major}{sys.version_info.minor}-"
    "darwin.so"
)

for path in sys.path:
    if (lib := Path(path) / LIB_NAME).is_file():
        clib = PyDLL(lib)
        break
else:
    msg = f"{LIB_NAME} is not found in sys.path"
    raise FileNotFoundError(msg)


def test_objcclass_from_class() -> None:
    """Test ObjCClass_FromClass()."""
    clib.ObjCClass_FromClass.restype = py_object
    clib.ObjCClass_FromClass.argtypes = (c_void_p,)

    NSObject = ObjCClass("NSObject")  # noqa: N806
    NSString = ObjCClass("NSString")  # noqa: N806
    NSNumber = ObjCClass("NSNumber")  # noqa: N806

    assert NSObject is clib.ObjCClass_FromClass(NSObject.address)
    assert NSString is clib.ObjCClass_FromClass(NSString.address)
    assert NSNumber is clib.ObjCClass_FromClass(NSNumber.address)


def test_objcobject_from_id() -> None:
    """Test ObjCObject_FromId()."""
    clib.ObjCMethod_FromMethod.restype = py_object
    clib.ObjCMethod_FromMethod.argtypes = (c_void_p,)


def test_objcmethod_from_method() -> None:
    """Test ObjCMethod_FromMethod()."""
    clib.ObjCMethod_FromMethod.restype = py_object
    clib.ObjCMethod_FromMethod.argtypes = (c_void_p,)


def test_objcselector_from_sel() -> None:
    """Test ObjCSelector_FromSEL()."""
    clib.ObjCSelector_FromSEL.restype = py_object
    clib.ObjCSelector_FromSEL.argtypes = (c_void_p,)

    sel1 = ObjCSelector("someMethod")
    sel2 = ObjCSelector("someMethod:")
    sel3 = ObjCSelector("someMethod:arg:")

    assert sel1 is clib.ObjCSelector_FromSEL(sel1.address)
    assert sel2 is clib.ObjCSelector_FromSEL(sel2.address)
    assert sel3 is clib.ObjCSelector_FromSEL(sel3.address)


def test_objcbool_from_long() -> None:
    """Test ObjCBool_FromLong()."""
    clib.ObjCBool_FromLong.restype = py_object
    clib.ObjCBool_FromLong.argtypes = (c_void_p,)

    assert YES is clib.ObjCBool_FromLong(1)
    assert NO is clib.ObjCBool_FromLong(0)
