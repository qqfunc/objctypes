objctypes
=========

.. py:module:: objctypes


Attributes
----------

.. autoapisummary::

   objctypes.YES
   objctypes.NO


Classes
-------

.. autoapisummary::

   objctypes.ObjCClass
   objctypes.ObjCObject
   objctypes.ObjCMethod
   objctypes.ObjCSelector
   objctypes.ObjCBool


Package Contents
----------------

.. py:class:: ObjCClass

   Bases: :py:obj:`type`


   A Python wrapper class for an Objective-C class.

   Equivalent to
   `Class <https://developer.apple.com/documentation/objectivec/class>`__
   of Objective-C.


   .. py:method:: from_address(address: int, /) -> ObjCClass
      :classmethod:


      Retrieve an Objective-C class from the specified address.

      :param address: The address of the Objective-C class.
      :return: The Objective-C class that was retrieved.



   .. py:property:: address
      :type: int


      The address of the Objective-C class.



   .. py:property:: name
      :type: str


      The name of the Objective-C class.



.. py:class:: ObjCObject

   A Python wrapper class for an Objective-C object.

   Equivalent to
   `id <https://developer.apple.com/documentation/objectivec/id>`__ of
   Objective-C.


   .. py:method:: from_address(address: int, /) -> Self
      :classmethod:


      Retrieve an Objective-C object from the specified address.

      :param address: The address of the Objective-C object.
      :return: The Objective-C object that was retrieved.



   .. py:property:: address
      :type: int


      The address of the Objective-C object.



.. py:class:: ObjCMethod

   A Python wrapper class for an Objective-C method.

   Equivalent to
   `Method <https://developer.apple.com/documentation/objectivec/method?language=objc>`__
   of Objective-C.


   .. py:method:: from_address(address: int, /) -> Self
      :classmethod:


      Retrieve an Objective-C method from the specified address.

      :param address: The address of the Objective-C method.
      :return: The Objective-C method that was retrieved.



   .. py:property:: address
      :type: int


      The address of the Objective-C method.



   .. py:property:: name
      :type: str


      The name of the Objective-C method.



.. py:class:: ObjCSelector

   A Python wrapper class for an Objective-C selector.

   Equivalent to
   `SEL <https://developer.apple.com/documentation/objectivec/sel>`__
   of Objective-C.


   .. py:method:: from_address(address: int, /) -> Self
      :classmethod:


      Retrieve an Objective-C selector from the specified address.

      :param address: The address of the Objective-C selector.
      :return: The Objective-C selector that was retrieved.



   .. py:property:: address
      :type: int


      The address of the Objective-C selector.



   .. py:property:: name
      :type: str


      The name of the Objective-C selector.



.. py:class:: ObjCBool

   A Python wrapper class for an Objective-C Boolean.

   Equivalent to
   `BOOL <https://developer.apple.com/documentation/objectivec/bool>`__
   of Objective-C.


   .. py:method:: __bool__() -> bool


   .. py:method:: __and__(other: Self) -> Self


   .. py:method:: __xor__(other: Self) -> Self


   .. py:method:: __or__(other: Self) -> Self


   .. py:method:: __invert__() -> Self


   .. py:method:: __int__() -> int


   .. py:method:: __float__() -> float


.. py:data:: YES
   :type:  ObjCBool

   :class:`ObjCBool` value of true.

   Equivalent to
   `YES <https://developer.apple.com/documentation/objectivec/yes>`__ of
   Objective-C.


.. py:data:: NO
   :type:  ObjCBool

   :class:`ObjCBool` value of false.

   Equivalent to
   `NO <https://developer.apple.com/documentation/objectivec/no>`__ of
   Objective-C.


