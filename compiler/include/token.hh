#ifndef PROTY_TOKEN_HH
#define PROTY_TOKEN_HH

class Token {
public:
  typedef enum {
    binaryop,
    unaryop,
    integer,
    decimal,
    string,
    name,
    lpar,
    rpar,
    lsqb,
    rsqb,
    lbrace,
    rbrace,
    comma,
    eof,
    unknown,
  } Type;

private:
  Type type;
  std::string value;
  int precedence;

public:
  Token(Type type, std::string value, int precedence=0)
    : type(type), value(value), precedence(precedence) {}

  Type getType() { return type; }
  std::string getValue() { return value; }
  int getPrecedence() { return precedence; }
};

#endif
