#include <sstream>
#include <fstream>
#include <cstdlib>
#include "parser.hh"
#include "lexer.hh"
#include "ast.hh"

Node* Parser::parseFile(std::string file) {
  std::istream* stream;
  if (file == "<stdin>") stream = &std::cin;
  else stream = new std::ifstream(file.c_str(), std::ifstream::in);
  if (!*stream) {
    std::cerr << "cannot open file " + file << std::endl;
    exit (1);
  }

  lexer = new Lexer(stream);
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
  lexer = new Lexer(stream);
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
  else if (lexer->isNext(Token::assign)) {
    lexer->next();
    NameNode* name = dynamic_cast<NameNode*>(lhs);
    if (name) {
      Node* expr = parseExpression();
      Node* assign = new AssignNode(name->getValue(), expr);
      return assign;
    }
    else {
      std::cerr << "only names can be used for assignments" << std::endl;
      exit(1);
      return 0;
    }
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
  else if (lexer->isNext(Token::decimal)) {
    std::string value = lexer->next().getValue();
    std::stringstream ss(value);
    double v;
    ss >> v;
    prim = new FloatNode(v);
  }
  else if (lexer->isNext(Token::string)) {
    std::string value = lexer->next().getValue();
    prim = new StringNode(value);
  }
  else if (lexer->isNext(Token::name)) {
    std::string value = lexer->next().getValue();
    prim = new NameNode(value);
  }
  else if (lexer->isNext(Token::lsqb)) {
    prim = parseList();
  }
  else if (lexer->isNext(Token::doKw)) {
    prim = parseFunction();
  }
  else {
    std::cerr << "unexpected token " << lexer->next().getValue() << std::endl;
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

      if (lexer->isNext(Token::lpar)) {
        lexer->next();
        prim = new CallSlotNode(prim, n);
        while (true) {
          if (lexer->isNext(Token::rpar)) {
            lexer->next();
            break;
          }
          else {
            ((CallSlotNode*)prim)->addArg(parseExpression());
            if (lexer->isNext(Token::rpar)) {
              lexer->next();
              break;
            }
            else lexer->match(Token::comma, ",");
          }
        }
      }
      else if (lexer->isNext(Token::assign)) {
        lexer->next();
        Node* expr = parseExpression();
        prim = new SetSlotNode(prim, n, expr);
      }
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
