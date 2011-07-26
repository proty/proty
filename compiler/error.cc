#include "error.hh"
#include "lexer.hh"

void ParseError::printMessage() {
  std::cerr << "parse error in " << loc.filename << " at line " << loc.line << ":" << std::endl;
  std::cerr << "  " <<  message << std::endl; 
}
