import ast

class Parser(object):
    def __init__(self, lexer):
        self.tokens = lexer.tokenize()
        self.pos = 0
        self.root = ast.Block()

    def peek(self, i=0):
        return self.tokens[self.pos+i]

    def next(self):
        token = self.tokens[self.pos]
        self.pos += 1
        return token

    def is_next(self, name):
        return self.peek().name == name

    def match(self, name):
        token = self.next()
        if token.name != name:
            self.unexpected(name, token.name)
        return token

    def unexpected(self, expected, found):
        raise Exception("excpected %s, found %s" % (expected, found))

    def parse(self):
        self.parse_program()
        return self.root

    def parse_program(self):
        while not self.is_next("EOF"):
            expr = self.parse_expression()
            self.root.add(expr)

    def parse_block(self):
        pass

    def parse_expression(self):
        if self.is_next("INTEGER"):
            value = self.match("INTEGER").value
            expr = ast.Integer(value)

        elif self.is_next("STRING"):
            value = self.match("STRING").value
            expr = ast.String(value)

        elif self.is_next("NAME"):
            value = self.match("NAME").value
            expr = ast.Name(value)
        else:
            expr = None
            self.unexpected("expression", self.peek().name)

        if self.is_next("DOT"):
            self.match("DOT")
            expr2 = self.parse_expression()
            return ast.Message(expr, ".", [expr2])

        elif self.is_next("LPAR"):
            args = self.parse_call_args()
            return ast.Message(expr, "()", args)

        else:
            return expr

    def parse_call_args(self):
        self.match("LPAR")
        args = []

        while True:
            if self.is_next("RPAR"):
                self.match("RPAR")
                return args
            else:
                args.append(self.parse_expression())

            if self.is_next("RPAR"):
                self.match("RPAR")
                return args
            elif self.is_next("COMMAN"):
                self.match("COMMA")
            else:
                self.unexpected("comma or rpar", self.peek())