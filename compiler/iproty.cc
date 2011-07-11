#include <cstdlib>
#include <cstdio>
#include <readline/readline.h>
#include <readline/history.h>
#include "ast.hh"
#include "parser.hh"
#include "compiler.hh"
#include "program.hh"
#include "config.hh"

int main(int argc, char** argv) {
  std::cout << "Proty " VERSION " " BUILD_TYPE " (" __DATE__ ", " __TIME__ ")" << std::endl;
  #ifdef GIT_VERSION
  std::cout << "[" GIT_VERSION "]" << std::endl;
  #endif
  std::cout << std::endl;

  Parser* parser = new Parser;
  Compiler* compiler = new Compiler("interactive", false);

  while (true) {
    char* input = readline(">>> ");

    if (!input) break;
    add_history(input);

    Node* root = parser->parseStr(input);
    compiler->addNode(root);
    delete root;

    free(input);
  }

  Program* program = compiler->getProgram();
  program->run();

  return 0;
}
