#include "TcInterpretor.h"

///
/// TcVariableHolder
/// 

/**
 * Mehtod: TcVariableHolder::TcVariableHolder
 * Purpose: Default Constructor
 * Parameters: void
 * Returns: new Variable Holder
 */
TcVariableHolder::TcVariableHolder()
{
	mut = true;
}

/**
 * Mehtod: TcVariableHolder::TcVariableHolder
 * Purpose: Copy Constructor
 * Parameters: const TcVariableHolder& copy - Variable Holder to copy
 * Returns: new Variable Holder
 */
TcVariableHolder::TcVariableHolder(const TcVariableHolder& copy)
{
	mut = copy.mut;
	type.Set(copy.type);
	value = copy.value;
}

/**
 * Method: TcVariableHolder::TcVariableHolder
 * Purpose: Param Constructor
 * Parameters: bool mut - whether this variable is mutable or not
 *              const TString& type - the type of variable this is (empty for 'any')
 *              TrecPointer<TVariable> value - the actual value held
 * Returns: New variable holder
 */
TcVariableHolder::TcVariableHolder(bool mut, const TString& type, TrecPointer<TVariable> value)
{
	this->mut = mut;
	this->type = type;
	this->value = value;
}



class TcInterpretorIterator : public TVariableIterator
{
protected:
	/**
	 * The String to Analyse
	 */
	TrecSubPointer<TVariable, TcInterpretor> targetVar;

	/**
	 * Whether the function is considered Finished
	 */
	bool isDone;

	/**
	 * The current 'index' or how many times the function has been called previously
	 */
	UINT index;

	ReturnObject ret;

public:

	TcInterpretorIterator(TrecSubPointer<TVariable, TcInterpretor> targetVar)
	{
		this->targetVar = targetVar;
		assert(targetVar.Get());
		isDone = false;
		index = 0;
	}


	/**
	 * Method: TcInterpretorIterator::SetReverse
	 * Purpose: enables iterators to go through backwards
	 * Parameters: bool doReverse - if true, the iterator will now go in reverse (if supported)
	 *              bool reset - if true, will point to the beginning or end of the target variable
	 * Returns: bool - whether the operation was supported
	 *
	 * Attributes: abstract
	 */
	virtual bool SetReverse(bool doReverse, bool reset)
	{
		return false;
	}


	/**
	 * Method: TVariable::GetSize
	 * Purpose: Returns the estimated size of the value held
	 * Parameters: void
	 * Returns: UINT - The estimated size in bytes of the data
	 *
	 * Attributes: abstract
	 */
	virtual UINT GetSize() override;

	/**
	 * Method: TcInterpretorIterator::Traverse
	 * Purpose: Goues through the variable
	 * Parameters: UINT& currentIndex - the index of the retrieved variable
	 *              TString& currentName - the name of the retrieved variable
	 *              TrecPointer<TVariable>& value - the retireved variable
	 * Returns: bool - whether the variable was retrieved
	 *
	 * Attributes: abstract
	 */
	virtual bool Traverse(UINT& currentIndex, TString& currentName, TrecPointer<TVariable>& value)
	{
		if (isDone)
			return false;
		
		ret = targetVar->Run();

		if (ret.returnCode)
		{
			isDone = true;
			return false;
		}
		if (ret.mode != return_mode::rm_yield)
			isDone = true;

		currentIndex = index++;
		currentName.Empty();
		value = ret.errorObject;
		return true;
	}

	ReturnObject GetErrorInfo()
	{
		return ret;
	}
};


///
/// TcInterpretor 
///


TrecPointer<TVariable> TcInterpretor::GetIterator()
{
	TrecPointer<TVariable> v = TrecPointerKey::GetTrecPointerFromSoft<TVariable>(vSelf);
	return TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TcInterpretorIterator>(TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TcInterpretor>(v));
}

UINT TcInterpretor::UpdateVariable(const TString& name, TrecPointer<TVariable> value, bool addLocally, bool makeConst)
{
	if (!name.GetSize())
		return 3;


	for (UINT Rust = 0; Rust < variables.count(); Rust++)
	{
		TDataEntry<TcVariableHolder> varMarker;
		if (variables.GetEntryAt(Rust, varMarker))
		{
			if (!varMarker.key.Compare(name))
			{
				if (varMarker.object.mut)
				{
					varMarker.object.value = (value);
					variables.setEntry(name, varMarker.object);
					return 0;
				}
				else return 2;
			}
		}
	}
	UINT res = dynamic_cast<TcInterpretor*>(parent.Get()) ? dynamic_cast<TcInterpretor*>(parent.Get())->UpdateVariable(name, value, makeConst) : 1;
	if (res == 1 && addLocally)
	{
		TcVariableHolder hold;
		hold.mut = !makeConst;
		hold.type.Set(L"");
		hold.value = value;
		variables.addEntry(name, hold);
		return 0;
	}
	return res;
}

