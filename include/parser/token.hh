#ifndef PYRITE_TOKEN_HH
#define PYRITE_TOKEN_HH

#include "parser/location.hh"

namespace pyrite {

  /// Represents a single token.
  class Token {
    public:

      /// List of all existing tokens.
      typedef enum {
        unknown, eof,
        name, string, integer, decimal,
        ifKw, elseKw, whileKw, defKw, classKw, moduleKw, returnKw,
        endKw, importKw,
        lpar, rpar, lsqb, rsqb, lbrace, rbrace, arrow, at, hash,
        assign, colon, dblcolon, comma, dot, binOp, bTrue, bFalse, nil
      } Type;

    private:
      Type type;
      Location* loc;
      std::string content;

    public:
      Token() { }
      Token(Type, Location*, std::string content="");

      /// @returns the token type
      Type getType();
      /// @returns the Location of the token
      Location* getLocation();
      /// @returns the text of the token
      std::string getContent();
  };

}

#endif
