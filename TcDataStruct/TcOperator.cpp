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

class TcTernaryOperator : public TcOperator
{
protected:
	UCHAR traits;
public:
	TcTernaryOperator(UCHAR t = 0) : TcOperator(tc_int_op::conditional) { traits = t; }

	virtual void Inspect(TDataArray<TrecPointer<TVariable>>& params, ReturnObject& ret)
	{
		if (params.Size() != 3)
		{
			ret.returnCode = ret.ERR_UNSUPPORTED_OP;
			ret.errorMessage.Format(L"Expected 3 Operands for Ternary Operation: found %d", params.Size());
			return;
		}

	}

	virtual void PerformOperation(TDataArray<TrecPointer<TVariable>>& params, ReturnObject& ret)
	{
		Inspect(params, ret);
		if (ret.returnCode) return;

		ret.errorObject =IsOpVarTrue(traits, params[0]) ? params[1] : params[2];
	}
};

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

class TcEqualityOperator : public TcOperator
{
protected:
	tc_int_op operatorType;
	UCHAR strHandle;
public:
	TcEqualityOperator(tc_int_op op, UCHAR t = 0)
	{
		if (op > tc_int_op::not_e_t || op < tc_int_op::eq)
			throw L"Not Equality Operator provided!";
		operatorType = op;
	}

	virtual void Inspect(TDataArray<TrecPointer<TVariable>>& params, ReturnObject& ret) override
	{
		if (params.Size() != 2)
		{
			ret.returnCode = ret.ERR_UNSUPPORTED_OP;
			ret.errorMessage.Format(L"Expected 2 Operands for Bitwise-RIght Shift Operation: found %d", params.Size());
			return;
		}

		if (!(strHandle & 1) && (!params[0].Get() || !params[1].Get()))
		{
			ret.returnCode = ret.ERR_UNSUPPORTED_OP;
			ret.errorMessage.Set(L"Expected 2 NON-NULL Operands for Equality Operation");
			return;
		}

		if (!(strHandle & 0b11111100) && ((params[0].Get() && params[0]->GetVarType() == var_type::string) || (params[1].Get() && params[1]->GetVarType() == var_type::string)))
		{
			ret.returnCode = ret.ERR_UNSUPPORTED_OP;
			ret.errorMessage.Set(L"Equality Operation does not support String Operations");
			return;
		}

		// For Now, only allow Primitive Types and String types
		if ((params[0].Get() && (params[0]->GetVarType() != var_type::string && params[0]->GetVarType() != var_type::primitive)) ||
			(params[1].Get() && (params[1]->GetVarType() != var_type::string && params[1]->GetVarType() != var_type::primitive)))
		{
			ret.returnCode = ret.ERR_UNSUPPORTED_OP;
			ret.errorMessage.Set(L"Equality Operation Currently only supports Primitive and String types!");
			return;
		}
	}

