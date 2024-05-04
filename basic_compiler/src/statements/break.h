// #pragma once

// #include "../statement.h"
// #include "../expression.h"
// //make something to be called when break
// class ASTStatementBreak : public ASTStatement
// {
// public:
//     // Virtual functions. See base class for details.
//     std::unique_ptr<VarType> StatementReturnType(ASTFunction &func) override;
//     void Compile(llvm::Module &mod, llvm::IRBuilder<> &builder, ASTFunction &func) override;
//     std::string ToString(const std::string &prefix) override;
// };

#pragma once

#include "../statement.h"

class ASTStatementBreak : public ASTStatement
{
public:
    void Compile(llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction& func) override;
    std::string ToString(const std::string& prefix) override;
};
