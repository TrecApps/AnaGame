#include "TcOperator.h"
#include "TPrimitiveVariable.h"
#include "TStringVariable.h"
#include "DefaultObjectOperator.h"
#include "TSpecialVariable.h"
#include "TObjectVariable.h"

TcOperator::TcOperator()
{
	assert(false);
	opType = tc_int_op::none;
	isAssign = false;
}

TcOperator::TcOperator(tc_int_op op, bool isAssign_)
{
	this->opType = op;
	isAssign = isAssign_;
}

tc_int_op TcOperator::GetOpType()
{
	return this->opType;
}

UCHAR TcOperator::GetOperandCount()
{
	return 2;
}

bool TcOperator::IsAssign()
{
	return isAssign;
}

TrecPointer<TcOperator> TC_DATA_STRUCT GenerateDefaultOperator(tc_int_op op, UCHAR stringAdd)
{
	return TrecPointer<TcOperator>();
}


class TcBasicAddOp : public TcOperator
{
protected: 
	bool treatNullAsZero;
public:
	TcBasicAddOp(bool treatNullAsZero = false, bool isAssign = false) : TcOperator(isAssign ? tc_int_op::add_assign : tc_int_op::add, isAssign) { this->treatNullAsZero = treatNullAsZero; }

	virtual void Inspect(TDataArray<TrecPointer<TVariable>>& params, ReturnObject& ret)
	{
		if (params.Size() != 2)
		{
			ret.returnCode = ret.ERR_UNSUPPORTED_OP;
			ret.errorMessage.Format(L"Expected 2 Operands for Addition Operation: found %d", params.Size());
			return;
		}

		if (!params[0].Get() || !params[1].Get())
		{
			if (!treatNullAsZero)
			{
				ret.returnCode = ret.ERR_UNSUPPORTED_OP;
				ret.errorMessage.Format(L"Expected 2 Operands for Addition Operation: One is Null");
				return;
			}
			if (!params[0].Get())
				params[0] = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(0);
			if (!params[1].Get())
				params[1] = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(0);
		}
		if (params[0]->GetVarType() != var_type::primitive || params[1]->GetVarType() != var_type::primitive)
		{
			ret.returnCode = ret.ERR_UNSUPPORTED_OP;
			ret.errorMessage.Format(L"Expected 2 Operands for Addition Operation: One is Not a Number");
			return;
		}
	}

	virtual void PerformOperation(TDataArray<TrecPointer<TVariable>>& params, ReturnObject& ret)
	{
		Inspect(params, ret);
		if (ret.returnCode)
			return;

		DoubleLong p1 = DoubleLong::GetValueFromPrimitive(params[0]), p2 = DoubleLong::GetValueFromPrimitive(params[1]);
		DoubleLong dlRet = Add(p1, p2);
		switch (dlRet.type)
		{
		case double_long::dl_double:
			ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(dlRet.value.d);
			break;
		case double_long::dl_unsign:
			ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(dlRet.value.u);
			break;
		case double_long::dl_sign:
			ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(dlRet.value.s);
		}
	}
};


class TcStringAddOp : public TcOperator
{
protected:
	bool treatNullAsZero;
	UCHAR strHandle;
public:
	TcStringAddOp(UCHAR strHandle, bool treatNullAsZero = false, bool isAssign = false) : TcOperator(isAssign ? tc_int_op::add_assign : tc_int_op::add, isAssign)
	{
		this->treatNullAsZero = treatNullAsZero;
		this->strHandle = strHandle;
	}

	virtual void Inspect(TDataArray<TrecPointer<TVariable>>& params, ReturnObject& ret)
	{
		if (params.Size() != 2)
		{
			ret.returnCode = ret.ERR_UNSUPPORTED_OP;
			ret.errorMessage.Format(L"Expected 2 Operands for Addition Operation: found %d", params.Size());
			return;
		}

		if (!params[0].Get() || !params[1].Get())
		{
			if (!treatNullAsZero)
			{
				ret.returnCode = ret.ERR_UNSUPPORTED_OP;
				ret.errorMessage.Format(L"Expected 2 Operands for Addition Operation: One is Null");
				return;
			}
			if (!params[0].Get())
				params[0] = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(0);
			if (!params[1].Get())
				params[1] = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(0);
		}
		if ((params[0]->GetVarType() != var_type::primitive && params[0]->GetVarType() != var_type::string) 
			|| (params[1]->GetVarType() != var_type::primitive && params[1]->GetVarType() != var_type::string))
		{
			ret.returnCode = ret.ERR_UNSUPPORTED_OP;
			ret.errorMessage.Format(L"Expected 2 Operands for Addition Operation: One is Not a Number or a String");
			return;
		}
	}

