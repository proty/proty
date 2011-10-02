#include <sstream>
#include <fstream>
#include <cstdlib>
#include "parser.hh"
#include "lexer.hh"
#include "ast.hh"
#include "error.hh"

Node* Parser::parseFile(std::string file) {
  std::istream* stream;
  if (file == "<stdin>") stream = &std::cin;
  else stream = new std::ifstream(file.c_str(), std::ifstream::in);
  if (!*stream) {
    throw new GeneralError("cannot open file " + file);
  }

  lexer = new Lexer(stream, file);
  lexer->tokenize();

  Node* root = parse();

  delete lexer;
  if (stream != &std::cin) {
    delete stream;
  }

  return root;
}

Node* Parser::parseStr(char* str) {
  std::istream* stream = new std::istringstream(str);
  lexer = new Lexer(stream, "<string>");
  lexer->tokenize();

  Node* root = parse();

  delete lexer;

  return root;
}

Node* Parser::parse() {
  return parseBlock();
}

Node* Parser::parseBlock() {
  BlockNode* block = new BlockNode;

  bool braced;
  if (lexer->isNext(Token::lbrace)) {
    braced = true;
    lexer->next();
  }
  else {
    braced = false;
  }

  while (true) {
    switch (lexer->peek().getType()) {
      case Token::endKw:
        if (braced) {
          lexer->unexpected(lexer->next(), "}");
        }
        lexer->next();
        return block;

      case Token::rbrace:
        if (!braced) {
          lexer->unexpected(lexer->next(), "end");
        }
        lexer->next();
        return block;

      case Token::eof:
        return block;

      case Token::ifKw:
        lexer->next();
        block->add(parseIf());
        break;

      case Token::whileKw:
        lexer->next();
        block->add(parseWhile());
        break;

      case Token::tryKw:
        lexer->next();
        block->add(parseTry());
        break;

      case Token::catchKw:
        if (braced) {
          lexer->unexpected(lexer->next(), "}");
        }
        return block;

      case Token::elseKw:
        if (braced) {
          lexer->unexpected(lexer->next(), "}");
        }
        return block;

      case Token::hash:
        block->add(parseProperty());
        break;

      case Token::newline:
      case Token::semicolon:
        lexer->next();
        break;

      default:
        block->add(parseExpression());

        // verify that the expression ends with a newline/closing token
        if (!(lexer->isNext(Token::newline)
              || lexer->isNext(Token::semicolon)
              || lexer->isNext(Token::eof)
              || lexer->isNext(Token::rbrace))) {

          lexer->unexpected(lexer->next(), "new line");
        }
    }
  }
}

Node* Parser::parseExpression() {
  Node* expr;

  // expression in parentheses
  if (lexer->isNext(Token::lpar)) {
    lexer->next();
    expr = parseExpression();
    lexer->match(Token::rpar, ")");
  }
  else if (lexer->isNext(Token::excl)) {
    lexer->next();
    expr = parseExpression();
    expr = new UnaryOpNode("!", expr);
  }
  else {
    expr = parsePrimary();
  }


  // inplace operation
  if (lexer->isNext(Token::binaryop) && lexer->peek(1).getType() == Token::assign) {
    std::string op = lexer->next().getValue() + "=";
    lexer->next();
    Node* assignExpr = parseExpression();
    expr = new BinaryOpNode(op, expr, assignExpr);
  }

  // binary operation
  else if (lexer->isNext(Token::binaryop)) {
    expr = parseOperation(expr, 0);
  }

  // variable assignment
  else if (lexer->isNext(Token::assign)) {
    Token t = lexer->next();

    // verify that expr is a string
    if (!expr->getValue().empty()) {
      Node* assignExpr = parseExpression();
      expr = new AssignNode(expr->getValue(), assignExpr);
    }
    else {
      throw new ParseError(lexer->getFilename(), t.getLine(),
                           "only names can be used for assignments");
    }
  }

  return expr;
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

  // integer
  if (lexer->isNext(Token::integer)) {
    std::string value = lexer->next().getValue();
    std::stringstream ss(value);
    int v;
    ss >> v;
    prim = new IntegerNode(v);
  }

  // decimal
  else if (lexer->isNext(Token::decimal)) {
    std::string value = lexer->next().getValue();
    std::stringstream ss(value);
    double v;
    ss >> v;
    prim = new FloatNode(v);
  }

  // string
  else if (lexer->isNext(Token::string)) {
    std::string value = lexer->next().getValue();
    prim = new StringNode(value);
  }

  // variable
  else if (lexer->isNext(Token::name)) {
    std::string value = lexer->next().getValue();
    if (lexer->isNext(Token::colon)) {
      lexer->next();
      std::string name = lexer->match(Token::name, "module member name").getValue();
      prim = new ModuleMemberNode(value, name);
    }
    else prim = new NameNode(value);
  }

  // symbol
  else if (lexer->isNext(Token::colon)) {
    lexer->next();
    std::string name = lexer->match(Token::name, "symbol name").getValue();
    prim = new SymbolNode(name);
  }

  // list
  else if (lexer->isNext(Token::lsqb)) {
    prim = parseList();
  }

  // hash
  else if (lexer->isNext(Token::lbrace)) {
    prim = parseHash();
  }

  // function
  else if (lexer->isNext(Token::doKw)) {
    prim = parseFunction();
  }

  // unexpected
  else {
    throw new ParseError(lexer->getFilename(), lexer->peek().getLine(),
                         "unexpected token '" + lexer->peek().getValue() + "'");
  }

  while (lexer->isNext(Token::lpar) || lexer->isNext(Token::lsqb)
         || lexer->isNext(Token::dot)) {

    // simple function call
    if (lexer->isNext(Token::lpar)) {
      prim = parseCall(prim);
    }

    // subscript operator
    else if (lexer->isNext(Token::lsqb)) {
      lexer->next();
      Node* key = parseExpression();
      lexer->match(Token::rsqb, "]");

      Node* value = 0;
      if (lexer->isNext(Token::assign)) {
        lexer->next();
        value = parseExpression();
      }

      return new SubscriptNode(prim, key, value);
    }

    // slot access
    else if (lexer->isNext(Token::dot)) {
      lexer->next();
      std::string n = lexer->match(Token::name, "slot name").getValue();

      // is this a message?
      if (lexer->isNext(Token::lpar)) {
        lexer->next();
        prim = new SendNode(prim, n);

        // parse arguments
        while (true) {
          if (lexer->isNext(Token::rpar)) {
            lexer->next();
            break;
          }
          else {
            ((SendNode*)prim)->addArg(parseExpression());
            if (lexer->isNext(Token::rpar)) {
              lexer->next();
              break;
            }
            else lexer->match(Token::comma, ",");
          }
        }
      }

      // is there something to be assigned to the slot?
      else if (lexer->isNext(Token::assign)) {
        lexer->next();
        Node* expr = parseExpression();
        prim = new SetSlotNode(prim, n, expr);
      }

      // just a simple slot access
      else {
        prim = new GetSlotNode(prim, n);
      }
    }
  }

  return prim;
}

