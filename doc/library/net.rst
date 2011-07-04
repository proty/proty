:mod:`net` --- Network functions
================================

.. module:: net

Sockets
-------

.. function:: socket()

   Returns a **Socket** object.

.. method:: Socket.connect(host, port)

   Connects to the specified host on the given port.

.. method:: Socket.send(str)

   Sends ``str`` to the socket.

.. method:: Socket.recv(len)

   Receives data from the socket. The maximum amount of data is
   specified by ``len``.
