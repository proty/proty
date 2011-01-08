#include "parser.hh"
#include <sstream>

Parser::Parser(Lexer* lexer) {
  this->lexer = lexer;
  lexer->tokenize();
}

Node* Parser::parse() {
  return parseProgram();
}

Node* Parser::parseProgram() {
  BlockNode* prog = new BlockNode;
  while (!lexer->isNext(Token::eof)) {
    prog->add(parseExpression());
  }
  return prog;
}

Node* Parser::parseExpression() {
  Node* lhs = parseAtom();

  if (lexer->isNext(Token::binaryop)) {
    return parseOperation(lhs, 0);
  }
  else if (lexer->isNext(Token::lpar)) {
    return parseCall(lhs);
  }
  else return lhs;
}

Node* Parser::parseOperation(Node* lhs, int precedence) {
  while (lexer->isNext(Token::binaryop)
         && lexer->peek().getPrecedence() >= precedence) {
    Token op = lexer->next();
    Node* rhs = parseAtom();
    while (lexer->isNext(Token::binaryop)
           && (lexer->peek().getPrecedence() > op.getPrecedence())) {
      int nextPrecedence = lexer->peek().getPrecedence();
      rhs = parseOperation(rhs, nextPrecedence);
    }
    lhs = new BinaryOpNode(op.getValue(), lhs, rhs);
  }
  return lhs;
}

Node* Parser::parseAtom() {
  if (lexer->isNext(Token::integer)) {
    std::string value = lexer->next().getValue();
    std::stringstream ss(value);
    int v;
    ss >> v;
    return new IntegerNode(v);
  }
  else if (lexer->isNext(Token::string)) {
    std::string value = lexer->next().getValue();
    return new StringNode(value);
  }
  else if (lexer->isNext(Token::name)) {
    std::string value = lexer->next().getValue();
    return new NameNode(value);
  }
  else if (lexer->isNext(Token::lpar)) {
    lexer->next();
    Node* expr = parseExpression();
    lexer->match(Token::rpar, ")");
    return expr;
  }
  else {
    std::cerr << "unkown token " << lexer->next().getValue() << std::endl;
    exit(0);
    return 0;
  }
}

Node* Parser::parseCall(Node* callee) {
  lexer->match(Token::lpar);

  CallNode* call = new CallNode(callee);

  while (true) {
    if (lexer->isNext(Token::rpar)) {
      break;
    }
    else {
      call->addArg(parseExpression());
      if (lexer->isNext(Token::rpar)) {
        lexer->next();
        break;
      }
      else lexer->match(Token::comma, ",");
    }
  }

  return call;
}
