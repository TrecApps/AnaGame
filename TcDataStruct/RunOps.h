#pragma once
#include <TcRunner.h>

namespace AnagameAssembly
{
	typedef void (*AssemblyOp)(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner);

	void Nothing(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner);
	void Line(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner);
	void Statement(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner);

	void AddFF(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner);
	void AddDD(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner);
	void AddII(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner);
	void AddUU(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner);
	void AddLL(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner);
	void AddULUL(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner);
	void AddOO(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner);


	void SubFF(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner);
	void SubDD(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner);
	void SubII(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner);
	void SubUU(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner);
	void SubLL(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner);
	void SubULUL(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner);
	void SubOO(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner);

	void MulFF(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner);
	void MulDD(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner);
	void MulII(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner);
	void MulUU(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner);
	void MulLL(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner);
	void MulULUL(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner);
	void MulOO(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner);

	void DivFF(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner);
	void DivDD(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner);
	void DivII(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner);
	void DivUU(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner);
	void DivLL(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner);
	void DivULUL(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner);
	void DivOO(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner);

	void ModII(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner);
	void ModUU(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner);
	void ModLL(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner);
	void ModULUL(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner);
	void ModOO(ReturnObject& ret, TrecPointer<BinaryStack> binaryStack, TrecPointer<ObjectStack> objectStack, TrecPointer<TVariable> runner);


}