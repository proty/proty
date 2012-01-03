String
======

Strings are written in double quotes::

    "This is a simple string"

Escape sequences
----------------

Strings can contain the following escape sequences:

+--------------+-----------------+
| sequence     | function        |
+==============+=================+
| ``\a``       | bell            |
+--------------+-----------------+
| ``\b``       | backspace       |
+--------------+-----------------+
| ``\f``       | formfeed        |
+--------------+-----------------+
| ``\n``       | linefeed        |
+--------------+-----------------+
| ``\r``       | carriage return |
+--------------+-----------------+
| ``\t``       | horizontal tab  |
+--------------+-----------------+
| ``\v``       | vertical tab    |
+--------------+-----------------+
| ``\xhh``     | custom with the |
|              | hex value *hh*  |
+--------------+-----------------+

Methods
-------

.. function:: String.+(other)

   Appends another **String** to the string.

.. function:: String.==(other)

   Returns ``true`` if the string equals the **String** ``other``.

.. function:: String.!=(other)

   Returns ``true`` if the string doesn't equal the **String**
   ``other``.

.. function:: String.bool()

   Returns ``true`` if the length of the string is greater than zero,
   otherwise ``false``.

.. function:: String.length()

   Returns the length of the string as **Integer**.

.. function:: String.split(sep)

   Splits the string at occurencies of ``sep`` and returns a **List**
   of the seperated strings.
