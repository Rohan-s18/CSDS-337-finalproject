#pragma once

#include "../statement.h"

// Sometimes we need more than a single statement, like a code block. This is the purpose of this.
class ASTStatementBlock : public ASTStatement
{
public:

    // List of statements. Modify these as needed :}
    std::vector<std::unique_ptr<ASTStatement>> statements;

    // Virtual functions. See base class for details.
    std::unique_ptr<VarType> StatementReturnType(ASTFunction& func) override;
    void Compile(llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction& func) override;
    std::string ToString(const std::string& prefix) override;

};