	virtual void PerformOperation(TDataArray<TrecPointer<TVariable>>& params, ReturnObject& ret)
	{
		Inspect(params, ret);
		if (ret.returnCode)
			return;


		DoubleLong p1 = DoubleLong::GetValueFromPrimitive(params[0]), p2 = DoubleLong::GetValueFromPrimitive(params[1]);

		if (p1.type == double_long::dl_invalid && (0b00100000 & strHandle))
		{
			p1 = DoubleLong::GetValueFromString(params[0]->GetString());
		}

		if (p2.type == double_long::dl_invalid && (0b00000010 & strHandle))
		{
			p2 = DoubleLong::GetValueFromString(params[0]->GetString());
		}

		if ((p1.type == double_long::dl_invalid || p2.type == double_long::dl_invalid) ||
			(params[0]->GetVarType() == var_type::string && (0b01000000 & strHandle)) ||
			(params[1]->GetVarType() == var_type::string && (0b00000100 & strHandle)))
		{
			ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(params[0]->GetString() + params[1]->GetString());
			return;
		}

		DoubleLong dlRet = Add(p1, p2);

		if ((params[0]->GetVarType() == var_type::string && (0b10000000 & strHandle)) ||
			(params[1]->GetVarType() == var_type::string && (0b00001000 & strHandle)))
		{
			ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(dlRet.ToString());
			return;
		}
		switch (dlRet.type)
		{
		case double_long::dl_double:
			ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(dlRet.value.d);
			break;
		case double_long::dl_unsign:
			ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(dlRet.value.u);
			break;
		case double_long::dl_sign:
			ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(dlRet.value.s);
		}
		
	}
};

class TcBasicSubOp : public TcOperator
{
protected:
	bool treatNullAsZero;
public:
	TcBasicSubOp(bool treatNullAsZero = false, bool isAssign = false) : TcOperator(isAssign ? tc_int_op::sub_assign : tc_int_op::sub, isAssign) { this->treatNullAsZero = treatNullAsZero; }

	virtual void Inspect(TDataArray<TrecPointer<TVariable>>& params, ReturnObject& ret)
	{
		if (params.Size() != 2)
		{
			ret.returnCode = ret.ERR_UNSUPPORTED_OP;
			ret.errorMessage.Format(L"Expected 2 Operands for Subtraction Operation: found %d", params.Size());
			return;
		}

		if (!params[0].Get() || !params[1].Get())
		{
			if (!treatNullAsZero)
			{
				ret.returnCode = ret.ERR_UNSUPPORTED_OP;
				ret.errorMessage.Format(L"Expected 2 Operands for Subtraction Operation: One is Null");
				return;
			}
			if (!params[0].Get())
				params[0] = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(0);
			if (!params[1].Get())
				params[1] = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(0);
		}
		if (params[0]->GetVarType() != var_type::primitive || params[1]->GetVarType() != var_type::primitive)
		{
			ret.returnCode = ret.ERR_UNSUPPORTED_OP;
			ret.errorMessage.Format(L"Expected 2 Operands for Subtraction Operation: One is Not a Number");
			return;
		}
	}

	virtual void PerformOperation(TDataArray<TrecPointer<TVariable>>& params, ReturnObject& ret)
	{
		Inspect(params, ret);
		if (ret.returnCode)
			return;

		DoubleLong p1 = DoubleLong::GetValueFromPrimitive(params[0]), p2 = DoubleLong::GetValueFromPrimitive(params[1]);
		DoubleLong dlRet = Subtract(p1, p2);
		switch (dlRet.type)
		{
		case double_long::dl_double:
			ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(dlRet.value.d);
			break;
		case double_long::dl_unsign:
			ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(dlRet.value.u);
			break;
		case double_long::dl_sign:
			ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(dlRet.value.s);
		}
	}
};


