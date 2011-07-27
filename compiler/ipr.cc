#include <cstdlib>
#include <cstdio>
#include <readline/readline.h>
#include <readline/history.h>
#include "ast.hh"
#include "parser.hh"
#include "compiler.hh"
#include "program.hh"
#include "error.hh"
#include "config.hh"

int main(int argc, char** argv) {
  std::cout << "Proty " VERSION " " BUILD_TYPE " (" __DATE__ ", " __TIME__ ")" << std::endl;
  #ifdef GIT_VERSION
  std::cout << "[" GIT_VERSION "]" << std::endl;
  #endif
  std::cout << std::endl;

  Parser* parser = new Parser;
  Compiler* compiler = new Compiler("<ipr>", true);

  while (true) {
    char* input = readline(">>> ");

    if (!input) break;
    add_history(input);

    Node* root;
    try {
      root = parser->parseStr(input);
      compiler->run(root);
      delete root;
    }
    catch (Error* e) {
      e->printMessage();
      delete e;
      continue;
    }

    free(input);
  }

  return 0;
}
