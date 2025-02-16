"""Test functions for ObjCClass."""

import pytest

from objctypes import ObjCClass


def test_objcclass() -> None:
    """Test instantiation of ObjCClass."""
    ObjCClass("NSObject")
    ObjCClass("NSString")
    ObjCClass("NSNumber")

    with pytest.raises(NameError) as excinfo:
        ObjCClass("NonexistentClass")

    assert (
        str(excinfo.value)
        == "Objective-C class 'NonexistentClass' is not defined"
    )


def test_objcclass_cache() -> None:
    """Test if ObjCClass objects are cached."""
    NSObject = ObjCClass("NSObject")  # noqa: N806
    NSString = ObjCClass("NSString")  # noqa: N806
    NSNumber = ObjCClass("NSNumber")  # noqa: N806

    assert NSObject is ObjCClass("NSObject")
    assert NSString is ObjCClass("NSString")
    assert NSNumber is ObjCClass("NSNumber")

    assert NSObject is not NSString
    assert NSString is not NSNumber
    assert NSNumber is not NSObject


def test_objcclass_address() -> None:
    """Test ObjCClass.address."""
    NSObject = ObjCClass("NSObject")  # noqa: N806
    NSString = ObjCClass("NSString")  # noqa: N806
    NSNumber = ObjCClass("NSNumber")  # noqa: N806

    assert NSObject.address == ObjCClass("NSObject").address
    assert NSString.address == ObjCClass("NSString").address
    assert NSNumber.address == ObjCClass("NSNumber").address

    assert NSObject.address != NSString.address
    assert NSString.address != NSNumber.address
    assert NSNumber.address != NSObject.address


def test_objcclass_from_address() -> None:
    """Test ObjCClass.from_address()."""
    with pytest.raises(TypeError) as excinfo:
        ObjCClass.from_address(0)
    assert str(excinfo.value) == "the specified Objective-C class is Nil"

    NSObject = ObjCClass("NSObject")  # noqa: N806
    NSString = ObjCClass("NSString")  # noqa: N806
    NSNumber = ObjCClass("NSNumber")  # noqa: N806

    assert NSObject.address == ObjCClass.from_address(NSObject.address).address
    assert NSString.address == ObjCClass.from_address(NSString.address).address
    assert NSNumber.address == ObjCClass.from_address(NSNumber.address).address


def test_objcclass_repr() -> None:
    """Test ObjCClass.__str__() and ObjCClass.__repr__()."""
    NSObject = ObjCClass("NSObject")  # noqa: N806
    NSString = ObjCClass("NSString")  # noqa: N806
    NSNumber = ObjCClass("NSNumber")  # noqa: N806

    assert repr(NSObject) == "ObjCClass('NSObject')"
    assert repr(NSString) == "ObjCClass('NSString')"
    assert repr(NSNumber) == "ObjCClass('NSNumber')"


def test_objcclass_name() -> None:
    """Test ObjCClass.name."""
    NSObject = ObjCClass("NSObject")  # noqa: N806
    NSString = ObjCClass("NSString")  # noqa: N806
    NSNumber = ObjCClass("NSNumber")  # noqa: N806

    assert NSObject.name == "NSObject"
    assert NSString.name == "NSString"
    assert NSNumber.name == "NSNumber"


def test_objcclass_inheritance() -> None:
    """Test inheritance relationships of ObjCClass."""
    NSObject = ObjCClass("NSObject")  # noqa: N806
    NSString = ObjCClass("NSString")  # noqa: N806
    NSNumber = ObjCClass("NSNumber")  # noqa: N806

    assert issubclass(NSString, NSObject)
    assert issubclass(NSNumber, NSObject)
    assert not issubclass(NSObject, NSString)
    assert not issubclass(NSObject, NSNumber)
    assert not issubclass(NSNumber, NSString)
    assert not issubclass(NSString, NSNumber)
