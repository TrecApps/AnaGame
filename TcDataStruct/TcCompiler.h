#pragma once
#include <TString.h>
#include <TVariable.h>
#include <TFileShell.h>
///
///
/// File: TcCompiler.h
/// Purpose: Provides the Infrastructure for Anagame to Support Compilation from Multiple different languages
///		(including custom languages) into Anagame Byte Code. There will be three major Byte-Code levels:
///		1. Pure-Object-Oriented Code: every variable is wrapped in a TVariable Object, even primitive ones
///		2. Hybrid-Code: Objects and Containers are wrapped in TVariable Objects, but raw Primitives are represented in byte-arrays
///		3. Raw-Byte-Code: Objects, containers, as well as primitives are managed via raw bytes. 
/// 
/// The Anagame Compilation Process will consist of three stages:
///		1. Pre-Statement Generation: Lists of Statements (some of which might have a container of additional statements (think blocks)). 
///			These Pre-Statements contain information on where in the file it can be found, but not what type of statement it is
///		2. Base-Statement Generation: Lists of Statements broken down into components, with information on what type of statement it is and Expression breaks
///		3. Anagame Byte-Code Generation: Convert the Base-Statements into byte-code that is comparable to Java Byte code or Assembly
/// 

typedef enum class tc_statement_type
{
    _unset,         // Used for Default Constructors

    _var,        // Statement is a 'var' statement
                    // Note: for languages whose 'var' statement behaves like a JS 'let' statement, it is recommended to use 'let'
                    //      instead of 'var' (though interpretors are free to ignor this recommendation
    _let,            // Statement is a 'let' statement (or a language equivelent)
    _const,          // Statement is a 'const' statement, or an equivelent such as 'final'
    _type,           // Statement is a declaration that specified the type upfront
    _declare,        // Statement where previous types might not necessarily apply

    // flow blocks
    _if,             // If statement
    _else,           // Else Statement
    _else_if,        // Else if Statement
    _while,          // While loop
    _for,            // For loop (basic or 1-statement)
    _for_3,          // Three statement For-loop
    _do,             // do while
    _until,          // Until block (the opposite of a while loop, found in perl)
    _try_,            // Try block (code running in here might encounter an error)
    _catch,          // Catch block
    _finally_,        // finally block
    _throw,          // manually initialte an exception
    _switch,         // Switch statement
    _case,           // Case
    _default,        // Default
    _match,          // Similar to a switch statement, used in Rust

                    // flow controls

    _goto,           // goto statement
    _goto_target,    // target of a goto statement
    _return,         // Return statement
    _break,          // Break statement
    _continue,       // Continue statement
    _yield,          // Yield Statement
    _end,            // Statements that signify the end of a block

                    // Class/Function blocks

    _function,       // Statement declares a function (or a method)
    _function_gen,   // Generates an iterator that holds a function
    _method,         // Statement is a method declaration
    _constructor,    // Special method type
    _destructor,     // Special method type (used in C++)
    _class,          // Class declaraion
    _union,          // Union declaration
    _enum,           // enum declaration
    _delete,         // Delete statements

                    // Other statement types

    _module,         // Signifies some module of code
    _include,        // Points to code outside the file that shoud be relevent to code in this file

    _virtual_assign, // Used by JS interpretor to run assignment

    _regular         // Statement that does not fall under other statement types
}tc_statement_type;

class CompileMessage
{
public:
    TString message;
    UINT line;
    TrecPointer<TFileShell> file;
    bool isError;
    CompileMessage();
    CompileMessage(const CompileMessage& message);
};

class TcPreStatement
{
private:
	TString statementText;
	UINT startLine;
	TDataArray<TcPreStatement> subStatements;

public:
	TcPreStatement();
	TcPreStatement(const TcPreStatement& statement);

	void SetStatementText(const TString& text);
	TString GetStatementText()const;

	void SetStartLine(UINT start);
	UINT GetStartLine()const;
	UINT GetEndLine()const;

	UINT GetStatementCount()const;
	bool GetStatementAt(UINT index, TcPreStatement& statement) const;

	void AddStatement(const TcPreStatement& statement);

    static void GenerateStatements(TDataArray<TcPreStatement>& statements,TrecPointer<TVariable> languageAtts, TrecPointer<TFileShell> codeFile, TDataArray<CompileMessage>& messages);
};

class TcVariableContainer
{
public:
    typedef enum class var_source
    {
        global,
        parameter,
        scoped,

    }var_source;
    bool isObject;

    TString name;

};

class TcProcessor : public TVariable
{
public:
    class AgByteCode
    {
    public:
        UCHAR opCode;
        USHORT op1, op2;
        AgByteCode();
        AgByteCode(const AgByteCode& code);
    };

protected:
    TDataArray<AgByteCode> byteCodes;

public:
    TcProcessor();
    TcProcessor(const TcProcessor& proc);
    virtual ~TcProcessor();

    void AddByteCode(const AgByteCode& ref);

    // Methods from TVariable
    virtual var_type GetVarType() override;
    virtual TString GetString() override;
    virtual UINT Get4Value() override;
    virtual ULONG64 Get8Value() override;
    virtual UINT GetSize() override;
    virtual UINT GetVType() override;
};

class TcBaseStatement
{
private:
	TString statementText;
	UINT startLine;
	TDataArray<TrecPointer<TcBaseStatement>> subStatements;
    tc_statement_type type;

public:
    TcBaseStatement();
    TcBaseStatement(tc_statement_type type, const TcPreStatement& statement);
    TcBaseStatement(const TcBaseStatement& statement);

    UINT GetStartLine();
    UINT GetEndLine();

    static bool ConvertToBase(TDataArray<TcPreStatement>& preStatements, TDataArray<TcBaseStatement>& baseStatements, TrecPointer<TVariable> lang, TDataArray<CompileMessage>& messages);

    static bool ConvertBaseToByteCode(TDataArray<TcBaseStatement>& baseStatements, TrecPointer<TVariable> lang, TDataArray<CompileMessage>& messages, TcProcessor& processor);
};