#include "llvm.hh"
#include "compiler.hh"
#include "symtab.hh"
#include "runtime.hh"

using namespace llvm;

void Runtime::declareTypes(Compiler* c) {
  Type* ObjectTy = OpaqueType::get(getGlobalContext());
  c->module->addTypeName("struct.Object", ObjectTy);

  c->symtab->store("Object", c->declareExternObject("Object_proto"));
  c->symtab->store("Integer", c->declareExternObject("Integer_proto"));
  c->symtab->store("Float", c->declareExternObject("Float_proto"));
  c->symtab->store("String", c->declareExternObject("String_proto"));
  c->symtab->store("Exception", c->declareExternObject("Exception_proto"));
  c->symtab->store("Hash", c->declareExternObject("Hash_proto"));
  c->symtab->store("Function", c->declareExternObject("Function_proto"));
  c->symtab->store("Symbol", c->declareExternObject("Symbol_proto"));
  c->symtab->store("List", c->declareExternObject("List_proto"));
  c->symtab->store("Bool", c->declareExternObject("Bool_proto"));

  c->symtab->store("nil", c->declareExternObject("Qnil"));
  c->symtab->store("true", c->declareExternObject("Qtrue"));
  c->symtab->store("false", c->declareExternObject("Qfalse"));
}

void Runtime::declareFunctions(Compiler* c) {
  FunctionType* funcTy;
  std::vector<const Type*> args;

  // void runtime_init()
  funcTy = FunctionType::get(Type::getVoidTy(getGlobalContext()), false);
  Function::Create(funcTy, Function::ExternalLinkage, "runtime_init", c->module);

  // Object* Object_getSlot(Object*, Object*)
  args.clear();
  args.push_back(c->getObjectTy());
  args.push_back(c->getObjectTy());
  funcTy = FunctionType::get(c->getObjectTy(), args, false);
  Function::Create(funcTy, Function::ExternalLinkage, "Object_getSlot", c->module);

  // Object* Object_setSlot(Object*, Object*, Object*)
  args.clear();
  args.push_back(c->getObjectTy());
  args.push_back(c->getObjectTy());
  args.push_back(c->getObjectTy());
  funcTy = FunctionType::get(c->getObjectTy(), args, false);
  Function::Create(funcTy, Function::ExternalLinkage, "Object_setSlot", c->module);

  // Object* Object_call(Object*, int, ...)
  args.clear();
  args.push_back(c->getObjectTy());
  args.push_back(Type::getInt32Ty(getGlobalContext()));
  funcTy = FunctionType::get(c->getObjectTy(), args, true);
  Function::Create(funcTy, Function::ExternalLinkage, "Object_call", c->module);

  // Object* Object_send(Object*, Object*, int, ...)
  args.clear();
  args.push_back(c->getObjectTy());
  args.push_back(c->getObjectTy());
  args.push_back(Type::getInt32Ty(getGlobalContext()));
  funcTy = FunctionType::get(c->getObjectTy(), args, true);
  Function::Create(funcTy, Function::ExternalLinkage, "Object_send", c->module);

  // Object* Integer_new(int)
  args.clear();
  args.push_back(Type::getInt32Ty(getGlobalContext()));
  funcTy = FunctionType::get(c->getObjectTy(), args, false);
  Function::Create(funcTy, Function::ExternalLinkage, "Integer_new", c->module);

  // Object* Float_new(double)
  args.clear();
  args.push_back(Type::getDoubleTy(getGlobalContext()));
  funcTy = FunctionType::get(c->getObjectTy(), args, false);
  Function::Create(funcTy, Function::ExternalLinkage, "Float_new", c->module);

  // Object* Function_new(FuncPtr, int)
  std::vector<const Type*> funcPtrArgTypes;
  funcPtrArgTypes.push_back(c->getObjectTy());
  FunctionType* funcPtrTy = FunctionType::get(c->getObjectTy(), funcPtrArgTypes, true);

  args.clear();
  args.push_back(funcPtrTy->getPointerTo());
  args.push_back(Type::getInt32Ty(getGlobalContext()));
  funcTy = FunctionType::get(c->getObjectTy(), args, false);
  Function::Create(funcTy, Function::ExternalLinkage, "Function_new", c->module);

  // Object* Symbol_get(const char*)
  args.clear();
  args.push_back(Type::getInt8PtrTy(getGlobalContext()));
  funcTy = FunctionType::get(c->getObjectTy(), args, false);
  Function::Create(funcTy, Function::ExternalLinkage, "Symbol_get", c->module);

  // Object* String_new(const char*)
  args.clear();
  args.push_back(Type::getInt8PtrTy(getGlobalContext()));
  funcTy = FunctionType::get(c->getObjectTy(), args, false);
  Function::Create(funcTy, Function::ExternalLinkage, "String_new", c->module);

  // Object* List_new()
  funcTy = FunctionType::get(c->getObjectTy(), false);
  Function::Create(funcTy, Function::ExternalLinkage, "List_new", c->module);

  // Object* List_append(Object*, Object*)
  args.clear();
  args.push_back(c->getObjectTy());
  args.push_back(c->getObjectTy());
  funcTy = FunctionType::get(c->getObjectTy(), args, false);
  Function::Create(funcTy, Function::ExternalLinkage, "List_append", c->module);

  // Object* Hash_new()
  funcTy = FunctionType::get(c->getObjectTy(), false);
  Function::Create(funcTy, Function::ExternalLinkage, "Hash_new", c->module);

  // Object* Hash_set(Object*, Object*, Object*)
  args.clear();
  args.push_back(c->getObjectTy());
  args.push_back(c->getObjectTy());
  args.push_back(c->getObjectTy());
  funcTy = FunctionType::get(c->getObjectTy(), args, false);
  Function::Create(funcTy, Function::ExternalLinkage, "Hash_set", c->module);

  // exception handling declarations
  getDeclaration(c->module, llvm::Intrinsic::eh_exception);
  getDeclaration(c->module, llvm::Intrinsic::eh_selector);

  // debugging functions
  if (c->debug) {
    getDeclaration(c->module, llvm::Intrinsic::dbg_declare);
  }
}
