#include <iostream>
#include "parser/parser.hh"
#include "compiler/compiler.hh"
#include "version.hh"

void version() {
  std::cout << "Pyrite " PYRITE_VERSION " (" __DATE__ ", " __TIME__ ")" << std::endl;
  std::cout << PYRITE_COPYRIGHT << std::endl;
}

void help() {
  std::cerr << 
  "Usage: pyrite [options ...] [file] [arguments]\n"
  "-d           Dump code\n"
  "--help, -h   Shows this message\n"
  "-i           Interpret the genereted code\n"
  "-c [file]    Compile to file\n"
  "--version    Version and copyright info\n"
  "--           Stop reading options\n";
}

int main(int argc, char** argv) {
  using namespace pyrite;

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

  Tokenizer* t;
  try {
    t = new Tokenizer(file);
  }
  catch (std::string s) {
    std::cout << "pyrite: error: " << s << std::endl;
    return 0;
  }

  Parser* p = new Parser(t);
  ProgramModel* pm = p->parse();

  Compiler* c = new Compiler(file);
  Module* m = c->compile(pm);

  if (dump) m->dump();

  return 0;
}