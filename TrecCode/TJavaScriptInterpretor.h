#pragma once
#include <TInterpretor.h>
#include <TContainerVariable.h>
#include "TrecCode.h"

typedef enum class js_statement_type
{
    js_if,
    js_else,
    js_else_if,
    js_while,
    js_for,
    js_var,
    js_let,
    js_const,
    js_function,
    js_class,
    js_try,
    js_catch,
    js_finally,
    js_regular,
    js_continue,
    js_break,
    js_return,
    js_proto_add
}js_statement_type;


class _TREC_CODE_DLL JavaScriptStatement
{
public:
    JavaScriptStatement(js_statement_type type);
    JavaScriptStatement();
    JavaScriptStatement(const JavaScriptStatement& orig);

    void operator=(const JavaScriptStatement& other);

    TrecSubPointer<TVariable, TInterpretor> body;

    TrecPointer<TVariable> var;

    js_statement_type type;

    TString contents;

    UINT lineStart, lineEnd;

    ULONG64 fileStart, fileEnd;
};

class _TREC_CODE_DLL JavaScriptExpression
{
public:
    JavaScriptExpression();
    JavaScriptExpression(const JavaScriptExpression& orig);
    JavaScriptExpression(const TString& name, TrecPointer<TVariable> value);

    void operator=(const JavaScriptExpression& orig);

    TString varName;
    TrecPointer<TVariable> value;
};

/**
 * Enum Class: JS_Prototype_Op
 * Purpose: What Operation is being performed on the Prototype
 */
typedef enum class JS_Prototype_Op
{
    jpo_add_const,
    jpo_add_objs,
    jpo_delete
}JS_Prototype_Op;


/**
 * Class: TJavaScriptInterpretor
 * Purpose: Interpretors JavaScript files
 * 
 * SuperClass: TInterpretor
 */
