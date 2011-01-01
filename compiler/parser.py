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
        lhs = self.parse_primary()
        
        if self.is_next("OPERATOR"):
            return self.parse_expression_pre(lhs)
        else:
            return lhs

    def parse_expression_pre(self, lhs, precedence=0):
        while self.is_next("OPERATOR") and self.peek().precedence >= precedence:
            op = self.next()
            rhs = self.parse_primary()
            while self.is_next("OPERATOR") and self.peek().precedence > precedence:
                next_precedence = self.peek().precedence
                rhs = self.parse_expression_pre(rhs, next_precedence)
            lhs = ast.Operation(op.value, lhs, rhs)
        return lhs

    def parse_primary(self):
        if self.is_next("INTEGER"):
            value = self.match("INTEGER").value
            primary = ast.Integer(value)

        elif self.is_next("STRING"):
            value = self.match("STRING").value
            primary = ast.String(value)

        elif self.is_next("NAME"):
            value = self.match("NAME").value
            primary = ast.Name(value)

        else:
            primary = None
            self.unexpected("primary", self.peek().name)

        if self.is_next("LPAR"):
            args = self.parse_call_args()
            primary = ast.Message(primary, "()", args)

        return primary

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