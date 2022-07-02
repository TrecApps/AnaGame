#pragma once
#include "TcDataStruct.h"
#include <TVariable.h>

typedef enum class operator_type
{
	ot_add,
	ot_sub,
	ot_mul,
	ot_div,
	ot_mod,

	ot_inc,
	ot_dec,

	

	ot_assign,
	ot_assign_add,
}operator_type;


class TC_DATA_STRUCT TcOperator
{
public:
	virtual bool HasOp(operator_type op) = 0;
	virtual void process(operator_type op, ReturnObject& ret, TrecPointer<TVariable> op1, TrecPointer<TVariable> op2) = 0;

	virtual bool FromRightToLeft() = 0;
};

class TC_DATA_STRUCT TcOperatorContainer
{
protected:

};