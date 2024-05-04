#pragma once

#include "../statement.h"
#include "../expression.h"

class ASTStatementReturn : public ASTStatement
{
public:

    // expression to return
    std::unique_ptr<ASTExpression> returnExpression;

    // Virtual functions. See base class for details.
    std::unique_ptr<VarType> StatementReturnType(ASTFunction& func) override;
    void Compile(llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction& func) override;
    std::string ToString(const std::string& prefix) override;
};