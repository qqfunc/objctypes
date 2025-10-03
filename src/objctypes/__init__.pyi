from typing import Self, final, overload

@final  # NOTE: final?
class ObjCClass(type):  # NOTE: type?
    """A Python wrapper class for an Objective-C class.

    Equivalent to
    `Class <https://developer.apple.com/documentation/objectivec/class>`__
    of Objective-C.
    """

    @overload
    def __new__(cls, name: str, /) -> ObjCClass: ...
    @overload
    def __new__(
        cls,
        name: str,
        bases: tuple[ObjCClass, ...],
        dict: dict[str, object],
        /,
    ) -> ObjCClass: ...
    @classmethod
    def from_address(cls, address: int, /) -> ObjCClass:
        """Retrieve an Objective-C class from the specified address.

        :param address: The address of the Objective-C class.
        :return: The Objective-C class that was retrieved.
        """

    @property
    def address(cls) -> int:
        """The address of the Objective-C class."""

    @property
    def name(cls) -> str:
        """The name of the Objective-C class."""

class ObjCObject(metaclass=ObjCClass):
    """A Python wrapper class for an Objective-C object.

    Equivalent to
    `id <https://developer.apple.com/documentation/objectivec/id>`__ of
    Objective-C.
    """

    def __new__(cls, *args: tuple[object, ...]) -> Self: ...
    @classmethod
    def from_address(cls, address: int, /) -> Self:
        """Retrieve an Objective-C object from the specified address.

        :param address: The address of the Objective-C object.
        :return: The Objective-C object that was retrieved.
        """

    @property
    def address(self) -> int:
        """The address of the Objective-C object."""

@final
class ObjCMethod:
    """A Python wrapper class for an Objective-C method.

    Equivalent to
    `Method <https://developer.apple.com/documentation/objectivec/method?language=objc>`__
    of Objective-C.
    """

    @classmethod
    def from_address(cls, address: int, /) -> Self:
        """Retrieve an Objective-C method from the specified address.

        :param address: The address of the Objective-C method.
        :return: The Objective-C method that was retrieved.
        """

    @classmethod
    def from_class(
        cls,
        objc_class: ObjCClass,
        selector: ObjCSelector | str | bytes,
        /,
    ) -> Self:
        """Retrieve an Objective-C class method.

        :param objc_class: The Objective-C class of the Objective-C
            class method.
        :param selector: The selector of the Objective-C class method.
        :return: The Objective-C selector that was retrieved.
        :raise AttributeError: if the Objective-C class has no class
            method with the specified selector
        """

    @property
    def address(self) -> int:
        """The address of the Objective-C method."""

    @property
    def name(self) -> str:
        """The name of the Objective-C method."""

@final
class ObjCSelector:
    """A Python wrapper class for an Objective-C selector.

    Equivalent to
    `SEL <https://developer.apple.com/documentation/objectivec/sel>`__
    of Objective-C.
    """

    def __new__(cls, name: str = ..., /) -> Self: ...
    @classmethod
    def from_address(cls, address: int, /) -> Self:
        """Retrieve an Objective-C selector from the specified address.

        :param address: The address of the Objective-C selector.
        :return: The Objective-C selector that was retrieved.
        """

    @property
    def address(self) -> int:
        """The address of the Objective-C selector."""

    @property
    def name(self) -> str:
        """The name of the Objective-C selector."""

    @property
    def is_mapped(self) -> bool:
        """Whether the Objective-C selector is mapped."""

@final
class ObjCBool:
    """A Python wrapper class for an Objective-C Boolean.

    Equivalent to
    `BOOL <https://developer.apple.com/documentation/objectivec/bool>`__
    of Objective-C.
    """

    def __new__(cls, value: object = ..., /) -> Self: ...
    def __bool__(self) -> bool: ...
    def __and__(self, other: Self) -> Self: ...
    def __xor__(self, other: Self) -> Self: ...
    def __or__(self, other: Self) -> Self: ...
    def __invert__(self) -> Self: ...
    def __int__(self) -> int: ...
    def __float__(self) -> float: ...

YES: ObjCBool
""":class:`ObjCBool` value of true.

Equivalent to
`YES <https://developer.apple.com/documentation/objectivec/yes>`__ of
Objective-C.
"""

NO: ObjCBool
""":class:`ObjCBool` value of false.

Equivalent to
`NO <https://developer.apple.com/documentation/objectivec/no>`__ of
Objective-C.
"""