class TcBasicMulOp : public TcOperator
{
protected:
	bool treatNullAsZero;
public:
	TcBasicMulOp(bool treatNullAsZero = false, bool isAssign = false) : TcOperator(isAssign ? tc_int_op::mul_assign : tc_int_op::mult, isAssign) { this->treatNullAsZero = treatNullAsZero; }

	virtual void Inspect(TDataArray<TrecPointer<TVariable>>& params, ReturnObject& ret)
	{
		if (params.Size() != 2)
		{
			ret.returnCode = ret.ERR_UNSUPPORTED_OP;
			ret.errorMessage.Format(L"Expected 2 Operands for Multiplication Operation: found %d", params.Size());
			return;
		}

		if (!params[0].Get() || !params[1].Get())
		{
			if (!treatNullAsZero)
			{
				ret.returnCode = ret.ERR_UNSUPPORTED_OP;
				ret.errorMessage.Format(L"Expected 2 Operands for Multiplication Operation: One is Null");
				return;
			}
			if (!params[0].Get())
				params[0] = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(0);
			if (!params[1].Get())
				params[1] = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(0);
		}
		if (params[0]->GetVarType() != var_type::primitive || params[1]->GetVarType() != var_type::primitive)
		{
			ret.returnCode = ret.ERR_UNSUPPORTED_OP;
			ret.errorMessage.Format(L"Expected 2 Operands for Multiplication Operation: One is Not a Number");
			return;
		}
	}

	virtual void PerformOperation(TDataArray<TrecPointer<TVariable>>& params, ReturnObject& ret)
	{
		Inspect(params, ret);
		if (ret.returnCode)
			return;

		DoubleLong p1 = DoubleLong::GetValueFromPrimitive(params[0]), p2 = DoubleLong::GetValueFromPrimitive(params[1]);
		DoubleLong dlRet = Multiply(p1, p2);
		switch (dlRet.type)
		{
		case double_long::dl_double:
			ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(dlRet.value.d);
			break;
		case double_long::dl_unsign:
			ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(dlRet.value.u);
			break;
		case double_long::dl_sign:
			ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(dlRet.value.s);
		}
	}
};

class TcBasicDivOp : public TcOperator
{
protected:
	bool treatNullAsZero;
public:
	TcBasicDivOp(bool treatNullAsZero = false, bool isAssign = false) : TcOperator(isAssign ? tc_int_op::div_assign : tc_int_op::div, isAssign) { this->treatNullAsZero = treatNullAsZero; }

	virtual void Inspect(TDataArray<TrecPointer<TVariable>>& params, ReturnObject& ret)
	{
		if (params.Size() != 2)
		{
			ret.returnCode = ret.ERR_UNSUPPORTED_OP;
			ret.errorMessage.Format(L"Expected 2 Operands for Division Operation: found %d", params.Size());
			return;
		}

		if (!params[0].Get() || !params[1].Get())
		{
			if (!treatNullAsZero)
			{
				ret.returnCode = ret.ERR_UNSUPPORTED_OP;
				ret.errorMessage.Format(L"Expected 2 Operands for Division Operation: One is Null");
				return;
			}
			if (!params[0].Get())
				params[0] = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(0);
			if (!params[1].Get())
				params[1] = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(0);
		}
		if (params[0]->GetVarType() != var_type::primitive || params[1]->GetVarType() != var_type::primitive)
		{
			ret.returnCode = ret.ERR_UNSUPPORTED_OP;
			ret.errorMessage.Format(L"Expected 2 Operands for Division Operation: One is Not a Number");
			return;
		}
	}

	virtual void PerformOperation(TDataArray<TrecPointer<TVariable>>& params, ReturnObject& ret)
	{
		Inspect(params, ret);
		if (ret.returnCode)
			return;

		DoubleLong p1 = DoubleLong::GetValueFromPrimitive(params[0]), p2 = DoubleLong::GetValueFromPrimitive(params[1]);
		DoubleLong dlRet = Divide(p1, p2);
		switch (dlRet.type)
		{
		case double_long::dl_double:
			ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(dlRet.value.d);
			break;
		case double_long::dl_unsign:
			ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(dlRet.value.u);
			break;
		case double_long::dl_sign:
			ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(dlRet.value.s);
		}
	}
};

