#pragma once
#include "TByteVirtualMachine.h"

/*
 * class TRegisterByteVirtualMachine
 * Purpose: Provide a virtual Machine for the Anagame platform that supports raw byte-data
 * as well as a set of Registers for. The Set of byte code in this machine can be used to convert
 * it to a form of assembly code that works for non-AnaGame Machines that also use registers. It will be
 * up to the Individual TAssembler with a valid configuration to draw links between AnaGame Registers and 
 * target Registers
 */
class TRegisterByteVirtualMachine :
	public TByteVirtualMachine
{
};

