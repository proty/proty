#include <iostream>
#include "lexer.hh"
#include "parser.hh"
#include "compiler.hh"

void version() {
  std::cerr << "Proty " << VERSION << " (" __DATE__ ", " __TIME__ ")" << std::endl;
  std::cerr << std::endl;
  std::cerr << "This program comes with ABSOLUTELY NO WARRANTY." << std::endl;
  std::cerr << "This is free software, and you are welcome to redistribute it" << std::endl;
  std::cerr << "under certain conditions; see proty.cc/license for details." << std::endl;
}

void help() {
  std::cerr <<
  "Usage: proty [options ...] [file] [arguments]\n"
  "-d           Dump code\n"
  "--help, -h   Shows this message\n"
  "-r           Run the genereted code\n"
  "-c [file]    Compile to file\n"
  "--version    Version and copyright info\n"
  "--           Stop reading options\n";
}

int main(int argc, char** argv) {
  std::string file = "<stdin>";
  std::string output;
  bool dump = false;

  for (int count = 1; count < argc; count++) {
    if (argv[count][0] == '-') {
      if      (argv[count] == std::string("-d"))          { dump = true; }
      else if (argv[count] == std::string("-h"))          { help(); return 0; }
      else if (argv[count] == std::string("--help"))      { help(); return 0; }
      else if (argv[count] == std::string("-c"))          { output = argv[++count]; }
      else if (argv[count] == std::string("--version"))   { version(); return 0; }
      else if (argv[count] == std::string("--"))          { break; }
      else {
        std::cerr << "pyrite: invalid option " << argv[count] << std::endl;
        return 1;
      }
    }
    else { file = std::string(argv[count]); break; }
  }

  try {
    Lexer* lexer = new Lexer(file);
    Parser* parser = new Parser(lexer);
    Node* root = parser->parse();
  }
  catch (std::string s) {
    std::cout << "error: " << s << std::endl;
    return 0;
  }

  /*
  Compiler* c = new Compiler(file);
  Module* m = c->compile(pm);

  if (dump) m->dump();

  std::vector<GenericValue> args;
  Function* main = c->executionEngine->FindFunctionNamed("main");
  c->executionEngine->runStaticConstructorsDestructors(false);
  c->executionEngine->runFunction(main, args);
  */

  return 0;
}
