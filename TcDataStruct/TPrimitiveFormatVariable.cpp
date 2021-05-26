#include "TPrimitiveFormatVariable.h"

/**
 * Method: TPrimitiveFormatVariable::TPrimitiveFormatVariable
 * Purpose: Constructor
 * Parameters: TPrimitiveVariable& - reference to an existing variable without formatting
 * Returns: New Primitive Variable object
 */
TPrimitiveFormatVariable::TPrimitiveFormatVariable(TPrimitiveVariable& var) : TPrimitiveVariable(var)
{
	flags = width = precision = 0;
	useExponent = false;
}

/**
 * Method: TPrimitiveFormatVariable::TPrimitiveFormatVariable
 * Purpose: Constructor
 * Parameters: const TPrimitiveFormatVariable& - reference to an existing variable without formatting
 * Returns: New Primitive Variable object
 */
TPrimitiveFormatVariable::TPrimitiveFormatVariable(const TPrimitiveFormatVariable& var): TPrimitiveVariable(var)
{
	this->flags = var.flags;
	this->precision = var.precision;
	this->width = var.width;
	this->useExponent = var.useExponent;
}

/**
 * Method: TPrimitiveFormatVariable::SetRightJustify
 * Purpose: Sets the Right Justify Flag
 * Parameters: bool rJust - whether the formatting should be Right Justified (default of printing statements, and thus default of this method)
 * Returns: void
 */
void TPrimitiveFormatVariable::SetRightJustify(bool rJust)
{
	ThreadLock();
	flags = rJust ? (flags | 0b00000001) : (flags & 0b11111110);
	ThreadRelease();
}

/**
 * Method: GetRightJustify
 * Purpose: Reports whether the variable is set to right Justify
 * Parameters: void
 * Returns: bool - whether the variable is set to right justify
 */
bool TPrimitiveFormatVariable::GetRightJustify()
{
	ThreadLock();
	bool ret = flags & 0b00000001;
	ThreadRelease();
	return ret;
}

/**
 * Method: TPrimitiveFormatVariable::SetBlank
 * Purpose: Sets the Blank Flag
 * Parameters: bool rBlank - whether the formatting should have a blank (default of printing statements is false, and thus default of this method)
 * Returns: void
 */
void TPrimitiveFormatVariable::SetBlank(bool rBlank)
{
	ThreadLock();
	flags = rBlank ? (flags | 0b00000010) : (flags & 0b11111101);
	ThreadRelease();
}

/**
 * Method: GetBlank
 * Purpose: Reports whether the variable is set to right Justify
 * Parameters: void
 * Returns: bool - whether the variable is set to print a Blank
 */
bool TPrimitiveFormatVariable::GetBlank()
{
	ThreadLock();
	bool ret = flags & 0b00000010;
	ThreadRelease();
	return ret;
}

/**
 * Method: TPrimitiveFormatVariable::SetBlank
 * Purpose: Sets the Prefix Flag
 * Parameters: bool rPrefix - whether the formatting should have a blank (default of printing statements is false, and thus default of this method)
 * Returns: void
 */
void TPrimitiveFormatVariable::SetPrefix(bool rPrefix)
{
	ThreadLock();
	flags = rPrefix ? (flags | 0b00000100) : (flags & 0b11111011);
	ThreadRelease();
}

/**
 * Method: GetPrefix
 * Purpose: Reports whether the variable is set to right Justify
 * Parameters: void
 * Returns: bool - whether the variable is set to right justify
 */
bool TPrimitiveFormatVariable::GetPrefix()
{
	ThreadLock();
	bool ret = flags & 0b00000100;
	ThreadRelease();
	return ret;
}

/**
 * Method: TPrimitiveFormatVariable::SetFormattingWidth
 * Purpose: Sets the formatting width
 * Parameters: UCHAR w - the width of the Method
 * Returns: void
 *
 * Note: For memory safety, the max width is set to 20. Any higher value in the parameter will default to 20
 */
void TPrimitiveFormatVariable::SetFormattingWidth(UCHAR w)
{
	ThreadLock();
	width = w;
	if (width > 20)
		width = 20;
	ThreadRelease();
}

/**
 * Method: TPrimitiveFormatVariable::GetFormattingWidth
 * Purpose: Gets the formatting width
 * Parameters: void
 * Returns: UCHAR w - the width of the Method
 */
UCHAR TPrimitiveFormatVariable::GetFormattingWidth(UCHAR w)
{
	ThreadLock();
	UCHAR ret = width;
	ThreadRelease();
	return ret;
}

/**
 * Method: TPrimitiveFormatVariable::SetFormattingPrecision
 * Purpose: Sets the formatting Precision
 * Parameters: UCHAR w - the Precision of the Method
 * Returns: void
 *
 * Note: For memory safety, the max Precision is set to 20. Any higher value in the parameter will default to 20
 */
void TPrimitiveFormatVariable::SetFormattingPrecision(UCHAR w)
{
	ThreadLock();
	precision = w;
	if (precision > 20)
		precision = 20;
	ThreadRelease();
}

/**
 * Method: TPrimitiveFormatVariable::GetFormattingPrecision
 * Purpose: Gets the formatting Precision
 * Parameters: void
 * Returns: UCHAR w - the Precision of the Method
 */
UCHAR TPrimitiveFormatVariable::GetFormattingPrecision(UCHAR w)
{
	ThreadLock();
	UCHAR ret = precision;
	ThreadRelease();
	return ret;
}

TrecPointer<TVariable> TPrimitiveFormatVariable::Clone()
{
	ThreadLock();
	auto ret = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveFormatVariable>(*this);
	ThreadRelease();
	return ret;
}

/**
 * Method: TPrimitiveVariable::GetVarType
 * Purpose: Reports the type of varible that this object represents
 * Parameters: void
 * Returns: var_type - the type of variable this represents
 *
 * Attributes: override
 */
var_type TPrimitiveFormatVariable::GetVarType()
{
	return var_type::primitive_formatted;
}

TString TPrimitiveFormatVariable::GetString()
{
	ThreadLock();
	TString format(L"%");

	if (flags & 0b00000001)
		format.AppendChar(L'-');
	if (flags & 0b00000010)
		format.AppendChar(L' ');
	if (flags & 0b00000100)
		format.AppendChar(L'#');

	if (width)
		format.AppendFormat(L"%i", width);
	if (precision)
		format.AppendFormat(L".%i", precision);
	TString ret(TPrimitiveVariable::GetString(format));
	ThreadRelease();
	return ret;
}

bool TPrimitiveFormatVariable::GetUseExponent()
{
	return useExponent;
}

void TPrimitiveFormatVariable::SetUseExponent(bool useExponent)
{
	this->useExponent = useExponent;
}
