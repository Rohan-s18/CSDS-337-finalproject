#include "int2Bool.h"

std::unique_ptr<VarType> ASTExpressionInt2Bool::ReturnType(ASTFunction& func)
{
    return VarTypeSimple::BoolType.Copy(); // Of course Int2Bool returns a bool what else would it.
}

bool ASTExpressionInt2Bool::IsLValue(ASTFunction& func)
{
    return false; // Even if converting a variable we need to load from it first to convert its raw value into a bool.
}

llvm::Value* ASTExpressionInt2Bool::Compile(llvm::IRBuilder<>& builder, ASTFunction& func)
{
    // Make sure operand is valid int type.
    if (!operand->ReturnType(func)->Equals(&VarTypeSimple::IntType))
        throw std::runtime_error("ERROR: Expected integer operand in int2bool but got another type instead!");

    // Finally compile the cast, we must use an R-Value to cast (we can't just use a raw variable).
    // return createICMPNe which uses the defined operand from int2bool.h
    return builder.CreateICmpNE(operand->CompileRValue(builder, func), llvm::ConstantInt::get(VarTypeSimple::IntType.GetLLVMType(builder.getContext()), 0));
}

std::string ASTExpressionInt2Bool::ToString(const std::string& prefix)
{
    return "int2bool\n" + prefix + "└──" + operand->ToString(prefix + "   ");
}