/*
Author: Rohan Singh, Benjamin Luo and William Cankar
QBASIC Abstruct Syntax Tree
*/
#include <cstdio>
#include <boost/make_shared.hpp>
#include <boost/foreach.hpp>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Type.h>

#include "ast.hpp"
#include "type.hpp"
#include "llvmwrapper.hpp"

#define debug std::printf

static	NumberExprOperation		numberop;
static	StringExprOperation 	stringop;
static	ArrayExprOperation		arrayop;
static	FunctionExprOperation	funcop;
static	PointerTypeOperation	pointerop;
static	StringExprOperation		structop;

ExprOperation* NumberExprTypeAST::getop()
{
	return & numberop;
}

ExprOperation* StringExprTypeAST::getop()
{
	return & stringop;
}

ExprOperation* ArrayExprTypeAST::getop()
{
	return &arrayop;
}

ExprOperation* CallableExprTypeAST::getop()
{
	return &funcop;
}

ExprOperation* PointerTypeAST::getop()
{
	return &pointerop;
}

ExprOperation* StructExprTypeAST::getop()
{
	return &structop;
}

PointerTypeASTPtr VoidExprTypeAST::getpointetype()
{
    return PointerTypeASTPtr();
}

PointerTypeASTPtr NumberExprTypeAST::getpointetype()
{
	return boost::make_shared<PointerTypeAST>(GetNumberExprTypeAST());
}

PointerTypeASTPtr StringExprTypeAST::getpointetype()
{
	return boost::make_shared<PointerTypeAST>(GetStringExprTypeAST());
}


ExprASTPtr ExprOperation::operator_assign(ASTContext, NamedExprASTPtr lval, ExprASTPtr rval)
{
	debug("can not assign to  this target\n");
    exit(2);
}

ExprASTPtr ExprOperation::operator_add(ASTContext, ExprASTPtr lval, ExprASTPtr rval)
{
	debug("can not add this target\n");
    exit(2);
}

ExprASTPtr ExprOperation::operator_sub(ASTContext, ExprASTPtr lval, ExprASTPtr rval)
{
	debug("can not sub this target\n");
    exit(2);
}

ExprASTPtr ExprOperation::operator_mul(ASTContext, ExprASTPtr lval, ExprASTPtr rval)
{
	debug("can not mul this target\n");
    exit(2);
}

ExprASTPtr ExprOperation::operator_div(ASTContext, ExprASTPtr lval, ExprASTPtr rval)
{
	debug("can not div this target\n");
    exit(2);
}

ExprASTPtr ExprOperation::operator_comp(ASTContext, MathOperator op, ExprASTPtr lval, ExprASTPtr rval)
{
    debug("can comp non-comp target\n");
    exit(2);
}

ExprASTPtr ExprOperation::operator_call(ASTContext, NamedExprASTPtr target, ExprListASTPtr callargslist)
{
	debug("can not call on a non-callable target\n");
	exit(2);
}

//	call get on lval and rval, then wrapper an value to NumberExprAST;
ExprASTPtr NumberExprOperation::operator_assign(ASTContext ctx, NamedExprASTPtr lval, ExprASTPtr rval)
{
	llvm::Value * LHS =	lval->getptr(ctx);
	llvm::Value * RHS =	rval->getval(ctx);

 	llvm::IRBuilder<> builder(ctx.block); 

	LHS = builder.CreateBitCast(LHS,qbc::getplatformlongtype()->getPointerTo());
  	builder.CreateStore(RHS,LHS);
	return lval->type(ctx)->createtemp(ctx,LHS,NULL);
} 
ExprASTPtr StringExprOperation::operator_assign(ASTContext ctx, NamedExprASTPtr lval, ExprASTPtr rval)
{
	llvm::IRBuilder<> builder(ctx.block);

	llvm::FunctionCallee llvmfunc_free =  qbc::getbuiltinprotype(ctx,"free");
	llvm::FunctionCallee llvmfunc_strdup = qbc::getbuiltinprotype(ctx,"strdup");

	builder.CreateCall(llvmfunc_free,lval->getval(ctx));

	llvm::Value * dupedstr = builder.CreateCall(llvmfunc_strdup,rval->getval(ctx),"dumpstring");

	builder.CreateStore(dupedstr,lval->getptr(ctx));
	return lval;
}
 
ExprASTPtr ArrayExprOperation::operator_assign(ASTContext ctx, NamedExprASTPtr lval, ExprASTPtr rval)
{ 

	debug("operator assign for array number %p\n",lval.get());

	ArrayExprTypeAST * reallval =dynamic_cast<ArrayExprTypeAST*>(lval->type(ctx).get());
 
	debug("assign to array element , %p \n" , reallval);

	return reallval->elementtype->getop()->operator_assign(ctx,lval,rval);
}
 
