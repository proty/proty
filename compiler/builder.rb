class Builder
  def initialize
    @output = ""
  end

  def instr(name, params)
    @output += name.to_s + " " + params.join(", ") + "\n"
  end

  def write
    return @output
  end
end
