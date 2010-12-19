import re

class Token(object):
    def __init__(self, name, value):
        self.name = name
        self.value = value

class Lexer(object):
    def __init__(self, filename):
        f = open(filename)
        self.data = f.read()
        self.pos = 0

    def peek(self, i=0):
        return self.data[self.pos+i]

    def next(self):
        char = self.data[self.pos]
        self.pos += 1
        return char

    def tokenize(self):
        tokens = []
        self.pos = 0
        while True:
            # check for eof
            if not self.pos < len(self.data):
                tokens.append(Token("EOF", "<eof>"))
                return tokens

            # number
            elif self.peek().isdigit():
                num = self.next()
                while self.peek().isdigit():
                    num += self.next()
                tokens.append(Token("INTEGER", num))

            # string
            elif self.peek() == '"':
                self.next() # skip quote
                string = self.next()
                while self.peek() != '"':
                    string += self.next()
                self.next() # skip quote
                tokens.append(Token("STRING", string))

            # name
            elif re.match("^[A-Za-z0-9_]$", self.peek()):
                name = self.next()
                while re.match("^[A-Za-z0-9_]$", self.peek()):
                    name += self.next()
                tokens.append(Token("NAME", name))

            # whitespace
            elif self.peek() in [' ', '\t', '\n']:
                self.next()

            # comment
            elif self.peek() == '#':
                while self.peek() != '\n' and self.pos < len(self.data):
                    self.next() 

            #  lpar
            elif self.peek() == '(':
                tokens.append(Token("LPAR", self.next()))

            # rpar
            elif self.peek() == ')':
                tokens.append(Token("RPAR", self.next()))

            # dot
            elif self.peek() == '.':
                tokens.append(Token("DOT", self.next()))

            # comma
            elif self.peek() == ',':
                tokens.append(Token("COMMA", self.next()))

            # assign/equal
            elif self.peek() == '=':
                self.next()
                if self.peek() == '=':
                    self.next()
                    tokens.append(Token("BINOP", "=="))
                else:
                    tokens.append(Token("ASSIGN", "="))  

            # simply binary operators
            elif self.peek() in ['+', '-', '*', '/', '%']:
                tokens.append(Token("BINOP", self.next()))

            # greater/greater equal
            elif self.peek() == '>':
                self.next()
                if self.peek() == '=':
                    self.next()
                    tokens.append(Token("BINOP", ">="))
                else:
                    tokens.append(Token("BINOP", ">"))

            # less/less equal
            elif self.peek() == '<':
                self.next()
                if self.peek() == '=':
                    self.next()
                    tokens.append(Token("BINOP", "<="))
                else:
                    tokens.append(Token("BINOP", "<"))

            else:
                tokens.append(Token("UNKNOWN", self.next()))
