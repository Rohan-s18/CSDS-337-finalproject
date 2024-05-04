#pragma once

#include "../expression.h"
#include <vector>

// Call a function.
class ASTExpressionCall : public ASTExpression
{

    // Value to call.
    std::unique_ptr<ASTExpression> callee;

    // Values to pass to the call.
    std::vector<std::unique_ptr<ASTExpression>> arguments;

public:

    // Create a new call expression.
    // callee: The expression to call.
    // arguments: Arguments to pass to the call.
    ASTExpressionCall(std::unique_ptr<ASTExpression> callee, std::vector<std::unique_ptr<ASTExpression>> arguments) : callee(std::move(callee)), arguments(std::move(arguments)) {}

    // Create a new call expression.
    // callee: The expression to call.
    // arguments: Arguments to pass to the call.
    static auto Create(std::unique_ptr<ASTExpression> callee, std::vector<std::unique_ptr<ASTExpression>> arguments)
    {
        return std::make_unique<ASTExpressionCall>(std::move(callee), std::move(arguments));
    }

    // Virtual functions. See base class for details.
    std::unique_ptr<VarType> ReturnType(ASTFunction& func) override;
    bool IsLValue(ASTFunction& func) override;
    llvm::Value* Compile(llvm::IRBuilder<>& builder, ASTFunction& func) override;
    std::string ToString(const std::string& prefix) override;

};