	virtual void PerformOperation(TDataArray<TrecPointer<TVariable>>& params, ReturnObject& ret) override
	{
		Inspect(params, ret);
		if (ret.returnCode) return;

		int eqResult = 0;
		bool eqType = false;

		// 0 means both params are equal
		// 1 means that the second param is greater
		// -1 means that the first param is greater

		if (!params[0].Get() && !params[0].Get())
		{
			// They are equal (and we support null)
			eqType = true;
		}
		else if (!params[0].Get())
		{
			eqResult = (0b00000010 & strHandle) && params[1]->GetVarType() == var_type::special_value &&
				dynamic_cast<TSpecialVariable*>(params[1].Get())->GetSpecialValue() == SpecialVar::sp_undefined ? -1 : 1;
		}
		else if (!params[1].Get())
		{
			eqResult = (0b00000010 & strHandle) && params[0]->GetVarType() == var_type::special_value && 
				dynamic_cast<TSpecialVariable*>(params[0].Get())->GetSpecialValue() == SpecialVar::sp_undefined ? 1 : -1;
		}
		else if (params[0]->GetVarType() == var_type::primitive && params[1]->GetVarType() == var_type::primitive)
		{
			DoubleLong dl1 = DoubleLong::GetValueFromPrimitive(params[0]);
			DoubleLong dl2 = DoubleLong::GetValueFromPrimitive(params[1]);

			bool eqType = dl1.type == dl2.type && params[0]->GetVType() == params[1]->GetVType();

			if (dl1 > dl2)
				eqResult = -1;
			else if (dl1 < dl2)
				eqResult = 1;
		}
		else if (params[0]->GetVarType() == var_type::string && params[1]->GetVarType() == var_type::string)
		{
			UCHAR pStringHandle = 0b00001100 & strHandle;
			eqType = true;
			switch (pStringHandle)
			{
			case 0b00001000:
				eqResult = params[0]->GetString().Compare(params[1]->GetString());
				break;
			case 0b00001100: // This mode uses String Compare (no Case Mode)
				eqResult = params[0]->GetString().CompareNoCase(params[1]->GetString());
				break;
			case 0b00000100: // This mode uses the Length of Strings
				if (params[0]->GetSize() > params[1]->GetSize())
					eqResult = -1;
				else if (params[0]->GetSize() < params[1]->GetSize())
					eqResult = 1;
				break;
			case 0:
				if (params[0].Get() > params[1].Get())
					eqResult = -1;
				else if (params[0].Get() < params[1].Get())
					eqResult = 1;
			}
		}
		else if (params[0]->GetVarType() == var_type::string && params[1]->GetVarType() == var_type::primitive)
		{
			eqResult = 1;
			if (strHandle & 0b00010000)
			{
				DoubleLong d1 = DoubleLong::GetValueFromString(params[0]->GetString());
				DoubleLong d2 = DoubleLong::GetValueFromPrimitive(params[1]);

				if (d1.type != double_long::dl_invalid &&
					d2.type != double_long::dl_invalid)
				{
					if (d1 == d2)
						eqResult = 0;
					else if (d1 > d2)
						eqResult = -1;
				}
			}
		}
		else if (params[1]->GetVarType() == var_type::string && params[0]->GetVarType() == var_type::primitive)
		{
			eqResult = -1;
			if (strHandle & 0b00010000)
			{
				DoubleLong d2 = DoubleLong::GetValueFromString(params[1]->GetString());
				DoubleLong d1 = DoubleLong::GetValueFromPrimitive(params[0]);

				if (d1.type != double_long::dl_invalid &&
					d2.type != double_long::dl_invalid)
				{
					if (d1 == d2)
						eqResult = 0;
					else if (d1 > d2)
						eqResult = 1;
				}
			}
		}


		switch (operatorType)
		{
		case tc_int_op::not_e_t: // If the tye is not the same OR if the value is not the same, then return true
			ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(eqResult || !eqType);
			return;
		case tc_int_op::eq:
			eqType = true;
		case tc_int_op::eq_t:
			ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(!eqResult && eqType);
			return;
		case tc_int_op::not_e: // If the tye is not the same OR if the value is not the same, then return true
			ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(eqResult != 0);
			return;

		case tc_int_op::gt:
			ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(eqResult < 0);
			return;
		case tc_int_op::gte:
			ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(eqResult <= 0);
			return;
		case tc_int_op::lt:
			ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(eqResult > 0);
			return;
		case tc_int_op::lte:
			ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(eqResult >= 0);
			return;
		}

	}
};

