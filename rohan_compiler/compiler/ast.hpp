/*
Author: Rohan Singh, Benjamin Luo and William Cankar
QBASIC Abstruct Syntax Tree
*/


#ifndef __AST_H__
#define __AST_H__

#include <string>
#include <list>
#include <map>
#include <iostream>

#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>

#include <llvm/IR/Value.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Type.h>
#include "qbc.h"

enum MathOperator {
	OPERATOR_ADD = 1 , // +
	OPERATOR_SUB , // -
	OPERATOR_MUL , // *
	OPERATOR_DIV , // /
	OPERATOR_MOD , // % , MOD
	OPERATOR_POWER , // ^

	OPERATOR_EQUL , // == , =
	OPERATOR_NOTEQUL, // <> , >< , != as in basic
	OPERATOR_LESS , // <
	OPERATOR_LESSEQU , // <=
	OPERATOR_GREATER , // >
	OPERATOR_GREATEREQUL , // >=
};


enum Linkage {
	STATIC = 1,			
	EXTERN,				
	IMPORTC,			
};

enum ReferenceType {
	BYVALUE,		
	BYREF,			
};

class CodeBlockAST;
class FunctionDimAST;
class ASTContext
{
public:
	ASTContext()
		: llvmfunc(0)
		, codeblock(0)
		, block(0)
		, func(0)
	{}

	llvm::Function*		llvmfunc;	
	CodeBlockAST*		codeblock;	
	llvm::BasicBlock*	block;		
	llvm::Module*		module;		
	FunctionDimAST*		func;
};

// allow us to use shared ptr to manage the memory
class AST // :public boost::enable_shared_from_this<AST>
{
public:
	AST();
	virtual ~AST() = 0; // use pure virtual deconstruction
private:
	AST(const AST &);
	AST & operator =(const AST &);
};

class StatementAST: public AST
{
public:
    StatementAST():parent(0){}
	CodeBlockAST * parent;	
	std::string	LABEL;		// label , if there is. then we can use goto
							// must be uniq among function bodys
	virtual llvm::BasicBlock* Codegen(ASTContext) = 0;
	virtual ~StatementAST() {}
};
typedef StatementAST* StatementASTPtr;

typedef std::list<StatementASTPtr> StatementsAST;

class EmptyStmtAST : public StatementAST
{
public:
	EmptyStmtAST() {}
    llvm::BasicBlock* Codegen(ASTContext);
};

#include "type.hpp"


class DimAST: public StatementAST
{
public:
	DimAST(const std::string _name, ExprTypeASTPtr _type);
	// ExprType type; // the type of the expresion
	std::string		name; 
	ExprTypeASTPtr	type;
	virtual llvm::BasicBlock* Codegen(ASTContext) = 0; // generate alloca
	// gengerate deconstructions , default is don't do that, override on subclass
	virtual llvm::BasicBlock* valuedegen(ASTContext ctx){ return ctx.block; };

	virtual	llvm::Value*	getptr(ASTContext ctx) = 0; // the location for the allocated value
	virtual	llvm::Value*	getval(ASTContext ctx) = 0;

    virtual ~DimAST() {}
};
typedef boost::shared_ptr<DimAST>	DimASTPtr;


class AssigmentAST : public StatementAST
{
	AssignmentExprASTPtr	assignexpr;
public:
    AssigmentAST(NamedExprAST* lval, ExprAST* rval);	
    llvm::BasicBlock* Codegen(ASTContext);
};

class ExprStmtAST : public StatementAST
{
	ExprASTPtr		expr;
public:
	ExprStmtAST(ExprAST * exp);
    virtual llvm::BasicBlock* Codegen(ASTContext);
};


class ReturnAST : public StatementAST
{
	ExprASTPtr			expr;
public:
    ReturnAST(ExprAST* expr);
    virtual llvm::BasicBlock* Codegen(ASTContext);
};


class FunctionDimAST;
class CodeBlockAST : public StatementAST
{
public:
	std::vector<StatementASTPtr>			statements;	

	CodeBlockAST*							parent;		
	std::map<std::string, DimAST*>			symbols;	

    virtual llvm::BasicBlock* Codegen(ASTContext ctx);
	virtual llvm::BasicBlock* GenLeave(ASTContext);

	int  find(StatementAST* child);
	void addchild(StatementAST* item);
	void addchild(StatementsAST* items);

	CodeBlockAST() : parent(NULL) {}
    CodeBlockAST(StatementsAST * items);
	CodeBlockAST(StatementAST * item);

};
typedef boost::shared_ptr<CodeBlockAST>	CodeBlockASTPtr;