class _TREC_CODE_DLL TJavaScriptInterpretor :
    public TInterpretor
{
    friend class TJavaScriptInterpretor;
    friend class TJavaScriptClassInterpretor;
public:
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
     *              bool clearVariables - whether to clrear any exising Variables the Interpretor might hold
     * Returns: ReportObject - objct indicating the success of the program or falure information
     *
     * Note: this method is intended to be called in interpretors that represent specific methods or functions
     * 
     * Attributes: override
     */
    virtual ReportObject Run(TDataArray<TrecPointer<TVariable>>& params, bool clearVariables = true)override;

    virtual void ProcessStatements(ReportObject& ro);

    void setLine(UINT line);

    

protected:
    void AddAssignStatement(const TString& expression, TrecPointer<TVariable> var);
    /**
     * Method: TInterpretor::ProcessAddition
     * Purpose: Filters variables in preparation for performing an addition operation
     * Parameters: TrecPointer<TVariable> var1 - The first variable, representing the first addend
     *              TrecPointer<TVariable> var2 - the second variable, representing the second addend
     * Returns: ReportObject - the result of performing the operation, including error information if it occurs
     *
     * Attributes: virtual
     */
    virtual ReportObject ProcessAddition(TrecPointer<TVariable> var1, TrecPointer<TVariable> var2) override;

    /**
     * Method: TInterpretor::ProcessPrototypeOperation
     * Purpose: Enables operations dealing with prototypes
     * Parameters: const TString& className - the name of the type to apply the operation to
     *              const TString& attName - the name of the attribute on the type
     *              const TrecPointer<TVariable> value - value to assign
     *              JS_Prototype_Op operation - the operation to performed
     * Returns: ReportObject - the results of the operation
     */
    ReportObject ProcessPrototypeOperation(const TString& className, const TString& attName, const TrecPointer<TVariable> value, JS_Prototype_Op operation);
protected:
    UINT line;

    TDataArray<JavaScriptStatement> statements;

    bool hasOddMiltiLineStrMarkers(const TString& str);

    UINT InsertSemiColons();

    void ProcessParenthBlock(ReportObject& ro, const TString& currentCode, UINT line);


    ULONG64 GetBlockEnd();
    
    // Statement Process Methods
    void ProcessIf(TDataArray<JavaScriptStatement>& statements, UINT& cur, const JavaScriptStatement& statement, ReportObject& ro);
    void ProcessElse(TDataArray<JavaScriptStatement>& statements, UINT& cur, const JavaScriptStatement& statement, ReportObject& ro);
    void ProcessWhile(TDataArray<JavaScriptStatement>& statements, UINT cur, const JavaScriptStatement& statement, ReportObject& ro);
    void ProcessFor(TDataArray<JavaScriptStatement>& statements, UINT cur, const JavaScriptStatement& statement, ReportObject& ro);
    void ProcessVar(TDataArray<JavaScriptStatement>& statements, UINT cur, const JavaScriptStatement& statement, ReportObject& ro);
    void ProcessLet(TDataArray<JavaScriptStatement>& statements, UINT cur, const JavaScriptStatement& statement, ReportObject& ro);
    void ProcessConst(TDataArray<JavaScriptStatement>& statements, UINT cur, const JavaScriptStatement& statement, ReportObject& ro);
    void ProcessFunction(TDataArray<JavaScriptStatement>& statements, UINT cur, const JavaScriptStatement& statement, ReportObject& ro, bool addToVars = true);
    void ProcessClass(TDataArray<JavaScriptStatement>& statements, UINT cur, const JavaScriptStatement& statement, ReportObject& ro);
    void ProcessReg(TDataArray<JavaScriptStatement>& statements, UINT cur, const JavaScriptStatement& statement, ReportObject& ro);
    void ProcessReturn(TDataArray<JavaScriptStatement>& statements, UINT cur, const JavaScriptStatement& statement, ReportObject& ro);

    void ProcessTry(TDataArray<JavaScriptStatement>& statements, UINT& cur, const JavaScriptStatement& statement, ReportObject& ro);
    void ProcessCatch(TDataArray<JavaScriptStatement>& statements, UINT& cur, const JavaScriptStatement& statement, ReportObject& ro);
    void ProcessFinally(TDataArray<JavaScriptStatement>& statements, UINT cur, const JavaScriptStatement& statement, ReportObject& ro);

    // Helper Process Methods
    void AssignmentStatement(TDataArray<JavaScriptStatement>& statements, UINT cur, const JavaScriptStatement& statement, ReportObject& ro, TDataMap<TVariableMarker>& variables);
    void ProcessExpression(TDataArray<JavaScriptStatement>& statements, UINT& cur, TString& exp, UINT line, ReportObject& ro);
    void ProcessArrayExpression(TDataArray<JavaScriptStatement>& statements, UINT cur, TString& exp, UINT line, ReportObject& ro);
    UINT ProcessFunctionDef(TDataArray<JavaScriptStatement>& statements, UINT cur, TString& exp, UINT line, ReportObject& ro);
    void InspectNumber(TString& exp, UINT line, ReportObject& ro);
    bool InspectVariable(TDataArray<JavaScriptStatement>& statements, UINT& cur, TString& exp, UINT line, ReportObject& ro);
    UINT ProcessProcedureCall(TDataArray<JavaScriptStatement>& statements, UINT cur, TString& exp, UINT line, ReportObject& ro, TrecPointer<TVariable> obj);
    void ProcessJsonExpression(TDataArray<JavaScriptStatement>& statements, UINT cur, TString& exp, UINT line, ReportObject& ro);

    bool InspectVariable(const TString& exp);

    // Operator Handler Methods
    void HandlePreExpr(TDataArray<JavaScriptStatement>& statements, UINT cur, TDataArray<JavaScriptExpression>& expresions, TDataArray<TString>& operators, ReportObject& ro);
    void HandleExponents(TDataArray<JavaScriptStatement>& statements, UINT cur, TDataArray<JavaScriptExpression>& expresions, TDataArray<TString>& operators, ReportObject& ro);
    void HandleMultDiv(TDataArray<JavaScriptStatement>& statements, UINT cur, TDataArray<JavaScriptExpression>& expresions, TDataArray<TString>& operators, ReportObject& ro);
    void HandleAddSub(TDataArray<JavaScriptStatement>& statements, UINT cur, TDataArray<JavaScriptExpression>& expresions, TDataArray<TString>& operators, ReportObject& ro);
    void HandleBitwiseShift(TDataArray<JavaScriptStatement>& statements, UINT cur, TDataArray<JavaScriptExpression>& expresions, TDataArray<TString>& operators, ReportObject& ro);
    void HandleLogicalComparison(TDataArray<JavaScriptStatement>& statements, UINT cur, TDataArray<JavaScriptExpression>& expresions, TDataArray<TString>& operators, ReportObject& ro);
    void HandleEquality(TDataArray<JavaScriptStatement>& statements, UINT cur, TDataArray<JavaScriptExpression>& expresions, TDataArray<TString>& operators, ReportObject& ro);
    void HandleBitwiseAnd(TDataArray<JavaScriptStatement>& statements, UINT cur, TDataArray<JavaScriptExpression>& expresions, TDataArray<TString>& operators, ReportObject& ro);
    void HandleBitwiseXor(TDataArray<JavaScriptStatement>& statements, UINT cur, TDataArray<JavaScriptExpression>& expresions, TDataArray<TString>& operators, ReportObject& ro);
    void HandleBitwiseOr(TDataArray<JavaScriptStatement>& statements, UINT cur, TDataArray<JavaScriptExpression>& expresions, TDataArray<TString>& operators, ReportObject& ro);
    void HandleLogicalAnd(TDataArray<JavaScriptStatement>& statements, UINT cur, TDataArray<JavaScriptExpression>& expresions, TDataArray<TString>& operators, ReportObject& ro);
    void HandleLogicalOr(TDataArray<JavaScriptStatement>& statements, UINT cur, TDataArray<JavaScriptExpression>& expresions, TDataArray<TString>& operators, ReportObject& ro);
    void HandleNullish(TDataArray<JavaScriptStatement>& statements, UINT cur, TDataArray<JavaScriptExpression>& expresions, TDataArray<TString>& operators, ReportObject& ro);
    void HandleConditional(TDataArray<JavaScriptStatement>& statements, UINT cur, TDataArray<JavaScriptExpression>& expresions, TDataArray<TString>& operators, ReportObject& ro);
    void HandleAssignment(TDataArray<JavaScriptStatement>& statements, UINT cur, TDataArray<JavaScriptExpression>& expresions, TDataArray<TString>& operators, ReportObject& ro);
    void HandleComma(TDataArray<JavaScriptStatement>& statements, UINT cur, TDataArray<JavaScriptExpression>& expresions, TDataArray<TString>& operators, ReportObject& ro);

    /**
     * Method: TJavaScriptInterpretor::IsTruthful
     * Purpose: Determines whether an expression is truthful or not
     * Parameters: TrecPointer<TVariable> var - the variable to analyze.
     * Returns: bool
     */
    bool IsTruthful(TrecPointer<TVariable> var);

    /**
     * Method: TJavaScriptInterpretor::GetValueFromPrimitive
     * Purpose: Helper method for interpreting arithmetic operations by extracting the value from the variable
     * Parameters: TrecPointer<TVariable> var - the variable believed holding the primitive value
     * Returns: DoubleLong - the value held by the variable
     * 
     * Note: Added to ensure that if a boolean needs to be converted to a Number, it can happen
     */
    virtual DoubleLong GetValueFromPrimitive(TrecPointer<TVariable> var)override;

    bool IsEqual(TrecPointer<TVariable> var1, TrecPointer<TVariable> var2, bool isEqual, bool castType);
    
    // Sets up a console Object
    TrecPointer<TVariable> GetJsConsole();
};


// Functions to support the JavaScript Console



