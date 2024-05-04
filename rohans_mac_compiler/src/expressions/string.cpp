#include "string.h"

#include <regex>

std::unique_ptr<VarType> ASTExpressionString::ReturnType(ASTFunction& func)
{
    return VarTypeSimple::StringType.Copy(); // Of course we are returning a string, what else would it be.
}

bool ASTExpressionString::IsLValue(ASTFunction& func)
{
    return false; // It's a constant, of course it's not an L-Value.
}

llvm::Value* ASTExpressionString::Compile(llvm::IRBuilder<>& builder, ASTFunction& func)
{
    return builder.CreateGlobalStringPtr(value); // Simply just create a global string to return.
}

std::string ASTExpressionString::ToString(const std::string& prefix)
{
    std::string modified = std::regex_replace(value, std::regex("\n"), "\\n"); // We want escaped strings to show up as non-escaped.
    modified = std::regex_replace(modified, std::regex("\r"), "\\r");
    modified = std::regex_replace(modified, std::regex("\t"), "\\t");
    return "\"" + modified + "\"\n";
}