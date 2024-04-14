#pragma once

#include "../expression.h"

// An expression that is a constant float.
class ASTExpressionFloat : public ASTExpression
{
    // Constant float value.
    double value;

public:

    // Create a new constant float expression.
    // val: Constant float value to create.
    explicit ASTExpressionFloat(double val) : value(val) {}

    // Create a new constant float expression.
    // val: Constant float value to create.
    static auto Create(double val)
    {
        return std::make_unique<ASTExpressionFloat>(val);
    }

    // Virtual functions. See base class for details.
    std::unique_ptr<VarType> ReturnType(ASTFunction& func) override;
    bool IsLValue(ASTFunction& func) override;
    llvm::Value* Compile(llvm::IRBuilder<>& builder, ASTFunction& func) override;
    std::string ToString(const std::string& prefix) override;
};