Node* Parser::parseList() {
  lexer->match(Token::lsqb, "[");

  ListNode* ln = new ListNode();

  while (true) {
    if (lexer->isNext(Token::rsqb)) {
      lexer->next();
      break;
    }
    else {
      ln->add(parseExpression());
      if (!lexer->isNext(Token::rsqb)) {
        lexer->match(Token::comma, ",");
      }
    }
  }
  return ln;
}

Node* Parser::parseHash() {
  lexer->match(Token::lbrace, "{");

  HashNode* hn = new HashNode();

  while (true) {
    if (lexer->isNext(Token::rbrace)) {
      lexer->next();
      break;
    }
    else {
      Node* key = parseExpression();
      lexer->match(Token::arrow, "=>");
      Node* value = parseExpression();
      hn->add(key, value);
      if (!lexer->isNext(Token::rbrace)) {
        lexer->match(Token::comma, ",");
      }
    }
  }

  return hn;
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
      if (!lexer->isNext(Token::rpar)) {
        lexer->match(Token::comma, ",");
      }
    }
  }

  return call;
}

Node* Parser::parseFunction() {
  lexer->match(Token::doKw, "do");

  FunctionNode* f = new FunctionNode();
  f->addArg("self");

  if (lexer->isNext(Token::lpar)) {
    lexer->next();
    while (true) {
      if (lexer->isNext(Token::rpar)) {
        lexer->next();
        break;
      }
      else {
        std::string arg = lexer->match(Token::name, "argument name").getValue();
        f->addArg(arg);
        if (!lexer->isNext(Token::rpar)) {
          lexer->match(Token::comma, ",");
        }
      }
    }
  }

  f->setBlock(parseBlock());

  return f;
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

Node* Parser::parseWhile() {
  Node* cond = parseExpression();
  Node* block = parseBlock();
  return new WhileNode(cond, block);
}

Node* Parser::parseTry() {
  Node* tryBlock = parseBlock();
  lexer->match(Token::catchKw, "catch");

  std::string ename = lexer->match(Token::name, "exception name").getValue();

  Node* catchBlock = parseBlock();

  Node* elseBlock = 0;
  if (lexer->isNext(Token::elseKw)) {
    elseBlock = parseBlock();
  }

  return new TryNode(tryBlock, ename, catchBlock, elseBlock);
}

Node* Parser::parseProperty() {
  lexer->match(Token::hash, "#");
  std::string name = lexer->match(Token::name, "property name").getValue();

  PropertyNode* prop = new PropertyNode(name);

  lexer->match(Token::lpar, "(");
  while (true) {
    std::string value = lexer->match(Token::name, "property value").getValue();
    prop->addValue(value);

    if (lexer->isNext(Token::comma)) {
      lexer->next();
    }
    else {
      lexer->match(Token::rpar, ")");
      break;
    }
  }

  return prop;
}
