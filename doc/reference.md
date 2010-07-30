The Pyrite Reference Manual
===========================

Syntax
------

Program Structure
-----------------

Functions
---------

### Function definitions

without types:

    def multiply(a, b)
        return a*b

with types:

    def multiply(a : Integer, b : Integer) => Integer
        return a*b

Classes
-------

### Class definitions

    class Car
        color : String
        speed : Integer
        running : Bool

### Method definitions

    class Car < Object
        ...
        def start()
            @running = true

        def stop()
            @running = false

Variables
---------

### Variable assignments

without types:

    let a = 5
    a = 9
    a = "Hello, World"

with types:

    let a : Integer = 5
    a = 9
    a = "Hello, World" // Error - a is of type Integer!      