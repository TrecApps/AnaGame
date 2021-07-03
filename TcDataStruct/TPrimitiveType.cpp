#include "TPrimitiveType.h"

TPrimitiveType::TPrimitiveType(UINT size, var_primitive_cat cat)
{
    type = 0;
    switch (size)
    {
    case 1:
        type = type | 0b00010000;
        break;
    case 2:
        type = type | 0b00100000;
        break;
    case 4:
        type = type | 0b00110000;
        break;
    case 8:
        type = type | 0b01000000;
        break;
    default:
        throw L"Not Standard Byte Size!";
    }

    switch (cat)
    {
    case var_primitive_cat::vpc_bool:
        type = type | 0b00000001;
        break;
    case var_primitive_cat::vpc_float:
        type = type | 0b00000010;
        break;
    case var_primitive_cat::vpc_char:
        type = type | 0b00000100;
        break;
    case var_primitive_cat::vpc_uint:
        type = type | 0b00001000;
    }
}

TPrimitiveType::TPrimitiveType()
{
    type = 0b00110000;
}

TPrimitiveType::TPrimitiveType(const TPrimitiveType& copy) : TPrimitiveType(copy.GetSize(), copy.getPimitiveType())
{
}

var_category TPrimitiveType::GetVarGategory()
{
    return var_category::vc_primitive;
}

UINT TPrimitiveType::GetSize()const 
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

var_primitive_cat TPrimitiveType::getPimitiveType() const
{
    ThreadLock();
    var_primitive_cat ret = var_primitive_cat::vpc_int;
    switch (type & 0b00001111)
    {
    case 0b00000001:
        ret = var_primitive_cat::vpc_bool;
        break;
    case 0b00000010:
        ret = var_primitive_cat::vpc_float;
        break;
    case 0b00000100:
        ret = var_primitive_cat::vpc_char;
        break;
    case 0b00001000:
        ret = var_primitive_cat::vpc_uint;
    }
    ThreadRelease();
    return ret;
}
