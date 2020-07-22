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

    memcpy_s(&this->value, 8, &value, 4);
    this->value >> 32;

    type = type_four | type_float;
}

/**
 * Method: TPrimitiveVariable::Set
 * Purpose: Sets the value and type of this Primitive Variable
 * Parameters: double value - the value to set, as a double
 * Returns: void
 */
void TPrimitiveVariable::Set(double value)
{
    memcpy_s(&this->value, 8, &value, 8);

    type = type_eight | type_float;
}

/**
 * Method: TPrimitiveVariable::Set
 * Purpose: Sets the value and type of this Primitive Variable
 * Parameters: signed char value - the value to set, as a 1 byte int
 * Returns: void
 */
void TPrimitiveVariable::Set(signed char value)
{
    memcpy_s(&this->value, 8, &value, 1);
    this->value >> 56;

    type = type_one;
}

/**
 * Method: TPrimitiveVariable::Set
 * Purpose: Sets the value and type of this Primitive Variable
 * Parameters: char - the value to set, as a 1 byte char
 * Returns: void
 */
void TPrimitiveVariable::Set(char value)
{
    memcpy_s(&this->value, 8, &value, 1);
    this->value >> 56;

    type = type_one | type_unsigned;
}

/**
 * Method: TPrimitiveVariable::Set
 * Purpose: Sets the value and type of this Primitive Variable
 * Parameters: UCHAR value - the value to set, as a UCHAR
 * Returns: void
 */
void TPrimitiveVariable::Set(UCHAR value)
{
    memcpy_s(&this->value, 8, &value, 2);
    this->value >> 48;

    type = type_two | type_unsigned;
}

/**
 * Method: TPrimitiveVariable::Set
 * Purpose: Sets the value and type of this Primitive Variable
 * Parameters: short value - the value to set, as a short
 * Returns: void
 */
void TPrimitiveVariable::Set(short value)
{
    memcpy_s(&this->value, 8, &value, 2);
    this->value >> 48;

    type = type_two;
}

/**
 * Method: TPrimitiveVariable::Set
 * Purpose: Sets the value and type of this Primitive Variable
 * Parameters: USHORT value - the value to set, as a USHORT
 * Returns: void
 */
void TPrimitiveVariable::Set(USHORT value)
{
    memcpy_s(&this->value, 8, &value, 2);
    this->value >> 48;

    type = type_two | type_char;
}

/**
 * Method: TPrimitiveVariable::Set
 * Purpose: Sets the value and type of this Primitive Variable
 * Parameters: WCHAR value - the value to set, as a WCHAR
 * Returns: void
 */
void TPrimitiveVariable::Set(WCHAR value)
{
    memcpy_s(&this->value, 8, &value, 2);
    this->value >> 48;

    type = type_two | type_char;
}

/**
 * Method: TPrimitiveVariable::Set
 * Purpose: Sets the value and type of this Primitive Variable
 * Parameters: int value - the value to set, as an int
 * Returns: void
 */
void TPrimitiveVariable::Set(int value)
{
    memcpy_s(&this->value, 8, &value, 4);
    this->value >> 32;

    type = type_four;
}

/**
 * Method: TPrimitiveVariable::Set
 * Purpose: Sets the value and type of this Primitive Variable
 * Parameters: UINT value - the value to set, as a UINT
 * Returns: void
 */
void TPrimitiveVariable::Set(UINT value)
{
    memcpy_s(&this->value, 8, &value, 4);
    this->value >> 32;

    type = type_four | type_unsigned;
}

/**
 * Method: TPrimitiveVariable::Set
 * Purpose: Sets the value and type of this Primitive Variable
 * Parameters: LONG64 value - the value to set, as a LONG64
 * Returns: void
 */
void TPrimitiveVariable::Set(LONG64 value)
{
    memcpy_s(&this->value, 8, &value, 8);
    type = type_eight;
}

/**
 * Method: TPrimitiveVariable::Set
 * Purpose: Sets the value and type of this Primitive Variable
 * Parameters: ULONG64 value - the value to set, as a ULONG64
 * Returns: void
 */
void TPrimitiveVariable::Set(ULONG64 value)
{
    this->value = value;

    type = type_eight | type_unsigned;
}

/**
 * Method: TPrimitiveVariable::Set
 * Purpose: Sets the value and type of this Primitive Variable
 * Parameters: bool value - the value to set, as a bool
 * Returns: void
 */
void TPrimitiveVariable::Set(bool value)
{
    if (value)
        this->value = 1LL;
    else
        this->value = 0LL;

    type = 0;
    type = type_bool | type_one;
}

/**
 * Method: TPrimitiveVarible::IsObject
 * Purpose: Reports whether the variable holds an object or not
 * Parameters: void
 * Returns: bool - whether the variable is an object or not
 */
bool TPrimitiveVariable::IsObject()
{
    return false;
}

/**
 * Method: TPrimitiveVarible::GetObject
 * Purpose: Returns the Object held by the variable, or null if variable is a raw data type
 * Parameters: void
 * Returns: TrecPointer<TObject> - The Object referered by the variable (or null if not an object)
 *
 * Note: Call "IsObject" first before calling this method as there is no point if the "IsObject" returns false
 */
TrecPointer<TObject> TPrimitiveVariable::GetObject()
{
    return TrecPointer<TObject>();
}

/**
 * Method: TPrimitiveVarible::IsString
 * Purpose: Reports whether the variable holds a string or not
 * Parameters: void
 * Returns: bool - whether the variable is a string or not
 */
bool TPrimitiveVariable::IsString()
{
    return false;
}

/**
 * Method: TPrimitiveVarible::GetObject
 * Purpose: Returns the Object held by the variable, or null if variable is a raw data type
 * Parameters: void
 * Returns: TString - The TString referered by the variable (empty if not a string)
 *
 * Note: Call "IsObject" first before calling this method as there is no point if the "IsObject" returns false
 */
TString TPrimitiveVariable::GetString()
{
    return TString();
}

/**
 * Method: TPrimitiveVarible::Get4Value
 * Purpose: Returns the value held by the variable assuming four bytes (it is up to the interpretor to determine if conversion needs to be done)
 * Parameters: void
 * Returns: UINT - The value held as a UINT (0 if not a primitive type
 */
UINT TPrimitiveVariable::Get4Value()
{
    return static_cast<UINT>(value);
}

/**
 * Method: TPrimitiveVarible::Get8Value
 * Purpose: Returns the value held by the variable assuming eight bytes (it is up to the interpretor to determine if conversion needs to be done)
 * Parameters: void
 * Returns: ULONG64 - The value held as an 8 bit integer (0 if not a primitive type)
 */
ULONG64 TPrimitiveVariable::Get8Value()
{
    return value;
}

/**
 * Method: TPrimitiveVarible::GetSize
 * Purpose: Returns the estimated size of the value held
 * Parameters: void
 * Returns: UINT - The estimated size in bytes of the data
 */
UINT TPrimitiveVariable::GetSize()
{
    switch (type >> 4)
    {
    case 0b0001:
        return 1;
    case 0b0010:
        return 2;
    case 0b0011:
        return 4;
    case 0b0100:
        return 8;
    }
    return 0;
}

/**
 * Method: TPrimitiveVarible::GetType
 * Purpose: Returns the basic type of the object
 * Parameters: void
 * Returns: UCHAR - The value held as a UINT (0 if not a primitive type)
 */
UINT TPrimitiveVariable::GetType()
{
    return type;
}