ExprASTPtr NumberExprOperation::operator_add(ASTContext ctx, ExprASTPtr lval, ExprASTPtr rval)
{
	llvm::Value * LHS =	lval->getval(ctx);
	llvm::Value * RHS =	rval->getval(ctx);
	llvm::IRBuilder<> builder(ctx.block);
	llvm::Value * result = builder.CreateAdd(LHS,RHS);
 
	return lval->type(ctx)->createtemp(ctx,result,NULL);
}
 
ExprASTPtr StringExprOperation::operator_add(ASTContext ctx, ExprASTPtr lval, ExprASTPtr rval)
{
	llvm::IRBuilder<> builder(ctx.block);

	auto llvmfunc_calloc =  qbc::getbuiltinprotype(ctx,"malloc");
	auto llvmfunc_strlen =  qbc::getbuiltinprotype(ctx,"strlen");
	auto llvmfunc_strcpy = qbc::getbuiltinprotype(ctx,"strcpy");
	auto llvmfunc_strcat = qbc::getbuiltinprotype(ctx,"strcat");

	llvm::Value * string_left_length = builder.CreateCall(llvmfunc_strlen,lval->getval(ctx));
	llvm::Value * string_right_length = builder.CreateCall(llvmfunc_strlen,rval->getval(ctx));

	llvm::Value * result_length = builder.CreateAdd(string_left_length, string_right_length);

	llvm::Value * resultstring = builder.CreateCall(llvmfunc_calloc,result_length);

	builder.CreateCall(llvmfunc_strcpy, {resultstring, lval->getval(ctx)});
	builder.CreateCall(llvmfunc_strcat, {resultstring, rval->getval(ctx)});
	return 	lval->type(ctx)->createtemp(ctx, resultstring, NULL);
}

 
ExprASTPtr NumberExprOperation::operator_sub(ASTContext ctx, ExprASTPtr lval, ExprASTPtr rval)
{
	llvm::Value * LHS =	lval->getval(ctx);
	llvm::Value * RHS =	rval->getval(ctx);
	llvm::IRBuilder<> builder(ctx.block);
	llvm::Value * result = builder.CreateSub(LHS,RHS);
 
	return	lval->type(ctx)->createtemp(ctx,result,NULL);
}
 
ExprASTPtr NumberExprOperation::operator_mul(ASTContext ctx, ExprASTPtr lval, ExprASTPtr rval)
{
	llvm::Value * LHS =	lval->getval(ctx);
	llvm::Value * RHS =	rval->getval(ctx);
	llvm::IRBuilder<> builder(ctx.block);
	llvm::Value * result;

	result = builder.CreateMul(LHS,RHS); 
	return lval->type(ctx)->createtemp(ctx,result,NULL);
}
 
ExprASTPtr NumberExprOperation::operator_div(ASTContext ctx, ExprASTPtr lval, ExprASTPtr rval)
{
	llvm::Value * LHS =	lval->getval(ctx);
	llvm::Value * RHS =	rval->getval(ctx);
	llvm::IRBuilder<> builder(ctx.block);
	llvm::Value * result = builder.CreateSDiv(LHS,RHS);
 
	return lval->type(ctx)->createtemp(ctx,result,NULL);
}

ExprASTPtr NumberExprOperation::operator_comp(ASTContext ctx, MathOperator op, ExprASTPtr lval, ExprASTPtr rval)
{
	llvm::Value * LHS =	lval->getval(ctx);
	llvm::Value * RHS =	rval->getval(ctx);
	auto t1 = LHS->getType();
	auto t2 = RHS->getType();

	llvm::IRBuilder<> builder(ctx.block);
	if (t1 != t2)
	{
		LHS = builder.CreateBitCast(LHS,qbc::getplatformlongtype()->getPointerTo());
		RHS = builder.CreateBitCast(RHS,qbc::getplatformlongtype()->getPointerTo());
	}
	llvm::Value * result;

	switch(op){
		case OPERATOR_LESS:
			result = builder.CreateICmpSLT(LHS,RHS);
			break;
		case OPERATOR_LESSEQU:
			result = builder.CreateICmpSLE(LHS,RHS);
			break;
		case OPERATOR_GREATER:
			result = builder.CreateICmpSGT(LHS,RHS);
			break;
		case OPERATOR_GREATEREQUL:
			result = builder.CreateICmpSGE(LHS,RHS);
			break;
		case OPERATOR_EQUL:
			result = builder.CreateICmpEQ(LHS,RHS);
			break;
	}

	//TODO , 构造临时 Number 对象.
	return lval->type(ctx)->createtemp(ctx,result,NULL);
}

