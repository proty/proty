#include <iostream>
#include "ast.hh"
#include "parser.hh"
#include "compiler.hh"
#include "program.hh"
#include "error.hh"
#include "config.hh"

void version() {
  std::cerr << "Proty " VERSION " " BUILD_TYPE " (" __DATE__ ", " __TIME__ ")" << std::endl;
  #ifdef GIT_VERSION
  std::cerr << "[" GIT_VERSION "]" << std::endl;
  #endif
  std::cerr << std::endl;
  std::cerr << "This program comes with ABSOLUTELY NO WARRANTY." << std::endl;
  std::cerr << "This is free software, and you are welcome to redistribute it" << std::endl;
  std::cerr << "under certain conditions; see proty.cc/license for details." << std::endl;
}

void help() {
  std::cerr <<
  "Usage: proty [options ...] [input file]\n"
  "  -d           Dump LLVM code\n"
  "  -g           Enable debugging symbols\n"
  "  --help, -h   Shows this message\n"
  "  -m           Compile to a module\n"
  "  -o [file]    Set output file\n"
  "  --version    Shows version information\n"
  "  --           Stop reading options\n";
}

int main(int argc, char** argv) {
  std::string file = "<stdin>";
  std::string output;

  bool dump = false;
  bool debug = false;
  bool module = false;

  for (int count = 1; count < argc; count++) {
    if (argv[count][0] == '-') {
      if      (argv[count] == std::string("-d"))          { dump = true; }
      else if (argv[count] == std::string("-g"))          { debug = true; }
      else if (argv[count] == std::string("-h"))          { help(); return 0; }
      else if (argv[count] == std::string("--help"))      { help(); return 0; }
      else if (argv[count] == std::string("-m"))          { module = true; }
      else if (argv[count] == std::string("-o"))          { output = argv[++count]; }
      else if (argv[count] == std::string("--version"))   { version(); return 0; }
      else if (argv[count] == std::string("--"))          { break; }
      else {
        help();
        std::cerr << "proty: invalid option " << argv[count] << std::endl;
        return 1;
      }
    }
    else { file = std::string(argv[count]); break; }
  }

  std::string module_name = "<stdin>";
  if (file != "<stdin>") {
    int fsize = file.size();
    module_name = file.substr(0, fsize-3);

    std::string ext = file.substr(fsize-3, fsize);
    if (ext != ".pr") {
      std::cerr << "proty: unknown file extension" << std::endl;
      return 10;
    }

    if (module && output.empty()) {
      output = module_name + ".bc";
    }
  }

  Parser* parser = new Parser;

  Node* root;
  try {
    root = parser->parseFile(file);
  }
  catch (Error* e) {
    e->printMessage();
    return 1;
  }

  Compiler* compiler = new Compiler(module_name);
  compiler->debug = debug;

  try {
    compiler->addNode(root);
  }
  catch (Error* e) {
    e->printMessage();
    return 2;
  }

  Program* program = compiler->getProgram(!module);

  delete parser;
  delete root;
  delete compiler;

  if (dump) program->dump();

  if (!output.empty()) {
    program->writeToFile(output);
  }
  else {
    program->run();
  }

  return 0;
}
