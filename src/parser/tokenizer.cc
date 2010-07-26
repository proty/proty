#include "parser/tokenizer.hh"
#include <sstream>

namespace pyrite {

  Tokenizer::Tokenizer(std::string file) {
    loc = new Location(file, 1);

    if (file == "<stdin>") stream = &std::cin;
    else stream = new std::ifstream(file.c_str(), std::ifstream::in);
    if (!*stream) { throw "can't open file " + file; }

    indent_length = 0;
    indent_level = 0;
  }

  void Tokenizer::tokenize() {
    tokens = new std::vector<Token>();
    Token t;
    do {
      t = nextToken();
      tokens->push_back(t);
    }
    while (t.getType() != Token::eof);
    pos = 0;
  }

  Token Tokenizer::next() {
    return tokens->at(pos++);
  }

  Token Tokenizer::peek(int i) {
    return tokens->at(pos+i);
  }

  void Tokenizer::skip() {
    pos++;
  }

  void Tokenizer::back() {
    pos--;
  }

  Token Tokenizer::nextToken() {
    enum {
        st_none,
        st_name,
        st_comment,
        st_string,
        st_integer,
        st_decimal
    } state = st_none;

    std::stringstream buf;

    while (true) {
      if (stream->eof()) break;

      char currch = stream->get();
      char nextch = stream->peek();

      if (currch == '\n') {
        loc->incLineNumber();

        // Get number of spaces
        int length = 0;
        while (stream->peek() == ' ') {
          stream->get();
          length++;
        }
        if (stream->peek() == '\n') continue;

        // if there has been no indent before
        if ((!indent_length && indent_level == 0)) {
          if (length != 0) {
            indent_length = length;
            indent_level++;
            return Token(Token::indent, loc->copy(), "indent");
          }
        }
        else {
          // if the indent doesn't match the indents before
          if (length % indent_length != 0) {
            throw "invalid indentation";
            return Token(Token::dedent, loc->copy(), "dedent");
          }
          else {
            // if the indent is greater than before
            if (length > indent_level*indent_length) {
              indent_level++;
              return Token(Token::indent, loc->copy(), "indent");
            }
            // if the indent is smaller than before
            else if (length < indent_level*indent_length) {
              // not a good way, but working for multiple dedents at the same time
              for (int i = 1; i <  indent_level - (length / indent_length); i++) {
                indent_level--;
                tokens->push_back(Token(Token::dedent, loc->copy(), "dedent"));
              }
              indent_level--;
              return Token(Token::dedent, loc->copy(), "dedent");
            }
          }
        }
      }

      switch(state) {
        case st_none:
          if (isspace(currch)) {
            continue;
          }
          else if (isalpha(currch) || currch == '_') {
            buf << currch;
            if (isalpha(nextch) || nextch == '_') {
              state = st_name;
            }
            else {
              return Token(Token::name, loc->copy(), buf.str());
            }
          }
          else if (currch == '"') {
            state = st_string;
          }
          else if (isdigit(currch)) {
            buf << currch;
            if (isdigit(nextch)) {
              state = st_integer;
            }
            else if (nextch == '.') {
              state = st_decimal;
            }
            else {
              return Token(Token::integer, loc->copy(), buf.str());
            }
          }
          else if (currch == '.') {
            return Token(Token::dot, loc->copy(), ".");
          }
          else if (currch == '(') {
            return Token(Token::lpar, loc->copy(), "(");
          }
          else if (currch == ')') {
            return Token(Token::rpar, loc->copy(), ")");
          }
          else if (currch == '[') {
            return Token(Token::lsqb, loc->copy(), "]");
          }
          else if (currch == ']') {
            return Token(Token::rsqb, loc->copy(), "[");
          }
          else if (currch == '{') {
            return Token(Token::lbrace, loc->copy(), "{");
          }
          else if (currch == '}') {
            return Token(Token::rbrace, loc->copy(), "}");
          }
          else if (currch == '=') {
            if (nextch == '=') {
              stream->get();
              return Token(Token::eq, loc->copy(), "==");
            }
            else if (nextch == '>') {
              stream->get();
              return Token(Token::arrow, loc->copy(), "=>");
            }
            else return Token(Token::assign, loc->copy(), "=");
          }
          else if (currch == '>') {
            if (nextch == '=') {
              stream->get();
              return Token(Token::geq, loc->copy(), ">=");
            }
            else return Token(Token::gr, loc->copy(), ">");
          }
          else if (currch == '<') {
            if (nextch == '=') {
              stream->get();
              return Token(Token::leq, loc->copy(), "<=");
            }
            else return Token(Token::le, loc->copy(), "<");
          }
          else if (currch == '!') {
            if (nextch == '=') {
              stream->get();
              return Token(Token::neq, loc->copy(), "!=");
            }
            else return Token(Token::unknown, loc->copy(), "unknown");
          }
          else if (currch == '+') {
            return Token(Token::plus, loc->copy(), "+");
          }
          else if (currch == '-') {
            return Token(Token::minus, loc->copy(), "-");
          }
          else if (currch == '*') {
            return Token(Token::asterisk, loc->copy(), "*");
          }
          else if (currch == '/') {
            if (nextch == '/') {
              stream->get();
              state = st_comment;
            }
            else {
              return Token(Token::slash, loc->copy(), "/");
            }
          }
          else if (currch == '%') {
            return Token(Token::percent, loc->copy(), "%");
          }
          else if (currch == ',') {
            return Token(Token::comma, loc->copy(), ",");
          }
          else if (currch == ':') {
            if (nextch == ':') {
              stream->get();
              return Token(Token::dblcolon, loc->copy(), "::");
            }
           else return Token(Token::colon, loc->copy(), ":");
          }
          else return Token(Token::unknown, loc->copy(), "unknown");
          break;

        case st_name:
          buf << currch;
          if (!isalnum(nextch) && nextch != '_') {
            state = st_none;

            std::string name = buf.str();

            if (name == "if")
              return Token(Token::ifKw, loc->copy(), "if");
            else if (name == "else")
              return Token(Token::elseKw, loc->copy(), "else");
            else if (name == "while")
              return Token(Token::whileKw, loc->copy(), "while");
            else if (name == "def")
              return Token(Token::defKw, loc->copy(), "def");
            else if (name == "class")
              return Token(Token::classKw, loc->copy(), "class");
            else if (name == "module")
              return Token(Token::moduleKw, loc->copy(), "module");
            else if (name == "return")
              return Token(Token::returnKw, loc->copy(), "return");
            else if (name == "import")
              return Token(Token::importKw, loc->copy(), "import");
            else if (name == "or")
              return Token(Token::logOr, loc->copy(), "or");
            else if (name == "and")
              return Token(Token::logAnd, loc->copy(), "and");
            else if (name == "true")
              return Token(Token::bTrue, loc->copy(), "true");
            else if (name == "false")
              return Token(Token::bFalse, loc->copy(), "false");
            else
              return Token(Token::name, loc->copy(), name);
          }
          break;

        case st_comment:
          if (currch == '\n')
            state = st_none;
          break;

        case st_string:
          buf << currch;
          if (nextch == '"') {
            stream->get();
            state = st_none;
            return Token(Token::string, loc->copy(), buf.str());
          }
          break;

        case st_integer:
          buf << currch;
          if (nextch == '.') {
            state = st_decimal;
          }
          else if (!isdigit(nextch)) {
            state = st_none;
            return Token(Token::integer, loc->copy(), buf.str());
          }
          break;

        case st_decimal:
          buf << currch;
          if (!isdigit(nextch)) {
            state = st_none;
            return Token(Token::decimal, loc->copy(), buf.str());
          }
          break;

        default:
          throw "tokenizer error";
      }
    }

    if (state == st_none) {
      return Token(Token::eof, loc->copy(), "end of file");
    }
    else {
      throw "tokenizer error";
    }
  }

}
