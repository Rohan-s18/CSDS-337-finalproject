﻿/*
Author: Rohan Singh, Benjamin Luo and William Cankar
QBASIC Abstruct Syntax Tree
*/
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>

#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Constant.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Support/Allocator.h>

#include "llvmwrapper.hpp"
#include "ast.hpp"
#include "type.hpp"

#define debug	std::printf

AST::AST() {}
AST::~AST() {}

PrintStmtAST::PrintStmtAST(PrintIntroAST * intro,ExprListAST* args)
	: callargs(args)
	, print_intro(intro)
{}

PrintIntroAST::PrintIntroAST()
	: ConstNumberExprAST(0)
{}


DimAST::DimAST(const std::string _name, ExprTypeASTPtr _type)
	: name(_name), type(_type)
{}

VariableDimAST::VariableDimAST(const std::string _name, ExprTypeASTPtr _type)
	: DimAST(_name, _type)
{}

FunctionDimAST::FunctionDimAST(const std::string _name, ExprTypeASTPtr _type, ArgumentDimsAST * _callargs)
	: DimAST(_name, _type)
	, callargs(_callargs)
	, returnblock(NULL)
	, target(NULL)
	, retval(NULL)
{}

DefaultMainFunctionAST::DefaultMainFunctionAST(CodeBlockAST * body)
	: FunctionDimAST("main", ExprTypeASTPtr(new CallableExprTypeAST(VoidExprTypeAST::GetVoidExprTypeAST())))
{
	this->body = CodeBlockASTPtr(body);
}

ReferenceAST::ReferenceAST(std::string* tID)
	: ID(*tID)
{}

void ExprListAST::Append(ExprAST* exp)
{
    expression_list.push_back( ExprASTPtr(exp));
}

ExprStmtAST::ExprStmtAST(ExprAST* exp)
	: expr(exp)
{}

ReturnAST::ReturnAST(ExprAST* _expr)
	: expr(_expr)
{}

int CodeBlockAST::find(StatementAST* child)
{
	int index = 0;
	BOOST_FOREACH( StatementASTPtr item , statements)
	{
		if(item == child)
			return index;
		index++;
	}
	debug("not a child??????????\n");
	exit(1);
}

void CodeBlockAST::addchild(StatementAST* item)
{
    if(item)
	{
        this->statements.push_back(StatementASTPtr(item));
        item->parent = this;
    }
}
void CodeBlockAST::addchild(StatementsAST* items)
{
	std::list< StatementASTPtr >::iterator it = items->begin();
	for(;it != items->end() ; it ++)
	{
		(*it)->parent = this;
        this->statements.push_back(*it);
	}
}

CodeBlockAST::CodeBlockAST(StatementsAST* items)
	: parent(NULL)
{
    addchild(items);
}

CodeBlockAST::CodeBlockAST(StatementAST* item)
	: parent(NULL)
{
    addchild(item);
}

AssigmentAST::AssigmentAST(NamedExprAST* lval, ExprAST* rval)
	: assignexpr(new AssignmentExprAST(lval, rval))
{}

ArgumentDimAST::ArgumentDimAST(const std::string _name, ExprTypeASTPtr _type)
	: VariableDimAST(_name, _type)
	, modified_stackvar(NULL)
{}

WhileLoopAST::WhileLoopAST(ExprASTPtr _condition, CodeBlockAST* body)
	: condition(_condition)
	, LoopAST(body)
{}

ForLoopAST::ForLoopAST(NamedExprAST* id, ExprAST* _start, ExprAST* _end, ExprAST* _step, CodeBlockAST* body)
	: LoopAST(body)
	, start(_start)
	, end(_end)
	, step(_step)
	, refID(id)
{
	if(!step)
		step.reset(new ConstNumberExprAST(1));
}

StrucDimAST::StrucDimAST(const std::string _name,VariableDimList _members)
{
	this->Typename = _name;
	this->members = _members;
}
