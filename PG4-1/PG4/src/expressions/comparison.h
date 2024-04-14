#pragma once

#include "../expression.h"

// Comparison types.
enum ASTExpressionComparisonType
{
    Equal,
    NotEqual,
    LessThan,
    LessThanOrEqual,
    GreaterThan,
    GreaterThanOrEqual
};

// An expression that is a comparison.
class ASTExpressionComparison : public ASTExpression
{
    // Type of comparison to do.
    ASTExpressionComparisonType type;

    // Operands to work with.
    std::unique_ptr<ASTExpression> a1;
    std::unique_ptr<ASTExpression> a2;

public:

    // Create a new comparison expression.
    // type: Type of comparison to do.
    // a1: Left operand.
    // a2: Right operand.
    ASTExpressionComparison(ASTExpressionComparisonType type, std::unique_ptr<ASTExpression> a1, std::unique_ptr<ASTExpression> a2) : type(type), a1(std::move(a1)), a2(std::move(a2)) {}

    // Create a new comparison expression.
    // type: Type of comparison to do.
    // a1: Left operand.
    // a2: Right operand.
    static auto Create(ASTExpressionComparisonType type, std::unique_ptr<ASTExpression> a1, std::unique_ptr<ASTExpression> a2)
    {
        return std::make_unique<ASTExpressionComparison>(type, std::move(a1), std::move(a2));
    }

    // Virtual functions. See base class for details.
    std::unique_ptr<VarType> ReturnType(ASTFunction& func) override;
    bool IsLValue(ASTFunction& func) override;
    llvm::Value* Compile(llvm::IRBuilder<>& builder, ASTFunction& func) override;
    std::string ToString(const std::string& prefix) override;
};