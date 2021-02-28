#pragma once
#include "TObject.h"
#include "TString.h"
#include "TFileShell.h"

/**
 * Method: StatementCollector::
 * Purpose:
 * Parameters:
 * Returns:
 */

/**
 * Enum class: code_statement_type
 * Purpose: Allows Interpretors to esily id statements by type
 */
typedef enum class code_statement_type
{
    cst_not_set,        // The type of statement has yet to be determined (All statements should start out with this type)
                        //  Interpretors should go through statements and determine the appropriate type

    cst_preprocess,     // Statement is a preprocesser

    // Declarations

    cst_var,            // Statement is a 'var' statement
                        // Note: for languages whose 'var' statement behaves like a JS 'let' statement, it is recommended to use 'let'
                        //      instead of 'var' (though interpretors are free to ignor this recommendation
    cst_let,            // Statement is a 'let' statement (or a language equivelent)
    cst_const,          // Statement is a 'const' statement, or an equivelent such as 'final'
    cst_type,           // Statement is a declaration that specified the type upfront
    cst_declare,        // Statement where previous types might not necessarily apply

    // flow blocks

    cst_if,             // If statement
    cst_else,           // Else Statement
    cst_while,          // While loop
    cst_for,            // For loop
    cst_do,             // do while
    cst_until,          // Until block (the opposite of a while loop, found in perl)
    cst_try,            // Try block (code running in here might encounter an error)
    cst_catch,          // Catch block
    cst_finally,        // finally block
    cst_throw,          // manually initialte an exception
    cst_switch,         // Switch statement
    cst_match,          // Similar to a switch statement, used in Rust

    // flow controls

    cst_goto,           // goto statement
    cst_goto_target,    // target of a goto statement
    cst_return,         // Return statement
    cst_break,          // Break statement
    cst_continue,       // Continue statement

    // Class/Function blocks

    cst_function,       // Statement declares a function (or a method)
    cst_method,         // Statement is a method declaration
    cst_constructor,    // Special method type
    cst_destructor,     // Special method type (used in C++)
    cst_class,          // Class declaraion
    cst_union,          // Union declaration
    cst_enum,           // enum declaration

    // Other statement types

    cst_module,         // Signifies some module of code
    cst_include,        // Points to code outside the file that shoud be relevent to code in this file
    
    cst_regular         // Statement that does not fall under other statement types
} code_statement_type;

/**
 * Class: CodeStatement
 * Purpose: Holds a basic statement, containing the statement, line numbers
 */
class CodeStatement :
    public TObject
{
public:
    /**
     * Method: CodeStatement::CodeStatement
     * Purpose: Default Constructor
     * Parameters: void
     * Returns: new Code statement object
     */
    CodeStatement();

    /**
     * Method: CodeStatement::CodeStatement
     * Purpose: Copy Constructor
     * Parameters: const CodeStatement& copy - the existing statement to copy from
     * Returns: new copy of Code Statement
     */
    CodeStatement(const CodeStatement& copy);

    UINT lineStart;     // the line in the file the statement begins
    UINT lineEnd;       // The line where the statement ends
    TString statement;  // The Statement
    TDataArray<TrecPointer<CodeStatement>> block;
    TrecPointer<CodeStatement> next;
    TrecPointerSoft<CodeStatement> parent;
    code_statement_type statementType;
};

/**
 * Class: StatementCollector
 * Purpose: Allows Interpretors to Easily read a source file and parse statements, taking into account Strings, parsing out blocks,
 *  and filtering out comments
 */
class StatementCollector
{
public:
    /**
     * Method: StatementCollector::StatementCollector
     * Purpose: Default Constructor
     * Parameters: void
     * Returns: New Statement Collector
     * 
     * Note: By Default, the Collector will configure itself for JavaScript. To set it for other languages
     *  Use the various set methods before calling 'ProcessFile' method
     */

    StatementCollector();
    /**
     * Method: StatementCollector::StatementCollector
     * Purpose: Copy Constructor
     * Parameters: const StatementCollector& copy - the collector to copy from
     * Returns: New Copy of existing Collector
     */
    StatementCollector(const StatementCollector& copy);

    /**
     * Method: StatementCollector::SetStatementSeperator
     * Purpose: Sets the tokens used for seperating statements
     * Parameters:  TDataArray<TString>& data - the list of Statement Seperators
     * Returns: void
     * 
     * Note: If the Provided list is empty, then the end of the Line will be used as the statement seperator.
     *      Otherwise, the Collector will not use the newline as a statement terminator unless the statement starts
     *      with a token specified by the One-Line Array (used with the C-Prepeorcessor in mind)
     */
    void SetStatementSeperator(TDataArray<TString>& data);


    /**
     * Method: StatementCollector::SetSingleLine
     * Purpose: Sets the tokens used to signify the start of a single line method
     * Parameters: TDataArray<TString>& data - list of single-line tokens
     * Returns: void
     */
    void SetSingleLine(TDataArray<TString>& data);

    /**
     * Method: StatementCollector::SetMultiLineStart
     * Purpose: Sets the tokens that signify the start of a multi-line comment
     * Parameters: TDataArray<TString>& data - list of multi-line comment starters
     * Returns: void
     */
    void SetMultiLineStart(TDataArray<TString>& data);

