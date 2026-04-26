"""Test functions for ObjCMethod."""

import pytest

from objctypes import ObjCMethod


def test_objcmethod_from_address() -> None:
    """Test ObjCMethod.from_address()."""
    with pytest.raises(ValueError) as excinfo:
        ObjCMethod.from_address(0)
    assert str(excinfo.value) == "The specified Objective-C method is NULL"


def test_objcmethod_from_address_wrong_arg() -> None:
    """Test ObjCMethod.from_address() with wrong arguments."""
    with pytest.raises(TypeError) as excinfo:
        ObjCMethod.from_address("wrong argument")  # ty: ignore[invalid-argument-type]
    assert (
        str(excinfo.value)
        == "ObjCMethod.from_address() argument 1 must be int, not str"
    )
