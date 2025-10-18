"""Test functions for ObjCMethod."""

import pytest

from objctypes import ObjCMethod


def test_objcmethod_from_address_wrong_arg() -> None:
    """Test ObjCMethod.from_address() with wrong arguments."""
    with pytest.raises(TypeError) as excinfo:
        ObjCMethod.from_address("wrong argument")  # type: ignore[arg-type]
    assert (
        str(excinfo.value)
        == "ObjCMethod.from_address() argument 1 must be int, not str"
    )
