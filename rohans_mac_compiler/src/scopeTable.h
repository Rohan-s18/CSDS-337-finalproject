#pragma once

#include "varType.h"
#include <llvm/IR/Value.h>
#include <map>
#include <string>

// Forward declarations.
struct ASTFunction;

// Table that keeps track of function definitions and variables.
class ScopeTable
{
private:

    // Keep track of variable types.
    std::map<std::string, std::unique_ptr<VarType>> types;

    // Keep track of variable values.
    std::map<std::string, llvm::Value*> values;

public:

    // Add a variable/function to the scope table.
    // name: Name of the variable to add.
    // type: Type of the variable to add.
    // value: Value of the variable to add (can be left null).
    // Returns: If the operation succeeds. If it doesn't, this means another variable with the same name is already present.
    bool AddVariable(const std::string& name, std::unique_ptr<VarType> type, llvm::Value* value = nullptr);

    // Get a variable's type. Note that you should not modify the pointer, or else bad things can happen.
    // name: Name of the variable to fetch.
    // Returns: Returns the type of the variable. Is null if the variable was not found.
    VarType* GetVariableType(const std::string& name);

    // Get a variable's value.
    // name: Name of the variable to fetch.
    // Returns: Returns the value of the variable. Is null if the variable was not found or has a null value.
    llvm::Value* GetVariableValue(const std::string& name);

    // Set a variable's value.
    // name: Name of the variable to set.
    // value: New value of the variable.
    // Returns: If the variable was set (which will only happen if it exists).
    bool SetVariableValue(const std::string& name, llvm::Value* value);

};