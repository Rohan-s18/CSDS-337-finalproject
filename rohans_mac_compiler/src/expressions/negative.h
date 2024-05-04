#pragma once

#include "../expression.h"

// An expression that causes negation on one value
class ASTExpressionNegation : public ASTExpression
{
    // Operands to work with.
    std::unique_ptr<ASTExpression> a;

    // Return type to cache.
    VarTypeSimple* returnType = nullptr;

public:

    // Create a new negation expression.
    // a: expression of the negation statement.
    ASTExpressionNegation(std::unique_ptr<ASTExpression> a) : a(std::move(a)) {}

    // Create a new negation expression.
    // a: expression of the negation statement.
    static auto Create(std::unique_ptr<ASTExpression> a)
    {
        return std::make_unique<ASTExpressionNegation>(std::move(a));
    }

    // Virtual functions. See base class for details.
    std::unique_ptr<VarType> ReturnType(ASTFunction& func) override;
    bool IsLValue(ASTFunction& func) override;
    llvm::Value* Compile(llvm::IRBuilder<>& builder, ASTFunction& func) override;
    std::string ToString(const std::string& prefix) override;
};