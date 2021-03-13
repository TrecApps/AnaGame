#include "pch.h"
#include "TPrimitiveVariable.h"


/**
 * Method: TPrimitiveVariable::TPrimitiveVariable
 * Purpose: Constructor
 * Parameters: float value - the value to set, as a float
 * Returns: New TPrimitive variable that represents a float
 */
TPrimitiveVariable::TPrimitiveVariable(float value)
{
    Set(value);
}

/**
 * Method: TPrimitiveVariable::TPrimitiveVariable
 * Purpose: Constructor
 * Parameters: double value - the value to set, as a double
 * Returns: New TPrimitive variable that represents a double
 */
TPrimitiveVariable::TPrimitiveVariable(double value)
{
    Set(value);
}


/**
 * Method: TPrimitiveVariable::TPrimitiveVariable
 * Purpose: Constructor
 * Parameters: signed char value - the value to set, as a 1 byte int
 * Returns: New TPrimitive variable that represents a 1 byte int
 */
TPrimitiveVariable::TPrimitiveVariable(signed char value)
{
    Set(value);
}


/**
 * Method: TPrimitiveVariable::TPrimitiveVariable
 * Purpose: Constructor
 * Parameters: char - the value to set, as a 1 byte char
 * Returns: New TPrimitive variable that represents a char
 */
TPrimitiveVariable::TPrimitiveVariable(char value)
{
    Set(value);
}


/**
 * Method: TPrimitiveVariable::TPrimitiveVariable
 * Purpose: Constructor
 * Parameters: UCHAR value - the value to set, as a UCHAR
 * Returns: New TPrimitive variable that represents a UCHAR
 */
TPrimitiveVariable::TPrimitiveVariable(UCHAR value)
{
    Set(value);
}


/**
 * Method: TPrimitiveVariable::TPrimitiveVariable
 * Purpose: Constructor
 * Parameters: short value - the value to set, as a short
 * Returns: New TPrimitive variable that represents a short
 */
TPrimitiveVariable::TPrimitiveVariable(short value)
{
    Set(value);
}


/**
 * Method: TPrimitiveVariable::TPrimitiveVariable
 * Purpose: Constructor
 * Parameters: USHORT value - the value to set, as a USHORT
 * Returns: New TPrimitive variable that represents a USHORT
 */
TPrimitiveVariable::TPrimitiveVariable(USHORT value)
{
    Set(value);
}


/**
 * Method: TPrimitiveVariable::TPrimitiveVariable
 * Purpose: Constructor
 * Parameters: WCHAR value - the value to set, as a WCHAR
 * Returns: New TPrimitive variable that represents a WCHAR
 */
TPrimitiveVariable::TPrimitiveVariable(WCHAR value)
{
    Set(value);
}


/**
 * Method: TPrimitiveVariable::TPrimitiveVariable
 * Purpose: Constructor
 * Parameters: int value - the value to set, as an int
 * Returns: New TPrimitive variable that represents an int
 */
TPrimitiveVariable::TPrimitiveVariable(int value)
{
    Set(value);
}


/**
 * Method: TPrimitiveVariable::TPrimitiveVariable
 * Purpose: Constructor
 * Parameters: UINT value - the value to set, as a UINT
 * Returns: New TPrimitive variable that represents a UINT
 */
TPrimitiveVariable::TPrimitiveVariable(UINT value)
{
    Set(value);
}


/**
 * Method: TPrimitiveVariable::TPrimitiveVariable
 * Purpose: Constructor
 * Parameters: LONG64 value - the value to set, as a LONG64
 * Returns: New TPrimitive variable that represents a LONG64
 */
TPrimitiveVariable::TPrimitiveVariable(LONG64 value)
{
    Set(value);
}

TPrimitiveVariable::TPrimitiveVariable()
{
    Set(0ull);
}

TString TPrimitiveVariable::GetString(TString format)
{
    ThreadLock();
    TString ret;
    if (type & TPrimitiveVariable::type_bool)
    {
        if (value)
            ret.Set("true");
        else
            ret.Set("false");
    }
    else if (type & TPrimitiveVariable::type_char && type & TPrimitiveVariable::type_one)
    {
        char v = static_cast<char>(value);
        ret.Format(format + L"c", v);
    }
    else if (type & TPrimitiveVariable::type_char && type & TPrimitiveVariable::type_two)
    {
        WCHAR v[2] = { static_cast<WCHAR>(value), L'\0' };
        ret.Format(format + L"ls", v);
    }
    else if (type & TPrimitiveVariable::type_float)
    {
        double d;

        memcpy_s(&d, sizeof(d), &value, sizeof(value));

        ret.Format(format + L"f", d);
    }
    else if (type & TPrimitiveVariable::type_unsigned)
    {
        ret.Format(format + L"llu", value);
    }
    else
    {
        LONG64 l = static_cast<LONG64>(value);

        ret.Format(format + L"lld", l);
    }
    ThreadRelease();
    return ret;
}


