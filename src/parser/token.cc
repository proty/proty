#include "parser/token.hh"

namespace pyrite {

  Token::Token(Type t, Location* l, std::string c) {
    type = t;
    loc = l;
    content = c;
  }

  Token::Type Token::getType() {
    return type;
  }

  std::string Token::getContent() {
    return content;
  }

  Location* Token::getLocation() {
    return loc;
  }

}