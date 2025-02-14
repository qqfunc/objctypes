"""Test functions for ObjCSelector."""

from objctypes import ObjCSelector


def test_objcselector() -> None:
    """Test instantiation of ObjCSelector."""
    ObjCSelector("someMethod")
    ObjCSelector("someMethod:")
    ObjCSelector("someMethod:arg:")


def test_objcselector_cache() -> None:
    """Test if ObjCSelector objects are cached."""
    sel1 = ObjCSelector("someMethod")
    sel2 = ObjCSelector("someMethod:")
    sel3 = ObjCSelector("someMethod:arg:")

    assert sel1 is ObjCSelector("someMethod")
    assert sel2 is ObjCSelector("someMethod:")
    assert sel3 is ObjCSelector("someMethod:arg:")

    assert sel1 is not sel2
    assert sel2 is not sel3
    assert sel3 is not sel1


def test_objcselector_address() -> None:
    """Test ObjCSelector.address."""
    sel1 = ObjCSelector("someMethod")
    sel2 = ObjCSelector("someMethod:")
    sel3 = ObjCSelector("someMethod:arg:")

    assert sel1.address == ObjCSelector("someMethod").address
    assert sel2.address == ObjCSelector("someMethod:").address
    assert sel3.address == ObjCSelector("someMethod:arg:").address

    assert sel1.address != sel2.address
    assert sel2.address != sel3.address
    assert sel3.address != sel1.address


def test_objcselector_from_address() -> None:
    """Test ObjCSelector.from_address()."""
    sel1 = ObjCSelector("someMethod")
    sel2 = ObjCSelector("someMethod:")
    sel3 = ObjCSelector("someMethod:arg:")

    assert sel1 is ObjCSelector.from_address(sel1.address)
    assert sel2 is ObjCSelector.from_address(sel2.address)
    assert sel3 is ObjCSelector.from_address(sel3.address)


def test_objcselector_repr() -> None:
    """Test ObjCSelector.__str__() and ObjCSelector.__repr__()."""
    sel1 = ObjCSelector("someMethod")
    sel2 = ObjCSelector("someMethod:")
    sel3 = ObjCSelector("someMethod:arg:")

    assert repr(sel1) == "ObjCSelector('someMethod')"
    assert repr(sel2) == "ObjCSelector('someMethod:')"
    assert repr(sel3) == "ObjCSelector('someMethod:arg:')"

    assert str(sel1) == "someMethod"
    assert str(sel2) == "someMethod:"
    assert str(sel3) == "someMethod:arg:"


def test_objcselector_name() -> None:
    """Test ObjCSelector.name."""
    sel1 = ObjCSelector("someMethod")
    sel2 = ObjCSelector("someMethod:")
    sel3 = ObjCSelector("someMethod:arg:")

    assert sel1.name == "someMethod"
    assert sel2.name == "someMethod:"
    assert sel3.name == "someMethod:arg:"
