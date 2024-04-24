/*
Author: Rohan Singh, Benjamin Luo and William Cankar
QBASIC Abstruct Syntax Tree
*/
#pragma once
#include "ast.hpp"

namespace qbc{

llvm::Value * getnull();

llvm::Value * getconstint(int v);

llvm::Value * getconstfalse();
llvm::Value * getconsttrue();

llvm::Value * getconstlong(long v);

llvm::FunctionCallee getbuiltinprotype(ASTContext ctx, const std::string name);

llvm::Type * getbooltype();
llvm::Type * getplatformlongtype();

extern llvm::LLVMContext getGlobalContext;

}
