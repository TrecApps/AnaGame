#pragma once
#include <TcRunner.h>
#include <TFileShell.h>

typedef enum class runner_op_code
{
    // Basic Runner tweaks
    reserved,       // Blank Value to hold (does nothing)
    line,           // Tells the Runner which code line the next set of operands was on
    statement,      // Tells the Runner which Statement the next set of operands was on

    // Basic Arithmetic
    add_ff,         // Adds 2 floating point numbers and places the result on the stack
    add_dd,         // Adds 2 double numbers and places the result onto the stask
    add_ii,         // Adds 2 integers and places the result onto the stack
    add_uu,         // Adds 2 unsigned integers and places the result onto the stack
    add_ll,         // Adds 2 longs and places the result onto the stack
    add_ulul,       // Adds 2 unsligned longs and places the result onto the stack
    add_oo,         // Adds 2 Objects together (looks to the Object Stack)
    sub_ff,         // Subtracts 2 floating point numbers and places the result on the stack
    sub_dd,         // Subtracts 2 double numbers and places the result onto the stask
    sub_ii,         // Subtracts 2 integers and places the result onto the stack
    sub_uu,         // Subtracts 2 unsigned integers and places the result onto the stack
    sub_ll,         // Subtracts 2 longs and places the result onto the stack
    sub_ulul,       // Subtracts 2 unsligned longs and places the result onto the stack
    sub_oo,         // Subtracts 2 Objects together (looks to the Object Stack)
    mul_ff,         // Multiplies 2 floating point numbers and places the result on the stack
    mul_dd,         // Multiplies 2 double numbers and places the result onto the stask
    mul_ii,         // Multiplies 2 integers and places the result onto the stack
    mul_uu,         // Multiplies 2 unsigned integers and places the result onto the stack
    mul_ll,         // Multiplies 2 longs and places the result onto the stack
    mul_ulul,       // Multiplies 2 unsligned longs and places the result onto the stack
    mul_oo,         // Multiplies 2 Objects together (looks to the Object Stack)
    div_ff,         // Divides 2 floating point numbers and places the result on the stack
    div_dd,         // Divides 2 double numbers and places the result onto the stask
    div_ii,         // Divides 2 integers and places the result onto the stack
    div_uu,         // Divides 2 unsigned integers and places the result onto the stack
    div_ll,         // Divides 2 longs and places the result onto the stack
    div_ulul,       // Divides 2 unsligned longs and places the result onto the stack
    div_oo,         // Divides 2 Objects together (looks to the Object Stack)
    mod_ii,         // Mod Divides 2 integers and places the result onto the stack
    mod_uu,         // Mod Divides 2 unsigned integers and places the result onto the stack
    mod_ll,         // Mod Divides 2 longs and places the result onto the stack
    mod_ulul,       // Mod Divides 2 unsligned longs and places the result onto the stack
    mod_oo,         // Mod Divides 2 Objects together (looks to the Object Stack)

    // Number casting
    cast_if,        // Converts an int to a float
    cast_fi,        // Converts a float to an int
    cast_fd,        // Converts a float to a double
    cast_df,        // Converts a double to a float 
    cast_iu,        // Converts an int to a uint
    cast_ui,        // Converts a uint to an int
    cast_uul,       // Doubles a uint
    cast_ulu,       // Halves a 64-bit uint
    cast_il,        // Doubles an integer
    cast_li,        // Halves a 64-bit int

    // Basic Stack 
    push_1,         // Pushes a 32-bit value onto the stack
    push_2,         // Pushes a 64-bit value onto the stack
    push_b,         // Retrieves a value from within the binary stack and places it on top
    pop_n,          // Pops the specified number of 4-byte values off of the binary stack
    push_o,         // Retrieves a value from within the object stack and places it on top
    pop_o,          // Pops the specified number of objects off of the Object stack
}runner_op_code;



/**
 * Class: AnagameRunner
 * Purpose: Represents Anagame's Virtual Machine, at the Procedure Level
 */
class AnagameRunner : TcRunner
{
public:
    class RunnerCode {
    public:
        BYTE op_code;
        UINT operand1, operand2;
        RunnerCode();
        RunnerCode(const RunnerCode& copy);
        RunnerCode(runner_op_code code, UINT op1 = 0, UINT op2 = 0);
    };

    static ReturnObject GenerateRunners(TDataMap<TrecSubPointer<TVariable, AnagameRunner>>& runners, TrecPointer<TFileShell> file);

    /**
     * Method: AnagameRunner::AnagameRunner
     * Purpose: Constructor
     * Parameters: void
     * Returns: new Runner
     */
    AnagameRunner();


    /**
     * Mehtod: AnagameRunner::SetCurrentStack
     * Purpose: Sets the binary Stack for the runner (meant for Anagame Binary and Hybrid Runners)
     * Parameters: TrecPointer<BinaryStack> stack - binary Stack
     * Returns: void
     */
    virtual void SetCurrentStack(TrecPointer<BinaryStack> stack);


    /**
     * Mehtod: AnagameRunner::SetCurrentStack
     * Purpose: Sets the binary Stack for the runner (meant for Anagame Binary and Hybrid Runners)
     * Parameters: TrecPointer<BinaryStack> stack - binary Stack
     * Returns: void
     */
    virtual void SetCurrentStack(TrecPointer<ObjectStack> stack);

    /**
     * Method: TcRunner::Run
     * Purpose: Allows the Interpretor to run the Code As Is
     * Parameters: void
     * Returns: ReturnObject - information about the Run
     */
    virtual ReturnObject Run() ;

    /**
     * Method: TcRunner::SetInitialVariables
     * Purpose: Allows Callers to Set Intial Variables, i.e. parameters in a function/method call
     * Parameters: TDataArray<TrecPointer<TVariable>>& params - the parameters to set when calling the function
     * Returns: void
     */
    virtual void SetIntialVariables(TDataArray<TrecPointer<TVariable>>& params);

    /**
     * Method: AnagameRunner::SetCurrentLine
     * Purpose: Sets the current line of the code being processed
     * Parameters: UINT line - th eline to set
     * Returns: void
     */
    void SetCurrentLine(UINT line);

    /**
     * Method: AnagameRunner::SetCurrentCodeLine
     * Purpose: Sets the current line of the code being processed
     * Parameters: UINT line - th eline to set
     * Returns: void
     */
    void SetCurrentCodeLine(UINT line);
    

protected:
    bool isBinary, isObject;
    TString file, name;
    TDataArray<TString> variableCites;
    TDataArray<TString> codeLines;      // Easier to Highlight the lines
    

    UINT currentLine, currentCodeLines;

    TrecPointer<BinaryStack> binaryStack;
    TrecPointer<ObjectStack> objectStack;

    TDataArray<RunnerCode> code;
};

