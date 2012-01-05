.. highlightlang:: bash

Installation
============

In order to install proty you should check if there is a package
available for your operation system or linux distribution. If not you
have to build proty from source (see :ref:`from-source`).

Arch Linux
----------

There is a build script in the AUR to build the most recent proty
release. If you have yaourt installed you can install proty by
invoking this command::

    $ yaourt -S proty

If not you can install proty like this::

    $ mkdir proty && cd proty
    $ wget http://aur.archlinux.org/packages/pr/proty/PKGBUILD
    $ makepkg -i

.. _from-source:

From Source
-----------

To build proty from source you have to download the latest source
release from `proty.cc <http://proty.cc>`_. Extract it and execute
the following commands::

    $ mkdir build && cd build
    $ cmake /path/to/source
    $ make
    $ sudo make install
