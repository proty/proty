#ifndef PROTY_TOKEN_HH
#define PROTY_TOKEN_HH

class Token {
public:
  typedef enum {
    binaryop,
    unaryop,
    integer,
    decimal,
    lpar,
    rpar,
    lsqb,
    rsqb,
    lbrace,
    rbrace,
    unknown,
  } TokenType;

private:
  TokenType type;
  std::string value;
  int precedence;

public:
  Token(TokenType type, std::string value, int precedence=0)
    : type(type), value(value), precedence(precedence) {}

  TokenType getType() { return type; }
  std::string getValue() { return value; }
  int getPrecedence() { return precedence; }
};

#endif