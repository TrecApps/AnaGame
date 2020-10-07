
#include "TVariable.h"

bool VarFunction::IsTrue(TrecPointer<TVariable> var, bool& result, UCHAR def)
{
	return false;
}

/**
 * Method: TVariable::SetSelf
 * Purpose: Allows the Variable to have a reference to itself
 * Parameters: TrecPointer<TVariable> - reference to assign
 * Returns: void
 */
void TVariable::SetSelf(TrecPointer<TVariable>)
{
}

/**
 * Method: TVariableMarker::TVariableMarker
 * Purpose: Constructor
 * Parameters: bool makeMutable - whether this variable should be able to be changed later
 *              TrecPointer<TVariable> var - the new variable
 * Returns: New Variable marker
 */
TVariableMarker::TVariableMarker(bool makeMutable, TrecPointer<TVariable> var)
{
	mutableVar = makeMutable;
	this->var = var;
}

/**
 * Method: TVariableMarker::TVariableMarker
 * Purpose: Default Constructor
 * Parameters: void
 * Returns: New Variable marker
 */
TVariableMarker::TVariableMarker()
{
	mutableVar = true;
}

/**
 * Method: TVariableMarker::IsMutable
 * Purpose: Reports whether the variable is mutable (can be changed) or not
 * Parameters: void
 * Returns: bool - mutability status
 */
TVariableMarker::TVariableMarker(const TVariableMarker& orig)
{
	mutableVar = orig.IsMutable();
	var = orig.GetVariable();
}

/**
 * Method: TVariableMarker::operator=
 * Purpose: Overloads assignment operator for the class
 * Parameters: const TVariableMarker& orig -  the marker to copy from
 * Returns: void
 */
void TVariableMarker::operator=(const TVariableMarker& orig)
{
	mutableVar = orig.IsMutable();
	var = orig.GetVariable();
}

/**
 * Method: TVariableMarker::SetVariable
 * Purpose: Sets the Variable if allowed
 * Parameters: TrecPointer<TVariable> var - the new variable to assign to
 * Returns: bool - whether the new variable was set
 */
bool TVariableMarker::SetVariable(TrecPointer<TVariable> var)
{
	if (mutableVar)
		var = var;
	return mutableVar;
}

/**
 * Method: TVariableMarker::GetVariable
 * Purpose: Reports the Variable being held
 * Parameters: void
 * Returns: TrecPointer<TVariable> var - the variable being held
 */
TrecPointer<TVariable> TVariableMarker::GetVariable()const
{
	return var;
}

/**
 * Method: TVariableMarker::IsMutable
 * Purpose: Reports whether the variable is mutable (can be changed) or not
 * Parameters: void
 * Returns: bool - mutability status
 */
bool TVariableMarker::IsMutable()const 
{
	return this->mutableVar;
}
