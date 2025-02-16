"""Test functions for ObjCBool."""

from objctypes import NO, YES, ObjCBool


def test_objcbool() -> None:
    """Test instantiation of ObjCBool."""
    assert ObjCBool(1) == YES
    assert ObjCBool(0) == NO

    # Test if YES and NO are different.
    assert YES is not NO
    assert YES != NO

    # Test if the default value is NO.
    assert ObjCBool() == NO


def test_objcbool_cache() -> None:
    """Test if ObjCBool objects are cached."""
    assert YES is ObjCBool(1)
    assert NO is ObjCBool(0)


def test_objcbool_repr() -> None:
    """Test ObjCBool.__str__() and ObjCBool.__repr__()."""
    assert repr(YES) == "ObjCBool(True)"
    assert repr(NO) == "ObjCBool(False)"

    assert str(YES) == "YES"
    assert str(NO) == "NO"


def test_objcbool_bool() -> None:
    """Test ObjCBool.__bool__()."""
    assert bool(YES)
    assert not bool(NO)


def test_objcbool_invert() -> None:
    """Test ObjCBool.__invert__()."""
    assert ~YES == NO
    assert ~NO == YES
    assert isinstance(~YES, ObjCBool)
    assert isinstance(~NO, ObjCBool)


def test_objcbool_int() -> None:
    """Test ObjCBool.__int__()."""
    int_yes = int(YES)
    int_no = int(NO)

    # Test values of of ObjCBool.__int__().
    assert int_yes == 1
    assert int_no == 0

    # Test type of ObjCBool.__int__() is int.
    assert isinstance(int_yes, int)
    assert isinstance(int_no, int)


def test_objcbool_float() -> None:
    """Test ObjCBool.__float__()."""
    float_yes = float(YES)
    float_no = float(NO)

    # Test values of of ObjCBool.__float__().
    assert float_yes == 1.0
    assert float_no == 0.0

    # Test type of ObjCBool.__float__() is float.
    assert isinstance(float_yes, float)
    assert isinstance(float_no, float)