TrecPointer<TcOperator>TC_DATA_STRUCT GenerateDefaultOperator(tc_int_op op, bool treatNullAsZero, UCHAR stringAdd, UCHAR container)
{
	switch (op)
	{
		// Arithmetic
	case tc_int_op::add:
		return stringAdd ? TrecPointerKey::GetNewTrecPointerAlt<TcOperator, TcStringAddOp>(treatNullAsZero):
			TrecPointerKey::GetNewTrecPointerAlt<TcOperator, TcBasicAddOp>(stringAdd, treatNullAsZero);
	case tc_int_op::add_assign:
		return stringAdd ? TrecPointerKey::GetNewTrecPointerAlt<TcOperator, TcStringAddOp>(treatNullAsZero,true) :
			TrecPointerKey::GetNewTrecPointerAlt<TcOperator, TcBasicAddOp>(treatNullAsZero,true);
	case tc_int_op::sub:
		return TrecPointerKey::GetNewTrecPointerAlt<TcOperator, TcBasicSubOp>(treatNullAsZero);
	case tc_int_op::sub_assign:
		return TrecPointerKey::GetNewTrecPointerAlt<TcOperator, TcBasicSubOp>(treatNullAsZero,true);
	case tc_int_op::mult:
		return TrecPointerKey::GetNewTrecPointerAlt<TcOperator, TcBasicMulOp>(treatNullAsZero);
	case tc_int_op::mul_assign:
		return TrecPointerKey::GetNewTrecPointerAlt<TcOperator, TcBasicMulOp>(treatNullAsZero, true);
	case tc_int_op::div:
		return TrecPointerKey::GetNewTrecPointerAlt<TcOperator, TcBasicDivOp>(treatNullAsZero);
	case tc_int_op::div_assign:
		return TrecPointerKey::GetNewTrecPointerAlt<TcOperator, TcBasicDivOp>(treatNullAsZero, true);
	case tc_int_op::mod:
		return TrecPointerKey::GetNewTrecPointerAlt<TcOperator, TcBasicModOp>(treatNullAsZero);
	case tc_int_op::mod_assign:
		return TrecPointerKey::GetNewTrecPointerAlt<TcOperator, TcBasicModOp>(treatNullAsZero, true);

		// Logical
	case tc_int_op::and_l:
		return TrecPointerKey::GetNewTrecPointerAlt<TcOperator, TcLogicalAndOp>(stringAdd);
	case tc_int_op::and_assign:
		return TrecPointerKey::GetNewTrecPointerAlt<TcOperator, TcLogicalAndOp>(stringAdd, true);
	case tc_int_op::or_l:
		return TrecPointerKey::GetNewTrecPointerAlt<TcOperator, TcLogicalOrOp>(stringAdd);
	case tc_int_op::or_assign:
		return TrecPointerKey::GetNewTrecPointerAlt<TcOperator, TcLogicalOrOp>(stringAdd, true);
	case tc_int_op::xor_l:
		return TrecPointerKey::GetNewTrecPointerAlt<TcOperator, TcLogicalXorOp>(stringAdd);
	case tc_int_op::xor_assign:
		return TrecPointerKey::GetNewTrecPointerAlt<TcOperator, TcLogicalXorOp>(stringAdd, true);
	case tc_int_op::not_l:
		return TrecPointerKey::GetNewTrecPointerAlt<TcOperator, TcLogicalNotOp>(stringAdd);

		// Bitwise
	case tc_int_op::and_b:
		return TrecPointerKey::GetNewTrecPointerAlt<TcOperator, TcBitwiseAndOp>(stringAdd);
	case tc_int_op::b_and_assign:
		return TrecPointerKey::GetNewTrecPointerAlt<TcOperator, TcBitwiseAndOp>(stringAdd, true);
	case tc_int_op::or_b:
		return TrecPointerKey::GetNewTrecPointerAlt<TcOperator, TcBitwiseOrOp>(stringAdd);
	case tc_int_op::b_or_assign:
		return TrecPointerKey::GetNewTrecPointerAlt<TcOperator, TcBitwiseOrOp>(stringAdd, true);
	case tc_int_op::xor_b:
		return TrecPointerKey::GetNewTrecPointerAlt<TcOperator, TcBitwiseXorOp>(stringAdd);
	case tc_int_op::b_xor_assign:
		return TrecPointerKey::GetNewTrecPointerAlt<TcOperator, TcBitwiseXorOp>(stringAdd, true);
	case tc_int_op::left_b:
		return TrecPointerKey::GetNewTrecPointerAlt<TcOperator, TcBitwiseLeftOp>(stringAdd);
	case tc_int_op::b_left_assign:
		return TrecPointerKey::GetNewTrecPointerAlt<TcOperator, TcBitwiseLeftOp>(stringAdd, true);
	case tc_int_op::right_b:
		return TrecPointerKey::GetNewTrecPointerAlt<TcOperator, TcBitwiseRightOp>(stringAdd);
	case tc_int_op::b_right_assign:
		return TrecPointerKey::GetNewTrecPointerAlt<TcOperator, TcBitwiseRightOp>(stringAdd, true);

		// Equality
	case tc_int_op::eq:
	case tc_int_op::eq_t:
	case tc_int_op::gt:
	case tc_int_op::gte:
	case tc_int_op::lt:
	case tc_int_op::lte:
	case tc_int_op::not_e:
	case tc_int_op::not_e_t:
		return TrecPointerKey::GetNewTrecPointerAlt<TcOperator, TcEqualityOperator>(op, stringAdd);
	case tc_int_op::conditional:
		return TrecPointerKey::GetNewTrecPointerAlt<TcOperator, TcTernaryOperator>(stringAdd);
	}
	return TrecPointer<TcOperator>();
}

TcOperatorGroup::TcOperatorGroup()
{
	rightToLeft = true;
}

TcOperatorGroup::TcOperatorGroup(const TcOperatorGroup& copy)
{
	operators = copy.operators;
	rightToLeft = copy.rightToLeft;
}
