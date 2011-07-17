#ifndef PROTY_PARSER_HH
#define PROTY_PARSER_HH

#include <iostream>

class Lexer;
class Node;

class Parser {
private:
  Lexer* lexer;

public:
  Node* parseFile(std::string);
  Node* parseStr(char*);

private:
  Node* parse();
  Node* parseBlock();
  Node* parseExpression();
  Node* parseOperation(Node*, int);
  Node* parsePrimary();
  Node* parseList();
  Node* parseCall(Node*);
  Node* parseFunction();
  Node* parseIf();
  Node* parseWhile();
  Node* parseTry();
};

#endif