/**
 * Method: TPrimitiveVariable::TPrimitiveVariable
 * Purpose: Constructor
 * Parameters: ULONG64 value - the value to set, as a ULONG64
 * Returns: New TPrimitive variable that represents a ULONG64
 */
TPrimitiveVariable::TPrimitiveVariable(ULONG64 value)
{
    Set(value);
}


/**
 * Method: TPrimitiveVariable::TPrimitiveVariable
 * Purpose: Constructor
 * Parameters: bool value - the value to set, as a bool
 * Returns: New TPrimitive variable that represents a bool
 */
TPrimitiveVariable::TPrimitiveVariable(bool value)
{
    Set(value);
}

/**
 * Method: TPrimitiveVariable::Set
 * Purpose: Sets the value and type of this Primitive Variable
 * Parameters: float value - the value to set, as a float
 * Returns: void
 */
void TPrimitiveVariable::Set(float value)
{
    ThreadLock();
    memcpy_s(&this->value, 8, &value, 4);
    this->value = this->value >> 32;

    type = type_four | type_float;
    ThreadRelease();
}

/**
 * Method: TPrimitiveVariable::Set
 * Purpose: Sets the value and type of this Primitive Variable
 * Parameters: double value - the value to set, as a double
 * Returns: void
 */
void TPrimitiveVariable::Set(double value)
{
    ThreadLock();
    memcpy_s(&this->value, 8, &value, 8);

    type = type_eight | type_float;
    ThreadRelease();
}

/**
 * Method: TPrimitiveVariable::Set
 * Purpose: Sets the value and type of this Primitive Variable
 * Parameters: signed char value - the value to set, as a 1 byte int
 * Returns: void
 */
void TPrimitiveVariable::Set(signed char value)
{
    ThreadLock();
    memcpy_s(&this->value, 8, &value, 1);
    this->value = this->value >> 56;

    type = type_one;
    ThreadRelease();
}

/**
 * Method: TPrimitiveVariable::Set
 * Purpose: Sets the value and type of this Primitive Variable
 * Parameters: char - the value to set, as a 1 byte char
 * Returns: void
 */
void TPrimitiveVariable::Set(char value)
{
    ThreadLock();
    memcpy_s(&this->value, 8, &value, 1);
    this->value = this->value >> 56;

    type = type_one | type_unsigned;
    ThreadRelease();
}

/**
 * Method: TPrimitiveVariable::Set
 * Purpose: Sets the value and type of this Primitive Variable
 * Parameters: UCHAR value - the value to set, as a UCHAR
 * Returns: void
 */
void TPrimitiveVariable::Set(UCHAR value)
{
    ThreadLock();
    memcpy_s(&this->value, 8, &value, 2);
    this->value = this->value >> 48;

    type = type_two | type_unsigned;
    ThreadRelease();
}

/**
 * Method: TPrimitiveVariable::Set
 * Purpose: Sets the value and type of this Primitive Variable
 * Parameters: short value - the value to set, as a short
 * Returns: void
 */
void TPrimitiveVariable::Set(short value)
{
    ThreadLock();
    memcpy_s(&this->value, 8, &value, 2);
    this->value = this->value >> 48;

    type = type_two;
    ThreadRelease();
}

/**
 * Method: TPrimitiveVariable::Set
 * Purpose: Sets the value and type of this Primitive Variable
 * Parameters: USHORT value - the value to set, as a USHORT
 * Returns: void
 */
void TPrimitiveVariable::Set(USHORT value)
{
    ThreadLock();
    memcpy_s(&this->value, 8, &value, 2);
    this->value = this->value >> 48;

    type = type_two | type_char;
    ThreadRelease();
}

/**
 * Method: TPrimitiveVariable::Set
 * Purpose: Sets the value and type of this Primitive Variable
 * Parameters: WCHAR value - the value to set, as a WCHAR
 * Returns: void
 */
void TPrimitiveVariable::Set(WCHAR value)
{
    ThreadLock();
    memcpy_s(&this->value, 8, &value, 2);
    this->value = this->value >> 48;

    type = type_two | type_char;
    ThreadRelease();
}

