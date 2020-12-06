#pragma once
#include <TObject.h>


typedef enum class var_category
{
	vc_primitive,
	vc_class
}var_category;

class TVariableType : public TObject
{
public:
	virtual var_category GetVarGategory() = 0;
};

