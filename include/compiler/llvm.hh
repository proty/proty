#ifndef PYRITE_Lllvm_HH
#define PYRITE_Lllvm_HH

#include <llvm/DerivedTypes.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/JIT.h>
#include <llvm/LLVMContext.h>
#include <llvm/Linker.h>
#include <llvm/Module.h>
#include <llvm/PassManager.h>
#include <llvm/Analysis/Verifier.h>
#include <llvm/Target/TargetData.h>
#include <llvm/Target/TargetSelect.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Support/IRBuilder.h>
#include <llvm/System/Path.h>
#include <llvm/ValueSymbolTable.h>
using namespace llvm;

#endif
