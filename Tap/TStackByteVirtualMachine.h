#pragma once
#include "TByteVirtualMachine.h"

/*
 * class TStackByteVirtualMachine
 * Purpose: Provide a virtual Machine for the Anagame platform that supports raw byte-data
 * that is processed via a Stack. The Set of byte code in this machine can be used to convert
 * it to a form of assembly code that works for non-AnaGame Machines that also use the Stack
 */
class TStackByteVirtualMachine :
	public TByteVirtualMachine
{
};

