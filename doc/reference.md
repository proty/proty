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
    end

with types:

    def multiply(a : Integer, b : Integer) => Integer
        return a*b
    end

Classes
-------

### Class definitions

    class Car
        color : String
        speed : Integer
        running : Bool
    end

### Method definitions

    class Car < Object
        ...
        def start()
            @running = true
        end

        def stop()
            @running = false
        end
    end

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
