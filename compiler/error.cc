#include "error.hh"

void GeneralError::printMessage() {
  std::cerr << message << std::endl;
}

void ParseError::printMessage() {
  std::cerr << "parse error in " << loc.filename << " at line " << loc.line << ":" << std::endl;
  std::cerr << "  " <<  message << std::endl;
}

void CompileError::printMessage() {
  std::cerr << "compile error:" << std::endl;
  std::cerr << "  " <<  message << std::endl;
}
