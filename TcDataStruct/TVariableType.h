#pragma once
#include <TObject.h>
#include "TcDataStruct.h"

typedef enum class var_category
{
	vc_primitive,
	vc_class
}var_category;

class TC_DATA_STRUCT TVariableType : public TObject
{
public:
	virtual var_category GetVarGategory() = 0;
};

