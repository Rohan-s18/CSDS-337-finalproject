#pragma once

#include "../expression.h"
#include "../statement.h"

// modifying whie for For
class ASTStatementFor : public ASTStatement
{
    // body
    std::unique_ptr<ASTStatement> bodyStatement;
    // init statement
    std::unique_ptr<ASTStatement> initStatement;
    // Condition to check.
    std::unique_ptr<ASTExpression> condition;
    // increment statement
    std::unique_ptr<ASTStatement> incrementStatement;

public:

    // Create a new for statement.
    // condition: Condition to check.
    // thenStatement: Statement to execute while the condition is true.
    ASTStatementFor(
        std::unique_ptr<ASTStatement> bodyStatement, std::unique_ptr<ASTStatement> initStatement, 
        std::unique_ptr<ASTExpression> condition, std::unique_ptr<ASTStatement> incrementStatement) : 
        bodyStatement(std::move(bodyStatement)), initStatement(std::move(initStatement)), condition(std::move(condition)), incrementStatement(std::move(incrementStatement))

    {
    }

    // Create a new if statement.
    // condition: Condition to check.
    // thenStatement: Statement to execute while the condition is true.
    static auto Create(std::unique_ptr<ASTStatement> bodyStatement, std::unique_ptr<ASTStatement> initStatement,
    std::unique_ptr<ASTExpression> condition, std::unique_ptr<ASTStatement> incrementStatement)
    {
        return std::make_unique<ASTStatementFor>(std::move(bodyStatement), std::move(initStatement), 
        std::move(condition), std::move(incrementStatement));
    }

    // Virtual functions. See base class for details.
    virtual std::unique_ptr<VarType> StatementReturnType(ASTFunction& func) override;
    virtual void Compile(llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction& func) override;
    virtual std::string ToString(const std::string& prefix) override;

};