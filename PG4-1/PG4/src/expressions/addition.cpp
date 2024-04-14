#include "addition.h"

std::unique_ptr<VarType> ASTExpressionAddition::ReturnType(ASTFunction& func)
{
    if (!returnType) // If the return type has not been gotten yet.
    {
        if (!ASTExpression::CoerceMathTypes(func, a1, a2, returnType)) // This will force our arguments to be the same type and outputs which one it is.
            throw std::runtime_error("ERROR: Can not coerce types in addition expression! Are they both either ints or floats?");
    }
    return std::make_unique<VarTypeSimple>(*returnType); // Make a copy of our return type :}
}

bool ASTExpressionAddition::IsLValue(ASTFunction& func)
{
    return false; // If we are adding values together, they must be usable R-Values. Adding these together just results in an R-Value.
}

llvm::Value* ASTExpressionAddition::Compile(llvm::IRBuilder<>& builder, ASTFunction& func)
{
    // Compile the values as needed. Remember, we can only do operations on R-Values.
    auto retType = ReturnType(func);
    if (retType->Equals(&VarTypeSimple::IntType)) // Do standard addition on integer operands since we return an int.
        return builder.CreateAdd(a1->CompileRValue(builder, func), a2->CompileRValue(builder, func));
    else if (retType->Equals(&VarTypeSimple::FloatType)) // Do addition on floating point operands since we return a float.
        return builder.CreateFAdd(a1->CompileRValue(builder, func), a2->CompileRValue(builder, func));
    else // Call to return type should make this impossible, but best to keep it here just in case of a bug.
        throw std::runtime_error("ERROR: Can not perform addition! Are both inputs either ints or floats?");
}

std::string ASTExpressionAddition::ToString(const std::string& prefix)
{
    std::string ret = "(+)\n";
    ret += prefix + "├──" + a1->ToString(prefix + "│  ");
    ret += prefix + "└──" + a2->ToString(prefix + "   ");
    return ret;
}