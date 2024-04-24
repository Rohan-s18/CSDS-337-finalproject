/*
Author: Rohan Singh, Benjamin Luo and William Cankar
QBASIC Abstruct Syntax Tree
*/ 
#endif

class ReferenceAST : public AST
{
public:
	std::string ID; //the name of the variable
    ReferenceAST( std::string * tID );
    virtual ~ReferenceAST(){};
};
typedef boost::shared_ptr<ReferenceAST> ReferenceASTPtr;

// TODO ? how?
class MemberReferenceAST : public ReferenceAST
{
    MemberReferenceAST(std::string* members);
};

class ExprOperation;
class ExprAST;
class PointerTypeAST;

typedef boost::shared_ptr<ExprAST>	ExprASTPtr;
typedef boost::shared_ptr<PointerTypeAST> PointerTypeASTPtr;

// type of the expression
class ExprTypeAST : public AST
{
	std::string		_typename;
	size_t			_size; // sizeof() 
public:
	// name of type
	virtual std::string name(ASTContext){return _typename;};

	virtual llvm::Type	* llvm_type(ASTContext ctx) = 0;

	// allocate one on stack , if possible , name it
	virtual llvm::Value * Alloca(ASTContext ctx, const std::string _name){return NULL;}

	// dereference the pointer 
	virtual llvm::Value * deref(ASTContext ctx, llvm::Value *v);

	// initalize the default vaule
	virtual void initalize(ASTContext, llvm::Value * Ptr) {};

	// generate the call to deconstruction function here!
	virtual void destory(ASTContext, llvm::Value * Ptr) {};

	virtual	size_t size(){return _size;};

	// get operation table
	virtual ExprOperation * getop() = 0;

	virtual	PointerTypeASTPtr	getpointetype() =0;

	// create a temp from llvm::Valut *
	virtual ExprASTPtr		createtemp(ASTContext ,llvm::Value * , llvm::Value *ptr)=0;

public:
    ExprTypeAST(){}
    ExprTypeAST( size_t size , const std::string __typename );
    virtual ~ExprTypeAST(){}; // do nothing
protected:
    void printf(const char* arg1);
};
typedef boost::shared_ptr<ExprTypeAST>		ExprTypeASTPtr;

// expression
class ExprAST: public AST //
{
public:
#if 0
#endif
	virtual ExprTypeASTPtr type(ASTContext ) = 0;
#if 0
#endif
	virtual llvm::Value *getval(ASTContext) = 0;
	virtual llvm::Value *getptr(ASTContext) = 0;

    virtual ~ExprAST(){}
};


#if 0
#endif
class EmptyExprAST : public ExprAST
{
public:
	virtual ExprTypeASTPtr type(ASTContext );

    virtual llvm::Value* getval(ASTContext ){ return 0;}
    virtual llvm::Value* getptr(ASTContext ){exit(132);};
};
#if 0

#endif
class TempExprAST : public ExprAST{
	ExprTypeASTPtr _type;
public:
	llvm::Value * val;
	llvm::Value	* ptr;
	ASTContext ctx;
	
    virtual ExprTypeASTPtr type(ASTContext ctx){return _type;}

	TempExprAST(ASTContext _ctx,llvm::Value * _val, llvm::Value *ptr,ExprTypeASTPtr type);
    virtual llvm::Value* getval(ASTContext ) { return val;}
    virtual llvm::Value* getptr(ASTContext ){return ptr;}
    virtual ~TempExprAST(){};
};


class TempNumberExprAST : public TempExprAST
{
public:
    TempNumberExprAST(ASTContext ctx,llvm::Value * numberresult,llvm::Value * pointertoval);
    virtual llvm::Value* getval(ASTContext );
};

class ConstNumberExprAST : public ExprAST
{
	int v;
public:
	ConstNumberExprAST( long num){ v = num;};
public:
	
    virtual ExprTypeASTPtr type(ASTContext );

    virtual llvm::Value* getval(ASTContext );
	virtual llvm::Value* getptr(ASTContext ){exit(128);};
};

