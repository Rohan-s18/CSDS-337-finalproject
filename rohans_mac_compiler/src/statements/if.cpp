#include "if.h"

#include "../function.h"

std::unique_ptr<VarType> ASTStatementIf::StatementReturnType(ASTFunction& func)
{
    // This is a bit of a strange case. We don't know for certain what the return type is unless both if and else return something.
    // In the case they both return something, have to make sure the return types match.

    // If we don't have an else statement, then we can't guarantee a return.
    if (!elseStatement) return nullptr;

    // Get return types. Return if either do not return anything.
    auto thenRet = thenStatement->StatementReturnType(func);
    auto elseRet = elseStatement->StatementReturnType(func);
    if (!thenRet || !elseRet) return nullptr;

    // Check for matching return types.
    if (thenRet->Equals(elseRet.get())) return std::move(thenRet); // Return if equal.
    else throw std::runtime_error("ERROR: If/Else statements both return a value but their return types don't match!");
}

void ASTStatementIf::Compile(llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction& func)
{
    // Compile the condition. TODO: TO BOOLEAN CAST CONVERSION?
    if (!condition->ReturnType(func)->Equals(&VarTypeSimple::BoolType))
        throw std::runtime_error("ERROR: Expected condition that returns a boolean value but got another type instead!");
    llvm::Value* cond = condition->Compile(builder, func);

    // Create blocks.
    auto* funcVal = (llvm::Function*)func.GetVariableValue(func.name);
    llvm::BasicBlock* thenBlock = llvm::BasicBlock::Create(builder.getContext(), "thenBlock", funcVal);
    llvm::BasicBlock* elseBlock = nullptr;
    if (elseStatement) elseBlock = llvm::BasicBlock::Create(builder.getContext(), "elseBlock", funcVal);
    llvm::BasicBlock* contBlock = llvm::BasicBlock::Create(builder.getContext(), "contBlock", funcVal);

    // Make jumps to blocks.
    builder.CreateCondBr(cond, thenBlock, elseBlock ? elseBlock : contBlock); // Use else as false if exists, otherwise go to continuation.

    // Compile the then block and then jump to continuation block.
    builder.SetInsertPoint(thenBlock);
    thenStatement->Compile(mod, builder, func);
    if (!thenStatement->StatementReturnType(func)) builder.CreateBr(contBlock); // Only create branch if no return encountered.

    // Compile the else block if applicable.
    if (elseBlock)
    {
        builder.SetInsertPoint(elseBlock);
        elseStatement->Compile(mod, builder, func);
        if (!elseStatement->StatementReturnType(func)) builder.CreateBr(contBlock); // Only create branch if no return encountered.
    }

    // Resume compilation at continuation block.
    builder.SetInsertPoint(contBlock);
}

std::string ASTStatementIf::ToString(const std::string& prefix)
{
    std::string output = "if " + condition->ToString("") + "\n";
    output += prefix + (thenStatement ? "├──" : "└──") + thenStatement->ToString(prefix + "   ");
    if (elseStatement) output += prefix + (elseStatement ? "├──" : "└──") + elseStatement->ToString(prefix + "   ");
    return output;
}