class TcBasicModOp : public TcOperator
{
protected:
	bool treatNullAsZero;
public:
	TcBasicModOp(bool treatNullAsZero = false, bool isAssign = false) : TcOperator(isAssign ? tc_int_op::add_assign : tc_int_op::add, isAssign) { this->treatNullAsZero = treatNullAsZero; }

	virtual void Inspect(TDataArray<TrecPointer<TVariable>>& params, ReturnObject& ret)
	{
		if (params.Size() != 2)
		{
			ret.returnCode = ret.ERR_UNSUPPORTED_OP;
			ret.errorMessage.Format(L"Expected 2 Operands for Multiplication Operation: found %d", params.Size());
			return;
		}

		if (!params[0].Get() || !params[1].Get())
		{
			if (!treatNullAsZero)
			{
				ret.returnCode = ret.ERR_UNSUPPORTED_OP;
				ret.errorMessage.Format(L"Expected 2 Operands for Multiplication Operation: One is Null");
				return;
			}
			if (!params[0].Get())
				params[0] = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(0);
			if (!params[1].Get())
				params[1] = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(0);
		}
		if (params[0]->GetVarType() != var_type::primitive || params[1]->GetVarType() != var_type::primitive)
		{
			ret.returnCode = ret.ERR_UNSUPPORTED_OP;
			ret.errorMessage.Format(L"Expected 2 Operands for Multiplication Operation: One is Not a Number");
			return;
		}
	}

	virtual void PerformOperation(TDataArray<TrecPointer<TVariable>>& params, ReturnObject& ret)
	{
		Inspect(params, ret);
		if (ret.returnCode)
			return;

		DoubleLong p1 = DoubleLong::GetValueFromPrimitive(params[0]), p2 = DoubleLong::GetValueFromPrimitive(params[1]);
		DoubleLong dlRet = ModDivide(p1, p2);
		switch (dlRet.type)
		{
		case double_long::dl_double:
			ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(dlRet.value.d);
			break;
		case double_long::dl_unsign:
			ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(dlRet.value.u);
			break;
		case double_long::dl_sign:
			ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(dlRet.value.s);
		}
	}
};

bool IsOpVarTrue(UCHAR traits, TrecPointer<TVariable> v)
{
	if (!v.Get()) return false;

	switch (v->GetVarType())
	{
	case var_type::special_value:
		return false;
	case var_type::collection:
		return (traits & 0b00000010) || v->GetSize();
	case var_type::string:
		return (traits & 0b00000100) || v->GetSize();
	case var_type::primitive:
	case var_type::primitive_formatted:
		return v->Get8Value() != 0;
	case var_type::native_object:
		return dynamic_cast<TObjectVariable*>(v.Get())->GetObjectW().Get() != nullptr;
	default:
		return true;
	}
}

class TcLogicalAndOp : public TcOperator
{
protected:
	UCHAR traits;
public:
	TcLogicalAndOp(UCHAR t = 0, bool isAssign = false) : TcOperator(isAssign ? tc_int_op::and_assign : tc_int_op::and_l, isAssign) { this->traits = t; }

	virtual void Inspect(TDataArray<TrecPointer<TVariable>>& params, ReturnObject& ret)
	{
		if (params.Size() != 2)
		{
			ret.returnCode = ret.ERR_UNSUPPORTED_OP;
			ret.errorMessage.Format(L"Expected 2 Operands for Logical-And Operation: found %d", params.Size());
			return;
		}

	}

	virtual void PerformOperation(TDataArray<TrecPointer<TVariable>>& params, ReturnObject& ret)
	{
		Inspect(params, ret);
		if (ret.returnCode) return;

		ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(IsOpVarTrue(traits, params[0]) && IsOpVarTrue(traits, params[1]));
	}
};

class TcLogicalOrOp : public TcOperator
{
protected:
	UCHAR traits;
public:
	TcLogicalOrOp(UCHAR t = 0, bool isAssign = false) : TcOperator(isAssign ? tc_int_op::or_assign : tc_int_op::or_l, isAssign) { this->traits = t; }

	virtual void Inspect(TDataArray<TrecPointer<TVariable>>& params, ReturnObject& ret)
	{
		if (params.Size() != 2)
		{
			ret.returnCode = ret.ERR_UNSUPPORTED_OP;
			ret.errorMessage.Format(L"Expected 2 Operands for Logical-Or Operation: found %d", params.Size());
			return;
		}

	}

