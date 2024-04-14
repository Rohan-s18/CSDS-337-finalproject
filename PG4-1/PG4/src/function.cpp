#include "function.h"

#include "ast.h"
#include "types/simple.h"
#include <llvm/IR/Verifier.h>

ASTFunction::ASTFunction(AST& ast, const std::string& name, std::unique_ptr<VarType> returnType, ASTFunctionParameters parameters, bool variadic) : ast(ast), name(name)
{

    // Create the function type.
    auto params = std::move(parameters);
    std::vector<std::unique_ptr<VarType>> paramTypes;
    for (auto& param : params) // We must copy each type from the parameters.
    {
        paramTypes.push_back(std::get<0>(param)->Copy()); // This copies the first item of the tuple, which is a var type pointer.
    }
    funcType = std::make_unique<VarTypeFunction>(std::move(returnType), std::move(paramTypes), variadic);

    // Add to scope table, we need to error if it already exists.
    if (!ast.scopeTable.AddVariable(name, funcType->Copy()))
    {
        throw std::runtime_error("ERROR: Function or global variable with name " + name + " already exists.");
    }

    // Add parameters as stack variables. It's ok for us to do it since we are the ones setting up the parameters for stack variables.
    for (auto& param : params)
    {
        this->parameters.push_back(std::get<1>(param));
        AddStackVar(std::move(param));
    }

}

void ASTFunction::AddStackVar(ASTFunctionParameter var)
{

    // Add variable to the scope table and error if it already exists.
    if (!scopeTable.AddVariable(std::get<1>(var), std::move(std::get<0>(var))))
    {
        throw std::runtime_error("ERROR: Variable " + std::get<1>(var) + " is already defined in function " + name + "!");
    }
    stackVariables.push_back(std::get<1>(var));

}

VarType* ASTFunction::GetVariableType(const std::string& name)
{
    VarType* ret;
    if (ret = scopeTable.GetVariableType(name), !ret) // Continue only if function scope table doesn't have value.
    {
        if (ret = ast.scopeTable.GetVariableType(name), !ret) // Continue only if AST scope table doesn't have value.
        {
            throw std::runtime_error("ERROR: In function " + this->name + ", cannot resolve variable or function " + name + "!");
        }
        else return ret;
    }
    else return ret;
}

llvm::Value* ASTFunction::GetVariableValue(const std::string& name)
{
    llvm::Value* ret;
    if (ret = scopeTable.GetVariableValue(name), !ret) // Continue only if function scope table doesn't have value.
    {
        if (ret = ast.scopeTable.GetVariableValue(name), !ret) // Continue only if AST scope table doesn't have value.
        {
            throw std::runtime_error("ERROR: In function " + this->name + ", cannot resolve variable or function " + name + "!");
        }
        else return ret;
    }
    else return ret;
}

void ASTFunction::SetVariableValue(const std::string& name, llvm::Value* value)
{
    if (!scopeTable.SetVariableValue(name, value)) // Continue only if function scope table doesn't have value.
    {
        if (!ast.scopeTable.SetVariableValue(name, value)) // Continue only if AST scope table doesn't have value.
        {
            throw std::runtime_error("ERROR: In function " + this->name + ", cannot resolve variable or function " + name + "!");
        }
    }
}

void ASTFunction::Define(std::unique_ptr<ASTStatement> definition)
{
    if (!this->definition) // Define only if not already defined.
    {
        this->definition = std::move(definition);
    }
    else throw std::runtime_error("ERROR: Function " + name + " already has a definition!");
}

void ASTFunction::Compile(llvm::Module& mod, llvm::IRBuilder<>& builder)
{

    // First, add a new function declaration to our scope.
    auto func = llvm::Function::Create((llvm::FunctionType*)funcType->GetLLVMType(builder.getContext()), llvm::GlobalValue::LinkageTypes::ExternalLinkage, name, mod);
    ast.scopeTable.SetVariableValue(name, func);

    // Set parameter names.
    unsigned idx = 0;
    for (auto& arg : func->args()) arg.setName(parameters[idx++]);

    // Only continue if the function has a definition.
    if (!definition) return;

    // Create a new basic block to start insertion into.
    llvm::BasicBlock* bb = llvm::BasicBlock::Create(builder.getContext(), "entry", func);
    builder.SetInsertPoint(bb);

    /*
        So there's a lot going on here and it needs a bit of explanation.
        In LLVM, registers can only be assigned once which is not what we want for mutable variables.
        In order to combat this, we allocate memory on the stack to an LLVM register (llvm::Value*).
        This works, as while the memory location itself is constant, we can load and store to that stack location as much as we want.
        We can allocate stack memory with an "alloca" instruction.
        This must be done in the entry block for stack allocations to be automatically optimized to registers on the target machine where appropriate.
        Once we allocate memory on the stack for each stack variable, we must then store it to the scope table so we know where each variable "lives".
        Now that we did that, we can use our GetVariableValue and SetVariableValue functions to get the pointer to the variable, which we can load from or store to!
        Note that how the stored stack variables are pointers to a value is what makes us classify it as an L-Value!
    */
    for (auto& stackVar : stackVariables)
    {
        scopeTable.SetVariableValue(
            stackVar,
            builder.CreateAlloca(scopeTable.GetVariableType(stackVar)->GetLLVMType(builder.getContext()), nullptr, stackVar)
        );
    }

    // Now we need to store the initial values of the function arguments into their stack equivalents.
    for (auto& arg : func->args())
    {
        builder.CreateStore(&arg, scopeTable.GetVariableValue(arg.getName().data())); // We are storing the argument into the pointer to the stack variable gotten by fetching it from the scope table.
    }

    // Check the function body to make sure it returns what we expect it to.
    std::unique_ptr<VarType> retType = definition->StatementReturnType(*this);
    bool satisfiesType = !retType && funcType->returnType->Equals(&VarTypeSimple::VoidType); // If we return nothing and expect void, it works.
    if (!satisfiesType && retType) satisfiesType = retType->Equals(funcType->returnType.get()); // If we return something, make sure we return what is expected.
    if (!satisfiesType)
    {
        throw std::runtime_error("ERROR: Function " + name + " does not return what it should!");
    }

    // Generate the function.
    definition->Compile(mod, builder, *this);

    // Add an implicit return void if necessary.
    if (!retType)
    {
        builder.CreateRetVoid();
    }

    // Verify and optimize the function.
    llvm::verifyFunction(*func, &llvm::errs());
    ast.fpm.run(*func);

}

std::string ASTFunction::ToString(const std::string& prefix)
{
    std::string output = name + "\n";
    output += prefix + "└──" + (definition == nullptr ? "nullptr\n" : definition->ToString(prefix + "   "));
    return output;
}