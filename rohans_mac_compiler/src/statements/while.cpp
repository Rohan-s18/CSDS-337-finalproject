#include "while.h"

#include "../function.h"

std::unique_ptr<VarType> ASTStatementWhile::StatementReturnType(ASTFunction& func)
{

    // It is completely possible for a while's condition to never be true, so even if does return something it's not confirmed.
    return nullptr;

}

void ASTStatementWhile::Compile(llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction& func)
{

    /*

        A while loop can be desugared to basic blocks. Take the following example:

            while (condition)
            {
                doThing();
            }

        This is really just another way of saying:

            whileLoop:
                if (condition) goto whileLoopBody else goto whileLoopEnd;

            whileLoopBody:
                doThing();
                goto whileLoop;

            whileLoopEnd:
                ...

    */

    // Create the basic blocks.
    auto* funcVal = (llvm::Function*)func.GetVariableValue(func.name);
    auto whileLoop = llvm::BasicBlock::Create(builder.getContext(), "whileLoop", funcVal);
    auto whileLoopBody = llvm::BasicBlock::Create(builder.getContext(), "whileLoopBody", funcVal);
    auto whileLoopEnd = llvm::BasicBlock::Create(builder.getContext(), "whileLoopEnd", funcVal);

    // Jump to the while loop.
    builder.CreateBr(whileLoop);

    // Compile condition and jump to the right block.
    builder.SetInsertPoint(whileLoop);
    auto conditionVal = condition->CompileRValue(builder, func);
    builder.CreateCondBr(conditionVal, whileLoopBody, whileLoopEnd);

    // Compile the body. Note that we need to not create a jump if there is a return.
    builder.SetInsertPoint(whileLoopBody);
    thenStatement->Compile(mod, builder, func);
    if (!thenStatement->StatementReturnType(func)) builder.CreateBr(whileLoop);

    // Continue from the end of the created while loop.
    builder.SetInsertPoint(whileLoopEnd);

}

std::string ASTStatementWhile::ToString(const std::string& prefix)
{
    std::string output = "while\n";
    output += prefix + "├──" + condition->ToString(prefix + "│  ");
    output += prefix + "└──" + thenStatement->ToString(prefix + "   ");
    return output;
}