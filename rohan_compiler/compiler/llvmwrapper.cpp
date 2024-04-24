﻿/*
Author: Rohan Singh, Benjamin Luo and William Cankar
QBASIC Abstruct Syntax Tree
*/

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include "qbc.h"
#include "ast.hpp"
#include "llvmwrapper.hpp"


namespace qbc{

static unsigned sizeofint()
{
	return sizeof(int)*8;
}

static unsigned sizeoflong()
{
	return sizeof(long)*8;
}

llvm::LLVMContext getGlobalContext;

llvm::Value * getnull()
{
	return llvm::ConstantPointerNull::get(llvm::Type::getInt8PtrTy(getGlobalContext));
}

llvm::Value * getconstfalse()
{
	return llvm::ConstantInt::get(getGlobalContext,llvm::APInt(1,0,true));
}

llvm::Value * getconsttrue()
{
	return llvm::ConstantInt::get(getGlobalContext,llvm::APInt(1,1,true));
}

llvm::Value * getconstint(int v)
{
	return llvm::ConstantInt::get(getGlobalContext,llvm::APInt(sizeofint(),(uint64_t)v,true));
}

llvm::Value * getconstlong(long v)
{
	return llvm::ConstantInt::get(getGlobalContext,llvm::APInt(sizeoflong(),(uint64_t)v,true));
}

llvm::Type * getbooltype()
{
	return llvm::Type::getInt1Ty(getGlobalContext);
}

llvm::Type * getplatformlongtype()
{
	switch(sizeof(long)){
		case 8:
			return llvm::Type::getInt64Ty(getGlobalContext);
		case 4:
			return llvm::Type::getInt32Ty(getGlobalContext);
		case 2:
			return llvm::Type::getInt16Ty(getGlobalContext);
	}
}

#define GETBUILTINTYPE_ENTER()	\
	llvm::IRBuilder<> builder(ctx.block); std::vector<llvm::Type *> args

#define	BUILTINTYPE_DEFINE(x ,  ret , block )	\
	static llvm::FunctionCallee getbuiltinprotype_##x(ASTContext ctx) \
	{\
		GETBUILTINTYPE_ENTER(); \
		block \
		llvm::FunctionCallee func = ctx.module->getOrInsertFunction(#x, \
			llvm::FunctionType::get(builder.get##ret##Ty(), args,false)); \
		return func; \
	}

static llvm::FunctionCallee getbuiltinprotype_printf(ASTContext ctx)
{
	GETBUILTINTYPE_ENTER();
	args.push_back(builder.getInt8PtrTy());

	auto printf_func = ctx.module->getOrInsertFunction("printf",
										llvm::FunctionType::get(builder.getInt32Ty(), args,
		/*必须为true, 这样才能接受可变参数*/true));

	return printf_func;
}

static llvm::FunctionCallee getbuiltinprotype_brt_print(ASTContext ctx)
{
	GETBUILTINTYPE_ENTER();
	args.push_back(getplatformlongtype());

	auto brt_print =
			ctx.module->getOrInsertFunction("brt_print",
										llvm::FunctionType::get(builder.getInt32Ty(), args,
		/*必须为true, 这样才能接受可变参数*/true));

	return brt_print;
}

BUILTINTYPE_DEFINE(malloc , Int8Ptr , {
	args.push_back(getplatformlongtype());}  )

BUILTINTYPE_DEFINE(calloc , Int8Ptr , {
	args.push_back(getplatformlongtype());
	args.push_back(getplatformlongtype());}  )

BUILTINTYPE_DEFINE(free , Void , {args.push_back(builder.getInt8PtrTy());}  )

BUILTINTYPE_DEFINE(strdup , Int8Ptr , {args.push_back(builder.getInt8PtrTy());}  )

static llvm::FunctionCallee getbuiltinprotype_strlen(ASTContext ctx)
{
	GETBUILTINTYPE_ENTER();

	args.push_back(builder.getInt8PtrTy());

	llvm::FunctionCallee func = ctx.module->getOrInsertFunction("strlen",
										llvm::FunctionType::get(getplatformlongtype(), args,false));
	return func;
}

BUILTINTYPE_DEFINE(strcpy , Int8Ptr , {
	args.push_back(builder.getInt8PtrTy());
	args.push_back(builder.getInt8PtrTy());}  )

BUILTINTYPE_DEFINE(strcat , Int8Ptr , {
	args.push_back(builder.getInt8PtrTy());
	args.push_back(builder.getInt8PtrTy());}  )

BUILTINTYPE_DEFINE(strcmp , Int32 , {
	args.push_back(builder.getInt8PtrTy());
	args.push_back(builder.getInt8PtrTy());}  )

BUILTINTYPE_DEFINE(btr_qbarray_new , Void , {
	args.push_back(builder.getInt8PtrTy());
	args.push_back(getplatformlongtype());}  )


BUILTINTYPE_DEFINE(btr_qbarray_free , Void , {
	args.push_back(builder.getInt8PtrTy());}  )

BUILTINTYPE_DEFINE(btr_qbarray_at , Int8Ptr , {
	args.push_back(builder.getInt8PtrTy());
	args.push_back(getplatformlongtype());}  )

#undef BUILTINTYPE_DEFINE
#undef GETBUILTINTYPE_ENTER

// 从字符串获得标准C库和内置BRT库的标准声明.
llvm::FunctionCallee getbuiltinprotype(ASTContext ctx, const std::string name)
{
	llvm::FunctionCallee retfunc = ctx.module->getFunction(name);
#define			RETURNBUILTINENTRY(x)	\
	if(name == #x ) { return getbuiltinprotype_##x (ctx); }

	if(!retfunc){ // 根据函数名字生成.
		RETURNBUILTINENTRY(printf)
		RETURNBUILTINENTRY(brt_print)
		RETURNBUILTINENTRY(malloc)
		RETURNBUILTINENTRY(calloc)
		RETURNBUILTINENTRY(free)
		RETURNBUILTINENTRY(strdup)
		RETURNBUILTINENTRY(strlen)
		RETURNBUILTINENTRY(strcpy)
		RETURNBUILTINENTRY(strcat)
		RETURNBUILTINENTRY(strcmp)
		RETURNBUILTINENTRY(btr_qbarray_new)
		RETURNBUILTINENTRY(btr_qbarray_free)
		RETURNBUILTINENTRY(btr_qbarray_at)

		printf("no define for %s yet\n",name.c_str());
		exit(1);
	}
#undef RETURNBUILTINENTRY
	return retfunc;
}

}
