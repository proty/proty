.. highlightlang:: c
.. c-api-intro:

==============
 Introduction
==============

Proty offers a very simple C API because the runtime library is
written entirely in C.

Include Files
=============

To get access to all necessary functions and definitions include the
following header::

    #include <proty/runtime.h>

Create a Module
===============

A module must contain a ``foo_init()`` function where *foo* is the
name of the module::

    Object* foo_init() {
      Object* foo = Object_new(Object_proto);
      return foo;
    }

This function will be called with the ``load("foo")`` function in a
proty program::

    foo = load("foo")

``foo`` is now the object returned by the ``foo_init()`` function.
