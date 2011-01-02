#include "lexer.hh"
#include <fstream>
#include <sstream>

Lexer::Lexer(std::string filename) {
  if (filename == "<stdin>") stream = &std::cin;
  else stream = new std::ifstream(filename.c_str(), std::ifstream::in);
  if (!*stream) { throw "cannot open file " + filename; }

  tokens = new std::vector<Token>;
}

void Lexer::add(Token token) {
  tokens->push_back(token);
}

void Lexer::tokenize() {
  std::stringstream buf;

  while (true) {
    if (stream->eof()) break;

    char currch = stream->get();
    char nextch = stream->peek();

    if (isspace(currch)) {
      continue;
    }
    else if (isdigit(currch)) {
      
    }
    else if (currch == '(') {
      add(Token(Token::lpar, "("));
    }
    else if (currch == ')') {
      add(Token(Token::rpar, ")"));
    }
    else if (currch == '[') {
      add(Token(Token::lsqb, "]"));
    }
    else if (currch == ']') {
      add(Token(Token::rsqb, "["));
    }
    else if (currch == '{') {
      add(Token(Token::lbrace, "{"));
    }
    else if (currch == '}') {
      add(Token(Token::rbrace, "}"));
    }
    else if (currch == '=') {
      if (nextch == '=') {
        stream->get();
        add(Token(Token::binaryop, "=="));
      }
      else add(Token(Token::binaryop, "="));
    }
    else if (currch == '>') {
      if (nextch == '=') {
        stream->get();
        add(Token(Token::binaryop, ">="));
      }
      else add(Token(Token::binaryop, ">"));
    }
    else if (currch == '<') {
      if (nextch == '=') {
        stream->get();
        add(Token(Token::binaryop, "<="));
      }
      else add(Token(Token::binaryop, "<"));
    }
    else if (currch == '!') {
      if (nextch == '=') {
        stream->get();
        add(Token(Token::binaryop, "!="));
      }
      else add(Token(Token::unknown, "unknown"));
    }
    else if (currch == '+') {
      add(Token(Token::binaryop, "+"));
    }
    else if (currch == '-') {
      add(Token(Token::binaryop, "-"));
    }
    else if (currch == '*') {
      add(Token(Token::binaryop, "*"));
    }
    else if (currch == '/') {
      if (nextch == '/') {
        stream->get();
        //state = st_comment;
      }
      else {
        add(Token(Token::binaryop, "/"));
      }
    }
    else if (currch == '%') {
      add(Token(Token::binaryop, "%"));
    }
  }
}