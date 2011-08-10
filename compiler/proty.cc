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
  bool compileModule = false;

  for (int count = 1; count < argc; count++) {
    if (argv[count][0] == '-') {
      if      (argv[count] == std::string("-d"))          { dump = true; }
      else if (argv[count] == std::string("-g"))          { debug = true; }
      else if (argv[count] == std::string("-h"))          { help(); return 0; }
      else if (argv[count] == std::string("--help"))      { help(); return 0; }
      else if (argv[count] == std::string("-m"))          { compileModule = true; }
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

  std::string moduleName = "<stdin>";
  if (file != "<stdin>") {
    moduleName = file;
    int start = 0;
    int length = moduleName.size();

    // strip path if necessary
    size_t found_slash = moduleName.find_last_of("/");
    if (found_slash < length) start = found_slash + 1;

    // strip and verify file extension
    std::string ext = moduleName.substr(length-3, 3);
    length = length - start - 3;

    if (ext != ".pr") {
      std::cerr << "proty: unknown file extension" << std::endl;
      return 10;
    }

    moduleName = moduleName.substr(start, length);

    if (compileModule && output.empty()) {
      output = moduleName + ".bc";
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

  Compiler* compiler = new Compiler(moduleName);
  compiler->debug = debug;

  try {
    compiler->compile(root);
  }
  catch (Error* e) {
    e->printMessage();
    return 2;
  }

  Program* program = compiler->getProgram(!compileModule);

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
