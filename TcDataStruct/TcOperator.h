#pragma once
#include <TObject.h>
#include <TcInterpretor.h>
#include "TcDataStruct.h"

/**
 * Class: TcOperator
 * Purpose: Represents an Operation that can be performed in/on an expression
 */
class TC_DATA_STRUCT TcOperator :
    public TObject
{
protected:
    tc_int_op opType; // Type of Operation
    bool isAssign;  // Whether the assignment should take place

    TcOperator();
    TcOperator(tc_int_op op, bool isAssign = false);

public:

    /**
     * Method: TcOperator::GetOpType
     * Purpose: Reports the OpType that this Operator represents
     * Parameters: void
     * Returns: tc_int_op - the operator type
     */
    tc_int_op GetOpType();

    /**
     * Method: TcOperator::GetOperandCount
     * Purpose: Reports how many operands this Operator works on
     * Parameters: void
     * Returns: UCHAR - the count, usually 2 but can be 3
     * 
     * Attributes: virtual
     */
    virtual UCHAR GetOperandCount();

    /**
     * Method: TcOperator::IsAssign
     * Purpose: reports whether this Operator is meant to act as an assignment operator (since the operator doesn't do the assignment itself)
     * Parameters: void
     * Returns: bool - is this an assignment operator
     */
    bool IsAssign();

    /**
     * Method: TcOperator::Inspect
     * Purpose: Checks to see if the Operation can be performed, but does not actualy do the operation
     * Parameters: TDataArray<TrecPointer<TVariable>>& params - the parameters to act upon
     *              ReturnObject& ret - signal of success
     * Returns: void
     * 
     * Attributes: abstract
     */
    virtual void Inspect(TDataArray<TrecPointer<TVariable>>& params, ReturnObject& ret) = 0;

    /**
     * Method: TcOperator::PerformOperation
     * Purpose: Attempts to Perform the Operation
     * Parameters: TDataArray<TrecPointer<TVariable>>& params - the parameters to act upon
     *              ReturnObject& ret - signal of success
     * Returns: void
     *
     * Attributes: abstract
     */
    virtual void PerformOperation(TDataArray<TrecPointer<TVariable>>& params, ReturnObject& ret) = 0;
};

TrecPointer<TcOperator> TC_DATA_STRUCT GenerateDefaultOperator(tc_int_op op, bool treatNullAsZero, UCHAR stringAdd, UCHAR container = 0);