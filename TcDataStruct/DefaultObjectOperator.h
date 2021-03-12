#pragma once
#include <ObjectOperator.h>
#include "TcDataStruct.h"

/**
 * Union: doubleLong
 * Purpose: Holds either a double, unsigned or signed integer, all 8 bytes
 */
union doubleLong
{
    LONG64 s;
    ULONG64 u;
    double d;
};

/**
 * Enum Class: double_long
 * Purpose: used to keep track of the status of the doubleLong union
 */
enum class double_long
{
    dl_sign,  // Set to a signed integer
    dl_unsign,// Set to an unsigned integer
    dl_double,// Set to double
    dl_invalid // Not properly set
};

/**
 * Class: DoubleLong
 * Purpose: Aids interpretors in basic arithmetic byt hold values and keeping track of their types
 */
class TC_DATA_STRUCT DoubleLong
{
public:
    DoubleLong(ULONG64 val);
    DoubleLong(LONG64 val);
    DoubleLong(double val);
    DoubleLong();
    doubleLong value;
    double_long type;

    bool operator<(const DoubleLong& o);
    bool operator<=(const DoubleLong& o);
    bool operator>=(const DoubleLong& o);
    bool operator>(const DoubleLong& o);
    bool operator==(const DoubleLong& o);

    ULONG64 ToUnsignedLong()const;

    ULONG64 GetBitAnd(const DoubleLong& o);
    ULONG64 GetBitOr(const DoubleLong& o);
    ULONG64 GetBitXor(const DoubleLong& o);

    /**
     * Method: TInterpretor::GetValueFromPrimitive
     * Purpose: Helper method for interpreting arithmetic operations by extracting the value from the variable
     * Parameters: TrecPointer<TVariable> var - the variable believed holding the primitive value
     * Returns: DoubleLong - the value held by the variable
     */
    static DoubleLong GetValueFromPrimitive(TrecPointer<TVariable> var);
};

DoubleLong Add(const DoubleLong& v1, const DoubleLong& v2);
DoubleLong Subtract(const DoubleLong& v1, const DoubleLong& v2);
DoubleLong Multiply(const DoubleLong& v1, const DoubleLong& v2);
DoubleLong Divide(const DoubleLong& v1, const DoubleLong& v2);
DoubleLong ModDivide(const DoubleLong& v1, const DoubleLong& v2);
DoubleLong Exponent(const DoubleLong& v1, const DoubleLong& v2);

TString GetStringFromPrimitive(TrecPointer<TVariable> var)

/**
 * Class: ObjectOperator
 * Purpose: Enables various operations to be performed on two different TVariable types
 */
class DefaultObjectOperator : public ObjectOperator
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
    virtual TrecPointer<TVariable> Add(TrecPointer<TVariable> op1, TrecPointer<TVariable> op2) override;

    /**
     * Method: ObjectOperator::Subtract
     * Purpose: Provides a means through which Subtraction can be performed
     * Parameters: TrecPointer<TVariable> op1 - the first operator
     *				TrecPointer<TVariable> op2 - the second operator
     * Returns: TrecPointer<TVariable> - the result of the operation
     *
     * Note: If the result is null, that is a sign that the operation didn't work
     */
    virtual TrecPointer<TVariable> Subtract(TrecPointer<TVariable> op1, TrecPointer<TVariable> op2) override;

    /**
     * Method: ObjectOperator::Multiply
     * Purpose: Provides a means through which multiplication can be performed
     * Parameters: TrecPointer<TVariable> op1 - the first operator
     *				TrecPointer<TVariable> op2 - the second operator
     * Returns: TrecPointer<TVariable> - the result of the operation
     *
     * Note: If the result is null, that is a sign that the operation didn't work
     */
    virtual TrecPointer<TVariable> Multiply(TrecPointer<TVariable> op1, TrecPointer<TVariable> op2) override;

    /**
     * Method: ObjectOperator::Divide
     * Purpose: Provides a means through which Division can be performed
     * Parameters: TrecPointer<TVariable> op1 - the first operator
     *				TrecPointer<TVariable> op2 - the second operator
     * Returns: TrecPointer<TVariable> - the result of the operation
     *
     * Note: If the result is null, that is a sign that the operation didn't work
     */
    virtual TrecPointer<TVariable> Divide(TrecPointer<TVariable> op1, TrecPointer<TVariable> op2) override;

    /**
     * Method: ObjectOperator::Mod
     * Purpose: Provides a means through which Mod Division can be performed
     * Parameters: TrecPointer<TVariable> op1 - the first operator
     *				TrecPointer<TVariable> op2 - the second operator
     * Returns: TrecPointer<TVariable> - the result of the operation
     *
     * Note: If the result is null, that is a sign that the operation didn't work
     */
    virtual TrecPointer<TVariable> Mod(TrecPointer<TVariable> op1, TrecPointer<TVariable> op2) override;

    /**
     * Method: ObjectOperator::Pow
     * Purpose: Provides a means through which the power of the First to the second Operand can be determined
     * Parameters: TrecPointer<TVariable> op1 - the first operator
     *				TrecPointer<TVariable> op2 - the second operator
     * Returns: TrecPointer<TVariable> - the result of the operation
     *
     * Note: If the result is null, that is a sign that the operation didn't work
     */
    virtual TrecPointer<TVariable> Pow(TrecPointer<TVariable> op1, TrecPointer<TVariable> op2) override;

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
    virtual TrecPointer<TVariable> Equality(TrecPointer<TVariable> op1, TrecPointer<TVariable> op2, equality_op op) override;

};

