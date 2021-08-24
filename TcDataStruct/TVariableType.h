#pragma once
#include <TObject.h>
#include "TcDataStruct.h"

/**
 * Enum Class: var_category
 * Purpose: Sorts variable type by type
 */
typedef enum class var_category
{
	vc_primitive, // Primitive variable type
	vc_class		// Complex Variable type
}var_category;

/**
 * Class: TVariableType
 * Purpose: Mandates a way to report what type of variable type is managed here
 */
class TC_DATA_STRUCT TVariableType : public TObject
{
public:
	virtual var_category GetVarGategory() = 0;
};

