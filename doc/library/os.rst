:mod:`os` --- Operating system interfaces
=========================================

Environment variables
---------------------

.. function:: os.getenv(varname)

   Returns the value of the environment variable ``varname``.

.. function:: os.setenv(varname, value)

   Sets the environment variable ``varname`` to ``value``.

Process management
------------------

.. function:: os.system(command)

   Execute ``command`` in a subshell. Returns the status code of the
   command.

.. function:: os.exit(status)

   Exit with the specified ``status`` code.
