#pragma once

#include "scopeTable.h"
#include "statement.h"
#include "types/function.h"
#include "varType.h"
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Function.h>
#include <string>
#include <vector>

// Forward declarations.
class AST;

// Parameter typedef for simplicity.
typedef std::tuple<std::unique_ptr<VarType>, std::string> ASTFunctionParameter;

// Function parameters typedef for simplicity.
typedef std::vector<ASTFunctionParameter> ASTFunctionParameters;

// Function type that has a declaration and optional definition.
class ASTFunction
{
private:

    // List of all the parameters.
    std::vector<std::string> parameters;

    // List of all stack variables.
    std::vector<std::string> stackVariables;

    // Function scope table.
    ScopeTable scopeTable;

    // Pointer to the AST.
    AST& ast;

    // Actual Function definition.
    std::unique_ptr<ASTStatement> definition = nullptr;

public:

    // Name of the function.
    std::string name;

    // Function type.
    std::unique_ptr<VarTypeFunction> funcType;

    // Create a new function. Will automatically be added to the AST's scope table.
    // ast: AST to link to. Will be added to its scope table.
    // name: Name of the function to create.
    // returnType: The type of variable the function will return.
    // parameters: Collection of variable types and names to pass to the function call.
    // variadic: If the function is a variadic function.
    ASTFunction(AST& ast, const std::string& name, std::unique_ptr<VarType> returnType, ASTFunctionParameters parameters, bool variadic = false);

    // Add a new stack variable to the function's scope table. Don't add function parameters, those are already added.
    // var: Variable declaration to add to the stack.
    void AddStackVar(ASTFunctionParameter var);

    // Get a variable's type. Note that you should not modify the pointer, or else bad things can happen.
    // name: Name of the variable to fetch.
    // Returns: Returns the type of the variable. If the variable does not exist, an exception is thrown.
    VarType* GetVariableType(const std::string& name);

    // Get a variable's value.
    // name: Name of the variable to fetch.
    // Returns: Returns the value of the variable. If the variable does not exist, an exception is thrown.
    llvm::Value* GetVariableValue(const std::string& name);

    // Set a variable's value. Will cause an exception if the variable does not exist.
    // name: Name of the variable to set.
    // value: New value of the variable.
    void SetVariableValue(const std::string& name, llvm::Value* value);

    // Give the function a definition! This is necessary if the function is not linked to when compiling (as in its our own function creation).
    // definition: Function definition, which is just a statement.
    void Define(std::unique_ptr<ASTStatement> definition);

    // Get a string representation of this function.
    // prefix: the prefix of this node's children. This string has length 3 * the depth of this node.
    std::string ToString(const std::string& prefix);

    // Compile the function, needed during codegen phase to show LLVM our function exists.
    // mod: Module to add the function to.
    // builder: IR builder used to build instructions.
    void Compile(llvm::Module& mod, llvm::IRBuilder<>& builder);
};