#pragma once

#include "../expression.h"

// An expression that simply converts a float type to an int type.
class ASTExpressionFloat2Int : public ASTExpression
{
    // Operand to work on.
    std::unique_ptr<ASTExpression> operand;

public:

    // Create a float to int conversion.
    // operand: Expression to convert to an int. Make sure it is a float type, or else this will fail.
    explicit ASTExpressionFloat2Int(std::unique_ptr<ASTExpression> operand) : operand(std::move(operand)) {}

    // Create a float to int conversion.
    // operand: Expression to convert to an int. Make sure it is a float type, or else this will fail.
    static auto Create(std::unique_ptr<ASTExpression> operand)
    {
        return std::make_unique<ASTExpressionFloat2Int>(std::move(operand));
    }

    // Virtual functions. See base class for details.
    std::unique_ptr<VarType> ReturnType(ASTFunction& func) override;
    bool IsLValue(ASTFunction& func) override;
    llvm::Value* Compile(llvm::IRBuilder<>& builder, ASTFunction& func) override;
    std::string ToString(const std::string& prefix) override;
};