class ConstStringExprAST :public ExprAST
{
	llvm::Value *		val;
	std::string			str;
public:
	ConstStringExprAST(const std::string _str);
	virtual ExprTypeASTPtr type(ASTContext );
    virtual llvm::Value* getval(ASTContext );
	virtual llvm::Value* getptr(ASTContext ){exit(129);};
};

class TempStringExprAST : public TempExprAST
{
public:
    TempStringExprAST(ASTContext ctx,llvm::Value * result , llvm::Value *ptr);
    virtual ~TempStringExprAST();
};

#if 0
#endif
class DimAST;
class NamedExprAST : public ExprAST
{
public:
	ReferenceASTPtr			ID;

	NamedExprAST(ReferenceAST * _ID);

	virtual ExprTypeASTPtr	type(ASTContext ) = 0;

	virtual llvm::Value*	getptr(ASTContext) = 0;

	// resolve and get where it got defined
	virtual DimAST* nameresolve(ASTContext ctx);

    virtual ~NamedExprAST(){}
};
typedef boost::shared_ptr<NamedExprAST> NamedExprASTPtr;
#if 0
// 变量引用. 如果变量是个函数, 则是函数指针类型哦 ~ 
// 如果变量是数组, 则是数组类型哦.        
#endif
class VariableExprAST : public NamedExprAST
{
public:
    VariableExprAST(ReferenceAST* ID);
	virtual llvm::Value* getval(ASTContext );

    virtual llvm::Value* getptr(ASTContext );

    virtual DimAST* nameresolve(ASTContext ctx);

	virtual ExprTypeASTPtr type(ASTContext ctx);
    virtual ~VariableExprAST(){}
};

// expression of math 
class CalcExprAST : public ExprAST{
	MathOperator	op;
	ExprASTPtr		lval,rval;
	ExprASTPtr		result; // cache the result :)
public:
	CalcExprAST(ExprAST * , MathOperator op , ExprAST * );
	virtual ExprTypeASTPtr type(ASTContext);
    virtual llvm::Value* getval(ASTContext);
	virtual llvm::Value* getptr(ASTContext){exit(177);}
};

#if 0
#endif
class AssignmentExprAST : public ExprAST
{
	NamedExprASTPtr lval;
	ExprASTPtr 		rval;
public:
	AssignmentExprAST(NamedExprAST* , ExprAST *);
	
    virtual ExprTypeASTPtr type(ASTContext);
    virtual llvm::Value* getval(ASTContext);
    virtual llvm::Value* getptr(ASTContext ){exit(127);};
};

typedef boost::shared_ptr<AssignmentExprAST> AssignmentExprASTPtr;

class ExprListAST : public AST //
{
public:
	std::vector<ExprASTPtr>	expression_list;

    ExprListAST(){}
	void Append(ExprAST* exp);
    virtual ~ExprListAST(){};
};
typedef boost::shared_ptr<ExprListAST> ExprListASTPtr;

class CallExprAST : public ExprAST
{
	NamedExprASTPtr				calltarget; // should be callable
	ExprListASTPtr				callargs;
public:
	CallExprAST(NamedExprAST* , ExprListAST * exp_list = NULL);
	virtual ExprTypeASTPtr	type(ASTContext);

    virtual llvm::Value* getptr(ASTContext); // cann't get the address
    virtual llvm::Value* getval(ASTContext);
};

typedef boost::shared_ptr<CallExprAST>	CallExprASTPtr;


// wrapper class
class ExprType {
	ExprTypeASTPtr	_type;
public:
	ExprType(ExprTypeASTPtr type):_type(type){}
	operator	ExprTypeASTPtr(){ExprTypeASTPtr tmp = _type; delete this;  return tmp;}
};


