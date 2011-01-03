#include "parser.hh"
#include "lexer.hh"
#include "ast.hh"
#include <sstream>

Parser::Parser(Lexer* lexer) {
  this->lexer = lexer;
  lexer->tokenize();
}

Node* Parser::parse() {
  return parseProgram();
}

Node* Parser::parseProgram() {
  while (!lexer->isNext(Token::eof)) {
    parseExpression();
  }
  return 0;
}

Node* Parser::parseExpression() {
  Node* lhs = parseAtom();

  if (lexer->isNext(Token::binaryop)) {
    return parseExpressionPre(lhs);
  }
  else return lhs;
}

Node* Parser::parseExpressionPre(Node* lhs, int precedence) {
  while (lexer->isNext(Token::binaryop)) {
    Token op = lexer->next();
    Node* rhs = parseAtom();
    while (lexer->isNext(Token::binaryop)
           && lexer->peek().getPrecedence() > precedence) {
      int nextPrecedence = lexer->peek().getPrecedence();
      rhs = parseExpressionPre(rhs, nextPrecedence);
    }
    lhs = new BinaryOpNode(op.getValue(), lhs, rhs);
  }
  return lhs;
}

Node* Parser::parseAtom() {
  if (lexer->isNext(Token::integer)) {
    std::string value = lexer->match(Token::integer).getValue();
    std::stringstream ss(value);
    int v;
    ss >> v;
    return new IntegerNode(v);
  }
  else if (lexer->isNext(Token::string)) {
    std::string value = lexer->match(Token::string).getValue();
    return new StringNode(value);
  }
  else if (lexer->isNext(Token::name)) {
    std::string value = lexer->match(Token::name).getValue();
    return new NameNode(value);
  }
  return 0;
}
