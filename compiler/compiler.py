import builder

class Compiler(object):
  def __init__(self):
    self.builder = builder.Builder()

  def compile(self):
    self.builder.instr("add", ["new Integer(1)", "new Integer(2)"])
    self.builder.instr("print", ["new Integer(1)"])
    print(self.builder.write())

c = Compiler()
c.compile()
