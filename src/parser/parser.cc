#include "parser/parser.hh"
#include "parser/parseerror.hh"
#include <cstdlib>

namespace pyrite {

  Parser::Parser(Tokenizer* t) {
    tokenizer = t;
  }

  void Parser::unexpected(Token t, std::string expected) {
    std::string error = "Unexpected Token: " + t.getContent();
    if (!expected.empty()) error += ", Expected: " + expected;
    throw new ParseError(t.getLocation(), error);
  }

  Token Parser::match(std::string name, Token::Type type) {
    Token t = tokenizer->peek();
    if (t.getType() != type) {
      unexpected(t, name);
    }
    return tokenizer->next();
  }

  bool Parser::isNext(Token::Type type) {
    return tokenizer->peek().getType() == type;
  }

  ProgramModel* Parser::parse() {
    tokenizer->tokenize();
    try {
      ProgramModel* p = parseProgram();
      return p;
    }
    catch (ParseError* e) {
      e->alert();
      std::exit(1);
    }
  }

  ProgramModel* Parser::parseProgram() {
    ProgramModel* p = new ProgramModel();
    while (true) {
      Token t = tokenizer->peek();
      switch (t.getType()) {
        case Token::moduleKw:
          tokenizer->skip();
          //b->addExpr(parseModule());
          break;

        case Token::classKw:
          tokenizer->skip();
          p->addExpr(parseClass());
          break;

        case Token::defKw:
          tokenizer->skip();
          p->addExpr(parseFunction());
          break;

        case Token::whileKw:
          tokenizer->skip();
          p->addExpr(parseWhile());
          break;

        case Token::ifKw:
          tokenizer->skip();
          p->addExpr(parseIf());
          break;

        case Token::importKw: {
          tokenizer->skip();
          std::string name = match("name of package to import", Token::name).getContent();
          /* p->addImport(name); */
          break;
        }

        case Token::eof:
          return p;

        default:
          p->addExpr(parseExpression());
      }
    }
  }

  BlockModel* Parser::parseBlock() {
    BlockModel* b = new BlockModel();

    match("indent", Token::indent);

    while (true) {
      Token t = tokenizer->peek();

      switch (t.getType()) {
        case Token::dedent:
          tokenizer->skip();
          // fall through
        
        case Token::eof:
          return b;

        case Token::elseKw:
          return b;

        case Token::whileKw:
          tokenizer->skip();
          b->addExpr(parseWhile());
          break;

        case Token::ifKw:
          tokenizer->skip();
          b->addExpr(parseIf());
          break;

        case Token::returnKw: {
          t = tokenizer->next();
          ExprModel* e = parseExpression();
          b->addExpr(new ReturnModel(e));
          break;
        }

        default:
          b->addExpr(parseExpression());
      }
    }

    return 0;
  }

  ModuleModel* Parser::parseModule() {
    Token t = match("module name", Token::name);

    ModuleModel* mm = new ModuleModel(t.getContent());

    while (true) {
      Token t = tokenizer->peek();

      switch (t.getType()) {
        case Token::dedent:
          tokenizer->skip();
          // fall through

        case Token::eof:
          return mm;

        case Token::defKw:
          tokenizer->skip();
          mm->addFunction(parseFunction());
          break;

        case Token::classKw:
          tokenizer->skip();
          mm->addClass(parseClass());
          break;

        default:
          unexpected(t, "statement");
      }
    }

    return 0;
  }

  ClassModel* Parser::parseClass() {
    Token t = match("class name", Token::name);

    ClassModel* cm = new ClassModel(t.getContent());

    t = tokenizer->peek();
    if (t.getType() == Token::binOp && t.getContent() == "<") {
      tokenizer->skip();
      std::string base = match("base class", Token::name).getContent();
      cm->setBase(base);
    }

    match("indent", Token::indent);

    while (true) {
      t = tokenizer->peek();

      switch (t.getType()) {
        case Token::dedent:
          tokenizer->skip();
          // fall through

        case Token::eof:
          return cm;

        case Token::defKw:
          tokenizer->skip();
          cm->addMethod(parseFunction());
          break;

        case Token::name: {
          tokenizer->skip();
          std::string key = t.getContent();
          match("=", Token::assign);
          ExprModel* val = parseExpression();
          cm->addAttribute(key, val);
          break;
        }

        default:
          unexpected(t, "statement");
      }
    }
    return 0;
  }

  FunctionModel* Parser::parseFunction() {
    Token t = tokenizer->next();
    if (!(t.getType() == Token::name || t.getType() == Token::binOp)) {
      unexpected(t, "function name");
    }
    std::string name = t.getContent();

    FunctionArgsModel* args = parseFunctionArgs();

    BlockModel* block = parseBlock();

    return new FunctionModel(name, args, block);
  }