/**
 * Method: TPrimitiveVariable::Set
 * Purpose: Sets the value and type of this Primitive Variable
 * Parameters: int value - the value to set, as an int
 * Returns: void
 */
void TPrimitiveVariable::Set(int value)
{
    ThreadLock();
    this->value = 0ULL;
    memcpy_s(&this->value, 8, &value, 4);
    //this->value = this->value >> 32;

    type = type_four;
    ThreadRelease();
}

/**
 * Method: TPrimitiveVariable::Set
 * Purpose: Sets the value and type of this Primitive Variable
 * Parameters: UINT value - the value to set, as a UINT
 * Returns: void
 */
void TPrimitiveVariable::Set(UINT value)
{
    ThreadLock();
    memcpy_s(&this->value, 8, &value, 4);
    this->value = this->value >> 32;

    type = type_four | type_unsigned;
    ThreadRelease();
}

/**
 * Method: TPrimitiveVariable::Set
 * Purpose: Sets the value and type of this Primitive Variable
 * Parameters: LONG64 value - the value to set, as a LONG64
 * Returns: void
 */
void TPrimitiveVariable::Set(LONG64 value)
{
    ThreadLock();
    memcpy_s(&this->value, 8, &value, 8);
    type = type_eight;
    ThreadRelease();
}

/**
 * Method: TPrimitiveVariable::Set
 * Purpose: Sets the value and type of this Primitive Variable
 * Parameters: ULONG64 value - the value to set, as a ULONG64
 * Returns: void
 */
void TPrimitiveVariable::Set(ULONG64 value)
{
    ThreadLock();
    this->value = value;

    type = type_eight | type_unsigned;
    ThreadRelease();
}

/**
 * Method: TPrimitiveVariable::Set
 * Purpose: Sets the value and type of this Primitive Variable
 * Parameters: bool value - the value to set, as a bool
 * Returns: void
 */
void TPrimitiveVariable::Set(bool value)
{
    ThreadLock();
    if (value)
        this->value = 1LL;
    else
        this->value = 0LL;

    type = 0;
    type = type_bool | type_one;
    ThreadRelease();
}

/**
 * Method: TPrimitiveVariable::BitShift
 * Purpose: Performs a bitshift operatoin on the variable
 * Parameters: bool rightshift - true for right shift, false for left shift
 *              UINT shiftCount - the number of bits to shoft by
 *              USHORT flags - flags that go into this shift
 * Returns: bool - whether the operation can be applied
 *
 * Note: Flags are as follows
 *
 *  0b0000000000000001 - Applies to boolean
 *  0b0000000000000010 - Applies to float
 *  0b0000000000000100 - Fill in-side with out-bit
 *  0b0000000000001000 - Cut down to 32 bits
 *  0b0000000000010000 - cut down to 16 bits
 *  0b0000000000011000 - cut down to 8 bits
 *  0b0000000000100000 - make unsigned
 *  0b0000000001100000 - make unsigned if float
 */
bool TPrimitiveVariable::BitShift(bool rightShift, UINT shiftCount, USHORT flags)
{
    ThreadLock();
    if (!(flags & 1) && type & 0b00000001)    // It was a boolean and the passed flag doesn't allow for that
    {
        ThreadRelease();
        return false;
    }
    if (!(flags & 2) && type & type_float)      // Type was a float and the passed flag doesn't allow for that
    {
        ThreadRelease();
        return false;
    }
    ULONG64 preShift = value;

    UINT moder = 64;
    if (type & type_four)
        moder = 32;
    else if (type & type_two)
        moder = 16;
    else if (type & type_one)
        moder = 8;

    shiftCount = shiftCount % moder;

    if (rightShift)
    {
        value = value >> shiftCount;

        if (flags & 4)
        {
            value += (preShift << (moder - shiftCount));
            switch (moder)
            {
            case 32:
                value = value & 0x00000000ffffffff;
                break;
            case 16:
                value = value & 0x000000000000ffff;
                break;
            case 8:
                value = value & 0x00000000000000ff;
            }
        }
    }
    else
    {
        value = value << shiftCount;
        if (flags & 4)
        {
            value += (preShift >> (moder - shiftCount));
            switch (moder)
            {
            case 32:
                value = value & 0x00000000ffffffff;
                break;
            case 16:
                value = value & 0x000000000000ffff;
                break;
            case 8:
                value = value & 0x00000000000000ff;
            }
        }
    }
    
    // See if new value needs to be cut down
    if (flags & 0b0000000000011000)
    {
        // it does
        if ((flags & 0b0000000000001000))
        {
            if (type & type_float)
            {
                value = preShift;
                ThreadRelease();
                return false;
            }

            // Don't cut down to 32 bits, cut down to 16
            value = value & 0x000000000000ffff;
        }
        else if (!(flags & 0b0000000000010000))
        {
            // Cut down to 32
            value = value & 0x00000000ffffffff;
        }
        else
        {
            if (type & type_float)
            {
                value = preShift;
                ThreadRelease();
                return false;
            }

            // cut down to 8
            value = value & 0x00000000000000ff;
        }
    }

    if (flags & 0b0000000000100000)
    {
        // here cast to unsigned int
        if (!(flags & 0b0000000001000000) && type & type_float)
        {
            // We are supposed to cast to unsigned but not if it is a float
            value = preShift;
            ThreadRelease();
            return false;
        }

        if (!(type & type_unsigned))
        {
            switch (moder)
            {
            case 64:
                if (value & 0x0000000080000000)
                    value = value ^ 0x00000000ffffffff;
                break;
            case 32:
                if (value & 0x0000000000800000)
                    value = value ^ 0x0000000000ffffff;
                break;
            case 16:
                if (value & 0x0000000000008000)
                    value = value ^ 0x000000000000ffff;
                break;
                if (value & 0x0000000000000080)
                    value = value ^ 0x00000000000000ff;
            }
        }
    }
    ThreadRelease();
    return true;
}

