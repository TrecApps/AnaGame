#pragma once
#include <TcRunner.h>

namespace AnagameAssembly
{
	typedef void (*AssemblyOp)(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);

	void Nothing(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);
	void Line(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);
	void Statement(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);

	void AddFF(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);
	void AddDD(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);
	void AddII(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);
	void AddUU(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);
	void AddLL(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);
	void AddULUL(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);
	void AddOO(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);


	void SubFF(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);
	void SubDD(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);
	void SubII(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);
	void SubUU(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);
	void SubLL(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);
	void SubULUL(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);
	void SubOO(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);

	void MulFF(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);
	void MulDD(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);
	void MulII(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);
	void MulUU(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);
	void MulLL(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);
	void MulULUL(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);
	void MulOO(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);

	void DivFF(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);
	void DivDD(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);
	void DivII(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);
	void DivUU(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);
	void DivLL(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);
	void DivULUL(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);
	void DivOO(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);

	void ModII(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);
	void ModUU(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);
	void ModLL(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);
	void ModULUL(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);
	void ModOO(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);

	void CastIF(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);
	void CastFI(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);
	void CastFD(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);
	void CastDF(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);
	void CastIU(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);
	void CastUI(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);
	void CastUUL(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);
	void CastULU(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);
	void CastIL(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);
	void CastLI(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);

	void Push1(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);
	void Push2(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);
	void PushB(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);
	void PopN(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);
	void PushO(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);
	void PopO(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);
	void PushStr(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);
	void StrCond(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);

	void JumpCond(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);
	void JumpMark(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);
	void Jump(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);
	void JumpF(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);
	void AssessTrue(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);

	void JumpErr(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);
	void Clearrr(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);
	void UnJumpErr(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner, UINT op1, UINT op2);
}