    /**
     * Method: StatementCollector::SetMultiLineEnd
     * Purpose: Sets the tokens that signify the end of a multi-line comment
     * Parameters: TDataArray<TString>& data - list of multi-line comment enders
     * Returns: void
     */
    void SetMultiLineEnd(TDataArray<TString>& data);


    /**
     * Method: StatementCollector::SetSingleString
     * Purpose: Sets tokens for the start of single line comments
     * Parameters: TDataArray<TString>& data - tokens that signify single quotes
     * Returns: void
     * 
     * Note: Even languages like c/C++, which use single quotes for single characters instead of strings, should
     *  specify the single quote here as the Statement Collector doesn't care if the 'string' is single char or multi char.
     *  If that character is ';', Then the coder didn't intend for the statement to end ther, but the collector would assume
     *  it does if the single quote is removed here
     */
    void SetSingleString(TDataArray<TString>& data);


    /**
     * Method: StatementCollector::SetMultiString
     * Purpose: Sets the tokens used for recognizing a multi-line String 
     * Parameters: TDataArray<TString>& data - list of tokens used for recognizing a multi-line string
     * Returns: void
     */
    void SetMultiString(TDataArray<TString>& data);


    /**
     * Method: StatementCollector::SetBlockStart
     * Purpose: Sets the tokens used for recognizing the start of a block
     * Parameters: TDataArray<TString>& data - Tokens used for the start of a block
     * Returns: void
     * 
     * Note: If this array is rendered emptied, then the collector will ignore the block end array and use
     *  indentation for determining blocks
     */
    void SetBlockStart(TDataArray<TString>& data);


    /**
     * Method: StatementCollector::SetBlockEnd
     * Purpose: Sets the tokens used for recognizing the end of a block
     * Parameters: TDataArray<TString>& data -  tokens used for finding the end of a block
     * Returns: void
     */
    void SetBlockEnd(TDataArray<TString>& data);


    /**
     * Method: StatementCollector::SetOneLine
     * Purpose: Sets the tokens used for identifying statements that should be terminated at the end of the line
     * Parameters: TDataArray<TString>& data - tokens that, if found at the start of the statement indicate that the statement should end at the line
     * Returns: void
     * 
     * Note: Multi-Lines
     */
    void SetOneLine(TDataArray<TString>& data);

    /**
     * Method: StatementCollector::TurnOffCommentEscape
     * Purpose: Turns off the use of the '\' character on single line comments
     * Parameters: void
     * Returns: void
     * 
     * Note: By default the escape functionality on single comments is on. This method will turn it off, but it cannot be turned back on.
     *      This has no effect on multi-line comments
     */
    void TurnOffCommentEscape();

    /**
     * Method: StatementCollector::TurnOffStringEscape
     * Purpose: Turns off the use of the '\' character on single line strings
     * Parameters: void
     * Returns: void
     *
     * Note: By default the escape functionality on single line strings is on. This method will turn it off, but it cannot be turned back on
     *      This has no effect on multi-line strings
     */
    void TurnOffStringEscape();

    /**
     * Method: StatementCollector::TurnOffStatementEscape
     * Purpose: Turns off the use of the '\' character on single line statements
     * Parameters: void
     * Returns: void
     *
     * Note: By default the escape functionality on single-line statement is on. This method will turn it off, but it cannot be turned back on
     *      For 
     */
    void TurnOffStatementEscape();

    /**
     * Method: StatementCollector::RunCollector
     * Purpose: Generates statements from the file, based on the configuration
     * Parameters: TrecPointer<TFileShell> file - the file to read
     *              TString& errorMessage - String to hold error information
     *              UINT& line - if an error is detected in statement processing, holds the line on which this was detected
     * Returns: USHORT - error message
     * 
     * Note: 0 stands for no error, 1 stands for error reading the file, 2 stands for error parsing statements
     */
    USHORT RunCollector(TrecPointer<TFileShell> file, TString& errorMessage, UINT& line);

    /**
     * Method: StatementCollector::CollectStatements
     * Purpose: Allows Interpretors to retrieve the statements generated
     * Parameters: TDataArray<CodeStatement>& statements - the statements held by the interpretor
     * Returns: void
     */
    void CollectStatement(TDataArray<CodeStatement>& statements);

private:
    TDataArray<CodeStatement> statements;

    bool nextLineComment,       // Use '\' to allow single line comments to go to the next line
        nextLineString,         // Use '\' to allow single line strings to go to the next line
        nextLineStatement;      // Use '\' to allow single line sttements to go to the next line


    TDataArray<TString> statementseperator, // How to recognize when a Statement ends
        singleLineComment,                  // Token for recognizing the start of a single line comment
        multiLineCommentStart,              // Tokens for recognizing start of  multiline comment
        multiLineCommentEnd,                // Tokens for recognizing End of a multiline comment
        singleString,                       // Tokens for recognizing a single line String
        multiString,                        // Tokens for recognizing a multiline String
        blockStart,                         // Tokens for recognizing the start of a block (if this is blank, indentation is assumed)
        blockEnd,                           // Tokens for recognizing the end of a block
        oneLineStatement;                   // Strings that, if they start the statement, signal that the statement should be one line
};
