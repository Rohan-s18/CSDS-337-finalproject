#include "for.h"
#include "break.h"

#include "../function.h"
//modifying while
std::unique_ptr<VarType> ASTStatementFor::StatementReturnType(ASTFunction &func)
{

    // It is completely possible for a for's condition to never be true, so even if does return something it's not confirmed.
    return nullptr;
}

void ASTStatementFor::Compile(llvm::Module &mod, llvm::IRBuilder<> &builder, ASTFunction &func)
{
    auto *funcVal = (llvm::Function *)func.GetVariableValue(func.name);
    auto forLoop = llvm::BasicBlock::Create(builder.getContext(), "forLoop", funcVal);
    auto forLoopBody = llvm::BasicBlock::Create(builder.getContext(), "forLoopBody", funcVal);
    auto forLoopEnd = llvm::BasicBlock::Create(builder.getContext(), "forLoopEnd", funcVal);

    initStatement->Compile(mod, builder, func);
    
    // Entry Block
    builder.CreateBr(forLoop);

    // Start of the loop
    builder.SetInsertPoint(forLoop);

    auto conditionVal = condition->CompileRValue(builder, func);
    builder.CreateCondBr(conditionVal, forLoopBody, forLoopEnd);

    builder.SetInsertPoint(forLoopBody);
    bodyStatement->Compile(mod, builder, func);


    incrementStatement->Compile(mod, builder, func);
    if (!bodyStatement->StatementReturnType(func))
        builder.CreateBr(forLoop);

    // Check for break statement
    // if (auto *breakStmt = dynamic_cast<ASTStatementBreak*>(bodyStatement.get())) {
    //     // If break statement encountered, jump to loop end
    //     builder.CreateBr(forLoopEnd);
    // } else {
    //     // Increment loop variable
    //     incrementStatement->Compile(mod, builder, func);
    //     // Jump back to loop start
    //     builder.CreateBr(forLoop);
    // }

    // Continue from the end of the created for loop.
    builder.SetInsertPoint(forLoopEnd);
}

std::string ASTStatementFor::ToString(const std::string &prefix)
{
    std::string output = "for\n";
    output += prefix + "├──" + initStatement->ToString(prefix + "│  ");
    output += prefix + "├──" + condition->ToString(prefix + "│  ");
    output += prefix + "├──" + incrementStatement->ToString(prefix + "│  ");
    output += prefix + "└──" + bodyStatement->ToString(prefix + "   ");
    return output;
}