ExprASTPtr StringExprOperation::operator_comp(ASTContext ctx,MathOperator op, ExprASTPtr lval, ExprASTPtr rval)
{
	llvm::Value * LHS =	lval->getval(ctx);
	llvm::Value * RHS =	rval->getval(ctx);
	llvm::IRBuilder<> builder(ctx.block);
	llvm::Value * result;

	switch(op){
		case OPERATOR_EQUL:{// call strcmp
			llvm::FunctionCallee func_strcmp = qbc::getbuiltinprotype(ctx,"strcmp");

			result = builder.CreateCall(func_strcmp, {LHS, RHS});

			result = builder.CreateIntCast(result, qbc::getplatformlongtype(), true);
			result = builder.CreateICmpEQ(result, qbc::getconstlong(0));
		}
		break;
		default:
			debug("string comp not supported");
			exit(1);

	}

	return NumberExprTypeAST::GetNumberExprTypeAST()->createtemp(ctx,result,NULL);
}


ExprASTPtr ArrayExprOperation::operator_call(ASTContext ctx, NamedExprASTPtr target, ExprListASTPtr callargslist)
{
	llvm::IRBuilder<>	builder(ctx.block);
	debug("array index\n");


	llvm::Value * arrayptr = target->getptr(ctx);

	llvm::Value * index = callargslist->expression_list.begin()->get()->getval(ctx);


	llvm::FunctionCallee func_qb_array_at = qbc::getbuiltinprotype(ctx,"btr_qbarray_at");

	llvm::Value * tmpval = builder.CreateCall(func_qb_array_at, {arrayptr, index});

	ArrayExprTypeAST * realtarget =dynamic_cast<ArrayExprTypeAST*>(target->nameresolve(ctx)->type.get());

	debug("realtarget is %p\n",realtarget);

	ExprASTPtr tmp = realtarget->elementtype->createtemp(ctx,NULL, tmpval);
	debug("array index,  little tmp created as %p\n",tmp.get());
	return tmp;
	exit(100);
}


ExprASTPtr FunctionExprOperation::operator_call(ASTContext ctx,NamedExprASTPtr calltarget,ExprListASTPtr callargs)
{
	llvm::IRBuilder<> builder(ctx.llvmfunc->getContext());
	builder.SetInsertPoint(ctx.block);

	// call functions TODO
	llvm::Value * ret = NULL;



	DimAST * funcdim = calltarget->nameresolve(ctx);

	llvm::FunctionCallee llvmfunc;

	llvmfunc = dynamic_cast<CallableExprTypeAST*>(funcdim)->defaultprototype(ctx,calltarget->ID->ID);


	std::vector<llvm::Value*> args;
	if(callargs && callargs->expression_list.size() )
	{
		for(ExprASTPtr expr : callargs->expression_list)
		{
			args.push_back( expr->getval(ctx) );
		}
	}

	return calltarget->type(ctx)->createtemp( ctx, builder.CreateCall(llvmfunc,args) ,NULL);
}

ExprASTPtr VoidExprTypeAST::createtemp(ASTContext, llvm::Value*, llvm::Value* ptr)
{
	// just a hack for that! not we can't actullay allocate VoidType.
	return ExprASTPtr(new ConstNumberExprAST(0));
}

ExprASTPtr NumberExprTypeAST::createtemp(ASTContext ctx, llvm::Value* val , llvm::Value *ptr)
{
	return boost::make_shared<TempNumberExprAST>(ctx,val,ptr);
}

ExprASTPtr StringExprTypeAST::createtemp(ASTContext ctx, llvm::Value* val , llvm::Value *ptr)
{
    return boost::make_shared<TempStringExprAST>(ctx,val,ptr);
}

ExprASTPtr ArrayExprTypeAST::createtemp(ASTContext ctx, llvm::Value*v , llvm::Value *ptr)
{
	debug("allocate for ArrayExprTypeAST , elementtype is\n");//, elementtype->name(ctx).c_str());
	exit(1);//	return this->elementtype->createtemp(ctx,v);
}

//TODO
ExprASTPtr CallableExprTypeAST::createtemp(ASTContext ctx, llvm::Value*v , llvm::Value *ptr)
{
	return this->returntype->createtemp(ctx,v,ptr);
}

