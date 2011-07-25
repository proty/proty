:mod:`io` --- Input/Output functions
====================================

Output
------

.. function:: io.print(obj)

   Prints an ``obj`` to *stdout*.

File handling
-------------

.. function:: io.open(filename, mode)

   Opens a file with the specificied mode. Returns a **File** object.

.. function:: File.read(len)

   Returns ``len`` bytes of the file.

.. function:: File.write(str)

   Writes ``str`` to the file.

.. function:: File.close()

   Closes the file.
