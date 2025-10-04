"""Test functions for ObjCObject."""

import pytest

from objctypes import ObjCObject


def test_objcobject() -> None:
    """Test instantiation of ObjCObject."""
    with pytest.raises(TypeError) as excinfo:
        ObjCObject()
    assert str(excinfo.value) == "ObjCObject cannot be initialized directly"


def test_objcobject_type() -> None:
    """Test ObjCObject type object."""
    assert repr(ObjCObject) == "<class 'objctypes.ObjCObject'>"
    assert ObjCObject.name == ""


def test_objcobject_cache() -> None:
    """Test if ObjCObject objects are cached."""


def test_objcobject_doc() -> None:
    """Test docstring of ObjCObject."""
    assert ObjCObject.__doc__ is not None


def test_objcobject_repr() -> None:
    """Test ObjCObject.__str__() and ObjCObject.__repr__()."""


def test_objcobject_from_address() -> None:
    """Test ObjCObject.from_address()."""


def test_objcobject_from_address_wrong_arg() -> None:
    """Test ObjCObject.from_address() with wrong arguments."""
    with pytest.raises(TypeError) as excinfo:
        ObjCObject.from_address("wrong argument")  # type: ignore[arg-type]
    assert (
        str(excinfo.value)
        == "ObjCObject.from_address() argument 1 must be int, not str"
    )


def test_objcobject_from_address_nil() -> None:
    """Test ObjCObject.from_address() with Nil."""
    with pytest.raises(TypeError) as excinfo:
        ObjCObject.from_address(0)
    assert str(excinfo.value) == "the specified Objective-C object is nil"


def test_objcobject_address() -> None:
    """Test ObjCObject.address."""
