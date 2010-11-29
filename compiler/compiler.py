import builder

class Compiler(object):
    def __init__(self):
        self.builder = builder.Builder()

    def compile(self, ast):
        self.builder.instr("add", ["new Integer(1)", "new Integer(2)"])
        self.builder.instr("print", ["new Integer(1)"])
        print(self.builder.write())

ast = [
  "+", [5, 5],
  "call", ["print", 5],
]

c = Compiler()
c.compile(ast)