class VoidExprTypeAST : public ExprTypeAST{
public:
	static ExprTypeASTPtr GetVoidExprTypeAST();
    virtual llvm::Type* llvm_type(ASTContext ctx);
    virtual llvm::Value* Alloca(ASTContext ctx, const std::string _name){return NULL;}
    virtual ExprOperation* getop(){return NULL;}
    virtual PointerTypeASTPtr getpointetype();
    virtual ExprASTPtr createtemp(ASTContext , llvm::Value* , llvm::Value *ptr );
};

class NumberExprTypeAST :public ExprTypeAST {
public:
    NumberExprTypeAST();
	
    virtual llvm::Type* llvm_type(ASTContext ctx);

    virtual size_t size(){return sizeof(long);};
	
	virtual llvm::Value* Alloca(ASTContext ctx, const std::string _name);
    virtual llvm::Value* deref(ASTContext ctx, llvm::Value* v);

    virtual ExprOperation* getop();

    virtual PointerTypeASTPtr getpointetype();

    virtual ExprASTPtr createtemp(ASTContext ctx,llvm::Value* val , llvm::Value *ptr);	

	static ExprTypeASTPtr   GetNumberExprTypeAST();
};

class StringExprTypeAST : public ExprTypeAST
{
public:
    StringExprTypeAST();
    virtual llvm::Type* llvm_type(ASTContext ctx);

    virtual size_t size(){return sizeof(size_t);}

	virtual llvm::Value* Alloca(ASTContext ctx, const std::string _name);
    virtual ExprOperation* getop();
    virtual PointerTypeASTPtr getpointetype();;
	
    virtual void destory(ASTContext , llvm::Value* Ptr);

    virtual ExprASTPtr createtemp(ASTContext , llvm::Value*  , llvm::Value *ptr);
	
	static ExprTypeASTPtr GetStringExprTypeAST();
};

class ArrayExprOperation;
class CallExprAST;
class ArrayExprTypeAST : public ExprTypeAST
{
	/**
	 * NOTE:
	 *
	 * An Array is of the type  struct QBArray
	 **/
	ExprTypeASTPtr	elementtype;
	friend class ArrayExprOperation;
	friend class CallExprAST;
public:
    ArrayExprTypeAST(ExprTypeASTPtr elementtype);
    virtual llvm::Type* llvm_type(ASTContext ctx);

    virtual size_t size(){return sizeof(struct QBArray);}

	virtual llvm::Value* Alloca(ASTContext ctx, const std::string _name);
    virtual ExprOperation* getop();
    virtual PointerTypeASTPtr getpointetype(){ ::printf("get pointer to type\n");exit(1);};
    virtual void destory(ASTContext , llvm::Value* Ptr);
    virtual ExprASTPtr createtemp(ASTContext , llvm::Value*  , llvm::Value *ptr);

public:
	static ExprTypeASTPtr create(ExprTypeASTPtr);
};
#if 0
//  函数对象类型. 这是基类
//  而一个函数声明本身也是一个 callable 类型
//  一个函数指针是 callable 类型
//  一个函数对象也是 callable 类型   
#endif
class CallableExprTypeAST : public ExprTypeAST{
	ExprTypeASTPtr	returntype;
	friend class FunctionDimAST;
	friend class CallExprAST;
public:
    CallableExprTypeAST(ExprTypeASTPtr	_returntype);
	static	llvm::FunctionCallee defaultprototype(ASTContext ctx,std::string functionname);
    virtual ExprOperation* getop();
	virtual PointerTypeASTPtr getpointetype(){::printf("get pointer to function\n");exit(1);};

    virtual llvm::Type* llvm_type(ASTContext ctx);
    virtual llvm::Value* Alloca(ASTContext ctx, const std::string _name);
    virtual ExprASTPtr createtemp(ASTContext , llvm::Value*  , llvm::Value *ptr);
};

