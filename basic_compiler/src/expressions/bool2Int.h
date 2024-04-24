#pragma once

#include "../expression.h"

// Expression convert int to bool
class ASTExpressionBool2Int : public ASTExpression
{
    // Use operand to stay consistent
    std::unique_ptr<ASTExpression> operand;

public:

    // Create a new constant bool2Int with operand
    explicit ASTExpressionBool2Int(std::unique_ptr<ASTExpression> operand) : operand(std::move(operand)) {}

    //call create and then return move
    static auto Create(std::unique_ptr<ASTExpression> operand)
    {
        return std::make_unique<ASTExpressionBool2Int>(std::move(operand));
    }

    // Virtual functions. See base class for details.
    std::unique_ptr<VarType> ReturnType(ASTFunction& func) override;
    bool IsLValue(ASTFunction& func) override;
    llvm::Value* Compile(llvm::IRBuilder<>& builder, ASTFunction& func) override;
    std::string ToString(const std::string& prefix) override;
};