/**
	 * Method: TcInterpretor::GetVarType
	 * Purpose: Reports the type of varible that this object represents
	 * Parameters: void
	 * Returns: var_type - the type of variable this represents
	 *
	 * Attributes: override
	 */
var_type TcInterpretor::GetVarType()
{
	return var_type::interpretor;
}


/**
 * Method: TInterpretor::TInterpretor
 * Purpose: Constructor
 * Parameters: TrecPointer<TInterpretor> parentInterpretor - the Interpretor that created this interpretor (use null if this is a root)
 * Returns: New TInterpretor Object
 */
TcInterpretor::TcInterpretor(TrecSubPointer<TVariable, TcInterpretor> parentInterpretor, TrecPointer<TEnvironment> env)
{
	if (parentInterpretor.Get())
	{
		auto p = TrecPointerKey::GetTrecPointerFromSub<>(parentInterpretor);
		parent = TrecPointerKey::GetSoftPointerFromTrec<TVariable>(p);
	}
	environment = env;
}

/**
 * Method: TcInterpretor::SetSelf
 * Purpose: Allows the Variable to have a reference to itself
 * Parameters: TrecPointer<TVariable> - reference to assign
 * Returns: void
 *
 * Attributes: override
 */
void TcInterpretor::SetSelf(TrecPointer<TVariable> self_)
{
	TVariable::SetSelf(self_);
	this->self = TrecPointerKey::GetSoftSubPointerFromSoft<TVariable, TcInterpretor>(TrecPointerKey::GetSoftPointerFromTrec<TVariable>(self_));
	if (this != self_.Get())
		throw L"Error! Not Properly called";
	this->self = TrecPointerKey::GetSoftSubPointerFromSoft<TVariable, TcInterpretor>(TrecPointerKey::GetSoftPointerFromTrec<TVariable>(self_));
}

/**
 * Method: TcInterpretor::CheckVarName
 * Purpose: Method Interpreters can use to inspect the variable name to make sure it is valid
 * Parameters: TString& varname - the variable name to check
 *              ReportObject& ro - the Object to modify based off of the findings
 * Returns: void
 */
void TcInterpretor::CheckVarName(const TString& varname, ReturnObject& ro)
{
int badChar = varname.FindOneOf(L"!@#$%^&*(){}[]-=+/,;:'\"\\`~");
if (badChar != -1)
{
	ro.returnCode = ReturnObject::ERR_IMPROPER_NAME;
	ro.errorMessage.Format(L"Name '%ws' is not a proper name for a variable!", varname.GetConstantBuffer().getBuffer());
	return;
}

WCHAR firstChar = varname[0];

if (!((firstChar >= L'a' && firstChar <= 'z') || (firstChar >= L'A' && firstChar <= 'Z') || firstChar == L'_'))
{
	ro.returnCode = ReturnObject::ERR_IMPROPER_NAME;
	ro.errorMessage.Format(L"Name '%ws' is not a proper name for a variable!", varname.GetConstantBuffer().getBuffer());
	return;
}

ro.returnCode = 0;
}

TrecPointer<TVariable> TcInterpretor::GetVariable(TString& varName, bool& present, bool currentScope)
{
	TcVariableHolder marker;

	if (!varName.Compare(selfWord))
	{
		if (this->methodObject.Get())
		{
			present = true;
			return methodObject;
		}
	}

	if (variables.retrieveEntry(varName, marker))
	{
		present = true;
		return marker.value;
	}

	if (!currentScope)
	{
		if (parent.Get())
			return dynamic_cast<TcInterpretor*>(parent.Get())->GetVariable(varName, present, false);
		if (environment.Get())
			return environment->GetVariable(varName, present);
	}
	present = false;
	return TrecPointer<TVariable>();
}

