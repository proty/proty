#include "model/models.hh"

namespace pyrite {

  Value* BinOpModel::codegen(Compiler* c) {
    Value* l = lhs->codegen(c);
    Value* r = rhs->codegen(c);
    if (l->getType()->isIntegerTy() || r->getType()->isIntegerTy()) {

      if (!(l->getType()->isIntegerTy() && r->getType()->isIntegerTy())) {
        throw "Both binary operators have to be either primitive or complex.";
      }

      if (op == "+")        return c->builder->CreateAdd(l, r, "addtmp");
      else if (op == "-")   return c->builder->CreateSub(l, r, "subtmp");
      else if (op == "*")   return c->builder->CreateMul(l, r, "multmp");
      else if (op == "/")   return c->builder->CreateSDiv(l, r, "divtmp");
      else if (op == "==")  return c->builder->CreateICmpEQ(l, r, "eqtmp");
      else if (op == "!=")  return c->builder->CreateICmpNE(l, r, "netmp");
      else if (op == ">")   return c->builder->CreateICmpSGT(l, r, "gttmp");
      else if (op == ">=")  return c->builder->CreateICmpSGE(l, r, "getmp");
      else if (op == "<")   return c->builder->CreateICmpSLT(l, r, "lttmp");
      else if (op == "<=")  return c->builder->CreateICmpSLE(l, r, "letmp");
      else if (op == "or")  return c->builder->CreateOr(l, r, "ortmp");
      else if (op == "and") return c->builder->CreateAnd(l, r, "andtmp");
      else return 0;
    }
    else {
      CallArgsModel* args = new CallArgsModel();
      args->push(rhs);
      MethodCallModel* call = new MethodCallModel(lhs, op, args);
      return call->codegen(c);
    }
  }
  
}
