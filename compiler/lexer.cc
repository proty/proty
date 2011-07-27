#include "lexer.hh"
#include "error.hh"
#include <fstream>
#include <sstream>
#include <cstdlib>

Lexer::Lexer(std::istream* stream, std::string filename) {
  this->stream = stream;
  this->filename = filename;

  tokens = new std::vector<Token>;
  pos = 0;
  line = 1;
}

Lexer::~Lexer() {
  delete tokens;
}

void Lexer::add(Token token) {
  Location loc;
  loc.filename = filename;
  loc.line = line;

  token.setLocation(loc);
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

void Lexer::unexpected(Token found, std::string expected) {
  throw new ParseError(found.getLocation(), "expected '" + expected + "', found '" + found.getValue() + "'");
}

Token Lexer::match(Token::Type to_match, std::string expected) {
  Token t = next();
  if (t.getType() != to_match) {
    unexpected(t, expected);
  }
  return t;
}

void Lexer::tokenize() {
  char currch = stream->get();
  char nextch = stream->peek();

  if (stream->eof()) {
    add(Token(Token::eof, "<eof>"));
    return;
  }

  while (true) {
    // whitespace
    if (isspace(currch)) {
      if (currch == '\n') {
         line++;
      }
      // ignore
    }

    // integer and decimal
    else if (isdigit(currch)) {
      std::stringstream buf;
      buf << currch;
      while(isdigit(stream->peek())) {
        buf << (char)stream->get();
      }
      // decimal
      if (stream->peek() == '.') {
        buf << (char)stream->get();
        while(isdigit(stream->peek())) {
          buf << (char)stream->get();
        }
        add(Token(Token::decimal, buf.str()));
      }
      else {
        add(Token(Token::integer, buf.str()));
      }
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
      else if (value == "try") add(Token(Token::tryKw, value));
      else if (value == "catch") add(Token(Token::catchKw, value));
      else add(Token(Token::name, value));
    }

    // string
    else if (currch == '"') {
      std::stringstream buf;
      while (stream->peek() != '"') {
        if (stream->peek() == '\\') {
          stream->get();
          switch (stream->get()) {
          case 'a':
            buf << '\a';
            break;
          case 'b':
            buf << '\b';
            break;
          case 'n':
            buf << '\n';
            break;
          case 'r':
            buf << '\r';
            break;
          case 't':
            buf << '\t';
            break;
          case '\\':
            buf << '\\';
            break;
          default:
            buf << '\\';
            buf << stream->unget();
            buf << stream->get();
          }
        }
        else {
          buf << (char)stream->get();
        }
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
      else add(Token(Token::assign, "="));
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
    else if (currch == '.') add(Token(Token::dot, "."));
    else if (currch == ':') add(Token(Token::colon, ":"));

    // division and comments
    else if (currch == '/') {
      if (nextch == '/') {
        while (!stream->eof() && stream->get() != '\n');
        line++;
      }
      else if (nextch == '*') {
        stream->get();
        while (!stream->eof() && !(stream->get() == '*'
               && stream->peek() == '/')) {
          if (stream->peek() == '\n') line++;
        }
        stream->get();
      }
      else {
        add(Token(Token::binaryop, "/", 15));
      }
    }

    // ignore shebang
    else if (currch == '#') {
      if (nextch == '!') {
        while (!stream->eof() && stream->get() != '\n');
        line++;
      }
    }

    else {
      // ignore eof
      if (!(currch == -1)) {
        add(Token(Token::unknown, "" + currch));
      }
    }

    if (stream->eof()) {
      add(Token(Token::eof, "<eof>"));
      break;
    }

    currch = stream->get();
    nextch = stream->peek();
  }
}
