#include "expression.h"
#include "expressions/bool2Int.h"
#include "expressions/float2Int.h"
#include "expressions/int2Bool.h"
#include "expressions/int2Float.h"

llvm::Value* ASTExpression::CompileRValue(llvm::IRBuilder<>& builder, ASTFunction& func)
{
    llvm::Value* raw = Compile(builder, func); // First get the naturally compiled value.
    if (IsLValue(func)) // If the value is an L-Value, we need to load it.
    {
        return builder.CreateLoad(ReturnType(func)->GetLLVMType(builder.getContext()), raw); // Use the return type from this expression to load the needed value.
    }
    else return raw; // It's already an R-Value.
}

void ASTExpression::ImplicitCast(ASTFunction& func, std::unique_ptr<ASTExpression>& srcExpression, VarType* destType)
{

    // If the types are equal, nothing needs to be done.
    auto srcType = srcExpression->ReturnType(func);
    if (srcType->Equals(destType)) return;

    // Destination type is a boolean.
    if (destType->Equals(&VarTypeSimple::BoolType))
    {
        if (srcType->Equals(&VarTypeSimple::FloatType)) // Cast to int first.
        {
            auto tmp = std::move(srcExpression);
            srcExpression = std::make_unique<ASTExpressionFloat2Int>(std::move(tmp));
            srcType = srcExpression->ReturnType(func);
        }
        if (srcType->Equals(&VarTypeSimple::IntType)) // Cast to bool now.
        {
            auto tmp = std::move(srcExpression);
            srcExpression = std::make_unique<ASTExpressionInt2Bool>(std::move(tmp));
            return; // Finished successfully.
        }
    }

    // Destination type is an int.
    if (destType->Equals(&VarTypeSimple::IntType))
    {
        if (srcType->Equals(&VarTypeSimple::FloatType))
        {
            auto tmp = std::move(srcExpression);
            srcExpression = std::make_unique<ASTExpressionFloat2Int>(std::move(tmp));
            return;
        }
        if (srcType->Equals(&VarTypeSimple::BoolType))
        {
            auto tmp = std::move(srcExpression);
            srcExpression = std::make_unique<ASTExpressionBool2Int>(std::move(tmp));
            return;
        }
    }

    // Destination type is a float.
    if (destType->Equals(&VarTypeSimple::FloatType))
    {
        if (srcType->Equals(&VarTypeSimple::BoolType)) // Cast to int first.
        {
            auto tmp = std::move(srcExpression);
            srcExpression = std::make_unique<ASTExpressionBool2Int>(std::move(tmp));
            srcType = srcExpression->ReturnType(func);
        }
        if (srcType->Equals(&VarTypeSimple::IntType)) // Cast to float now.
        {
            auto tmp = std::move(srcExpression);
            srcExpression = std::make_unique<ASTExpressionInt2Float>(std::move(tmp));
            return; // Finished successfully.
        }
    }

    // Nothing left to do.
    throw std::runtime_error("ERROR: Source expression can not be implicitly casted to the destination type!");

}

bool ASTExpression::CoerceMathTypes(ASTFunction& func, std::unique_ptr<ASTExpression>& a1, std::unique_ptr<ASTExpression>& a2, VarTypeSimple*& outCoercedType)
{

    // Gather return types.
    auto r1 = a1->ReturnType(func);
    auto r2 = a2->ReturnType(func);

    // Make sure r1 is either a float or int.
    bool r1Float = r1->Equals(&VarTypeSimple::FloatType);
    if (!r1Float && !r1->Equals(&VarTypeSimple::IntType)) return false;

    // Make sure r2 is either a float or int.
    bool r2Float = r2->Equals(&VarTypeSimple::FloatType);
    if (!r2Float && !r2->Equals(&VarTypeSimple::IntType)) return false;

    // Do casting as needed.
    if (r1Float)
    {

        // Both are floats, nothing needed.
        if (r2Float)
        {
            outCoercedType = &VarTypeSimple::FloatType;
        }

        // Cast r2 to a float.
        else
        {
            outCoercedType = &VarTypeSimple::FloatType;
            auto tmp = std::move(a2);
            a2 = std::make_unique<ASTExpressionInt2Float>(std::move(tmp));
        }

    }
    else
    {

        // Cast r1 to float.
        if (r2Float)
        {
            outCoercedType = &VarTypeSimple::FloatType;
            auto tmp = std::move(a1);
            a1 = std::make_unique<ASTExpressionInt2Float>(std::move(tmp));
        }

        // Both are ints. No casting needed.
        else
        {
            outCoercedType = &VarTypeSimple::IntType;
        }

    }
    return true;

}

bool ASTExpression::CoerceTypes(ASTFunction& func, std::unique_ptr<ASTExpression>& a1, std::unique_ptr<ASTExpression>& a2, VarTypeSimple*& outCoercedType)
{

    // All we really need to do is convert bool to int first if needed then coerce the math types.
    if (a1->ReturnType(func)->Equals(&VarTypeSimple::BoolType))
    {
        auto tmp = std::move(a1);
        a1 = std::make_unique<ASTExpressionBool2Int>(std::move(tmp));
    }
    if (a2->ReturnType(func)->Equals(&VarTypeSimple::BoolType))
    {
        auto tmp = std::move(a2);
        a2 = std::make_unique<ASTExpressionBool2Int>(std::move(tmp));
    }
    return CoerceMathTypes(func, a1, a2, outCoercedType);

}

std::unique_ptr<VarType> ASTExpression::StatementReturnType(ASTFunction& func)
{
    return nullptr; // Expression returns nothing statement wise.
}

void ASTExpression::Compile(llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction& func)
{

    // We can compile the expression itself, we just don't return anything since this is an expression and not a return statement.
    Compile(builder, func);

}