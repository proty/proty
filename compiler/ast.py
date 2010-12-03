class Node(object):
    pass

class Block(Node):
    def __init__(self):
        self.exps = []

    def add(self, exp):
        self.exps.append(exp)

    def compile(self, c):
        pass

class Module(Node):
    def __init__(self, name):
        self.name = name

    def compile(self, c):
        pass

class Class(Node):
    def __init__(self, name):
        self.name = name

    def compile(self, c):
        pass

class Function(Node):
    def __init__(self, name):
        self.name = name
        self.params = []

    def add_param(self, name):
        self.params.append(name)

    def set_block(self, block):
        self.block = block

    def compile(self, c):
        pass

class Call(Node):
    def __init__(self, name, params):
        self.name = name
        self.params = params

    def compile(self, c):
        pass

class BinOp(Node):
    def __init__(self, op, a, b):
        self.op = op
        self.a, self.b = a, b

    def compile(self, c):
        pass

class Integer(Node):
    def __init__(self, value):
        self.value = value

    def compile(self, c):
        pass

class String(Node):
    def __init__(self, value):
        self.value = value

    def compile(self, c):
        pass
