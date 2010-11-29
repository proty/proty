class Builder(object):
    def __init__(self):
        self.instructions = []

    def instr(self, name, params):
        self.instructions.append(name + "(" + ", ".join(params) + ");")

    def write(self):
        output = "#include <pyrite/vm/instructions.hh>\n\n"
        output += "using namespace pyrite;\n\n"
        output += "int main() {\n  "
        output += "\n".join(self.instructions)
        output += "\n}"
        return output