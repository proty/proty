:mod:`os` --- Operating system interfaces
=========================================

.. module:: os

Environment variables
---------------------

.. function:: getenv(varname)

   Returns the value of the environment variable ``varname``.

.. function:: setenv(varname, value)

   Sets the environment variable ``varname`` to ``value``.

Process management
------------------

.. function:: system(command)

   Execute ``command`` in a subshell. Returns the status code of the
   command.

.. function:: exit(status)

   Exit with the specified ``status`` code.
