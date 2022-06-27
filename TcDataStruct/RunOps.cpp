#include "RunOps.h"
#include "AnagameRunners.h"
#include "TStringVariable.h"

void AnagameAssembly::Nothing(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{

}

void AnagameAssembly::Line(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	TrecSubPointer<TVariable, AnagameRunner> aRunner = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, AnagameRunner>(runner);
	aRunner->SetCurrentLine(binaryStack->Pop1());
}

void AnagameAssembly::Statement(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	TrecSubPointer<TVariable, AnagameRunner> aRunner = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, AnagameRunner>(runner);
	aRunner->SetCurrentCodeLine(binaryStack->Pop1());
}

void AnagameAssembly::AddFF(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	UINT u1 = binaryStack->Pop1(), u2 = binaryStack->Pop1();
	float* f1 = (float*) &u1, *f2 = (float*) &u2;
	*f1 = *f1 + *f2;
	binaryStack->Push(u1);
}

void AnagameAssembly::AddDD(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	ULONG64 u1 = binaryStack->Pop2(), u2 = binaryStack->Pop2();
	double* f1 = (double*)&u1, * f2 = (double*)&u2;
	*f1 = *f1 + *f2;
	binaryStack->Push(u1);
}

void AnagameAssembly::AddII(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	UINT u1 = binaryStack->Pop1(), u2 = binaryStack->Pop1();
	int* f1 = (int*)&u1, * f2 = (int*)&u2;
	*f1 = *f1 + *f2;
	binaryStack->Push(u1);
}

void AnagameAssembly::AddUU(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	binaryStack->Push(binaryStack->Pop1() + binaryStack->Pop1());
}

void AnagameAssembly::AddLL(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	ULONG64 u1 = binaryStack->Pop2(), u2 = binaryStack->Pop2();
	LONG64* f1 = (LONG64*)&u1, * f2 = (LONG64*)&u2;
	*f1 = *f1 + *f2;
	binaryStack->Push(u1);
}

void AnagameAssembly::AddULUL(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	binaryStack->Push(binaryStack->Pop2() + binaryStack->Pop2());
}

void AnagameAssembly::AddOO(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
}

void AnagameAssembly::SubFF(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	UINT u1 = binaryStack->Pop1(), u2 = binaryStack->Pop1();
	float* f1 = (float*)&u1, * f2 = (float*)&u2;
	*f1 = *f2 - *f1;
	binaryStack->Push(u1);
}

void AnagameAssembly::SubDD(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	ULONG64 u1 = binaryStack->Pop2(), u2 = binaryStack->Pop2();
	double* f1 = (double*)&u1, * f2 = (double*)&u2;
	*f1 = *f2 - *f1;
	binaryStack->Push(u1);
}

void AnagameAssembly::SubII(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	UINT u1 = binaryStack->Pop1(), u2 = binaryStack->Pop1();
	int* f1 = (int*)&u1, * f2 = (int*)&u2;
	*f1 = *f2 - *f1;
	binaryStack->Push(u1);
}

void AnagameAssembly::SubUU(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	UINT u1 = binaryStack->Pop1(), u2 = binaryStack->Pop1();
	binaryStack->Push(u2 - u1);
}

void AnagameAssembly::SubLL(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	ULONG64 u1 = binaryStack->Pop2(), u2 = binaryStack->Pop2();
	LONG64* f1 = (LONG64*)&u1, *f2 = (LONG64*)&u2;
	*f1 = *f2 - *f1;
	binaryStack->Push(u1);
}

void AnagameAssembly::SubULUL(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	ULONG64 u1 = binaryStack->Pop2(), u2 = binaryStack->Pop2();
	binaryStack->Push(u2 - u1);
}

void AnagameAssembly::SubOO(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
}


void AnagameAssembly::MulFF(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	UINT u1 = binaryStack->Pop1(), u2 = binaryStack->Pop1();
	float* f1 = (float*)&u1, * f2 = (float*)&u2;
	*f1 = *f2 * *f1;
	binaryStack->Push(u1);
}

void AnagameAssembly::MulDD(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	ULONG64 u1 = binaryStack->Pop2(), u2 = binaryStack->Pop2();
	double* f1 = (double*)&u1, * f2 = (double*)&u2;
	*f1 = *f2 * *f1;
	binaryStack->Push(u1);
}

void AnagameAssembly::MulII(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	UINT u1 = binaryStack->Pop1(), u2 = binaryStack->Pop1();
	int* f1 = (int*)&u1, * f2 = (int*)&u2;
	*f1 = *f2 * *f1;
	binaryStack->Push(u1);
}

void AnagameAssembly::MulUU(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	UINT u1 = binaryStack->Pop1(), u2 = binaryStack->Pop1();
	binaryStack->Push(u2 * u1);
}

void AnagameAssembly::MulLL(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	ULONG64 u1 = binaryStack->Pop2(), u2 = binaryStack->Pop2();
	LONG64* f1 = (LONG64*)&u1, * f2 = (LONG64*)&u2;
	*f1 = *f2 * *f1;
	binaryStack->Push(u1);
}

void AnagameAssembly::MulULUL(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	ULONG64 u1 = binaryStack->Pop2(), u2 = binaryStack->Pop2();
	binaryStack->Push(u2 * u1);
}

void AnagameAssembly::MulOO(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
}


void AnagameAssembly::DivFF(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	UINT u1 = binaryStack->Pop1(), u2 = binaryStack->Pop1();
	float* f1 = (float*)&u1, * f2 = (float*)&u2;
	*f1 = *f2 / *f1;
	binaryStack->Push(u1);
}

void AnagameAssembly::DivDD(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	ULONG64 u1 = binaryStack->Pop2(), u2 = binaryStack->Pop2();
	double* f1 = (double*)&u1, * f2 = (double*)&u2;
	*f1 = *f2 / *f1;
	binaryStack->Push(u1);
}

void AnagameAssembly::DivII(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	UINT u1 = binaryStack->Pop1(), u2 = binaryStack->Pop1();
	int* f1 = (int*)&u1, * f2 = (int*)&u2;
	*f1 = *f2 * *f1;
	binaryStack->Push(u1);
}

void AnagameAssembly::DivUU(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	UINT u1 = binaryStack->Pop1(), u2 = binaryStack->Pop1();
	binaryStack->Push(u2 / u1);
}

void AnagameAssembly::DivLL(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	ULONG64 u1 = binaryStack->Pop2(), u2 = binaryStack->Pop2();
	LONG64* f1 = (LONG64*)&u1, * f2 = (LONG64*)&u2;
	*f1 = *f2 / *f1;
	binaryStack->Push(u1);
}

void AnagameAssembly::DivULUL(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	ULONG64 u1 = binaryStack->Pop2(), u2 = binaryStack->Pop2();
	binaryStack->Push(u2 / u1);
}

void AnagameAssembly::DivOO(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
}

void AnagameAssembly::ModII(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	UINT u1 = binaryStack->Pop1(), u2 = binaryStack->Pop1();
	int* f1 = (int*)&u1, * f2 = (int*)&u2;
	*f1 = *f2 % *f1;
	binaryStack->Push(u1);
}

void AnagameAssembly::ModUU(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	UINT u1 = binaryStack->Pop1(), u2 = binaryStack->Pop1();
	binaryStack->Push(u2 % u1);
}

void AnagameAssembly::ModLL(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	ULONG64 u1 = binaryStack->Pop2(), u2 = binaryStack->Pop2();
	LONG64* f1 = (LONG64*)&u1, * f2 = (LONG64*)&u2;
	*f1 = *f2 % *f1;
	binaryStack->Push(u1);
}

void AnagameAssembly::ModULUL(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	ULONG64 u1 = binaryStack->Pop2(), u2 = binaryStack->Pop2();
	binaryStack->Push(u2 % u1);
}

void AnagameAssembly::ModOO(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
}

void AnagameAssembly::CastIF(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	UINT val = binaryStack->Pop1();
	int* iVal = (int*)&val;
	float fVal = static_cast<float>(*iVal);
	UINT* nVal = (UINT*)&fVal;
	binaryStack->Push(*nVal);
}

void AnagameAssembly::CastFI(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	UINT val = binaryStack->Pop1();
	float* iVal = (float*)&val;
	int fVal = static_cast<int>(*iVal);
	UINT* nVal = (UINT*)&fVal;
	binaryStack->Push(*nVal);
}

void AnagameAssembly::CastFD(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	UINT val = binaryStack->Pop1();
	float* iVal = (float*)&val;
	double fVal = static_cast<double>(*iVal);
	ULONG64* nVal = (ULONG64*)&fVal;
	binaryStack->Push(*nVal);
}

void AnagameAssembly::CastDF(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	ULONG64 val = binaryStack->Pop2();
	double* dVal = (double*)&val;
	float fVal = static_cast<double>(*dVal);
	UINT* uVal = (UINT*)&fVal;
	binaryStack->Push(*uVal);
}

void AnagameAssembly::CastIU(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	UINT val = binaryStack->Pop1();
	int* iVal = (int*)&val;
	UINT fVal = static_cast<UINT>(*iVal);
	binaryStack->Push(fVal);
}

void AnagameAssembly::CastUI(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	UINT val = binaryStack->Pop1();
	int fVal = static_cast<int>(val);
	UINT* nVal = (UINT*)fVal;
	binaryStack->Push(*nVal);
}

void AnagameAssembly::CastUUL(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	binaryStack->Push(static_cast<ULONG64>(binaryStack->Pop1()));
}

void AnagameAssembly::CastULU(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	binaryStack->Push(static_cast<UINT>(binaryStack->Pop2()));
}

void AnagameAssembly::CastIL(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	UINT val = binaryStack->Pop1();
	int* iVal = (int*)&val;
	LONG64 fVal = static_cast<LONG64>(*iVal);
	ULONG64* nVal = (ULONG64*)&fVal;
	binaryStack->Push(*nVal);
}

void AnagameAssembly::CastLI(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	ULONG64 val = binaryStack->Pop2();
	LONG64* dVal = (LONG64*)&val;
	int fVal = static_cast<int>(*dVal);
	UINT* uVal = (UINT*)&fVal;
	binaryStack->Push(*uVal);
}

void AnagameAssembly::Push1(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	binaryStack->Push(op1);
}

void AnagameAssembly::Push2(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	ULONG64 op = op1;
	binaryStack->Push((op << 4) + op2);
}

void AnagameAssembly::PushB(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	UINT val = 0;
	if (!binaryStack->GetValueFromTop(op1, val))
	{
		ret.errorMessage.Format("INTERNAL ERROR! Stack was of insufficient Size %i when 'PushB %i' was activated!", binaryStack->GetSize(), op1);
		ret.returnCode = ret.ERR_INTERNAL;
		TrecSubPointer<TVariable, AnagameRunner> aRunner = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, AnagameRunner>(runner);
		// To Do: Add Error Information about how this error formed
		return;
	}
	binaryStack->Push(val);
}

void AnagameAssembly::PopN(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	for (UINT Rust = 0; Rust < op1; Rust++)
		binaryStack->Pop1();
}

void AnagameAssembly::PushO(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	TrecPointer<TVariable> val;
	if (!objectStack->GetValueFromTop(op1, val))
	{
		ret.errorMessage.Format("INTERNAL ERROR! Stack was of insufficient Size %i when 'PushO %i' was activated!", binaryStack->GetSize(), op1);
		ret.returnCode = ret.ERR_INTERNAL;
		TrecSubPointer<TVariable, AnagameRunner> aRunner = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, AnagameRunner>(runner);
		// To Do: Add Error Information about how this error formed
		return;
	}
	objectStack->Push(val);
}

void AnagameAssembly::PopO(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	for (UINT Rust = 0; Rust < op1; Rust++)
		objectStack->Pop();
}

void AnagameAssembly::PushStr(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	TrecSubPointer<TVariable, AnagameRunner> aRunner = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, AnagameRunner>(runner);
	TString val;
	if (!aRunner->GetStringAt(val, op1))
	{
		ret.errorMessage.Format("INTERNAL ERROR! String List was of insufficient Size when 'PushStr %i' was activated!", op1);
		ret.returnCode = ret.ERR_INTERNAL;
		// To Do: Add Error Information about how this error formed
		return;
	}
	objectStack->Push(TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(val));
}

void AnagameAssembly::StrCond(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	// To-Do: Find mechanisms to get Language Ops into Runner Ops, look for String condensation Method
}

void AnagameAssembly::JumpCond(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	UINT result = binaryStack->Pop1();
	TrecSubPointer<TVariable, AnagameRunner> aRunner = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, AnagameRunner>(runner);

	bool works = aRunner->Jump(result ? op1 : op2);
	if (!works)
	{
		ret.errorMessage.Format("INTERNAL ERROR! 'Jump %i' Does not point to a valid Jump Point!", result ? op1: op2);
		ret.returnCode = ret.ERR_INTERNAL;
		// To Do: Add Error Information about how this error formed
		return;
	}
}

void AnagameAssembly::JumpMark(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
}

void AnagameAssembly::Jump(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	TrecSubPointer<TVariable, AnagameRunner> aRunner = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, AnagameRunner>(runner);

	bool works = aRunner->Jump(op1);
	if (!works)
	{
		ret.errorMessage.Format("INTERNAL ERROR! 'Jump %i' Does not point to a valid Jump Point!", op1);
		ret.returnCode = ret.ERR_INTERNAL;
		// To Do: Add Error Information about how this error formed
		return;
	}
}

void AnagameAssembly::JumpF(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	TrecSubPointer<TVariable, AnagameRunner> aRunner = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, AnagameRunner>(runner);

	bool works = aRunner->Jump(op1);
	if (!works)
	{
		ret.errorMessage.Format("INTERNAL ERROR! 'Jump %i' Does not point to a valid Jump Point!", op1);
		ret.returnCode = ret.ERR_INTERNAL;
		// To Do: Add Error Information about how this error formed
		return;
	}
}

void AnagameAssembly::AssessTrue(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	auto obj = objectStack->Pop();
	// To-Do: Find mechanisms to get Language Ops into Runner Ops, look for String condensation Method

}

void AnagameAssembly::JumpErr(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	TrecSubPointer<TVariable, AnagameRunner> aRunner = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, AnagameRunner>(runner);

	bool works = aRunner->SetErrorJump(op1);
	if (!works)
	{
		ret.errorMessage.Format("INTERNAL ERROR! 'JumpErr %i' Does not point to a valid Jump Point!", op1);
		ret.returnCode = ret.ERR_INTERNAL;
		// To Do: Add Error Information about how this error formed
		return;
	}
}

void AnagameAssembly::Clearrr(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	ret.errorMessage.Empty();
	ret.returnCode = 0;
}

void AnagameAssembly::UnJumpErr(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2)
{
	TrecSubPointer<TVariable, AnagameRunner> aRunner = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, AnagameRunner>(runner);
	aRunner->SetErrorJump(0);
}
