#ifndef PYRITE_PARSER_HH
#define PYRITE_PARSER_HH

#include "parser/tokenizer.hh"
#include "model/models.hh"

namespace pyrite {

  /// Handles parsing of a program.
  /// The Parser parses the token stream
  /// from the Tokenizer and returns
  /// a BlockModel.
  class Parser {
    private:
      Tokenizer* tokenizer;
      
    public:
      Parser(Tokenizer*);
      ProgramModel* parse();

    private:
      ProgramModel* parseProgram();
      BlockModel* parseBlock();
      ModuleModel* parseModule();
      ClassModel* parseClass();
      FunctionModel* parseFunction(bool=false);
      FunctionArgsModel* parseFunctionArgs();
      ExprModel* parseExpression();
      CallModel* parseCall();
      CallArgsModel* parseCallArgs();
      WhileModel* parseWhile();
      IfModel* parseIf();
      AssignModel* parseAssign();

      /// Tests if the current token is
      /// of the expected type. If not
      /// it raises an error.
      Token match(std::string, Token::Type);

      /// Tests if the next token is of
      /// the given type.
      bool isNext(Token::Type type);

      /// Throws a ParseError of an
      /// unexpected token.
      void unexpected(Token, std::string expected="");
  };

}

#endif
