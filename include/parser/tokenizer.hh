#ifndef PYRITE_TOKENIZER_HH
#define PYRITE_TOKENIZER_HH

#include "parser/token.hh"
#include <fstream>
#include <vector>

namespace pyrite {

  /// Makes a file stream to
  /// a Token stream.
  class Tokenizer {
    private:
      Location* loc;
      std::istream* stream;

      std::vector<Token>* tokens;
      int pos;

    public:
      Tokenizer(std::string);

      /// Tokenizes a file.
      /// This method must be
      /// called before you can
      /// access the tokens.
      void tokenize();
      
      /// Get the next token.
      Token next();
      /// Peeks the next [i]th token.
      Token peek(int i=0);
      /// Skips tokens.
      void skip(int i=1);
      /// Goes back.
      void back(int i=1);

    private:
      /// Used by tokenize() to
      /// fill the Token vector.
      Token nextToken();
  };

}

#endif
