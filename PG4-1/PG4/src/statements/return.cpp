#include "return.h"

#include "../types/simple.h"

std::unique_ptr<VarType> ASTStatementReturn::StatementReturnType(ASTFunction& func)
{
    // If the contained statement returns something, return it.
    // Otherwise, we return void.
    return returnExpression ? returnExpression->ReturnType(func) : VarTypeSimple::VoidType.Copy();
}

void ASTStatementReturn::Compile(llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction& func)
{
    // If there is a contained expression, compile it.
    if (returnExpression)
    {
        returnExpression->Compile(mod, builder, func);
        if (returnExpression->ReturnType(func)->Equals(&VarTypeSimple::VoidType))
            throw std::runtime_error("ERROR: Illegal return of void type");
        builder.CreateRet(returnExpression->CompileRValue(builder, func));
    } else // If no contained expression exists, make a void return.
        builder.CreateRetVoid();
}

std::string ASTStatementReturn::ToString(const std::string& prefix)
{
    std::string output = "return\n";
    if (returnExpression)
        output += prefix + "└──" + returnExpression->ToString(prefix + "   ");
    return output;
}