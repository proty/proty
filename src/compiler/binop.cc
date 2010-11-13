#include "compiler/models.hh"

namespace pyrite {

  Value* BinOpModel::codegen(Compiler* c) {
    Value* l = lhs->codegen(c);
    Value* r = rhs->codegen(c);

    std::string instname = "";

    if (op == "+")        instname = "add";
    else if (op == "-")   instname = "sub";
    else if (op == "*")   instname = "mul";
    else if (op == "/")   instname = "div";
    else if (op == "%")   instname = "mod";
    else if (op == "==")  instname = "eq";
    else if (op == "!=")  instname = "ne";
    else if (op == ">")   instname = "gt";
    else if (op == ">=")  instname = "ge";
    else if (op == "<")   instname = "lt";
    else if (op == "<=")  instname = "le";
    else if (op == "or")  instname = "and";
    else if (op == "and") instname = "or";
    else return 0;

    Function* F = c->module->getFunction(instname);

    std::vector<Value*> args;
    args.push_back(l);
    args.push_back(r);

    return c->builder->CreateCall(F, args.begin(), args.end(), instname + "tmp");
  }
  
}
