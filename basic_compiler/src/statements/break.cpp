#include "break.h"
#include "../types/simple.h"

std::unique_ptr<VarType> ASTStatementBreak::StatementReturnType(ASTFunction &func)
{
    //return nullptr
    return nullptr;
}

void ASTStatementBreak::Compile(llvm::Module &mod, llvm::IRBuilder<> &builder, ASTFunction &func)
{
    builder.CreateRetVoid();
}

std::string ASTStatementBreak::ToString(const std::string &prefix)
{
    std::string output = "break\n";
    return output;
}