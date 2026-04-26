from types import GenericAlias
from typing import Any, Self, final

@final
class ObjCMetaClass(type):
    """A Python wrapper class for an Objective-C metaclass.

    Equivalent to the metaclass of an Objective-C class, which describes
    the class object itself (e.g. class methods).
    """

    @classmethod
    def from_address(cls, address: int, /) -> type[ObjCClass]:
        """Retrieve an Objective-C metaclass from the specified address.

        :param address: The address of the Objective-C metaclass.
        :return: The Objective-C metaclass that was retrieved.
        :raises ValueError: if the address points to a non-metaclass
            Objective-C object or regular class

        .. warning::
            Passing an invalid address may cause crashes.
        """

    @classmethod
    def from_name(cls, name: str, /) -> type[ObjCClass]:
        """Retrieve an Objective-C metaclass by class name.

        :param name: The name of the Objective-C class whose metaclass
            to retrieve.
        :return: The Objective-C metaclass that was retrieved.
        :raises NameError: if the specified name does not correspond to
            any Objective-C class
        """

    @property
    def address(cls) -> int:
        """The address of the Objective-C metaclass."""

    @property
    def name(cls) -> str:
        """The name of the Objective-C metaclass."""

class ObjCClass(type, metaclass=ObjCMetaClass):
    """A Python wrapper class for an Objective-C class.

    Equivalent to
    `Class <https://developer.apple.com/documentation/objectivec/class>`__
    of Objective-C.
    """

    def __init__(
        cls: type[ObjCClass],
        name: str,
        bases: tuple[ObjCClass, ...],
        attrs: dict[str, object],
        /,
    ) -> None: ...
    @classmethod
    def from_address(cls, address: int, /) -> type[ObjCObject]:
        """Retrieve an Objective-C class from the specified address.

        :param address: The address of the Objective-C class.
        :return: The Objective-C class that was retrieved.
        :raises ValueError: if the address points to a non-class
            Objective-C object or metaclass

        .. warning::
            Passing an invalid address may cause crashes.
        """

    @classmethod
    def from_name(cls, name: str, /) -> type[ObjCObject]:
        """Retrieve an Objective-C class from the specified name.

        :param name: The name of the Objective-C class.
        :return: The Objective-C class that was retrieved.
        :raises NameError: if the specified name does not correspond to
            any Objective-C class
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
    def __class_getitem__(cls, item: Any, /) -> GenericAlias: ...
    @classmethod
    def from_address(cls, address: int, /) -> Self:
        """Retrieve an Objective-C object from the specified address.

        :param address: The address of the Objective-C object.
        :return: The Objective-C object that was retrieved.
        :raises ValueError: if the address points to an Objective-C
            class

        .. warning::
            Passing an invalid address may cause crashes.
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
        :raises ValueError: if the address points to NULL

        .. warning::
            Passing an invalid address may cause crashes.
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
        :raises ValueError: if the address points to NULL

        .. warning::
            Passing an invalid address may cause crashes.
        """

    @property
    def address(self) -> int:
        """The address of the Objective-C selector."""

    @property
    def name(self) -> str:
        """The name of the Objective-C selector."""

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
