#pragma once

#include "../expression.h"

// An expression that stores one expression into another.
class ASTExpressionAssignment: public ASTExpression
{
    // Operands to work with.
    std::unique_ptr<ASTExpression> left;
    std::unique_ptr<ASTExpression> right;

public:

    // Create a new addition expression.
    // left: Value to store an expression into (has to be an L-Value).
    // right: What to store into the value on the left.
    ASTExpressionAssignment(std::unique_ptr<ASTExpression> left, std::unique_ptr<ASTExpression> right) : left(std::move(left)), right(std::move(right)) {}

    // Create a new addition expression.
    // left: Value to store an expression into (has to be an L-Value).
    // right: What to store into the value on the left.
    static auto Create(std::unique_ptr<ASTExpression> left, std::unique_ptr<ASTExpression> right)
    {
        return std::make_unique<ASTExpressionAssignment>(std::move(left), std::move(right));
    }

    // Virtual functions. See base class for details.
    std::unique_ptr<VarType> ReturnType(ASTFunction& func) override;
    bool IsLValue(ASTFunction& func) override;
    llvm::Value* Compile(llvm::IRBuilder<>& builder, ASTFunction& func) override;
    std::string ToString(const std::string& prefix) override;
};