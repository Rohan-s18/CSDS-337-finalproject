#pragma once

#include <llvm/IR/Type.h>

// Represents a type. It must get an LLVM type.
class VarType
{
public:

    // Create a copy of this type.
    // Returns: A completely new copy.
    virtual std::unique_ptr<VarType> Copy() = 0;

    // Convert the type data into something LLVM can use.
    // ctx: LLVM context needed to create a return type.
    // Returns: Type that LLVM can use.
    virtual llvm::Type* GetLLVMType(llvm::LLVMContext& ctx) = 0;

    // If this equals another given type.
    // other: Pointer to another variable type to compare against.
    // Returns: If this and the other type are equal.
    virtual bool Equals(VarType* other) = 0;

    // Must make the destructor virtual to make the compiler happy.
    virtual ~VarType() = default;

};