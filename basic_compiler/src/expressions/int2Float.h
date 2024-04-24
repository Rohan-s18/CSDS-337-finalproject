#pragma once

#include "../expression.h"

// An expression that simply converts an int type to a floating type.
class ASTExpressionInt2Float : public ASTExpression
{
    // Operand to work on.
    std::unique_ptr<ASTExpression> operand;

public:

    // Create a new integer to float conversion.
    // operand: Expression to convert to a float. Make sure it is an int type, or else this will fail.
    explicit ASTExpressionInt2Float(std::unique_ptr<ASTExpression> operand) : operand(std::move(operand)) {}

    // Create a new integer to float conversion.
    // operand: Expression to convert to a float. Make sure it is an int type, or else this will fail.
    static auto Create(std::unique_ptr<ASTExpression> operand)
    {
        return std::make_unique<ASTExpressionInt2Float>(std::move(operand));
    }

    // Virtual functions. See base class for details.
    std::unique_ptr<VarType> ReturnType(ASTFunction& func) override;
    bool IsLValue(ASTFunction& func) override;
    llvm::Value* Compile(llvm::IRBuilder<>& builder, ASTFunction& func) override;
    std::string ToString(const std::string& prefix) override;
};