class IFStmtAST : public StatementAST
{
public:
	IFStmtAST(ExprASTPtr expr) : _expr(expr) {}
	ExprASTPtr _expr;
	CodeBlockASTPtr _then;
	CodeBlockASTPtr _else;
    virtual llvm::BasicBlock* Codegen(ASTContext);
};


class LoopAST : public StatementAST
{
	CodeBlockASTPtr	loopbody; 
	
public:
	LoopAST(CodeBlockAST* body) : loopbody(body) {}

    llvm::BasicBlock*		  bodygen(ASTContext);
    virtual llvm::BasicBlock* Codegen(ASTContext) = 0;
};

// while 

class WhileLoopAST : public LoopAST
{
	ExprASTPtr	condition;
public:
	WhileLoopAST(ExprASTPtr _condition, CodeBlockAST* body);

    virtual llvm::BasicBlock* Codegen(ASTContext);
};

class ForLoopAST : public LoopAST
{
	NamedExprASTPtr	refID;
	ExprASTPtr		start,end;
	ExprASTPtr		step;

public:
    ForLoopAST(NamedExprAST * id, ExprAST * start, ExprAST * end, ExprAST * step, CodeBlockAST* body);
    virtual llvm::BasicBlock* Codegen(ASTContext);
};

class VariableDimAST : public DimAST
{
	llvm::Value* alloca_var;
public:
	VariableDimAST(const std::string _name,  ExprTypeASTPtr _type);
	virtual llvm::BasicBlock* Codegen(ASTContext);
    virtual llvm::BasicBlock* valuedegen(ASTContext ctx);
    virtual llvm::Value* getptr(ASTContext ctx);
	virtual	llvm::Value* getval(ASTContext ctx);
};
typedef boost::shared_ptr<VariableDimAST> VariableDimASTPtr;
typedef std::list<VariableDimASTPtr> VariableDimList;
 

class StrucDimAST : public StatementAST
{
public:
    StrucDimAST(const std::string _typename,VariableDimList members);
	std::string	Typename; 
	VariableDimList members; 
    virtual llvm::BasicBlock* Codegen(ASTContext ctx); 
};

class ArgumentDimAST : public VariableDimAST
{
	llvm::Value* modified_stackvar;
public:
	ArgumentDimAST(const std::string _name, ExprTypeASTPtr	_type);
	virtual llvm::BasicBlock* Codegen(ASTContext);
    virtual llvm::Value* getptr(ASTContext ctx);
	virtual	llvm::Value* getval(ASTContext ctx);
};
typedef boost::shared_ptr<ArgumentDimAST> ArgumentDimASTPtr;


class ArgumentDimsAST : public CodeBlockAST
{
public:
	std::vector<ArgumentDimASTPtr> dims;
    ArgumentDimsAST() {}
};
typedef boost::shared_ptr<ArgumentDimsAST> ArgumentDimsASTPtr;
 
class FunctionDimAST: public DimAST
{
	friend class ReturnAST;
private:
	llvm::BasicBlock*		returnblock; // insert before this ! please !
	llvm::Function*			target;
	llvm::Value*			retval; // allocated for return value, should use that for return.
	llvm::Value*			setret(ASTContext ctx, ExprASTPtr expr);
public:

	Linkage		linkage;  
	std::list<VariableDimASTPtr> args_type; //checked by CallExpr.
	ArgumentDimsASTPtr	callargs;  
	CodeBlockASTPtr	body; 

    FunctionDimAST(const std::string _name, ExprTypeASTPtr, ArgumentDimsAST * callargs = NULL);

    virtual llvm::BasicBlock* Codegen(ASTContext);

	//get a pointer to function
    virtual	llvm::Value* getptr(ASTContext ctx);
	virtual	llvm::Value* getval(ASTContext ctx)
	{
		return this->target;
	}
};

class DefaultMainFunctionAST : public FunctionDimAST
{
public:
	DefaultMainFunctionAST(CodeBlockAST * body);
};

typedef std::list<ExprASTPtr>	FunctionParameterListAST;

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
 
class PrintIntroAST : public ConstNumberExprAST
{
public:
	PrintIntroAST();
    llvm::BasicBlock* Codegen(ASTContext);
};
typedef boost::shared_ptr<PrintIntroAST> PrintIntroASTPtr;

class PrintStmtAST: public StatementAST
{
public:
	PrintIntroASTPtr	print_intro;
	ExprListASTPtr		callargs;
	PrintStmtAST(PrintIntroAST *, ExprListAST *);
    virtual llvm::BasicBlock* Codegen(ASTContext);
};

#endif // __AST_H__

