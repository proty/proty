:mod:`net` --- Network functions
================================

Sockets
-------

.. function:: net.socket()

   Returns a **Socket** object.

.. function:: Socket.connect(host, port)

   Connects to the specified host on the given port.

.. function:: Socket.send(str)

   Sends ``str`` to the socket.

.. function:: Socket.recv(len)

   Receives data from the socket. The maximum amount of data is
   specified by ``len``.
