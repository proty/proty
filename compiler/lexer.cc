#include "lexer.hh"
#include <fstream>
#include <sstream>

Lexer::Lexer(std::string filename) {
  if (filename == "<stdin>") stream = &std::cin;
  else stream = new std::ifstream(filename.c_str(), std::ifstream::in);
  if (!*stream) { 
    std::cerr << "cannot open file " + filename << std::endl;
    exit(1);
  }

  tokens = new std::vector<Token>;
  pos = 0;
}

void Lexer::add(Token token) {
  tokens->push_back(token);
}

Token Lexer::next() {
  return tokens->at(pos++);
}

Token Lexer::peek(int i) {
  return tokens->at(pos+i);
}

bool Lexer::isNext(Token::Type type) {
  return tokens->at(pos).getType() == type;
}

Token Lexer::match(Token::Type expected, std::string name) {
  Token t = next();
  if (t.getType() != expected) {
    std::cerr << "expected " + name + ", found " + t.getValue() << std::endl;
    exit(1);
  }
  return t;
}

void Lexer::tokenize() {
  std::stringstream buf;

  while (true) {
    if (stream->eof()) {
      add(Token(Token::eof, "<eof>"));
      break;
    }

    char currch = stream->get();
    char nextch = stream->peek();

    // whitespace
    if (isspace(currch)) continue;

    // integer
    else if (isdigit(currch)) {
      buf << currch;
      while(isdigit(stream->peek())) {
        buf << stream->get();
      }
      add(Token(Token::integer, buf.str()));
      buf.clear();
    }

    // name
    else if (isalpha(currch)) {
      buf << currch;
      while(isalpha(stream->peek())) {
        buf << stream->get();
      }
      add(Token(Token::name, buf.str()));
      buf.clear();
    }

    // string
    else if (currch == '"') {
      buf << currch;
      while (stream->peek() != '"') {
        buf << stream->get();
      }
      stream->get();
      add(Token(Token::string, buf.str()));
      buf.clear();
    }

    else if (currch == '(') add(Token(Token::lpar, "("));
    else if (currch == ')') add(Token(Token::rpar, ")"));
    else if (currch == '[') add(Token(Token::lsqb, "]"));
    else if (currch == ']') add(Token(Token::rsqb, "["));
    else if (currch == '{') add(Token(Token::lbrace, "{"));
    else if (currch == '}') add(Token(Token::rbrace, "}"));

    // = and ==
    else if (currch == '=') {
      if (nextch == '=') {
        stream->get();
        add(Token(Token::binaryop, "=="));
      }
      else add(Token(Token::binaryop, "="));
    }

    // > and >=
    else if (currch == '>') {
      if (nextch == '=') {
        stream->get();
        add(Token(Token::binaryop, ">="));
      }
      else add(Token(Token::binaryop, ">"));
    }

    // < and <=
    else if (currch == '<') {
      if (nextch == '=') {
        stream->get();
        add(Token(Token::binaryop, "<="));
      }
      else add(Token(Token::binaryop, "<"));
    }

    // != and @todo: !
    else if (currch == '!') {
      if (nextch == '=') {
        stream->get();
        add(Token(Token::binaryop, "!="));
      }
      else add(Token(Token::unknown, "unknown"));
    }

    else if (currch == '+') add(Token(Token::binaryop, "+"));
    else if (currch == '-') add(Token(Token::binaryop, "-"));
    else if (currch == '*') add(Token(Token::binaryop, "*"));
    else if (currch == '%') add(Token(Token::binaryop, "%"));

    // division and comment
    else if (currch == '/') {
      if (nextch == '/') {
        while (stream->get() != '\n');
      }
      else {
        add(Token(Token::binaryop, "/"));
      }
    }

    else {
      add(Token(Token::unknown, "" + currch));
    }
  }
}
