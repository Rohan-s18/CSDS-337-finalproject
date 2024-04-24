#include "int2Float.h"

std::unique_ptr<VarType> ASTExpressionInt2Float::ReturnType(ASTFunction& func)
{
    return VarTypeSimple::FloatType.Copy(); // Of course Int2Float returns a float what else would it.
}

bool ASTExpressionInt2Float::IsLValue(ASTFunction& func)
{
    return false; // Even if converting a variable we need to load from it first to convert its raw value into a float.
}

llvm::Value* ASTExpressionInt2Float::Compile(llvm::IRBuilder<>& builder, ASTFunction& func)
{
    // Make sure operand is valid int type.
    if (!operand->ReturnType(func)->Equals(&VarTypeSimple::IntType))
        throw std::runtime_error("ERROR: Expected integer operand in int2float but got another type instead!");

    // Finally compile the cast, we must use an R-Value to cast (we can't just use a raw variable).
    return builder.CreateSIToFP(operand->CompileRValue(builder, func), VarTypeSimple::FloatType.GetLLVMType(builder.getContext()));
}

std::string ASTExpressionInt2Float::ToString(const std::string& prefix)
{
    return "int2Float\n" + prefix + "└──" + operand->ToString(prefix + "   ");
}