class Builder
  def initialize
    @instructions = []
  end

  def instr(name, params)
    @instructions << name.to_s + "(" + params.join(", ") + ");"
  end

  def write
    @output = "#include <pyrite/vm/instructions.hh>\n\n"
    @output += "using namespace pyrite;\n\n"
    @output += "int main() {\n  "
    @output += @instructions.join("\n  ");
    @output += "\n}"
    return @output
  end
end