class PointerTypeAST : public ExprTypeAST
{
	ExprTypeASTPtr		pointeetype;
public:
	PointerTypeAST(ExprTypeASTPtr _pointeetype);
    virtual ExprOperation* getop();
    virtual llvm::Type* llvm_type(ASTContext ctx){return llvm::Type::getInt8PtrTy(ctx.module->getContext());};
    virtual PointerTypeASTPtr getpointetype(){::printf("pointer to pointer\n");exit(1);};
    virtual ExprASTPtr createtemp(ASTContext ctx, llvm::Value* val , llvm::Value *ptr ){
		::printf("pointer to tmp  val");
		exit(1);
	}
};

class StructExprTypeAST : public ExprTypeAST{
	size_t	_size;
public:
    StructExprTypeAST(const std::string __typename);
    size_t size(){return _size;}
	size_t size(size_t newsize){_size = newsize;return _size;} // set size

public:
	ExprASTPtr createtemp(ASTContext , llvm::Value* , llvm::Value* ptr){ return {}; }
	ExprOperation* getop();
	PointerTypeASTPtr getpointetype(){ return {}; }
	llvm::Type* llvm_type(ASTContext ctx);

public:
	static ExprTypeASTPtr create( const std::string __typename);
};

/////////////// 一下类型未实现.

//  浮点运算. 在不支持浮点数的平台这个类要提交浮点模拟.
class FlatExprTypeAST : ExprTypeAST {

};


// 引用类型.
class ReferenceTypeAST : public ExprTypeAST
{

};

///////////////////////// 运算符支持.
class ExprOperation{
public:
	virtual	ExprASTPtr operator_assign(ASTContext , NamedExprASTPtr lval, ExprASTPtr rval);

	virtual ExprASTPtr operator_add(ASTContext , ExprASTPtr lval, ExprASTPtr rval);

	virtual ExprASTPtr operator_sub(ASTContext , ExprASTPtr lval, ExprASTPtr rval);

	virtual ExprASTPtr operator_mul(ASTContext, ExprASTPtr lval, ExprASTPtr rval);

	virtual ExprASTPtr operator_div(ASTContext, ExprASTPtr lval, ExprASTPtr rval);

	virtual	ExprASTPtr operator_comp(ASTContext, MathOperator op, ExprASTPtr lval,ExprASTPtr rval);

	virtual ExprASTPtr operator_call(ASTContext, NamedExprASTPtr target, ExprListASTPtr callargslist);
};

class NumberExprOperation : public ExprOperation {
    virtual ExprASTPtr operator_assign(ASTContext , NamedExprASTPtr lval, ExprASTPtr rval);
    virtual ExprASTPtr operator_add(ASTContext , ExprASTPtr lval, ExprASTPtr rval);
	virtual ExprASTPtr operator_sub(ASTContext , ExprASTPtr lval, ExprASTPtr rval);
    virtual ExprASTPtr operator_mul(ASTContext , ExprASTPtr lval, ExprASTPtr rval);
    virtual ExprASTPtr operator_div(ASTContext , ExprASTPtr lval, ExprASTPtr rval);
    virtual ExprASTPtr operator_comp(ASTContext , MathOperator op, ExprASTPtr lval, ExprASTPtr rval);
};

class StringExprOperation : public ExprOperation {
    virtual ExprASTPtr operator_assign(ASTContext , NamedExprASTPtr lval, ExprASTPtr rval);
    virtual ExprASTPtr operator_add(ASTContext , ExprASTPtr lval, ExprASTPtr rval);
    virtual ExprASTPtr operator_comp(ASTContext ctx, MathOperator op, ExprASTPtr lval, ExprASTPtr rval);
};

class ArrayExprOperation : public ExprOperation{
	virtual ExprASTPtr operator_assign(ASTContext ctx,NamedExprASTPtr lval,ExprASTPtr rval);
    virtual ExprASTPtr operator_call(ASTContext , NamedExprASTPtr target, ExprListASTPtr callargslist);	
};

class FunctionExprOperation : public ExprOperation{
    virtual ExprASTPtr operator_call(ASTContext , NamedExprASTPtr target, ExprListASTPtr callargslist);
};

class PointerTypeOperation: public ExprOperation{

};

class StructTypeOperation : public ExprOperation{
	
};