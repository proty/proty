#include "parser/parseerror.hh"
#include <iostream>

namespace pyrite {
  
  void ParseError::alert() {
    std::cerr << std::endl;
    std::cerr << "Parse Error: " << std::endl;
    std::cerr << "  in " << loc->getSource() << ", line " << loc->getLineNumber() << std::endl;
    std::cerr << "    " << message << std::endl << std::endl;
  }
  
}