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

  bool Parser::is_next(Token::Type type) {
    return tokenizer->peek().getType() == type;
  }

  BlockModel* Parser::parse() {
    tokenizer->tokenize();
    try {
      BlockModel* b = parse_program();
      return b;
    }
    catch (ParseError* e) {
      e->alert();
      std::exit(1);
    }
  }

  BlockModel* Parser::parse_program() {
    BlockModel* b = new BlockModel();
    while (true) {
      Token t = tokenizer->peek();
      switch (t.getType()) {
        case Token::moduleKw:
          tokenizer->skip();
          b->push(parse_module());
          break;

        case Token::classKw:
          tokenizer->skip();
          b->push(parse_class());
          break;

        case Token::whileKw:
          tokenizer->skip();
          b->push(parse_while());
          break;

        case Token::ifKw:
          tokenizer->skip();
          b->push(parse_if());
          break;

        case Token::defKw:
          tokenizer->skip();
          b->push(parse_function());
          break;

        case Token::importKw: {
          tokenizer->skip();
          Token t2 = tokenizer->next();
          if (t2.getType() == Token::name) {
            b->push(new ImportModel(t2.getContent()));
          }
          else {
            unexpected(t2, "name of package to import");
          }
          break;
        }

        case Token::eof:
          return b;

        default:
          b->push(parse_expression());
      }
    }
  }

  BlockModel* Parser::parse_block() {
    BlockModel* b = new BlockModel();
    match("indent", Token::indent);
    
    while (true) {
      Token t = tokenizer->peek();

      switch (t.getType()) {
        case Token::dedent:
          tokenizer->skip();
        case Token::eof:
          return b;

        case Token::elseKw:
          return b;

        case Token::whileKw:
          tokenizer->skip();
          b->push(parse_while());
          break;

        case Token::ifKw:
          tokenizer->skip();
          b->push(parse_if());
          break;

        case Token::returnKw: {
          t = tokenizer->next();
          ExprModel* e = parse_expression();
          b->push(new ReturnModel(e));
          break;
        }

        default:
          b->push(parse_expression());
      }
    }

    return 0;
  }

  ModuleModel* Parser::parse_module() {
    Token t = match("module name", Token::name);
    match("indent", Token::indent);

    ModuleModel* mm = new ModuleModel(t.getContent());

    while (true) {
      Token t = tokenizer->peek();

      switch (t.getType()) {
        case Token::dedent:
          tokenizer->skip();
        case Token::eof:
          return mm;

        case Token::defKw:
          tokenizer->skip();
          mm->add_function(parse_function());
          break;

        case Token::classKw:
          tokenizer->skip();
          mm->add_class(parse_class());
          break;

        default:
          unexpected(t, "statement");
      }
    }

    return 0;
  }

  ClassModel* Parser::parse_class() {
    Token t = match("class name", Token::name);

    ClassModel* cm = new ClassModel(t.getContent());

    t = tokenizer->peek();
    if (t.getType() == Token::binOp && t.getContent() == "<") {
      tokenizer->skip();
      std::string base = match("base class", Token::name).getContent();
    }

    match("indent", Token::indent);

    while (true) {
      t = tokenizer->peek();

      switch (t.getType()) {
        case Token::dedent:
          tokenizer->skip();

        case Token::eof:
          return cm;

        case Token::defKw:
          tokenizer->skip();
          cm->add_method(parse_function());
          break;

        case Token::name: {
          tokenizer->skip();
          std::string n = t.getContent();
          TypeModel* tp;
          if (is_next(Token::colon)) {
            tokenizer->skip();
            tp = parse_type_name();
          }
          else {
            tp = new TypeModel("Object");
          }
          cm->add_attribute(n, tp);
          break;
        }

        default:
          unexpected(t, "statement");
      }
    }
    return 0;
  }

  FunctionModel* Parser::parse_function() {
    Token t = match("function name", Token::name);
    std::string name = t.getContent();

    FunctionArgsModel* args = parse_function_args();
    
    TypeModel* returnType;
    if (is_next(Token::arrow)) {
      match("arrow", Token::arrow);
      returnType = parse_type_name();
    }
    else {
      returnType = new TypeModel("Object");
    }

    BlockModel* block = parse_block();

    return new FunctionModel(name, args, returnType, block);
  }

  FunctionArgsModel* Parser::parse_function_args() {
    FunctionArgsModel* args = new FunctionArgsModel();

    match("(", Token::lpar);

    if (is_next(Token::rpar)) {
      tokenizer->skip();
      return args;
    }

    while (true) {
      Token t = match("parameter name", Token::name);
      std::string n = t.getContent();
      TypeModel* type;

      t = tokenizer->next();

      if (t.getType() == Token::colon) {
        type = parse_type_name();
        t = tokenizer->next();
      }
      else {
        type = new TypeModel("Object");
      }

      args->push(n, type);

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

  ExprModel* Parser::parse_expression() {
    Token t = tokenizer->next();
    ExprModel* lhs;
    switch (t.getType()) {
      case Token::lpar:
        lhs = parse_expression();
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
      case Token::at:
        break;
      case Token::name:
        if (is_next(Token::lpar)) {
          tokenizer->back();
          lhs = parse_call();
        }
        else if (is_next(Token::assign)) {
          tokenizer->back();
          lhs = parse_assign();
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
        if (is_next(Token::lpar)) {
          CallArgsModel* args = parse_call_args();
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
    ExprModel* rhs = parse_expression();
    return new BinOpModel(lhs, rhs, op);
  }

  CallModel* Parser::parse_call() {
    Token t = match("name", Token::name);
    NameModel* name = new NameModel(t.getContent());
    CallArgsModel* args = parse_call_args();
    return new CallModel(name, args);
  }

  CallArgsModel* Parser::parse_call_args() {
    CallArgsModel* args = new CallArgsModel();

    match("(", Token::lpar);

    Token t = tokenizer->peek();
    if (t.getType() == Token::rpar) {
      tokenizer->skip();
      return args;
    }

    while (true) {
      args->push(parse_expression());
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

  WhileModel* Parser::parse_while() {
    ExprModel* cond = parse_expression();
    BlockModel* block = parse_block();
    return new WhileModel(cond, block);
  }

  IfModel* Parser::parse_if() {
    ExprModel* cond = parse_expression();
    BlockModel* thenBlock = parse_block();
    BlockModel* elseBlock = 0;
    if (is_next(Token::elseKw)) {
      tokenizer->skip();
      elseBlock = parse_block();
    }
    return new IfModel(cond, thenBlock, elseBlock);
  }

  AssignModel* Parser::parse_assign() {
    Token t = tokenizer->peek(0);

    std::string name = match("variable name", Token::name).getContent();
    match("equal", Token::assign);

    ExprModel* e = parse_expression();
    return new AssignModel(name, e);
  }

  TypeModel* Parser::parse_type_name() {
    std::string name = match("type name", Token::name).getContent();
    return new TypeModel(name);
  }

}
