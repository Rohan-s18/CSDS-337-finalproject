#pragma once

#include "../expression.h"

// An expression that is a constant int.
class ASTExpressionInt : public ASTExpression
{
    // Constant int value.
    int value;

public:

    // Create a new constant int expression.
    // val: Constant int value to create.
    explicit ASTExpressionInt(int val) : value(val) {}

    // Create a new constant int expression.
    // val: Constant int value to create.
    static auto Create(int val)
    {
        return std::make_unique<ASTExpressionInt>(val);
    }

    // Virtual functions. See base class for details.
    std::unique_ptr<VarType> ReturnType(ASTFunction& func) override;
    bool IsLValue(ASTFunction& func) override;
    llvm::Value* Compile(llvm::IRBuilder<>& builder, ASTFunction& func) override;
    std::string ToString(const std::string& prefix) override;
};