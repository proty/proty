# The Pyrite Reference Manual #

## Syntax ##

## Program structure ##

## Modules ##

Usually every file is a module, but without
a specific namespace.A namespace for a file 
can be defined by putting *module foo* anywhere
in the file, but it's recommendable to put it
near the top.

    // mymodule.py
    module foo
    
    def bar()
        return "Hello"
    end

All classes and functions of this file are
now in the namespace *foo*.

To import a module use the *import* instruction:

    // myprogram.py
    import foo
    
    print foo.bar()

To use an object from inside a module use the dot-syntax
(object.subobject), in the example *foo.bar*.

## Functions ##

### Function definitions ###

without types:

    def multiply(a, b)
        return a*b
    end

with types:

    def multiply(a : Integer, b : Integer) => Integer
        return a*b
    end

## Classes ##

### Class definitions ###

    class Car
        color : String
        speed : Integer
        running : Bool
    end

### Method definitions ###

    class Car < Object
        ...
        def start()
            @running = true
        end

        def stop()
            @running = false
        end
    end

## Variables ##

### Variable assignments ###

without types:

    let a = 5
    a = 9
    a = "Hello, World"

with types:

    let a : Integer = 5
    a = 9
    a = "Hello, World" // Error - a is of type Integer!
