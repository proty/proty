from builder import Builder
from parser import Parser
import ast

class Compiler(object):
    def __init__(self):
        self.builder = Builder()

        parser = Parser()
        root = parser.get_block()
        self.compile(root)
        print(self.builder.write())

    def compile(self, node):
        node.compile(self)

    def compile_block(self, node):
        for exp in node.exps:
            self.compile(exp)

    def compile_binop(self, node):
        if node.op == "+": instr = "add"
        if node.op == "-": instr = "sub"
        if node.op == "*": instr = "mul"
        if node.op == "/": instr = "div"
        a = self.compile(node.a)
        b = self.compile(node.b)
        self.builder.write_instr(instr, [a, b])

    def compile_call(self, node):
        params = [self.compile(param) for param in node.params]
        if node.name == "print":
            self.builder.write_instr("print", params)
        else:
            self.builder.write_instr("call", [str(node.name), str(len(params))] + params)

    def compile_integer(self, node):
        return self.builder.build_instr("newint", [str(node.value)])

    def compile_string(self, node):
        return self.builder.build_instr("newstring", [str(node.value)])

c = Compiler()
