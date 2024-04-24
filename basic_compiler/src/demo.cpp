#include "ast.h"
#include "expressions/addition.h"
#include "expressions/assignment.h"
#include "expressions/call.h"
#include "expressions/comparison.h"
#include "expressions/int.h"
#include "expressions/int2Float.h"
#include "expressions/negative.h"
#include "expressions/string.h"
#include "expressions/variable.h"
#include "statements/block.h"
#include "statements/for.h"
#include <types/simple.h>
#include <iostream>

// Demo application.
int demo()
{
    // Test building a program.
    AST ast("TestMod");

    // First, add a declaration to puts.
    ASTFunctionParameters putsParams;
    putsParams.push_back(ASTFunctionParameter(VarTypeSimple::StringType.Copy(), "str"));
    ast.AddFunction("printf", VarTypeSimple::IntType.Copy(), std::move(putsParams), true);

    // Second, add the main method.
    auto mainFunc = ast.AddFunction("main", VarTypeSimple::VoidType.Copy(), ASTFunctionParameters {});
    mainFunc->AddStackVar(ASTFunctionParameter(VarTypeSimple::IntType.Copy(), "i")); // Iterator.
    auto statements = std::make_unique<ASTStatementBlock>();

    // Create a printf call to print Hello World.
    std::vector<std::unique_ptr<ASTExpression>> callParams;
    callParams.push_back(ASTExpressionString::Create("Hello World!\n"));
    // Add it to the AST.
    statements->statements.push_back
    (
        ASTExpressionCall::Create
        (
            ASTExpressionVariable::Create("printf"),
            std::move(callParams)
        )
    );

    // Create another statement to show it works.
    std::vector<std::unique_ptr<ASTExpression>> callParams2;
    callParams2.push_back(ASTExpressionString::Create("This is a multi-line program that can even print numbers like %d!\n"));
    callParams2.push_back
    (
        ASTExpressionInt::Create(7)
    );
    // Add it to the AST.
    statements->statements.push_back
    (
        ASTExpressionCall::Create
        (
            ASTExpressionVariable::Create("printf"),
            std::move(callParams2)
        )
    );

    // Create a 3rd statement that prints a string, a float, and an addition of a positive and negative integer
    std::vector<std::unique_ptr<ASTExpression>> callParams3;
    callParams3.push_back
    (
        ASTExpressionString::Create("This is a string. Also look, some numbers: %f, %d\n")
    );
    callParams3.push_back
    (
        ASTExpressionInt2Float::Create
        (
            ASTExpressionInt::Create(11)
        )
    );
    callParams3.push_back
    (
        ASTExpressionAddition::Create
        (
            ASTExpressionInt::Create(1),
            ASTExpressionNegation::Create
            (
                ASTExpressionInt::Create(5)
            )
        )
    );

    // Add it to the AST.
    statements->statements.push_back
    (
        ASTExpressionCall::Create
        (
            ASTExpressionVariable::Create("printf"),
            std::move(callParams3)
        )
    );

    // Create the for loop.
    std::vector<std::unique_ptr<ASTExpression>> callParams4;
    callParams4.push_back(ASTExpressionString::Create("For loop iteration %d.\n"));
    callParams4.push_back(ASTExpressionVariable::Create("i"));
    statements->statements.push_back
    (
        ASTStatementFor::Create
        (
            ASTExpressionCall::Create
            (
                ASTExpressionVariable::Create("printf"),
                std::move(callParams4)
            ),
            ASTExpressionAssignment::Create
            (
                ASTExpressionVariable::Create("i"),
                ASTExpressionInt::Create(0)
            ),
            ASTExpressionComparison::Create
            (
                ASTExpressionComparisonType::LessThan,
                ASTExpressionVariable::Create("i"),
                ASTExpressionInt::Create(3)
            ),
            ASTExpressionAssignment::Create
            (
                ASTExpressionVariable::Create("i"),
                ASTExpressionAddition::Create
                (
                    ASTExpressionVariable::Create("i"),
                    ASTExpressionInt::Create(1)
                )
            )
        )
    );

    // Define the main function.
    mainFunc->Define(std::move(statements));

    // Finally, compile and write LLVM assembly.
    ast.Compile();
    ast.WriteLLVMAssemblyToFile("testMod.ll");
    ast.WriteLLVMBitcodeToFile("testMod.bc");

    // Print out AST tree and return.
    std::cout << ast.ToString() << std::endl;
    return 0;

}
