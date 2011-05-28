#include "parser.hh"
#include <sstream>

Parser::Parser(Lexer* lexer) {
  this->lexer = lexer;
  lexer->tokenize();
}

Node* Parser::parse() {
  return parseBlock();
}

Node* Parser::parseBlock() {
  BlockNode* block = new BlockNode;
  while (true) {
    switch (lexer->peek().getType()) {
      case Token::endKw:
        lexer->next();
        return block;

      case Token::eof:
        return block;

      case Token::ifKw:
        lexer->next();
        block->add(parseIf());
        break;

      case Token::elseKw:
        return block;

      default:
        block->add(parseExpression());
    }
  }
}

Node* Parser::parseExpression() {
  Node* lhs = parsePrimary();

  if (lexer->isNext(Token::binaryop)) {
    return parseOperation(lhs, 0);
  }
  else if (lexer->isNext(Token::lpar)) {
    lexer->next();
    Node* expr = parseExpression();
    lexer->match(Token::rpar, ")");
    return expr;
  }
  else return lhs;
}

Node* Parser::parseOperation(Node* lhs, int precedence) {
  while (lexer->isNext(Token::binaryop)
         && lexer->peek().getPrecedence() >= precedence) {
    Token op = lexer->next();
    Node* rhs = parsePrimary();

    while (lexer->isNext(Token::binaryop)
           && (lexer->peek().getPrecedence() > op.getPrecedence())) {
      int nextPrecedence = lexer->peek().getPrecedence();
      rhs = parseOperation(rhs, nextPrecedence);
    }
    lhs = new BinaryOpNode(op.getValue(), lhs, rhs);
  }
  return lhs;
}

Node* Parser::parsePrimary() {
  Node* prim;
  if (lexer->isNext(Token::integer)) {
    std::string value = lexer->next().getValue();
    std::stringstream ss(value);
    int v;
    ss >> v;
    prim = new IntegerNode(v);
  }
  else if (lexer->isNext(Token::boolean)) {
    bool value = lexer->next().getValue() == "true";
    prim = new BoolNode(value);
  }
  else if (lexer->isNext(Token::nil)) {
    prim = new NilNode();
  }
  else if (lexer->isNext(Token::string)) {
    std::string value = lexer->next().getValue();
    prim = new StringNode(value);
  }
  else if (lexer->isNext(Token::name)) {
    std::string value = lexer->next().getValue();
    prim = new NameNode(value);
  }
  else if (lexer->isNext(Token::doKw)) {
    lexer->next();
    FunctionNode* f = new FunctionNode();

    f->setBlock(parseBlock());
    prim = f;
  }
  else {
    std::cerr << "unkown token " << lexer->next().getValue() << std::endl;
    exit(1);
    return 0;
  }

  while (lexer->isNext(Token::lpar) || lexer->isNext(Token::dot)) {
    if (lexer->isNext(Token::lpar)) {
      prim = parseCall(prim);
    }
    else if (lexer->isNext(Token::dot)) {
      lexer->next();
      std::string n = lexer->match(Token::name, "slot name").getValue();
      prim = new SlotNode(prim, n);
    }
  }
  return prim;
}

Node* Parser::parseCall(Node* callee) {
  lexer->match(Token::lpar);

  CallNode* call = new CallNode(callee);

  while (true) {
    if (lexer->isNext(Token::rpar)) {
      lexer->next();
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

Node* Parser::parseIf() {
  Node* cond = parseExpression();
  Node* thenNode = parseBlock();

  Node* elseNode = 0;
  if (lexer->isNext(Token::elseKw)) {
    lexer->next();
    elseNode = parseBlock();
  }

  return new IfNode(cond, thenNode, elseNode);
}