  FunctionArgsModel* Parser::parseFunctionArgs() {
    FunctionArgsModel* args = new FunctionArgsModel();

    if (!isNext(Token::lpar)) {
      return args;
    }
    else {
      tokenizer->skip();
    }

    if (isNext(Token::rpar)) {
      tokenizer->skip();
      return args;
    }

    while (true) {
      Token t = match("parameter name", Token::name);
      std::string n = t.getContent();

      t = tokenizer->next();

      args->push(n);

      if (t.getType() == Token::comma) {
        continue;
      }
      else if (t.getType() == Token::rpar) {
        return args;
      }
      else {
        unexpected(t, "), or another parameter");
        return 0;
      }
    }
  }

  ExprModel* Parser::parseExpression() {
    Token t = tokenizer->next();
    ExprModel* lhs;
    switch (t.getType()) {
      case Token::lpar:
        lhs = parseExpression();
        match(")", Token::rpar);
        break;

      case Token::integer:
        lhs = new IntegerModel(std::atoi(t.getContent().c_str()));
        break;

      case Token::decimal:
        lhs = new DoubleModel(std::atof(t.getContent().c_str()));
        break;

      case Token::string:
        lhs = new StringModel(t.getContent());
        break;

      case Token::at: {
        NameModel* self = new NameModel("self");
        std::string name = match("instance variable name", Token::name).getContent();

        if (isNext(Token::lpar)) {
          CallArgsModel* args = parseCallArgs();
          lhs = new MethodCallModel(self, name, args);
        }
        else if (isNext(Token::assign)) {
          tokenizer->skip();
          ExprModel* e = parseExpression();
          lhs = new AttributeModel(self, name, e);
        }
        else {
          lhs = new AttributeModel(self, name);
        }
        break;
      }

      case Token::name:
        if (isNext(Token::lpar)) {
          std::string name = t.getContent();
          CallArgsModel* args = parseCallArgs();
          lhs = new CallModel(name, args);
        }
        else if (isNext(Token::assign)) {
          std::string name = t.getContent();
          match("equal", Token::assign);
          ExprModel* e = parseExpression();
          lhs = new AssignModel(name, e);
        }
        else if (isNext(Token::dblcolon)) {
          std::string name = t.getContent();
          name += tokenizer->next().getContent();
          name += match("module variable name", Token::name).getContent();
          if (isNext(Token::lpar)) {
            CallArgsModel* args = parseCallArgs();
            lhs = new CallModel(name, args);
          }
          else {
            lhs = new NameModel(name);
          }
        }
        else {
          lhs = new NameModel(t.getContent());
        }
        break;

      case Token::bTrue:
        lhs = new BoolModel(true);
        break;

      case Token::bFalse:
        lhs = new BoolModel(false);
        break;

      case Token::nil:
        lhs = new NilModel();
        break;

      default:
        unexpected(t, "expression");
    }
    t = tokenizer->next();
    std::string op;
    switch (t.getType()) {
      case Token::binOp:
        op = t.getContent();
        break;

      case Token::dot: {
        std::string name = match("attribute name", Token::name).getContent();
        if (isNext(Token::lpar)) {
          CallArgsModel* args = parseCallArgs();
          return new MethodCallModel(lhs, name, args);
        }
        else {
          return new AttributeModel(lhs, name);
        }
      }

      default:
        tokenizer->back();
        return lhs;
    }
    ExprModel* rhs = parseExpression();
    return new BinOpModel(lhs, rhs, op);
  }

  CallArgsModel* Parser::parseCallArgs() {
    CallArgsModel* args = new CallArgsModel();

    match("(", Token::lpar);

    Token t = tokenizer->peek();
    if (t.getType() == Token::rpar) {
      tokenizer->skip();
      return args;
    }

    while (true) {
      args->push(parseExpression());
      t = tokenizer->next();
      if (t.getType() == Token::comma) {
        continue;
      }
      else if (t.getType() == Token::rpar) {
        return args;
      }
      else {
        unexpected(t, "), or another parameter");
      }
    }
  }

  WhileModel* Parser::parseWhile() {
    ExprModel* cond = parseExpression();
    BlockModel* block = parseBlock();
    return new WhileModel(cond, block);
  }

  IfModel* Parser::parseIf() {
    ExprModel* cond = parseExpression();
    BlockModel* thenBlock = parseBlock();
    BlockModel* elseBlock = 0;
    if (isNext(Token::elseKw)) {
      tokenizer->skip();
      elseBlock = parseBlock();
    }
    return new IfModel(cond, thenBlock, elseBlock);
  }

}
