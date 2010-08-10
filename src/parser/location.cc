#include "parser/location.hh"

namespace pyrite {

  Location::Location(std::string source, int lineno) {
    this->source = source;
    this->lineno = lineno;
  }

  int Location::getLineNumber() {
    return lineno;
  }

  void Location::incLineNumber() {
    lineno++;
  }

  std::string Location::getSource() {
    return source;
  }

  Location* Location::copy() {
    return new Location(source, lineno);
  }

}