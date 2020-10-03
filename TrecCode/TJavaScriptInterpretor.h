#pragma once
#include <TInterpretor.h>

typedef enum class js_statement_type
{
    js_if,
    js_while,
    js_for,
    js_var,
    js_let,
    js_const,
    js_function,
    js_class,
    js_regular

}js_statement_type;


class JavaScriptStatement
{
public:
    JavaScriptStatement(js_statement_type type);
    JavaScriptStatement(const JavaScriptStatement& orig);
    js_statement_type type;

    TString contents;

    UINT lineStart, lineEnd;

    ULONG64 fileStart, fileEnd;
};


/**
 * Class: TJavaScriptInterpretor
 * Purpose: Interpretors JavaScript files
 * 
 * SuperClass: TInterpretor
 */
class TJavaScriptInterpretor :
    public TInterpretor
{

    /**
     * Method: TJavaScriptInterpretor::TInterpretor
     * Purpose: Constructor
     * Parameters: TrecPointer<TInterpretor> parentInterpretor - the Interpretor that created this interpretor (use null if this is a root)
     * Returns: New TInterpretor Object
     */
    TJavaScriptInterpretor(TrecSubPointer<TVariable, TInterpretor> parentInterpretor, TrecPointer<TEnvironment> env);

    /**
     * Method: TJavaScriptInterpretor::SetCode
     * Purpose: Sets the initial file of the code
     * Parameters: TFile& the file to read
     * Returns: UINT - error code
     *
     * Note: call this method if this interpretor is the top level interpretor for the code file, it will filter the comments out
     * 
     * Attributes: override
     */
    virtual UINT SetCode(TFile&)override;

    /**
     * Method: TJavaScriptInterpretor::Run
     * Purpose: Runs the interpretor
     * Parameters: void
     * Returns: ReportObject - objct indicating the success of the program or falure information
     *
     * Note: this method is intended to be called in interpretors that are either top level or basic control blocks
     * 
     * Attributes: override
     */
    virtual ReportObject Run()override;


    /**
     * Method: TJavaScriptInterpretor::Run
     * Purpose: Runs the interpretor with the following parameters
     * Parameters: TDataArray<TVariable> params - list of parameters to provide
     * Returns: ReportObject - objct indicating the success of the program or falure information
     *
     * Note: this method is intended to be called in interpretors that represent specific methods or functions
     * 
     * Attributes: override
     */
    virtual ReportObject Run(TDataArray<TrecPointer<TVariable>>& params)override;

private:

    bool hasOddMiltiLineStrMarkers(const TString& str);

    UINT InsertSemiColons();

    void ProcessParenthBlock(ReportObject& ro, const TString& currentCode, UINT line);


    ULONG64 GetBlockEnd();

    /**
     * Method: TJavaScriptInterpretor::GetNextStatement
     * Purpose: retrieves the tokens 
     */
};

