#include "model/models.hh"

namespace pyrite {

  Value* StringModel::codegen(Compiler* c) {
    size_t pos = 0;
    while((pos = value.find("\\n", pos)) != std::string::npos) {
      value.replace(pos, 2, "\n");
      pos += 1;
    }

    Value* string = c->builder->CreateGlobalStringPtr(value.c_str(), ".str");

    Function* func = c->module->getFunction("newstring");
    std::vector<Value*> args;
    args.push_back(string);
    return c->builder->CreateCall(func, args.begin(), args.end(), "stringtmp");
  }

}