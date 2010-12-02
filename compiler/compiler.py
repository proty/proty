import builder

class Compiler(object):
    def __init__(self):
        self.builder = builder.Builder()

    def compile(self, ast):
        for exp in ast:
            self.compile_exp(exp)

        print(self.builder.write())

    def compile_exp(self, exp):
        if type(exp) == list:
            if   exp[0] == "+": self.compile_binop(exp) 
            elif exp[0] == "call": self.compile_call(exp[1])
        else:
            if type(exp)   == int: return self.compile_int(exp)
            elif type(exp) == str: return self.compile_string(exp)

    def compile_binop(self, exp):
        op, params = exp
        if op == "+": instr = "add"
        if op == "-": instr = "sub"
        if op == "*": instr = "mul"
        if op == "/": instr = "div"
        params = [self.compile_exp(param) for param in params]
        self.builder.write_instr(instr, params)

    def compile_call(self, exp):
        name, params = exp
        params = [self.compile_exp(param) for param in params]
        if name == "print":
            self.builder.write_instr("print", params)
        else:
            self.builder.write_instr("call", [str(name), str(len(params))] + params)

    def compile_int(self, val):
        return self.builder.build_instr("newint", [str(val)])

    def compile_string(self, val):
        return self.builder.build_instr("newstring", [str(val)])


ast = [
  ["+", [5, 5]],
  ["call", ["print", [5]]],
]

c = Compiler()
c.compile(ast)