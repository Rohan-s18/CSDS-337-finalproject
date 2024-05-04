#include "scopeTable.h"

bool ScopeTable::AddVariable(const std::string& name, std::unique_ptr<VarType> type, llvm::Value* value)
{
    if (types.find(name) == types.end()) // This is true if name doesn't exist in map.
    {
        types[name] = std::move(type);
        values[name] = value;
        return true; // We added a variable as expected.
    }
    else return false; // Variable already exists!
}

VarType* ScopeTable::GetVariableType(const std::string& name)
{
    auto foundType = types.find(name);
    if (foundType == types.end()) return nullptr; // Variable doesn't exist, return null.
    else return foundType->second.get(); // Variable exists, return pointer to type.
}

llvm::Value* ScopeTable::GetVariableValue(const std::string& name)
{
    auto foundType = values.find(name);
    if (foundType == values.end()) return nullptr; // Variable doesn't exist, return null.
    else return foundType->second; // Variable exists, return value.
}

bool ScopeTable::SetVariableValue(const std::string& name, llvm::Value* value)
{
    auto foundType = values.find(name);
    if (foundType == values.end()) return false; // Variable doesn't exist, return false.
    foundType->second = value; // Change the variable value and return true.
    return true;
}