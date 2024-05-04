// #include "break.h"
// #include "../types/simple.h"

// std::unique_ptr<VarType> ASTStatementBreak::StatementReturnType(ASTFunction &func)
// {
//     //return nullptr
//     return nullptr;
// }

// void ASTStatementBreak::Compile(llvm::Module &mod, llvm::IRBuilder<> &builder, ASTFunction &func)
// {
//     builder.CreateRetVoid();
// }

// std::string ASTStatementBreak::ToString(const std::string &prefix)
// {
//     std::string output = "break\n";
//     return output;
// }

#include "break.h"
#include "../function.h"
#include <stack>

void ASTStatementBreak::Compile(llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction& func)
{
    // Get the top exit block from the stack
    if (!func.loopExitBlocks.empty()) {
        llvm::BasicBlock* loopExitBlock = func.loopExitBlocks.top();
        builder.CreateBr(loopExitBlock);
    } else {
        throw std::runtime_error("ERROR: 'break' statement used outside of loop");
    }
}

std::string ASTStatementBreak::ToString(const std::string& prefix)
{
    return "break";
}
