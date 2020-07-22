#pragma once
#include "TVirtualMachine.h"

class TAssembler;

/*
 * class TByteVirtualMachine
 * Purpose: Provide a base for Byte Based (as opposed to Object-Based) Virtual Machines, both of which can 
 * serve as an Intermediary between Source-Code and a non-AnaGame final form, whether it is JVM or actual hard-ware
 * like x86 or ARM. These machines will be used to 
 */
class TByteVirtualMachine :	public TVirtualMachine
{
public:
	// To-Do: Provide means through which AnaGame Bytecode can be Converted into another form
	virtual UINT Compile(TrecPointer<TAssembler> assembler) = 0;
};

