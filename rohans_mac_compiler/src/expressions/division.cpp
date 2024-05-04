#include "division.h"

//modifying multiply
std::unique_ptr<VarType> ASTExpressionDivision::ReturnType(ASTFunction &func)
{
    if (!returnType) // If the return type has not been gotten yet.
    {
        if (!ASTExpression::CoerceMathTypes(func, a1, a2, returnType)) // This will force our arguments to be the same type and outputs which one it is.
            throw std::runtime_error("ERROR: Can not coerce types in division expression! Are they both either ints or floats?");
    }
    return std::make_unique<VarTypeSimple>(*returnType); // Make a copy of our return type :}
}

bool ASTExpressionDivision::IsLValue(ASTFunction &func)
{
    return false; // If we are dividing values together, they must be usable R-Values. Dividing these together just results in an R-Value.
}

llvm::Value *ASTExpressionDivision::Compile(llvm::IRBuilder<> &builder, ASTFunction &func)
{
    // Compile the values as needed. Remember, we can only do operations on R-Values.
    auto retType = ReturnType(func);
    if (retType->Equals(&VarTypeSimple::IntType)) // have to use SDiv
        return builder.CreateSDiv(a1->CompileRValue(builder, func), a2->CompileRValue(builder, func));
    else if (retType->Equals(&VarTypeSimple::FloatType)) // Do division on floating point operands since we return a float.
        return builder.CreateFDiv(a1->CompileRValue(builder, func), a2->CompileRValue(builder, func));
    else // Call to return type should make this impossible, but best to keep it here just in case of a bug.
        throw std::runtime_error("ERROR: Can not perform division! Are both inputs either ints or floats?");
}

std::string ASTExpressionDivision::ToString(const std::string &prefix)
{
    //change to divide
    std::string ret = "(/)\n";
    ret += prefix + "├──" + a1->ToString(prefix + "│  ");
    ret += prefix + "└──" + a2->ToString(prefix + "   ");
    return ret;
}