	virtual void PerformOperation(TDataArray<TrecPointer<TVariable>>& params, ReturnObject& ret)
	{
		Inspect(params, ret);
		if (ret.returnCode) return;

		ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(IsOpVarTrue(traits, params[0]) || IsOpVarTrue(traits, params[1]));
	}
};


class TcLogicalNotOp : public TcOperator
{
protected:
	UCHAR traits;
public:
	TcLogicalNotOp(UCHAR t = 0) : TcOperator(tc_int_op::not_l, false) { this->traits = t; }

	virtual UCHAR GetOperandCount()override
	{
		return 1;
	}

	virtual void Inspect(TDataArray<TrecPointer<TVariable>>& params, ReturnObject& ret)
	{
		if (params.Size() != 1)
		{
			ret.returnCode = ret.ERR_UNSUPPORTED_OP;
			ret.errorMessage.Format(L"Expected 1 Operands for Logical-Not Operation: found %d", params.Size());
			return;
		}

	}

	virtual void PerformOperation(TDataArray<TrecPointer<TVariable>>& params, ReturnObject& ret)
	{
		Inspect(params, ret);
		if (ret.returnCode) return;

		ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(!IsOpVarTrue(traits, params[0]));
	}
};


class TcLogicalXorOp : public TcOperator
{
protected:
	UCHAR traits;
public:
	TcLogicalXorOp(UCHAR t = 0, bool isAssign = false) : TcOperator(isAssign ? tc_int_op::xor_assign : tc_int_op::xor_l, isAssign) { this->traits = t; }

	virtual void Inspect(TDataArray<TrecPointer<TVariable>>& params, ReturnObject& ret)
	{
		if (params.Size() != 2)
		{
			ret.returnCode = ret.ERR_UNSUPPORTED_OP;
			ret.errorMessage.Format(L"Expected 2 Operands for Logical-Xor Operation: found %d", params.Size());
			return;
		}

	}

	virtual void PerformOperation(TDataArray<TrecPointer<TVariable>>& params, ReturnObject& ret)
	{
		Inspect(params, ret);
		if (ret.returnCode) return;

		ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(IsOpVarTrue(traits, params[0]) && IsOpVarTrue(traits, params[1]));
	}
};


class TcBitwiseAndOp : public TcOperator
{
protected:
	UCHAR traits;
public:
	TcBitwiseAndOp(UCHAR t = 0, bool isAssign = false) : TcOperator(isAssign ? tc_int_op::b_and_assign : tc_int_op::and_b, isAssign) { this->traits = t; }

	virtual void Inspect(TDataArray<TrecPointer<TVariable>>& params, ReturnObject& ret)
	{
		if (params.Size() != 2)
		{
			ret.returnCode = ret.ERR_UNSUPPORTED_OP;
			ret.errorMessage.Format(L"Expected 2 Operands for Bitwise-And Operation: found %d", params.Size());
			return;
		}

	}

	virtual void PerformOperation(TDataArray<TrecPointer<TVariable>>& params, ReturnObject& ret)
	{
		Inspect(params, ret);
		if (ret.returnCode) return;

		ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>( params[0]->Get8Value() & params[1]->Get8Value());
	}
};


class TcBitwiseOrOp : public TcOperator
{
protected:
	UCHAR traits;
public:
	TcBitwiseOrOp(UCHAR t = 0, bool isAssign = false) : TcOperator(isAssign ? tc_int_op::b_or_assign : tc_int_op::and_b, isAssign) { this->traits = t; }

	virtual void Inspect(TDataArray<TrecPointer<TVariable>>& params, ReturnObject& ret)
	{
		if (params.Size() != 2)
		{
			ret.returnCode = ret.ERR_UNSUPPORTED_OP;
			ret.errorMessage.Format(L"Expected 2 Operands for Bitwise-Or Operation: found %d", params.Size());
			return;
		}

	}

	virtual void PerformOperation(TDataArray<TrecPointer<TVariable>>& params, ReturnObject& ret)
	{
		Inspect(params, ret);
		if (ret.returnCode) return;

		ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(params[0]->Get8Value() | params[1]->Get8Value());
	}
};

