#pragma once
#include "TVariableType.h"

typedef enum class var_primitive_cat {
    vpc_uint,
    vpc_int,
    vpc_float,
    vpc_char,
    vpc_bool
}var_primitive_cat;


class TPrimitiveType :
    public TVariableType
{
public:
    TPrimitiveType(UINT size, var_primitive_cat cat);
    TPrimitiveType();
    TPrimitiveType(const TPrimitiveType& copy);


    virtual var_category GetVarGategory() override;
    UINT GetSize()const;
    var_primitive_cat getPimitiveType()const;
private:
    /**
     * The type of data actually being stored
     */
    UCHAR type;
};

