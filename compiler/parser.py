import ast

class Parser(object):
    def __init__(self):
        self.block = ast.Block()
        self.test_fill()

    def test_fill(self):
        self.block.add(ast.Call("print", [ast.BinOp("+", ast.Integer(5), ast.Integer(10))]))
        self.block.add(ast.String("Hello World"))

    def get_block(self):
        return self.block
