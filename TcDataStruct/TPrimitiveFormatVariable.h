#pragma once
#include "TPrimitiveVariable.h"

/**
 * Class: TPrimitiveFormatVariable
 * Purpose: Adds formatting data to the Primitive Variable
 */
class TPrimitiveFormatVariable :
    public TPrimitiveVariable
{
    friend class TrecPointerKey;
    friend class TPrimitiveFormatVariable;
public:

    // Foratting Specific Methods

    /**
     * Method: TPrimitiveFormatVariable::TPrimitiveFormatVariable
     * Purpose: Constructor
     * Parameters: TPrimitiveVariable& - reference to an existing variable without formatting
     * Returns: New Primitive Variable object
     */
    TPrimitiveFormatVariable(TPrimitiveVariable& var);

    /**
     * Method: TPrimitiveFormatVariable::TPrimitiveFormatVariable
     * Purpose: Constructor
     * Parameters: const TPrimitiveFormatVariable& - reference to an existing variable without formatting
     * Returns: New Primitive Variable object
     */
    TPrimitiveFormatVariable(const TPrimitiveFormatVariable& var);

    /**
     * Method: TPrimitiveFormatVariable::SetRightJustify
     * Purpose: Sets the Right Justify Flag
     * Parameters: bool rJust - whether the formatting should be Right Justified (default of printing statements, and thus default of this method)
     * Returns: void
     */
    void SetRightJustify(bool rJust = true);

    /**
     * Method: GetRightJustify
     * Purpose: Reports whether the variable is set to right Justify
     * Parameters: void
     * Returns: bool - whether the variable is set to right justify 
     */
    bool GetRightJustify();


    /**
     * Method: TPrimitiveFormatVariable::SetBlank
     * Purpose: Sets the Blank Flag
     * Parameters: bool rBlank - whether the formatting should have a blank (default of printing statements is false, and thus default of this method)
     * Returns: void
     */
    void SetBlank(bool rBlank = false);

    /**
     * Method: GetBlank
     * Purpose: Reports whether the variable is set to right Justify
     * Parameters: void
     * Returns: bool - whether the variable is set to print a Blank
     */
    bool GetBlank();

    /**
     * Method: TPrimitiveFormatVariable::SetBlank
     * Purpose: Sets the Prefix Flag
     * Parameters: bool rPrefix - whether the formatting should have a blank (default of printing statements is false, and thus default of this method)
     * Returns: void
     */
    void SetPrefix(bool rPrefix = false);

    /**
     * Method: GetPrefix
     * Purpose: Reports whether the variable is set to right Justify
     * Parameters: void
     * Returns: bool - whether the variable is set to right justify
     */
    bool GetPrefix();

    /**
     * Method: TPrimitiveFormatVariable::SetFormattingWidth
     * Purpose: Sets the formatting width
     * Parameters: UCHAR w - the width of the Method
     * Returns: void
     * 
     * Note: For memory safety, the max width is set to 20. Any higher value in the parameter will default to 20
     */
    void SetFormattingWidth(UCHAR w);

    /**
     * Method: TPrimitiveFormatVariable::GetFormattingWidth
     * Purpose: Gets the formatting width
     * Parameters: void
     * Returns: UCHAR w - the width of the Method
     */
    UCHAR GetFormattingWidth(UCHAR w);

    /**
     * Method: TPrimitiveFormatVariable::SetFormattingPrecision
     * Purpose: Sets the formatting Precision
     * Parameters: UCHAR w - the Precision of the Method
     * Returns: void
     * 
     * Note: For memory safety, the max Precision is set to 20. Any higher value in the parameter will default to 20
     */
    void SetFormattingPrecision(UCHAR w);

    /**
     * Method: TPrimitiveFormatVariable::GetFormattingPrecision
     * Purpose: Gets the formatting Precision
     * Parameters: void
     * Returns: UCHAR w - the Precision of the Method
     */
    UCHAR GetFormattingPrecision(UCHAR w);

    // Methods Derived from parent classes

    virtual TrecPointer<TVariable> Clone()override;


    /**
     * Method: TPrimitiveVariable::GetVarType
     * Purpose: Reports the type of varible that this object represents
     * Parameters: void
     * Returns: var_type - the type of variable this represents
     *
     * Attributes: override
     */
    virtual var_type GetVarType() override;

    /**
     * Method: TPrimitiveVarible::GetObject
     * Purpose: Returns the Object held by the variable, or null if variable is a raw data type
     * Parameters: void
     * Returns: TString - The TString referered by the variable (empty if not a string)
     *
     * Note: Call "IsObject" first before calling this method as there is no point if the "IsObject" returns false
     *
     * Attributes: override
     */
    virtual TString GetString()override;

protected:
    UCHAR flags, width, precision;

};

