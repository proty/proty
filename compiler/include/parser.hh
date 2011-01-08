#ifndef PROTY_PARSER_HH
#define PROTY_PARSER_HH

#include "lexer.hh"
#include "ast.hh"

class Parser {
private:
  Lexer* lexer;

public:
  Parser(Lexer* lexer);

  Node* parse();
  Node* parseProgram();
  Node* parseExpression();
  Node* parseOperation(Node*, int);
  Node* parseAtom();
  Node* parseCall(Node*);
};

#endif
