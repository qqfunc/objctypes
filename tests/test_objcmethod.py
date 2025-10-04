"""Test functions for ObjCMethod."""

import pytest

from objctypes import ObjCClass, ObjCMethod


def test_objcmethod_from_address_wrong_arg() -> None:
    """Test ObjCMethod.from_address() with wrong arguments."""
    with pytest.raises(TypeError) as excinfo:
        ObjCMethod.from_address("wrong argument")  # type: ignore[arg-type]
    assert (
        str(excinfo.value)
        == "ObjCMethod.from_address() argument 1 must be int, not str"
    )


def test_objcmethod_from_class() -> None:
    """Test instantiation of ObjCMethod from an Objective-C class."""
    NSObject = ObjCClass("NSObject")  # noqa: N806

    ObjCMethod.from_class(NSObject, "alloc")

    with pytest.raises(AttributeError) as excinfo:
        ObjCMethod.from_class(NSObject, "nonexistentMethod")

    assert (
        str(excinfo.value)
        == "Objective-C class NSObject has no class method 'nonexistentMethod'"
    )
