"""Test functions for ObjCMethod."""

import pytest

from objctypes import ObjCClass, ObjCMethod


def test_objcmethod_from_class() -> None:
    """Test instantiation of ObjCMethod from an Objective-C class."""
    NSObject = ObjCClass("NSObject")  # noqa: N806

    ObjCMethod.from_class(NSObject, "alloc")

    with pytest.raises(AttributeError) as excinfo:
        ObjCMethod.from_class(NSObject, "nonexistentMethod")

    assert (
        str(excinfo.value)
        == "Objective-C class NSObject has no method 'nonexistentMethod'"
    )
