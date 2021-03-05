#pragma once
#include "TVariable.h"

/**
 * Enum Class: equality_op
 * Purpose: Determines which equality is being determined
 */
typedef enum class equality_op
{
	eo_equals,		// ==
	eo_not,			// !=
	eo_greater,		// >
	eo_less,		// <
	eo_greater_eq,	// >=
	eo_less_eq		// <=
}equality_op;

/**
 * Class: ObjectOperator
 * Purpose: Enables various operations to be performed on two different TVariable types
 */
class ObjectOperator
{
	/**
	 * Method: ObjectOperator::Add
	 * Purpose: Provides a means through which Addition can be performed
	 * Parameters: TrecPointer<TVariable> op1 - the first operator
	 *				TrecPointer<TVariable> op2 - the second operator
	 * Returns: TrecPointer<TVariable> - the result of the operation
	 * 
	 * Note: If the result is null, that is a sign that the operation didn't work
	 */
	virtual TrecPointer<TVariable> Add(TrecPointer<TVariable> op1, TrecPointer<TVariable> op2) = 0;

	/**
	 * Method: ObjectOperator::Subtract
	 * Purpose: Provides a means through which Subtraction can be performed
	 * Parameters: TrecPointer<TVariable> op1 - the first operator
	 *				TrecPointer<TVariable> op2 - the second operator
	 * Returns: TrecPointer<TVariable> - the result of the operation
	 *
	 * Note: If the result is null, that is a sign that the operation didn't work
	 */
	virtual TrecPointer<TVariable> Subtract(TrecPointer<TVariable> op1, TrecPointer<TVariable> op2) = 0;

	/**
	 * Method: ObjectOperator::Multiply
	 * Purpose: Provides a means through which multiplication can be performed
	 * Parameters: TrecPointer<TVariable> op1 - the first operator
	 *				TrecPointer<TVariable> op2 - the second operator
	 * Returns: TrecPointer<TVariable> - the result of the operation
	 *
	 * Note: If the result is null, that is a sign that the operation didn't work
	 */
	virtual TrecPointer<TVariable> Multiply(TrecPointer<TVariable> op1, TrecPointer<TVariable> op2) = 0;

	/**
	 * Method: ObjectOperator::Divide
	 * Purpose: Provides a means through which Division can be performed
	 * Parameters: TrecPointer<TVariable> op1 - the first operator
	 *				TrecPointer<TVariable> op2 - the second operator
	 * Returns: TrecPointer<TVariable> - the result of the operation
	 *
	 * Note: If the result is null, that is a sign that the operation didn't work
	 */
	virtual TrecPointer<TVariable> Divide(TrecPointer<TVariable> op1, TrecPointer<TVariable> op2) = 0;

	/**
	 * Method: ObjectOperator::Mod
	 * Purpose: Provides a means through which Mod Division can be performed
	 * Parameters: TrecPointer<TVariable> op1 - the first operator
	 *				TrecPointer<TVariable> op2 - the second operator
	 * Returns: TrecPointer<TVariable> - the result of the operation
	 *
	 * Note: If the result is null, that is a sign that the operation didn't work
	 */
	virtual TrecPointer<TVariable> Mod(TrecPointer<TVariable> op1, TrecPointer<TVariable> op2) = 0;

	/**
	 * Method: ObjectOperator::Pow
	 * Purpose: Provides a means through which the power of the First to the second Operand can be determined
	 * Parameters: TrecPointer<TVariable> op1 - the first operator
	 *				TrecPointer<TVariable> op2 - the second operator
	 * Returns: TrecPointer<TVariable> - the result of the operation
	 *
	 * Note: If the result is null, that is a sign that the operation didn't work
	 */
	virtual TrecPointer<TVariable> Pow(TrecPointer<TVariable> op1, TrecPointer<TVariable> op2) = 0;

	/**
	 * Method: ObjectOperator::Equality
	 * Purpose: Provides a means through which Addition can be performed
	 * Parameters: TrecPointer<TVariable> op1 - the first operator
	 *				TrecPointer<TVariable> op2 - the second operator
	 *				equality_op op - the type of equality to check for
	 * Returns: TrecPointer<TVariable> - the result of the operation
	 *
	 * Note: If the result is null, that is a sign that the operation didn't work
	 */
	virtual TrecPointer<TVariable> Equality(TrecPointer<TVariable> op1, TrecPointer<TVariable> op2, equality_op op) = 0;

};