class TcBitwiseXorOp : public TcOperator
{
protected:
	UCHAR traits;
public:
	TcBitwiseXorOp(UCHAR t = 0, bool isAssign = false) : TcOperator(isAssign ? tc_int_op::b_xor_assign : tc_int_op::xor_b, isAssign) { this->traits = t; }

	virtual void Inspect(TDataArray<TrecPointer<TVariable>>& params, ReturnObject& ret)
	{
		if (params.Size() != 2)
		{
			ret.returnCode = ret.ERR_UNSUPPORTED_OP;
			ret.errorMessage.Format(L"Expected 2 Operands for Bitwise-And Operation: found %d", params.Size());
			return;
		}

	}

	virtual void PerformOperation(TDataArray<TrecPointer<TVariable>>& params, ReturnObject& ret)
	{
		Inspect(params, ret);
		if (ret.returnCode) return;

		ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(params[0]->Get8Value() & params[1]->Get8Value());
	}
};

class TcBitwiseLeftOp : public TcOperator
{
protected:
	UCHAR traits;
public:
	TcBitwiseLeftOp(UCHAR t = 0, bool isAssign = false) : TcOperator(isAssign ? tc_int_op::b_left_assign : tc_int_op::left_b, isAssign) { this->traits = t; }

	virtual void Inspect(TDataArray<TrecPointer<TVariable>>& params, ReturnObject& ret)
	{
		if (params.Size() != 2)
		{
			ret.returnCode = ret.ERR_UNSUPPORTED_OP;
			ret.errorMessage.Format(L"Expected 2 Operands for Bitwise-Left Shift Operation: found %d", params.Size());
			return;
		}

	}

	virtual void PerformOperation(TDataArray<TrecPointer<TVariable>>& params, ReturnObject& ret)
	{
		Inspect(params, ret);
		if (ret.returnCode) return;

		ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(params[0]->Get8Value() << params[1]->Get4Value());
	}
};

class TcBitwiseRightOp : public TcOperator
{
protected:
	UCHAR traits;
public:
	TcBitwiseRightOp(UCHAR t = 0, bool isAssign = false) : TcOperator(isAssign ? tc_int_op::b_right_assign : tc_int_op::right_b, isAssign) { this->traits = t; }

	virtual void Inspect(TDataArray<TrecPointer<TVariable>>& params, ReturnObject& ret)
	{
		if (params.Size() != 2)
		{
			ret.returnCode = ret.ERR_UNSUPPORTED_OP;
			ret.errorMessage.Format(L"Expected 2 Operands for Bitwise-RIght Shift Operation: found %d", params.Size());
			return;
		}

	}

	virtual void PerformOperation(TDataArray<TrecPointer<TVariable>>& params, ReturnObject& ret)
	{
		Inspect(params, ret);
		if (ret.returnCode) return;

		ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(params[0]->Get8Value() >> params[1]->Get4Value());
	}
};

