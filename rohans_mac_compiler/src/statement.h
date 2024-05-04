#pragma once

#include "varType.h"
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Value.h>

// Forward declarations.
class ASTFunction;

// A statement (like a single expression, collection of expressions, loop, if statement, etc).
class ASTStatement
{
public:

    // Get the return type of the statement.
    // func: Current AST function.
    // Returns: Either a return type if a return statement is definitive, or nullptr if there is none.
    virtual std::unique_ptr<VarType> StatementReturnType(ASTFunction& func) = 0;

    // Compile the code statement.
    // mod: LLVM module that contains the statement.
    // builder: Instruction builder tied to the current LLVM function.
    // func: AST function to compile.
    // Returns: The return value from the statement. IMPORTANT NOTE: This is a *return* value, not just a value from a single expression! This means unless the value is an explicit return value, you should return nullptr!
    virtual void Compile(llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction& func) = 0;

    // Recursively build a string representation of this statement.
    // Prefix: the string to be inserted on the left side of the tree.
    // Returns: a string representation of this statement, in the form of a tree data structure.
    virtual std::string ToString(const std::string& prefix) = 0;

    // Must make the destructor virtual to make the compiler happy.
    virtual ~ASTStatement() = default;
};