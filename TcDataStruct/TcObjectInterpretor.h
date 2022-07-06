#pragma once
#include "TcTypeInterpretor.h"
#include "TcExpression.h"
class TcObjectInterpretor :
    public TcRunner
{
public:


	class TcObjectStatement
	{
	protected:
		TrecPointer<TcExpression> expression;

	public:

		virtual void SetSubStatements(TrecSubPointer<TVariable, TcObjectInterpretor> next);
		virtual void SetNextStatement(TrecPointer<TcObjectStatement> next);
		virtual void SetExpression(TrecPointer<TcExpression> exp);
		virtual void Run(TrecSubPointer<TVariable, TcObjectInterpretor> runner, ReturnObject& ret);
	};

	class TcBlockStatement : public TcObjectStatement
	{
	protected:
		bool conditional,	// if true, use the expression to decide if it shoudl run or not
			repeat,			// Whether the block should repeat if the first run was done (if vs while loop)
			runFirst,		// Distinguishes between a regular while loop and a do_while loop
			negate;			// Whether to reverse the result of the expression (for things like an 'until' loop)

		TrecSubPointer<TVariable, TcObjectInterpretor> block;
		TrecPointer<TcObjectStatement> next;

		TString name;
	public:
		TcBlockStatement(const TString& name, bool conditional = true, bool repeat = false, bool runFirst = false, bool negate = false);

		virtual void SetSubStatements(TrecSubPointer<TVariable, TcObjectInterpretor> next);
		virtual void SetNextStatement(TrecPointer<TcObjectStatement> next);
		virtual void Run(TrecSubPointer<TVariable, TcObjectInterpretor> runner, ReturnObject& ret);
	};

	class TcForStatement : public TcBlockStatement
	{
	protected:
		TrecPointer<TcExpression> condition, loop;
	public:

		virtual void SetExpression(TrecPointer<TcExpression> exp);
		virtual void SetSubStatements(TrecSubPointer<TVariable, TcObjectInterpretor> next);
		virtual void SetNextStatement(TrecPointer<TcObjectStatement> next);
		virtual void Run(TrecSubPointer<TVariable, TcObjectInterpretor> runner, ReturnObject& ret);
	};

	class TcExceptionStatement : public TcBlockStatement
	{
	protected:
		bool isTry;
		TString errorName;
	public:
		virtual void SetExpression(TrecPointer<TcExpression> exp);
		virtual void SetSubStatements(TrecSubPointer<TVariable, TcObjectInterpretor> next);
		virtual void SetNextStatement(TrecPointer<TcObjectStatement> next);
		virtual void Run(TrecSubPointer<TVariable, TcObjectInterpretor> runner, ReturnObject& ret);
	};


protected:

	TDataArray<TrecPointer<TcObjectStatement>> statements;
	TrecSubPointer<TVariable, TcRunner> isTrue;
	TrecSubPointerSoft<TVariable, TcObjectInterpretor> parent, self;

	UINT yeildIndex;
	TrecPointerSoft<TEnvironment> environment;
public:

	virtual void SetSelf(TrecPointer<TVariable> self)override;

	TcObjectInterpretor(TrecPointerSoft<TEnvironment> environment, TrecSubPointer<TVariable, TcObjectInterpretor> parent);

	void SetTruthDetector(TrecSubPointer<TVariable, TcRunner> isTrue);

	void SetStatements(TDataArray<TrecPointer<TcObjectStatement>> statements);

	virtual ReturnObject Run() override;

	virtual void SetIntialVariables(TDataArray<TrecPointer<TVariable>>& params) override;

	virtual bool IsTruthful(TrecPointer<TVariable> var) override;
};

