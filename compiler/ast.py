class Node(object):
    pass

class Block(Node):
    def __init__(self):
        self.exps = []

    def add(self, exp):
        self.exps.append(exp)

    def compile(self, c):
        for exp in self.exps:
            exp.compile(c)

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

class Message(Node):
    def __init__(self, receiver, name, params):
        self.receiver = receiver
        self.name = name
        self.params = params

    def compile(self, c):
        params = [param.compile(c) for param in self.params]
        receiver = self.receiver.compile(c)
        print receiver, self.name, params
        c.builder.write_instr("send", [receiver, str(self.name), str(len(params))] + params)

class BinOp(Node):
    def __init__(self, op, a, b):
        self.op = op
        self.a, self.b = a, b

    def compile(self, c):
        if self.op == "+": instr = "add"
        if self.op == "-": instr = "sub"
        if self.op == "*": instr = "mul"
        if self.op == "/": instr = "div"
        a = a.compile(c)
        b = b.compile(c)
        c.builder.write_instr(instr, [a, b])

class Name(Node):
    def __init__(self, name):
        self.name = name

    def compile(self, c):
        return c.builder.build_instr("load", ['"%s"' % self.name])

class Integer(Node):
    def __init__(self, value):
        self.value = value

    def compile(self, c):
        return c.builder.build_instr("newint", [self.value])

class String(Node):
    def __init__(self, value):
        self.value = value

    def compile(self, c):
        return c.builder.build_instr("newstring", [self.value])
