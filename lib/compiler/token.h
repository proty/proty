#ifndef PROTY_TOKEN_H
#define PROTY_TOKEN_H

typedef enum TokenType {
  // operators
  Token_binaryop,
  Token_unaryop,
  Token_assign,

  // primitive data types
  Token_integer,
  Token_decimal,
  Token_string,
  Token_name,

  // keywords
  Token_doKw,
  Token_ifKw,
  Token_elseKw,
  Token_whileKw,
  Token_endKw,
  Token_tryKw,
  Token_catchKw,

  // braces
  Token_lpar,
  Token_rpar,
  Token_lsqb,
  Token_rsqb,
  Token_lbrace,
  Token_rbrace,

  // other symbols
  Token_comma,
  Token_dot,
  Token_colon,
  Token_semicolon,
  Token_hash,
  Token_excl,
  Token_arrow,

  Token_newline,
  Token_eof,
  Token_unknown,
} TokenType;

typedef struct Token {
  TokenType type;
  const char* value;
  int lineno;
  int precedence;
} Token;

#endif
