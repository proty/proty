:mod:`io` --- Input/Output functions
====================================

.. module:: io

Output
------

.. function:: print(obj)

   Prints an ``obj`` to *stdout*.

File handling
-------------

.. function:: open(filename, mode)

   Opens a file with the specificied mode. Returns a **File** object.

.. method:: File.read(len)

   Returns ``len`` bytes of the file.

.. method:: File.write(str)

   Writes ``str`` to the file.

.. method:: File.close()

   Closes the file.
