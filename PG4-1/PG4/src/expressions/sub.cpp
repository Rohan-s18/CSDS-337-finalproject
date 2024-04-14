#include "subtraction.h"

std::unique_ptr<VarType> ASTExpressionSubtraction::ReturnType(ASTFunction& func)
{
    if (!returnType) // If the return type has not been gotten yet.
    {
       if (!ASTExpression::CoerceMathTypes(func, a1, a2, returnType)) // This will force our arguments to be the same type and outputs which one it is.
            throw std::runtime_error("ERROR: Can not coerce types in subtraction expression! Are they both either ints or floats?");
    }
    return std::make_unique<VarTypeSimple>(*returnType); // Make a copy of our return type :}
}

bool ASTExpressionSubtraction::IsLValue(ASTFunction& func)
{
    return false; // If we are adding values together, they must be usable R-Values. Subtracting these together just results in an R-Value.
}

llvm::Value* ASTExpressionSubtraction::Compile(llvm::IRBuilder<>& builder, ASTFunction& func)
{
    // Compile the values as needed. Remember, we can only do operations on R-Values.
    auto retType = ReturnType(func);
    if (retType->Equals(&VarTypeSimple::IntType)) // Do standard subtraction on integer operands since we return an int.
        return builder.CreateSub(a1->CompileRValue(builder, func), a2->CompileRValue(builder, func));
    else if (retType->Equals(&VarTypeSimple::FloatType)) // Do subtraction on floating point operands since we return a float.
        return builder.CreateFSub(a1->CompileRValue(builder, func), a2->CompileRValue(builder, func));
    else // Call to return type should make this impossible, but best to keep it here just in case of a bug.
        throw std::runtime_error("ERROR: Can not perform subtraction! Are both inputs either ints or floats?");
}

std::string ASTExpressionSubtraction::ToString(const std::string& prefix)
{
    std::string ret = "(+)\n";
    ret += prefix + "├──" + a1->ToString(prefix + "│  ");
    ret += prefix + "└──" + a2->ToString(prefix + "   ");
    return ret;
}