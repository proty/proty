#ifndef PROTY_PARSER_HH
#define PROTY_PARSER_HH

#include <iostream>

class Lexer;
class Node;

class Parser {
private:
  Lexer* lexer;

public:
  Parser() {}
  Node* parseFile(std::string);
  Node* parseStr(char*);

private:
  Node* parse();
  Node* parseBlock();
  Node* parseExpression();
  Node* parseOperation(Node*, int);
  Node* parsePrimary();
  Node* parseList();
  Node* parseHash();
  Node* parseCall(Node*);
  Node* parseFunction();
  Node* parseIf();
  Node* parseWhile();
  Node* parseTry();
  Node* parseProperty();
};

#endif
