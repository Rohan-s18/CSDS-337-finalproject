#include "function.h"

#include <llvm/IR/DerivedTypes.h>

std::unique_ptr<VarType> VarTypeFunction::Copy()
{
    std::vector<std::unique_ptr<VarType>> newTypes; // Copy each of the parameter types.
    for (auto& type : parameterTypes)
    {
        newTypes.push_back(type->Copy());
    }
    return std::make_unique<VarTypeFunction>(returnType->Copy(), std::move(newTypes), varArgs);
}

llvm::Type* VarTypeFunction::GetLLVMType(llvm::LLVMContext& ctx)
{
    if (!funcType) // Fetch the LLVM type if it has not been defined already.
    {
        std::vector<llvm::Type*> params;
        for (auto& param : parameterTypes) // Gather LLVM types of all parameters.
        {
            params.push_back(param->GetLLVMType(ctx));
        }
        funcType = llvm::FunctionType::get(returnType->GetLLVMType(ctx), params, varArgs); // Create the function type with LLVM.
    }
    return funcType;
}

bool VarTypeFunction::Equals(VarType* other)
{
    VarTypeFunction* casted = dynamic_cast<VarTypeFunction*>(other);
    if (casted) // This is null if the cast fails (meaning it's not really a function type).
    {
        if (!returnType->Equals(casted->returnType.get())) return false;
        if (parameterTypes.size() != casted->parameterTypes.size()) return false;
        for (int i = 0; i < parameterTypes.size(); i++) // Compare each parameter to see if it matches.
        {
            if (!parameterTypes[i]->Equals(casted->parameterTypes[i].get())) return false;
        }
        return varArgs == casted->varArgs; // If we made it this far, it's the last thing we have to check.
    }
    else return false;
}