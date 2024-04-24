#pragma once

#include "statement.h"
#include "types/simple.h"
#include "varType.h"
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/IRBuilder.h>
#include <memory>

// Forward declarations.
class ASTFunction;

// Abstract class for expressions.
class ASTExpression : public ASTStatement {
public:

    // Get the return type of this expression. It can even be void.
    // func: Function that contains this expression.
    // Returns: A variable type that this expression returns.
    virtual std::unique_ptr<VarType> ReturnType(ASTFunction &func) = 0;

    // Returns if the result is an L-Value. See the design document for details.
    // func: Function that contains this expression.
    // Returns: If this expression results in an L-Value.
    virtual bool IsLValue(ASTFunction &func) = 0;

    // Compile the expression and get its returned value.
    // builder: LLVM IR builder.
    // func: Function that contains this expression.
    // Returns: An LLVM value. Can be null if void is returned, you may need ReturnType for the current expression or arguments to type check.
    virtual llvm::Value* Compile(llvm::IRBuilder<>& builder, ASTFunction& func) = 0;

    // Compile the expression as an R-Value (load from it if it is an L-Value already).
    // builder: LLVM IR builder.
    // func: Function that contains this expression.
    // Returns: An LLVM value. Can be null if void is returned.
    llvm::Value* CompileRValue(llvm::IRBuilder<>& builder, ASTFunction& func);

    // Add implicit casts as needed.
    // Ex: If the destination types are the same this does nothing. However, if the destination is a float and the source is an int a cast will be added. An exception is given if not possible to cast.
    // func: Function that contains this expression.
    // srcExpression: Reference to the expression to implicitly cast as needed.
    // destType: The destination type for the source expression to be casted to.
    static void ImplicitCast(ASTFunction& func, std::unique_ptr<ASTExpression>& srcExpression, VarType* destType);

    // Coerce math types.
    // Ex: If all operands are int, this sets out type as int and makes them stay ints, but if not makes them floats and sets out type as float.
    // func: Function that contains this expression.
    // a1: Reference to the first expression operand.
    // a2: Reference to the second expression operand.
    // outCoercedType: Set this to where to output the type. If int, everything is int, else it is a float and we upcasted the given expressions as needed.
    // Returns if the operation succeeds. If it does not succeed, there is at least one operand that is not an int or float.
    static bool CoerceMathTypes(ASTFunction& func, std::unique_ptr<ASTExpression>& a1, std::unique_ptr<ASTExpression>& a2, VarTypeSimple*& outCoercedType);

    // Coerce types, but including converting bool to int if exists.
    // Ex: If all operands are int, this sets out type as int and makes them stay ints, but if not makes them floats and sets out type as float.
    // func: Function that contains this expression.
    // a1: Reference to the first expression operand.
    // a2: Reference to the second expression operand.
    // outCoercedType: Set this to where to output the type. If int, everything is int, else it is a float and we upcasted the given expressions as needed.
    // Returns if the operation succeeds. If it does not succeed, there is at least one operand that is not an int or float.
    static bool CoerceTypes(ASTFunction& func, std::unique_ptr<ASTExpression>& a1, std::unique_ptr<ASTExpression>& a2, VarTypeSimple*& outCoercedType);

    // DO NOT CALL THIS FROM EXPRESSION SUBCLASSES! THERE'S A GOOD CHANCE IT WON'T DO WHAT YOU WANT IT TO!
    std::unique_ptr<VarType> StatementReturnType(ASTFunction& func) override;

    // DO NOT CALL THIS FROM EXPRESSION SUBCLASSES! THERE'S A GOOD CHANCE IT WON'T DO WHAT YOU WANT IT TO!
    void Compile(llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction& func) override;

    // Must make the destructor virtual to make the compiler happy.
    ~ASTExpression() override = default;

};