#ifndef PROTY_TOKEN_HH
#define PROTY_TOKEN_HH

class Token {
public:
  typedef enum {
    // operators
    binaryop,
    unaryop,

    // primitive data types
    integer,
    decimal,
    string,
    name,

    // keywords
    doKw,
    ifKw,
    elseKw,
    whileKw,
    endKw,
    tryKw,
    catchKw,

    // braces
    lpar,
    rpar,
    lsqb,
    rsqb,
    lbrace,
    rbrace,

    // other symbols
    comma,
    dot,

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
