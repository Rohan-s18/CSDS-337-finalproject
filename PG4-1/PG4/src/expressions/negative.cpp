#include "negative.h"

//modified multiplication
std::unique_ptr<VarType> ASTExpressionNegation::ReturnType(ASTFunction &func)
{   
    //add unique if else to handle int and float
    if (!returnType)
    {
        if (a->ReturnType(func)->Equals(&VarTypeSimple::IntType))
            returnType = &VarTypeSimple::IntType;
        else if (a->ReturnType(func)->Equals(&VarTypeSimple::FloatType))
            returnType = &VarTypeSimple::FloatType;
        else
            throw std::runtime_error("ERROR: Can not coerce types in negation expression! Are they both either ints or floats?");
    }
    return std::make_unique<VarTypeSimple>(*returnType); // Make a copy of our return type :}
}

bool ASTExpressionNegation::IsLValue(ASTFunction &func)
{
    return false;
}

llvm::Value *ASTExpressionNegation::Compile(llvm::IRBuilder<> &builder, ASTFunction &func)
{
    // Compile the values as needed. Remember, we can only do operations on R-Values.
    auto retType = ReturnType(func);
    if (retType->Equals(&VarTypeSimple::IntType)) // we return an int.
        return builder.CreateNeg(a->CompileRValue(builder, func));
    else if (retType->Equals(&VarTypeSimple::FloatType)) // we return a float.
        return builder.CreateFNeg(a->CompileRValue(builder, func));
    else // Call to return type should make this impossible, but best to keep it here just in case of a bug.
        throw std::runtime_error("ERROR: Can not perform Negation! Are both inputs either ints or floats?");
}

std::string ASTExpressionNegation::ToString(const std::string &prefix)
{
    //only have one addition of ret and have - symbol
    std::string ret = "(-)\n";
    ret += prefix + "├──" + a->ToString(prefix + "│  ");
    return ret;
}