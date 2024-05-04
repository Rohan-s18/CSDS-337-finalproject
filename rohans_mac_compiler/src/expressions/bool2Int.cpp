#include "bool2Int.h"

std::unique_ptr<VarType> ASTExpressionBool2Int::ReturnType(ASTFunction& func)
{
    return VarTypeSimple::IntType.Copy(); // Of course Bool2Int returns an int.
}

bool ASTExpressionBool2Int::IsLValue(ASTFunction& func)
{
    return false; // Even if converting a variable we need to load from it first to convert its raw value into an int.
}

llvm::Value* ASTExpressionBool2Int::Compile(llvm::IRBuilder<>& builder, ASTFunction& func)
{
    // Make sure operand is valid bool type.
    if (!operand->ReturnType(func)->Equals(&VarTypeSimple::BoolType))
        throw std::runtime_error("ERROR: Expected bool operand in bool2int but got another type instead!");

    // Finally compile the cast, we must use an R-Value to cast (we can't just use a raw variable).
    return builder.CreateFPToSI(operand->CompileRValue(builder, func), VarTypeSimple::IntType.GetLLVMType(builder.getContext()));
}

std::string ASTExpressionBool2Int::ToString(const std::string& prefix)
{
    return "bool2Int\n" + prefix + "└──" + operand->ToString(prefix + "   ");
}