#include "float2Int.h"

std::unique_ptr<VarType> ASTExpressionFloat2Int::ReturnType(ASTFunction& func)
{
    return VarTypeSimple::IntType.Copy(); // Of course Float2Int returns an int.
}

bool ASTExpressionFloat2Int::IsLValue(ASTFunction& func)
{
    return false; // Even if converting a variable we need to load from it first to convert its raw value into an int.
}

llvm::Value* ASTExpressionFloat2Int::Compile(llvm::IRBuilder<>& builder, ASTFunction& func)
{
    // Make sure operand is valid float type.
    if (!operand->ReturnType(func)->Equals(&VarTypeSimple::FloatType))
        throw std::runtime_error("ERROR: Expected float operand in float2int but got another type instead!");

    // Finally compile the cast, we must use an R-Value to cast (we can't just use a raw variable).
    return builder.CreateFPToSI(operand->CompileRValue(builder, func), VarTypeSimple::IntType.GetLLVMType(builder.getContext()));
}

std::string ASTExpressionFloat2Int::ToString(const std::string& prefix)
{
    return "float2Int\n" + prefix + "└──" + operand->ToString(prefix + "   ");
}