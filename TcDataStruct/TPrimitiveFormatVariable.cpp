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
}

/**
 * Method: TPrimitiveFormatVariable::SetRightJustify
 * Purpose: Sets the Right Justify Flag
 * Parameters: bool rJust - whether the formatting should be Right Justified (default of printing statements, and thus default of this method)
 * Returns: void
 */
void TPrimitiveFormatVariable::SetRightJustify(bool rJust)
{
	flags = rJust ? (flags | 0b00000001) : (flags & 0b11111110);
}

/**
 * Method: GetRightJustify
 * Purpose: Reports whether the variable is set to right Justify
 * Parameters: void
 * Returns: bool - whether the variable is set to right justify
 */
bool TPrimitiveFormatVariable::GetRightJustify()
{
	return flags & 0b00000001;
}

/**
 * Method: TPrimitiveFormatVariable::SetBlank
 * Purpose: Sets the Blank Flag
 * Parameters: bool rBlank - whether the formatting should have a blank (default of printing statements is false, and thus default of this method)
 * Returns: void
 */
void TPrimitiveFormatVariable::SetBlank(bool rBlank)
{
	flags = rBlank ? (flags | 0b00000010) : (flags & 0b11111101);
}

/**
 * Method: GetBlank
 * Purpose: Reports whether the variable is set to right Justify
 * Parameters: void
 * Returns: bool - whether the variable is set to print a Blank
 */
bool TPrimitiveFormatVariable::GetBlank()
{
	return flags & 0b00000010;
}

/**
 * Method: TPrimitiveFormatVariable::SetBlank
 * Purpose: Sets the Prefix Flag
 * Parameters: bool rPrefix - whether the formatting should have a blank (default of printing statements is false, and thus default of this method)
 * Returns: void
 */
void TPrimitiveFormatVariable::SetPrefix(bool rPrefix)
{
	flags = rPrefix ? (flags | 0b00000100) : (flags & 0b11111011);
}

/**
 * Method: GetPrefix
 * Purpose: Reports whether the variable is set to right Justify
 * Parameters: void
 * Returns: bool - whether the variable is set to right justify
 */
bool TPrimitiveFormatVariable::GetPrefix()
{
	return flags & 0b00000100;
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
	width = w;
	if (width > 20)
		width = 20;
}

/**
 * Method: TPrimitiveFormatVariable::GetFormattingWidth
 * Purpose: Gets the formatting width
 * Parameters: void
 * Returns: UCHAR w - the width of the Method
 */
UCHAR TPrimitiveFormatVariable::GetFormattingWidth(UCHAR w)
{
	return width;
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
	precision = w;
	if (precision > 20)
		precision = 20;
}

/**
 * Method: TPrimitiveFormatVariable::GetFormattingPrecision
 * Purpose: Gets the formatting Precision
 * Parameters: void
 * Returns: UCHAR w - the Precision of the Method
 */
UCHAR TPrimitiveFormatVariable::GetFormattingPrecision(UCHAR w)
{
	return precision;
}

TrecPointer<TVariable> TPrimitiveFormatVariable::Clone()
{
	return TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveFormatVariable>(*this);
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
	TString format(L"%");

	if (flags & 0b00000001)
		format.AppendChar(L'-');
	if (flags & 0b00000010)
		format.AppendChar(L'\s');
	if (flags & 0b00000100)
		format.AppendChar(L'#');

	if (width)
		format.AppendFormat(L"%i", width);
	if (precision)
		format.AppendFormat(L".%i", precision);

	return TPrimitiveVariable::GetString(format);
}
