#include "int.h"

std::unique_ptr<VarType> ASTExpressionInt::ReturnType(ASTFunction& func)
{
    return VarTypeSimple::IntType.Copy(); // Of course we are returning an int, what else would it be.
}

bool ASTExpressionInt::IsLValue(ASTFunction& func)
{
    return false; // It's a constant, of course it's not an L-Value.
}

llvm::Value* ASTExpressionInt::Compile(llvm::IRBuilder<>& builder, ASTFunction& func)
{
    return llvm::ConstantInt::get(VarTypeSimple::IntType.GetLLVMType(builder.getContext()), value); // Simply just create an int constant to return.
}

std::string ASTExpressionInt::ToString(const std::string& prefix)
{
    return std::to_string(value) + "\n";
}