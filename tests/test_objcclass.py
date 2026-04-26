"""Test functions for ObjCClass."""

import pytest

from objctypes import ObjCClass, ObjCMetaClass, ObjCObject


def test_objcclass() -> None:
    """Test instantiation of ObjCClass."""
    ObjCClass.from_name("NSObject")
    ObjCClass.from_name("NSString")
    ObjCClass.from_name("NSNumber")

    with pytest.raises(NameError) as excinfo:
        ObjCClass.from_name("NonexistentClass")

    assert (
        str(excinfo.value)
        == "Objective-C class 'NonexistentClass' is not defined"
    )


def test_objcclass_inheritance() -> None:
    """Test inheritance relationships of ObjCClass."""
    NSObject = ObjCClass.from_name("NSObject")  # noqa: N806
    NSString = ObjCClass.from_name("NSString")  # noqa: N806
    NSNumber = ObjCClass.from_name("NSNumber")  # noqa: N806

    assert issubclass(NSObject, ObjCObject)
    assert issubclass(NSString, ObjCObject)
    assert issubclass(NSNumber, ObjCObject)

    assert issubclass(NSString, NSObject)
    assert issubclass(NSNumber, NSObject)

    assert not issubclass(NSObject, NSString)
    assert not issubclass(NSObject, NSNumber)
    assert not issubclass(NSNumber, NSString)
    assert not issubclass(NSString, NSNumber)


def test_objcclass_cache() -> None:
    """Test if ObjCClass objects are cached."""
    NSObject = ObjCClass.from_name("NSObject")  # noqa: N806
    NSString = ObjCClass.from_name("NSString")  # noqa: N806
    NSNumber = ObjCClass.from_name("NSNumber")  # noqa: N806

    assert NSObject is ObjCClass.from_name("NSObject")
    assert NSString is ObjCClass.from_name("NSString")
    assert NSNumber is ObjCClass.from_name("NSNumber")

    assert NSObject is not NSString
    assert NSString is not NSNumber
    assert NSNumber is not NSObject


def test_objcclass_doc() -> None:
    """Test docstring of ObjCClass."""
    assert ObjCClass.__doc__ is not None


def test_objcclass_repr() -> None:
    """Test ObjCClass.__str__() and ObjCClass.__repr__()."""
    NSObject = ObjCClass.from_name("NSObject")  # noqa: N806
    NSString = ObjCClass.from_name("NSString")  # noqa: N806
    NSNumber = ObjCClass.from_name("NSNumber")  # noqa: N806

    assert repr(NSObject) == "<ObjCClass 'NSObject'>"
    assert repr(NSString) == "<ObjCClass 'NSString'>"
    assert repr(NSNumber) == "<ObjCClass 'NSNumber'>"


def test_objcclass_repr_objcobject_class() -> None:
    """Test ObjCClass.__repr__() for the ObjCObject class."""
    assert repr(ObjCObject) == "<class 'objctypes.ObjCObject'>"


def test_objcclass_from_address() -> None:
    """Test ObjCClass.from_address()."""
    NSObject = ObjCClass.from_name("NSObject")  # noqa: N806
    NSString = ObjCClass.from_name("NSString")  # noqa: N806
    NSNumber = ObjCClass.from_name("NSNumber")  # noqa: N806

    assert NSObject.address == ObjCClass.from_address(NSObject.address).address
    assert NSString.address == ObjCClass.from_address(NSString.address).address
    assert NSNumber.address == ObjCClass.from_address(NSNumber.address).address

    with pytest.raises(TypeError):
        ObjCClass.from_address(object())  # ty: ignore[invalid-argument-type]


def test_objcclass_from_address_wrong_arg() -> None:
    """Test ObjCClass.from_address() with wrong arguments."""
    with pytest.raises(TypeError) as excinfo:
        ObjCClass.from_address("wrong argument")  # ty: ignore[invalid-argument-type]
    assert (
        str(excinfo.value)
        == "ObjCClass.from_address() argument 1 must be int, not str"
    )


def test_objcclass_from_address_rejects_nil() -> None:
    """Test that ObjCClass.from_address() rejects Nil."""
    with pytest.raises(ValueError) as excinfo:
        ObjCClass.from_address(0)
    assert str(excinfo.value) == "The specified Objective-C class is Nil"


def test_objcclass_from_address_rejects_object() -> None:
    """Test that ObjCClass.from_address() rejects an object."""


def test_objcclass_from_address_rejects_metaclass() -> None:
    """Test that ObjCClass.from_address() rejects a metaclass."""
    NSObjectMeta = ObjCMetaClass.from_name("NSObject")  # noqa: N806

    with pytest.raises(ValueError) as excinfo:
        ObjCClass.from_address(NSObjectMeta.address)

    assert str(excinfo.value) == (
        f"The Objective-C class at {NSObjectMeta.address:#x} is a metaclass. "
        "Use ObjCMetaClass.from_address() instead."
    )


def test_objcclass_from_address_objcobject_class() -> None:
    """Test ObjCClass.from_address() for the ObjCObject class."""
    with pytest.raises(TypeError) as excinfo:
        ObjCClass.from_address(ObjCObject.address)

    assert (
        str(excinfo.value) == "ObjCObject is not an actual Objective-C class"
    )


def test_objcclass_address() -> None:
    """Test ObjCClass.address."""
    NSObject = ObjCClass.from_name("NSObject")  # noqa: N806
    NSString = ObjCClass.from_name("NSString")  # noqa: N806
    NSNumber = ObjCClass.from_name("NSNumber")  # noqa: N806

    assert NSObject.address == ObjCClass.from_name("NSObject").address
    assert NSString.address == ObjCClass.from_name("NSString").address
    assert NSNumber.address == ObjCClass.from_name("NSNumber").address

    assert NSObject.address != NSString.address
    assert NSString.address != NSNumber.address
    assert NSNumber.address != NSObject.address


def test_objcclass_address_objcobject_class() -> None:
    """Test ObjCClass.address for the ObjCObject class."""
    with pytest.raises(TypeError) as excinfo:
        _ = ObjCObject.address
    assert (
        str(excinfo.value) == "ObjCObject is not an actual Objective-C class"
    )


def test_objcclass_name() -> None:
    """Test ObjCClass.name."""
    NSObject = ObjCClass.from_name("NSObject")  # noqa: N806
    NSString = ObjCClass.from_name("NSString")  # noqa: N806
    NSNumber = ObjCClass.from_name("NSNumber")  # noqa: N806

    assert NSObject.name == "NSObject"
    assert NSString.name == "NSString"
    assert NSNumber.name == "NSNumber"


def test_objcclass_name_objcobject_class() -> None:
    """Test ObjCClass.name for the ObjCObject class."""
    with pytest.raises(TypeError) as excinfo:
        _ = ObjCObject.name
    assert (
        str(excinfo.value) == "ObjCObject is not an actual Objective-C class"
    )
