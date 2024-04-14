#include "comparison.h"

std::unique_ptr<VarType> ASTExpressionComparison::ReturnType(ASTFunction& func)
{
    return VarTypeSimple::BoolType.Copy();
}

bool ASTExpressionComparison::IsLValue(ASTFunction& func)
{
    return false; // If we are adding values together, they must be usable R-Values. Adding these together just results in an R-Value.
}

llvm::Value* ASTExpressionComparison::Compile(llvm::IRBuilder<>& builder, ASTFunction& func)
{
    VarTypeSimple* returnType;
    if (!ASTExpression::CoerceTypes(func, a1, a2, returnType)) // This will force our arguments to be the same type and outputs which one it is.
            throw std::runtime_error("ERROR: Can not coerce types in comparison expression! Are they all booleans, ints, and floats?");

    // Get values. Operations only work on R-Values.
    auto a1Val = a1->CompileRValue(builder, func);
    auto a2Val = a2->CompileRValue(builder, func);

    // Int type, do int comparisons.
    if (returnType->Equals(&VarTypeSimple::IntType))
    {
        switch (type)
        {
            case Equal: return builder.CreateICmp(llvm::CmpInst::ICMP_EQ, a1Val, a2Val);
            case NotEqual: return builder.CreateICmp(llvm::CmpInst::ICMP_NE, a1Val, a2Val);
            // need LessThan, LessThanOrEqual, GreaterThan, GreaterThanOrEqual
            case LessThan: return builder.CreateICmp(llvm::CmpInst::ICMP_SLT, a1Val, a2Val);
            case LessThanOrEqual: return builder.CreateICmp(llvm::CmpInst::ICMP_SLE, a1Val, a2Val);
            case GreaterThan: return builder.CreateICmp(llvm::CmpInst::ICMP_SGT, a1Val, a2Val);
            case GreaterThanOrEqual: return builder.CreateICmp(llvm::CmpInst::ICMP_SGE, a1Val, a2Val);
        }
    }

    // Float type, do float comparisons.
    if (returnType->Equals(&VarTypeSimple::FloatType))
    {
        switch (type)
        {
            //had to keep trying name combinatins as could not find original names
            case Equal: return builder.CreateFCmp(llvm::CmpInst::ICMP_EQ, a1Val, a2Val); // Use ordered operations to not allow NANS.
            case NotEqual: return builder.CreateFCmp(llvm::CmpInst::ICMP_NE, a1Val, a2Val);
            case LessThan: return builder.CreateFCmp(llvm::CmpInst::ICMP_SLT, a1Val, a2Val);
            case LessThanOrEqual: return builder.CreateFCmp(llvm::CmpInst::ICMP_SLE, a1Val, a2Val);
            case GreaterThan: return builder.CreateFCmp(llvm::CmpInst::ICMP_SGT, a1Val, a2Val);
            case GreaterThanOrEqual: return builder.CreateFCmp(llvm::CmpInst::ICMP_SGE, a1Val, a2Val);
        }
    }

    // make this else
    else{
        throw std::runtime_error("ERROR: Did not return value from comparison. Unsuccessful coercion of values or invalid comparison type!");
    }
    
}

std::string ASTExpressionComparison::ToString(const std::string& prefix)
{
    //add remainder for defined
    std::string op = "";
    switch (type)
    {
        case Equal: op = "="; break;
        case NotEqual: op = "!="; break;
        case LessThan: op = "<"; break;
        case LessThanOrEqual: op = "<="; break;
        case GreaterThan: op = ">"; break;
        case GreaterThanOrEqual: op = ">="; break;
    }
    std::string ret = "(" + op + ")\n";
    ret += prefix + "├──" + a1->ToString(prefix + "│  ");
    ret += prefix + "└──" + a2->ToString(prefix + "   ");
    return ret;
}