TrecPointer<TcOperator>TC_DATA_STRUCT GenerateDefaultOperator(tc_int_op op, bool treatNullAsZero, UCHAR stringAdd, UCHAR container)
{
	switch (op)
	{
		// Arithmetic
	case tc_int_op::add:
		return stringAdd ? TrecPointerKey::GetNewSelfTrecPointerAlt<TcOperator, TcStringAddOp>(treatNullAsZero):
			TrecPointerKey::GetNewSelfTrecPointerAlt<TcOperator, TcBasicAddOp>(stringAdd, treatNullAsZero);
	case tc_int_op::add_assign:
		return stringAdd ? TrecPointerKey::GetNewSelfTrecPointerAlt<TcOperator, TcStringAddOp>(treatNullAsZero,true) :
			TrecPointerKey::GetNewSelfTrecPointerAlt<TcOperator, TcBasicAddOp>(stringAdd, treatNullAsZero,true);
	case tc_int_op::sub:
		return TrecPointerKey::GetNewSelfTrecPointerAlt<TcOperator, TcBasicSubOp>(stringAdd, treatNullAsZero);
	case tc_int_op::sub_assign:
		return TrecPointerKey::GetNewSelfTrecPointerAlt<TcOperator, TcBasicSubOp>(stringAdd, treatNullAsZero,true);
	case tc_int_op::mult:
		return TrecPointerKey::GetNewSelfTrecPointerAlt<TcOperator, TcBasicMulOp>(stringAdd, treatNullAsZero);
	case tc_int_op::mul_assign:
		return TrecPointerKey::GetNewSelfTrecPointerAlt<TcOperator, TcBasicMulOp>(stringAdd, treatNullAsZero, true);
	case tc_int_op::div:
		return TrecPointerKey::GetNewSelfTrecPointerAlt<TcOperator, TcBasicDivOp>(stringAdd, treatNullAsZero);
	case tc_int_op::div_assign:
		return TrecPointerKey::GetNewSelfTrecPointerAlt<TcOperator, TcBasicDivOp>(stringAdd, treatNullAsZero, true);
	case tc_int_op::mod:
		return TrecPointerKey::GetNewSelfTrecPointerAlt<TcOperator, TcBasicModOp>(stringAdd, treatNullAsZero);
	case tc_int_op::mod_assign:
		return TrecPointerKey::GetNewSelfTrecPointerAlt<TcOperator, TcBasicModOp>(stringAdd, treatNullAsZero, true);

		// Logical
	case tc_int_op::and_l:
		return TrecPointerKey::GetNewSelfTrecPointerAlt<TcOperator, TcLogicalAndOp>(stringAdd);
	case tc_int_op::and_assign:
		return TrecPointerKey::GetNewSelfTrecPointerAlt<TcOperator, TcLogicalAndOp>(stringAdd, true);
	case tc_int_op::or_l:
		return TrecPointerKey::GetNewSelfTrecPointerAlt<TcOperator, TcLogicalOrOp>(stringAdd);
	case tc_int_op::or_assign:
		return TrecPointerKey::GetNewSelfTrecPointerAlt<TcOperator, TcLogicalOrOp>(stringAdd, true);
	case tc_int_op::xor_l:
		return TrecPointerKey::GetNewSelfTrecPointerAlt<TcOperator, TcLogicalXorOp>(stringAdd);
	case tc_int_op::xor_assign:
		return TrecPointerKey::GetNewSelfTrecPointerAlt<TcOperator, TcLogicalXorOp>(stringAdd, true);
	case tc_int_op::not_l:
		return TrecPointerKey::GetNewSelfTrecPointerAlt<TcOperator, TcLogicalNotOp>(stringAdd);

		// Bitwise
	case tc_int_op::and_b:
		return TrecPointerKey::GetNewSelfTrecPointerAlt<TcOperator, TcBitwiseAndOp>(stringAdd);
	case tc_int_op::b_and_assign:
		return TrecPointerKey::GetNewSelfTrecPointerAlt<TcOperator, TcBitwiseAndOp>(stringAdd, true);
	case tc_int_op::or_b:
		return TrecPointerKey::GetNewSelfTrecPointerAlt<TcOperator, TcBitwiseOrOp>(stringAdd);
	case tc_int_op::b_or_assign:
		return TrecPointerKey::GetNewSelfTrecPointerAlt<TcOperator, TcBitwiseOrOp>(stringAdd, true);
	case tc_int_op::xor_b:
		return TrecPointerKey::GetNewSelfTrecPointerAlt<TcOperator, TcBitwiseXorOp>(stringAdd);
	case tc_int_op::b_xor_assign:
		return TrecPointerKey::GetNewSelfTrecPointerAlt<TcOperator, TcBitwiseXorOp>(stringAdd, true);
	case tc_int_op::left_b:
		return TrecPointerKey::GetNewSelfTrecPointerAlt<TcOperator, TcBitwiseLeftOp>(stringAdd);
	case tc_int_op::b_left_assign:
		return TrecPointerKey::GetNewSelfTrecPointerAlt<TcOperator, TcBitwiseLeftOp>(stringAdd, true);
	case tc_int_op::right_b:
		return TrecPointerKey::GetNewSelfTrecPointerAlt<TcOperator, TcBitwiseRightOp>(stringAdd);
	case tc_int_op::b_right_assign:
		return TrecPointerKey::GetNewSelfTrecPointerAlt<TcOperator, TcBitwiseRightOp>(stringAdd, true);
	}
	return TrecPointer<TcOperator>();
}
