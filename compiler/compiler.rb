require 'builder'

class Compiler
  def initialize
    @builder = Builder.new
  end

  def compile
    @builder.instr(:add, ["1", "2"])
    puts @builder.write()
  end
end

c = Compiler.new
c.compile
