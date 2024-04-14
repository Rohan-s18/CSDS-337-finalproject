#include "float.h"

std::unique_ptr<VarType> ASTExpressionFloat::ReturnType(ASTFunction& func)
{
    return VarTypeSimple::FloatType.Copy(); // Of course we are returning an Float, what else would it be.
}

bool ASTExpressionFloat::IsLValue(ASTFunction& func)
{
    return false; // It's a constant, of course it's not an L-Value.
}

llvm::Value* ASTExpressionFloat::Compile(llvm::IRBuilder<>& builder, ASTFunction& func)
{
    return llvm::ConstantFP::get(VarTypeSimple::FloatType.GetLLVMType(builder.getContext()), value); // Simply just create an float constant to return.
}

std::string ASTExpressionFloat::ToString(const std::string& prefix)
{
    return std::to_string(value) + "\n";
}