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
      ProgramModel* parse_program();
      BlockModel* parse_block();
      ModuleModel* parse_module();
      ClassModel* parse_class();
      FunctionModel* parse_function();
      FunctionArgsModel* parse_function_args();
      ExprModel* parse_expression();
      CallModel* parse_call();
      CallArgsModel* parse_call_args();
      WhileModel* parse_while();
      IfModel* parse_if();
      AssignModel* parse_assign();
      TypeModel* parse_type_name();

      /// Tests if the current token is
      /// of the expected type. If not
      /// it raises an error.
      Token match(std::string, Token::Type);

      /// Tests if the next token is of
      /// the given type.
      bool is_next(Token::Type type);

      /// Throws a ParseError of an
      /// unexpected token.
      void unexpected(Token, std::string expected="");
  };

}

#endif
