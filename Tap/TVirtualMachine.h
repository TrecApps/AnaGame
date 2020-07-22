#pragma once
#include <TObject.h>
#include <TFile.h>
#include <map>
#include "TInstance.h"

/*
 * Class TVirtualMachine
 * Purpose: Serves as the base class for Virtual Machines supported by Anagame
 * 
 * Anagame Virtual Machines will serve as a junction in the Anagame Environment. They will
 * be able to run Taps as well as serve as provide their own assembly instructions. These sets of assembly
 * instructions will serve as the first intermediary target of the TrecCode compiler. Any Anagame supported
 * programming language must be able to target at least one of four Anagame Virtual Machine types. Some
 * of these machines will be used for a Virtual Environment upon which higher level languages, such as 
 * JavaScript or Python, will run. A couple of them will also be used as an intermediary between AnaGame bytecode
 * and non-AnaGame Environments such as x86, ARM, or even the JVM
 */
class TVirtualMachine :	public TObject
{
public:
	TVirtualMachine(TrecPointer<TInstance> instance);

	virtual UINT LoadByteFile(TFile& file) = 0;
	virtual UINT LoadAnagameAsmFile(TFile& file) = 0;

protected:
	TDataArray<BYTE> code;						// Used to hold the Byte code of the Machine
	std::map<TString, UINT> methodStart;		// Used to mark the procedure name to it's location
	TDataArray<TString> methodLine;				// Used to hold called methods, with indexes

	TrecPointer<TInstance> anagameInstance;		// Holds the instance that owns this Machine
};

