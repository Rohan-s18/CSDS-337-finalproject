#include "and.h"
#include "../function.h"
//modifiying or.cpp

std::unique_ptr<VarType> ASTExpressionAnd::ReturnType(ASTFunction& func)
{
    return VarTypeSimple::BoolType.Copy(); // a && b is always a boolean.
}

bool ASTExpressionAnd::IsLValue(ASTFunction& func)
{
    return false; // && operator works on two R-Values to produce an R-Value.
}

llvm::Value *ASTExpressionAnd::Compile(llvm::IRBuilder<>& builder, ASTFunction& func) // changed value* AST... to value *AST
{

    // Make sure to cast both sides as booleans first.
    ASTExpression::ImplicitCast(func, a1, &VarTypeSimple::BoolType);
    ASTExpression::ImplicitCast(func, a2, &VarTypeSimple::BoolType);

    // Create blocks. Check right is if left is false, and we need to check the right one too. Continue block happens if true.
    auto *funcVal = (llvm::Function*)func.GetVariableValue(func.name);
    llvm::BasicBlock *checkRight = llvm::BasicBlock::Create(builder.getContext(), "checkRight", funcVal);
    llvm::BasicBlock *cont = llvm::BasicBlock::Create(builder.getContext(), "cont", funcVal);

    // If left is true, not done. Just branch to the continue block where we know the final result will be set as true.
    llvm::Value *leftVal = a1->CompileRValue(builder, func);
    llvm::BasicBlock *lastBlockLeft = builder.GetInsertBlock(); // Get the current block we are on.
    builder.CreateCondBr(leftVal, checkRight, cont); // have to evaluate right as well before continuing

    // Compile the right expression if needed.
    builder.SetInsertPoint(checkRight);
    llvm::Value *rightVal = a2->CompileRValue(builder, func);
    llvm::BasicBlock *lastBlockRight = builder.GetInsertBlock(); // In case the block has changed, fix it.

    // Tell LLVM that it should either select the left value or the right one depending on where we came from.
    builder.SetInsertPoint(cont);
    llvm::PHINode* res = builder.CreatePHI(VarTypeSimple::BoolType.GetLLVMType(builder.getContext()), 2);
    res->addIncoming(leftVal, lastBlockLeft);
    res->addIncoming(rightVal, lastBlockRight);
    return res;

}

std::string ASTExpressionAnd::ToString(const std::string& prefix)
{
    //change symbol
    std::string ret = "(&&)\n";
    ret += prefix + "├──" + a1->ToString(prefix + "│  ");
    ret += prefix + "└──" + a2->ToString(prefix + "   ");
    return ret;
}