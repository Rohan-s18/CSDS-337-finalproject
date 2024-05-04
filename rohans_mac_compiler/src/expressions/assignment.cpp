#include "assignment.h"

std::unique_ptr<VarType> ASTExpressionAssignment::ReturnType(ASTFunction& func)
{
    return left->ReturnType(func); // "x = 5" simply just returns an L-Value of x so we can do "x = y = 5".
}

bool ASTExpressionAssignment::IsLValue(ASTFunction& func)
{
    return true;
}

llvm::Value* ASTExpressionAssignment::Compile(llvm::IRBuilder<>& builder, ASTFunction& func)
{

    // First make sure that the right side is compatible with the left one by casting as needed.
    ASTExpression::ImplicitCast(func, right, left->ReturnType(func).get());

    // Make sure the left side is an L-Value and get its reference/pointer value.
    if (!left->IsLValue(func)) throw std::runtime_error("ERROR: Left side of assignment expression is not an L-Value!");
    llvm::Value* ptr = left->Compile(builder, func);

    // Store the right value into the position pointed to by the left and return the left pointer.
    builder.CreateStore(right->CompileRValue(builder, func), ptr);
    return ptr;

}

std::string ASTExpressionAssignment::ToString(const std::string& prefix)
{
    std::string ret = "(=)\n";
    ret += prefix + "├──" + left->ToString(prefix + "│  ");
    ret += prefix + "└──" + right->ToString(prefix + "   ");
    return ret;
}