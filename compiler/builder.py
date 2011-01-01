class Builder(object):
    def __init__(self):
        self.instructions = []

    def write_instr(self, name, params):
        self.instructions.append(name + "(" + ", ".join(params) + ");")

    def build_instr(self, name, params):
        return name + "(" + ", ".join(params) + ")"

    def write(self, instr):
        self.instructions.append(instr)

    def build(self):
        print self.instructions
        output = "#include <proty/vm/instructions.hh>\n\n"
        output += "using namespace proty;\n\n"
        output += "int main() {\n"
        output += "\n".join(self.instructions) + ";"
        output += "\n}"
        return output