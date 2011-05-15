#include <iostream>
#include "lexer.hh"
#include "parser.hh"
#include "compiler.hh"
#include "config.hh"

void version() {
  std::cerr << "Proty " VERSION " " BUILD_TYPE " (" __DATE__ ", " __TIME__ ")" << std::endl;
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
  "-b [file]    Write LLVM Bitcode to file\n"
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
      else if (argv[count] == std::string("-b"))          { output = argv[++count]; }
      else if (argv[count] == std::string("--version"))   { version(); return 0; }
      else if (argv[count] == std::string("--"))          { break; }
      else {
        std::cerr << "proty: invalid option " << argv[count] << std::endl;
        return 1;
      }
    }
    else { file = std::string(argv[count]); break; }
  }

  Lexer* lexer = new Lexer(file);
  Parser* parser = new Parser(lexer);
  Node* root = parser->parse();

  Compiler* compiler = new Compiler(file);
  llvm::Module* module = compiler->compile(root);

  if (dump) module->dump();

  if (!output.empty()) {
    std::string error;
    llvm::raw_fd_ostream out(output.c_str(), error);
    llvm::WriteBitcodeToFile(module, out);
  }
  else {
    std::vector<llvm::GenericValue> args;
    llvm::Function* main = compiler->executionEngine->FindFunctionNamed("main");
    compiler->executionEngine->runStaticConstructorsDestructors(false);
    compiler->executionEngine->runFunction(main, args);
  }

  return 0;
}
