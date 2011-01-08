#include "lexer.hh"
#include <fstream>
#include <sstream>
#include <cstdlib>

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
  std::cout << token.getType() << ": " << token.getValue() << std::endl;
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
  while (true) {
    char currch = stream->get();
    char nextch = stream->peek();

    // whitespace
    if (isspace(currch)) continue;

    // integer
    else if (isdigit(currch)) {
      std::stringstream buf;
      buf << currch;
      while(isdigit(stream->peek())) {
        buf << (char)stream->get();
      }
      add(Token(Token::integer, buf.str()));
    }

    // name and keywords
    else if (isalpha(currch)) {
      std::stringstream buf;
      buf << currch;
      while(isalpha(stream->peek())) {
        buf << (char)stream->get();
      }
      std::string value = buf.str();

      if (value == "do") add(Token(Token::doKw, value));
      else if (value == "if") add(Token(Token::ifKw, value));
      else if (value == "else") add(Token(Token::elseKw, value));
      else if (value == "while") add(Token(Token::whileKw, value));
      else if (value == "end") add(Token(Token::endKw, value));
      else add(Token(Token::name, value));
    }

    // string
    else if (currch == '"') {
      std::stringstream buf;
      buf << currch;
      while (stream->peek() != '"') {
        buf << (char)stream->get();
      }
      stream->get();
      add(Token(Token::string, buf.str()));
    }

    else if (currch == '(') add(Token(Token::lpar, "("));
    else if (currch == ')') add(Token(Token::rpar, ")"));
    else if (currch == '[') add(Token(Token::lsqb, "]"));
    else if (currch == ']') add(Token(Token::rsqb, "["));
    else if (currch == '{') add(Token(Token::lbrace, "{"));
    else if (currch == '}') add(Token(Token::rbrace, "}"));
    else if (currch == ',') add(Token(Token::comma, ","));

    // = and ==
    else if (currch == '=') {
      if (nextch == '=') {
        stream->get();
        add(Token(Token::binaryop, "==", 20));
      }
      else add(Token(Token::binaryop, "=", 5));
    }

    // > and >=
    else if (currch == '>') {
      if (nextch == '=') {
        stream->get();
        add(Token(Token::binaryop, ">=", 20));
      }
      else add(Token(Token::binaryop, ">", 15));
    }

    // < and <=
    else if (currch == '<') {
      if (nextch == '=') {
        stream->get();
        add(Token(Token::binaryop, "<=", 20));
      }
      else add(Token(Token::binaryop, "<", 20));
    }

    // != and @todo: !
    else if (currch == '!') {
      if (nextch == '=') {
        stream->get();
        add(Token(Token::binaryop, "!=", 20));
      }
      else add(Token(Token::unknown, "unknown"));
    }

    else if (currch == '+') add(Token(Token::binaryop, "+", 10));
    else if (currch == '-') add(Token(Token::binaryop, "-", 10));
    else if (currch == '*') add(Token(Token::binaryop, "*", 15));
    else if (currch == '%') add(Token(Token::binaryop, "%", 15));
    else if (currch == '.') add(Token(Token::binaryop, ".", 25));

    // division and comment
    else if (currch == '/') {
      if (nextch == '/') {
        while (stream->get() != '\n');
      }
      else {
        add(Token(Token::binaryop, "/", 15));
      }
    }

    else {
      add(Token(Token::unknown, "" + currch));
    }

    if (stream->eof()) {
      add(Token(Token::eof, "<eof>"));
      break;
    }
  }
}
