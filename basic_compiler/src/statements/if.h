#pragma once

#include "../expression.h"
#include "../statement.h"

// For an if condition statement.
class ASTStatementIf : public ASTStatement
{

    // Condition to check.
    std::unique_ptr<ASTExpression> condition;

    // Then statement to execute.
    std::unique_ptr<ASTStatement> thenStatement;

    // Else statement to execute.
    std::unique_ptr<ASTStatement> elseStatement;

public:

    // Create a new if statement.
    // condition: Condition to check.
    // thenStatement: Statement to execute if the condition is true.
    // elseStatement: Statement to execute if the condition is false.
    ASTStatementIf(std::unique_ptr<ASTExpression> condition, std::unique_ptr<ASTStatement> thenStatement, std::unique_ptr<ASTStatement> elseStatement) : condition(std::move(condition)), thenStatement(std::move(thenStatement)), elseStatement(std::move(elseStatement)) {}

    // Create a new if statement.
    // condition: Condition to check.
    // thenStatement: Statement to execute if the condition is true.
    // elseStatement: Statement to execute if the condition is false.
    static auto Create(std::unique_ptr<ASTExpression> condition, std::unique_ptr<ASTStatement> thenStatement, std::unique_ptr<ASTStatement> elseStatement)
    {
        return std::make_unique<ASTStatementIf>(std::move(condition), std::move(thenStatement), std::move(elseStatement));
    }

    // Virtual functions. See base class for details.
    virtual std::unique_ptr<VarType> StatementReturnType(ASTFunction& func) override;
    virtual void Compile(llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction& func) override;
    virtual std::string ToString(const std::string& prefix) override;

};