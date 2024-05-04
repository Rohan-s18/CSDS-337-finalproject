#pragma once

#include "../varType.h"

// Simple type enumerations.
enum VarTypeSimpleEnumeration
{
    Void,
    Bool,
    Int,
    Float,
    String
};

// Simple type that is just an enumeration.
class VarTypeSimple : public VarType
{

    // Actual type stored.
    VarTypeSimpleEnumeration type;

public:

    // Constant types to help.
    static VarTypeSimple VoidType;
    static VarTypeSimple BoolType;
    static VarTypeSimple IntType;
    static VarTypeSimple FloatType;
    static VarTypeSimple StringType;

    // Create a new simple type from the enumeration. You probably don't need to use this!
    // type: What type of simple variable type to instantiate.
    VarTypeSimple(VarTypeSimpleEnumeration type) : type(type) {}

    // Virtual functions. See base class for details.
    virtual std::unique_ptr<VarType> Copy() override;
    virtual llvm::Type* GetLLVMType(llvm::LLVMContext& ctx) override;
    virtual bool Equals(VarType* other) override;

};