"""Test functions for ObjCMetaClass."""

import pytest

from objctypes import ObjCClass, ObjCMetaClass


def test_objcmetaclass_is_metaclass_of_objcclass() -> None:
    """Test that ObjCClass is an instance of ObjCMetaClass."""
    assert type(ObjCClass) is ObjCMetaClass


def test_objcmetaclass_inherits_type() -> None:
    """Test that ObjCMetaClass inherits from type."""
    assert issubclass(ObjCMetaClass, type)


def test_objcmetaclass_from_name() -> None:
    """Test ObjCMetaClass.from_name()."""
    ObjCMetaClass.from_name("NSObject")
    ObjCMetaClass.from_name("NSString")
    ObjCMetaClass.from_name("NSNumber")

    with pytest.raises(NameError) as excinfo:
        ObjCMetaClass.from_name("NonexistentClass")

    assert (
        str(excinfo.value)
        == "Objective-C class 'NonexistentClass' is not defined"
    )


def test_objcmetaclass_doc() -> None:
    """Test docstring of ObjCMetaClass."""
    assert ObjCMetaClass.__doc__ is not None


def test_objcmetaclass_repr() -> None:
    """Test ObjCMetaClass.__repr__()."""
    NSObjectMeta = ObjCMetaClass.from_name("NSObject")  # noqa: N806
    NSStringMeta = ObjCMetaClass.from_name("NSString")  # noqa: N806
    NSNumberMeta = ObjCMetaClass.from_name("NSNumber")  # noqa: N806

    assert repr(NSObjectMeta) == "<ObjCMetaClass 'NSObject'>"
    assert repr(NSStringMeta) == "<ObjCMetaClass 'NSString'>"
    assert repr(NSNumberMeta) == "<ObjCMetaClass 'NSNumber'>"


def test_objcmetaclass_cache() -> None:
    """Test if ObjCMetaClass objects are cached."""
    NSObjectMeta = ObjCMetaClass.from_name("NSObject")  # noqa: N806
    NSStringMeta = ObjCMetaClass.from_name("NSString")  # noqa: N806
    NSNumberMeta = ObjCMetaClass.from_name("NSNumber")  # noqa: N806

    assert NSObjectMeta is ObjCMetaClass.from_name("NSObject")
    assert NSStringMeta is ObjCMetaClass.from_name("NSString")
    assert NSNumberMeta is ObjCMetaClass.from_name("NSNumber")

    assert NSObjectMeta is not NSStringMeta
    assert NSStringMeta is not NSNumberMeta
    assert NSNumberMeta is not NSObjectMeta


def test_objcmetaclass_address() -> None:
    """Test ObjCMetaClass.address."""
    NSObjectMeta = ObjCMetaClass.from_name("NSObject")  # noqa: N806
    NSStringMeta = ObjCMetaClass.from_name("NSString")  # noqa: N806
    NSNumberMeta = ObjCMetaClass.from_name("NSNumber")  # noqa: N806

    assert NSObjectMeta.address == ObjCMetaClass.from_name("NSObject").address
    assert NSStringMeta.address == ObjCMetaClass.from_name("NSString").address
    assert NSNumberMeta.address == ObjCMetaClass.from_name("NSNumber").address

    assert NSObjectMeta.address != NSStringMeta.address
    assert NSStringMeta.address != NSNumberMeta.address
    assert NSNumberMeta.address != NSObjectMeta.address


def test_objcmetaclass_name() -> None:
    """Test ObjCMetaClass.name."""
    NSObjectMeta = ObjCMetaClass.from_name("NSObject")  # noqa: N806
    NSStringMeta = ObjCMetaClass.from_name("NSString")  # noqa: N806
    NSNumberMeta = ObjCMetaClass.from_name("NSNumber")  # noqa: N806

    assert NSObjectMeta.name == "NSObject"
    assert NSStringMeta.name == "NSString"
    assert NSNumberMeta.name == "NSNumber"


def test_objcmetaclass_from_address() -> None:
    """Test ObjCMetaClass.from_address()."""
    NSObjectMeta = ObjCMetaClass.from_name("NSObject")  # noqa: N806
    NSStringMeta = ObjCMetaClass.from_name("NSString")  # noqa: N806
    NSNumberMeta = ObjCMetaClass.from_name("NSNumber")  # noqa: N806

    assert (
        NSObjectMeta.address
        == ObjCMetaClass.from_address(NSObjectMeta.address).address
    )
    assert (
        NSStringMeta.address
        == ObjCMetaClass.from_address(NSStringMeta.address).address
    )
    assert (
        NSNumberMeta.address
        == ObjCMetaClass.from_address(NSNumberMeta.address).address
    )


def test_objcmetaclass_from_address_wrong_arg() -> None:
    """Test ObjCMetaClass.from_address() with wrong argument type."""
    with pytest.raises(TypeError) as excinfo:
        ObjCMetaClass.from_address("wrong argument")  # ty: ignore[invalid-argument-type]
    assert (
        str(excinfo.value)
        == "ObjCMetaClass.from_address() argument 1 must be int, not str"
    )


def test_objcmetaclass_from_address_nil() -> None:
    """Test ObjCMetaClass.from_address() with Nil."""
    with pytest.raises(TypeError) as excinfo:
        ObjCMetaClass.from_address(0)
    assert str(excinfo.value) == "the specified Objective-C metaclass is Nil"


def test_objcclass_from_address_rejects_metaclass() -> None:
    """Test that ObjCClass.from_address() rejects a metaclass."""
    NSObjectMeta = ObjCMetaClass.from_name("NSObject")  # noqa: N806

    with pytest.raises(TypeError) as excinfo:
        ObjCClass.from_address(NSObjectMeta.address)

    assert "ObjCMetaClass.from_address() instead" in str(excinfo.value)
