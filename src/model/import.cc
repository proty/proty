#include "model/models.hh"
#include "parser/parser.hh"

namespace pyrite {
  
  Value* ImportModel::codegen(Compiler* c) {
    std::string file = "lib/" + name + ".pr";

    Tokenizer* t = new Tokenizer(file);

    Parser* p = new Parser(t);
    BlockModel* b = p->parse();
  
    Compiler* c2 = new Compiler(file);
    Module* m = c2->compile(b);
    
    // Remove main function
    m->getFunction("main")->removeFromParent();
    
    c->linker->LinkInModule(m);
    
    return 0;
  }

}