TrecPointer<TVariable> TPrimitiveVariable::Clone()
{
    ThreadLock();
    TrecPointer<TVariable> ret = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(value);
    dynamic_cast<TPrimitiveVariable*>(ret.Get())->type = type;
    ThreadRelease();
    return ret;
}

/**
 * Method: TPrimitiveVarible::GetObject
 * Purpose: Returns the Object held by the variable, or null if variable is a raw data type
 * Parameters: void
 * Returns: TrecPointer<TObject> - The Object referered by the variable (or null if not an object)
 *
 * Note: Call "IsObject" first before calling this method as there is no point if the "IsObject" returns false
 */
TrecObjectPointer TPrimitiveVariable::GetObject()
{
    return TrecObjectPointer();
}

/**
 * Method: TPrimitiveVariable::GetVarType
 * Purpose: Reports the type of varible that this object represents
 * Parameters: void
 * Returns: var_type - the type of variable this represents
 */
var_type TPrimitiveVariable::GetVarType()
{
    return var_type::primitive;
}

/**
 * Method: TPrimitiveVarible::GetObject
 * Purpose: Returns the Object held by the variable, or null if variable is a raw data type
 * Parameters: void
 * Returns: TString - The TString referered by the variable (empty if not a string)
 */
TString TPrimitiveVariable::GetString()
{
    return GetString(TString(L"%"));
}

/**
 * Method: TPrimitiveVarible::Get4Value
 * Purpose: Returns the value held by the variable assuming four bytes (it is up to the interpretor to determine if conversion needs to be done)
 * Parameters: void
 * Returns: UINT - The value held as a UINT (0 if not a primitive type
 */
UINT TPrimitiveVariable::Get4Value()
{
    ThreadLock();
    UINT ret = static_cast<UINT>(value);
    ThreadRelease();
    return ret;
}

/**
 * Method: TPrimitiveVarible::Get8Value
 * Purpose: Returns the value held by the variable assuming eight bytes (it is up to the interpretor to determine if conversion needs to be done)
 * Parameters: void
 * Returns: ULONG64 - The value held as an 8 bit integer (0 if not a primitive type)
 */
ULONG64 TPrimitiveVariable::Get8Value()
{
    ThreadLock();
    ULONG64 ret = value;
    ThreadRelease();
    return ret;
}

/**
 * Method: TPrimitiveVarible::GetSize
 * Purpose: Returns the estimated size of the value held
 * Parameters: void
 * Returns: UINT - The estimated size in bytes of the data
 */
UINT TPrimitiveVariable::GetSize()
{
    ThreadLock();
    UINT ret = 0;
    switch (type >> 4)
    {
    case 0b0001:
        ret = 1;
        break;
    case 0b0010:
        ret = 2;
        break;
    case 0b0011:
        ret = 4;
        break;
    case 0b0100:
        ret = 8;
    }
    ThreadRelease();
    return ret;
}

/**
 * Method: TPrimitiveVarible::GetType
 * Purpose: Returns the basic type of the object
 * Parameters: void
 * Returns: UCHAR - The value held as a UINT (0 if not a primitive type)
 */
UINT TPrimitiveVariable::GetVType()
{
    ThreadLock();
    UINT ret = type;
    ThreadRelease();
    return ret;
}
