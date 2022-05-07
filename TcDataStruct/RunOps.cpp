#include "RunOps.h"
#include "AnagameRunners.h"

void AnagameAssembly::Nothing(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner)
{

}

void AnagameAssembly::Line(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner)
{
	TrecSubPointer<TVariable, AnagameRunner> aRunner = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, AnagameRunner>(runner);
	aRunner->SetCurrentLine(binaryStack->Pop1());
}

void AnagameAssembly::Statement(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner)
{
	TrecSubPointer<TVariable, AnagameRunner> aRunner = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, AnagameRunner>(runner);
	aRunner->SetCurrentCodeLine(binaryStack->Pop1());
}

void AnagameAssembly::AddFF(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner)
{
	UINT u1 = binaryStack->Pop1(), u2 = binaryStack->Pop1();
	float* f1 = (float*) &u1, *f2 = (float*) &u2;
	*f1 = *f1 + *f2;
	binaryStack->Push(u1);
}

void AnagameAssembly::AddDD(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner)
{
	ULONG64 u1 = binaryStack->Pop2(), u2 = binaryStack->Pop2();
	double* f1 = (double*)&u1, * f2 = (double*)&u2;
	*f1 = *f1 + *f2;
	binaryStack->Push(u1);
}

void AnagameAssembly::AddII(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner)
{
	UINT u1 = binaryStack->Pop1(), u2 = binaryStack->Pop1();
	int* f1 = (int*)&u1, * f2 = (int*)&u2;
	*f1 = *f1 + *f2;
	binaryStack->Push(u1);
}

void AnagameAssembly::AddUU(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner)
{
	binaryStack->Push(binaryStack->Pop1() + binaryStack->Pop1());
}

void AnagameAssembly::AddLL(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner)
{
	ULONG64 u1 = binaryStack->Pop2(), u2 = binaryStack->Pop2();
	LONG64* f1 = (LONG64*)&u1, * f2 = (LONG64*)&u2;
	*f1 = *f1 + *f2;
	binaryStack->Push(u1);
}

void AnagameAssembly::AddULUL(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner)
{
	binaryStack->Push(binaryStack->Pop2() + binaryStack->Pop2());
}

void AnagameAssembly::AddOO(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner)
{
}

void AnagameAssembly::SubFF(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner)
{
	UINT u1 = binaryStack->Pop1(), u2 = binaryStack->Pop1();
	float* f1 = (float*)&u1, * f2 = (float*)&u2;
	*f1 = *f2 - *f1;
	binaryStack->Push(u1);
}

void AnagameAssembly::SubDD(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner)
{
	ULONG64 u1 = binaryStack->Pop2(), u2 = binaryStack->Pop2();
	double* f1 = (double*)&u1, * f2 = (double*)&u2;
	*f1 = *f2 - *f1;
	binaryStack->Push(u1);
}

void AnagameAssembly::SubII(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner)
{
	UINT u1 = binaryStack->Pop1(), u2 = binaryStack->Pop1();
	int* f1 = (int*)&u1, * f2 = (int*)&u2;
	*f1 = *f2 - *f1;
	binaryStack->Push(u1);
}

void AnagameAssembly::SubUU(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner)
{
	UINT u1 = binaryStack->Pop1(), u2 = binaryStack->Pop1();
	binaryStack->Push(u2 - u1);
}

void AnagameAssembly::SubLL(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner)
{
	ULONG64 u1 = binaryStack->Pop2(), u2 = binaryStack->Pop2();
	LONG64* f1 = (LONG64*)&u1, *f2 = (LONG64*)&u2;
	*f1 = *f2 - *f1;
	binaryStack->Push(u1);
}

void AnagameAssembly::SubULUL(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner)
{
	ULONG64 u1 = binaryStack->Pop2(), u2 = binaryStack->Pop2();
	binaryStack->Push(u2 - u1);
}

void AnagameAssembly::SubOO(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner)
{
}


void AnagameAssembly::MulFF(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner)
{
	UINT u1 = binaryStack->Pop1(), u2 = binaryStack->Pop1();
	float* f1 = (float*)&u1, * f2 = (float*)&u2;
	*f1 = *f2 * *f1;
	binaryStack->Push(u1);
}

