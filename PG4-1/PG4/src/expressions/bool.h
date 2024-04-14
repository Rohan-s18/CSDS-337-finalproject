#pragma once

#include "../expression.h"

// An expression that is a constant bool.
class ASTExpressionBool : public ASTExpression
{
    // Constant bool value.
    bool value;

public:

    // Create a new constant bool expression.
    // val: Constant bool value to create.
    explicit ASTExpressionBool(bool val) : value(val) {}

    // Create a new constant bool expression.
    // val: Constant bool value to create.
    static auto Create(bool val)
    {
        return std::make_unique<ASTExpressionBool>(val);
    }

    // Virtual functions. See base class for details.
    std::unique_ptr<VarType> ReturnType(ASTFunction& func) override;
    bool IsLValue(ASTFunction& func) override;
    llvm::Value* Compile(llvm::IRBuilder<>& builder, ASTFunction& func) override;
    std::string ToString(const std::string& prefix) override;
};