/**
 * Method: TcInterprtor::PrepReturn
 * Purpose: Refactored Solution for setting an error message to return
 * Parameters: ReturnObject& ret - the object to operate on
 *              const TString& mess - the message to send
 *				const TString& stackMess - the message to prepend to the stack message
 *              USHORT code - the code to add
 *              LONG64 line - the line Number (use a negative value to ignore the stack trace)
 * Returns: void
 */
void TcInterpretor::PrepReturn(ReturnObject& ret, const TString& mess, const TString& stackMess, USHORT code, LONG64 line)
{
	ret.stackTrace.RemoveAll();
	ret.returnCode = code;
	ret.errorMessage.Set(mess);
	if (line > -1)
	{
		TString stack;
		stack.Format(L"%ws line %d", stackMess.GetConstantBuffer().getBuffer(), line);
		ret.stackTrace.push_back(stack);
	}
}

bool TcInterpretor::SetParent(TrecSubPointer<TVariable, TcInterpretor> parent_, bool replace)
{
	if (!parent_.Get())
		return false;
	if (!this->parent.Get() || replace)
	{
		auto p = TrecPointerKey::GetTrecPointerFromSub<>(parent_);
		this->parent = TrecPointerKey::GetSoftPointerFromTrec<TVariable>(p);
		return true;
	}
	return false;
}

TrecPointer<TVariable> TcInterpretor::Clone()
{
	auto subSelf = TrecPointerKey::GetSubPointerFromSoft<>(self);
	return TrecPointerKey::GetTrecPointerFromSub<>(subSelf);
}

UCHAR TcInterpretor::GetVarStatus(TString& varName)
{
	// 0 means not found
	// 1 means found an mutable
	// 2 means found and immutable
	TcVariableHolder hold;
	if (variables.retrieveEntry(varName, hold))
	{
		return hold.mut ? 1 : 2;
	}
	auto tParent = dynamic_cast<TcInterpretor*>(parent.Get());
	if (tParent)
		return tParent->GetVarStatus(varName);
	return 0;
}

TDataMap<TcVariableHolder> TcInterpretor::GetVariables()
{
	return variables;
}

void TcInterpretor::AddModifiers()
{
}


/**
 * Method: TCInterpretor::SetDefaultValues
 * Purpose: Allows Parameter Defaults to be set
 * Parameters: TDataArray<TrecPointer<TVariable>>& paramTypes - the Types of Initial parameters
 * Returns: void
 */
void TcInterpretor::SetParamDefaults(TDataArray<TrecPointer<TVariable>>& paramDefaults)
{
	this->defaultVars.RemoveAll();
	for (UINT Rust = 0; Rust < paramDefaults.Size(); Rust++)
	{
		this->defaultVars.push_back(paramDefaults[Rust]);
	}
}

/**
 * Method: TcInterpretor::SetActiveObject
 * Purpose: Sets the Active Object, enabling this Interpreotr to act as a method in an existing object
 * Parameters: TrecPointer<TVariable> var - the object to act on
 * Returns: void
 */
void TcInterpretor::SetActiveObject(TrecPointer<TVariable> var)
{
	this->methodObject = var;
}


///
/// ReturnObject
/// 

/**
 * Mehtod: ReturnObject::ReturnObject
 * Purpose: Default Constructor
 * Parameters: void
 * Returns: New Return Object
 */
ReturnObject::ReturnObject()
{
	returnCode = 0;
	nextCount = 0;
	mode = return_mode::rm_regular;
}

/**
 * Mehtod: ReturnObject::ReturnObject
 * Purpose: Copy Constructor
 * Parameters: const ReutnrObject& copy - the object to copy
 * Returns: new Return Object
 */
ReturnObject::ReturnObject(const ReturnObject& copy)
{
	errorMessage.Set(copy.errorMessage);
	errorObject = copy.errorObject;
	errorObject2 = copy.errorObject2;
	returnCode = copy.returnCode;
	mode = copy.mode;

	stackTrace = copy.stackTrace;

	nextCount = copy.nextCount;
}

void ReturnObject::operator=(const ReturnObject& copy)
{
	errorMessage.Set(copy.errorMessage);
	errorObject = copy.errorObject;
	errorObject2 = copy.errorObject2;
	returnCode = copy.returnCode;
	mode = copy.mode;

	stackTrace = copy.stackTrace;

	nextCount = copy.nextCount;
}

UINT TcInterpretorIterator::GetSize()
{
	return 0;
}