void AnagameAssembly::MulDD(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner)
{
	ULONG64 u1 = binaryStack->Pop2(), u2 = binaryStack->Pop2();
	double* f1 = (double*)&u1, * f2 = (double*)&u2;
	*f1 = *f2 * *f1;
	binaryStack->Push(u1);
}

void AnagameAssembly::MulII(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner)
{
	UINT u1 = binaryStack->Pop1(), u2 = binaryStack->Pop1();
	int* f1 = (int*)&u1, * f2 = (int*)&u2;
	*f1 = *f2 * *f1;
	binaryStack->Push(u1);
}

void AnagameAssembly::MulUU(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner)
{
	UINT u1 = binaryStack->Pop1(), u2 = binaryStack->Pop1();
	binaryStack->Push(u2 * u1);
}

void AnagameAssembly::MulLL(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner)
{
	ULONG64 u1 = binaryStack->Pop2(), u2 = binaryStack->Pop2();
	LONG64* f1 = (LONG64*)&u1, * f2 = (LONG64*)&u2;
	*f1 = *f2 * *f1;
	binaryStack->Push(u1);
}

void AnagameAssembly::MulULUL(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner)
{
	ULONG64 u1 = binaryStack->Pop2(), u2 = binaryStack->Pop2();
	binaryStack->Push(u2 * u1);
}

void AnagameAssembly::MulOO(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner)
{
}


void AnagameAssembly::DivFF(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner)
{
	UINT u1 = binaryStack->Pop1(), u2 = binaryStack->Pop1();
	float* f1 = (float*)&u1, * f2 = (float*)&u2;
	*f1 = *f2 / *f1;
	binaryStack->Push(u1);
}

void AnagameAssembly::DivDD(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner)
{
	ULONG64 u1 = binaryStack->Pop2(), u2 = binaryStack->Pop2();
	double* f1 = (double*)&u1, * f2 = (double*)&u2;
	*f1 = *f2 / *f1;
	binaryStack->Push(u1);
}

void AnagameAssembly::DivII(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner)
{
	UINT u1 = binaryStack->Pop1(), u2 = binaryStack->Pop1();
	int* f1 = (int*)&u1, * f2 = (int*)&u2;
	*f1 = *f2 * *f1;
	binaryStack->Push(u1);
}

void AnagameAssembly::DivUU(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner)
{
	UINT u1 = binaryStack->Pop1(), u2 = binaryStack->Pop1();
	binaryStack->Push(u2 / u1);
}

void AnagameAssembly::DivLL(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner)
{
	ULONG64 u1 = binaryStack->Pop2(), u2 = binaryStack->Pop2();
	LONG64* f1 = (LONG64*)&u1, * f2 = (LONG64*)&u2;
	*f1 = *f2 / *f1;
	binaryStack->Push(u1);
}

void AnagameAssembly::DivULUL(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner)
{
	ULONG64 u1 = binaryStack->Pop2(), u2 = binaryStack->Pop2();
	binaryStack->Push(u2 / u1);
}

void AnagameAssembly::DivOO(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner)
{
}

void AnagameAssembly::ModII(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner)
{
	UINT u1 = binaryStack->Pop1(), u2 = binaryStack->Pop1();
	int* f1 = (int*)&u1, * f2 = (int*)&u2;
	*f1 = *f2 % *f1;
	binaryStack->Push(u1);
}

void AnagameAssembly::ModUU(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner)
{
	UINT u1 = binaryStack->Pop1(), u2 = binaryStack->Pop1();
	binaryStack->Push(u2 % u1);
}

void AnagameAssembly::ModLL(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner)
{
	ULONG64 u1 = binaryStack->Pop2(), u2 = binaryStack->Pop2();
	LONG64* f1 = (LONG64*)&u1, * f2 = (LONG64*)&u2;
	*f1 = *f2 % *f1;
	binaryStack->Push(u1);
}

void AnagameAssembly::ModULUL(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner)
{
	ULONG64 u1 = binaryStack->Pop2(), u2 = binaryStack->Pop2();
	binaryStack->Push(u2 % u1);
}

void AnagameAssembly::ModOO(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner)
{
}
