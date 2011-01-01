from builder import Builder
from parser import Parser
from lexer import Lexer
import ast

class Compiler(object):
    def __init__(self, filename):
        self.builder = Builder()

        lexer = Lexer(filename)
        parser = Parser(lexer)
        root = parser.parse()


        root.compile(self)
        print(self.builder.build())

c = Compiler("test.pr")
