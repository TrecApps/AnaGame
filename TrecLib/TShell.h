#pragma once
#include "TObject.h"
#include "TString.h"
#include "TFile.h"

/*
 * class TShell
 * Purpose: provides support for command-line functionality in Anagame
 */
class _TREC_LIB_DLL TShell :
	public TObject
{
public:

	/**
	 * Method: TShell::TShell
	 * Purpose: Default Constructor
	 * Parameters: void
	 * Returns: New TShell
	 */
	TShell();

	/**
	 * Method: TShell::~TShell
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	~TShell();


	/**
	 * Method: TShell::SubmitCommand
	 * Purpose: Submits a command for processing
	 * Parameters: TSTring& command - the command to process
	 * Returns: void
	 */
	void SubmitCommand(TString& command);

	/**
	 * Method: TShell::GetOutput
	 * Purpose: Returns the standard output captured by the shell
	 * Parameters: void
	 * Returns: TString - standard output
	 */
	TString GetOutput();

	/**
	 * Method: TShell::GetError
	 * Purpose: Returns the Standard Error Captured by the Shell
	 * Parameters: void
	 * Returns: TString - the standard error returned
	 */
	TString GetError();


	/**
	 * Method: TShell::TerminateProcess
	 * Purpose: Terminates the currently running Process, intended to be call if the user presses CTRL-C
	 * Parameters: void
	 * Returns: void
	 */
	void TerminateProcess();


	/**
	 * Method: TShell::CheckProcess
	 * Purpose: Reports whether or not the shel is monitoring an active process
	 * Parameters: void
	 * Returns: bool - true if the Shell is monitoring an active process, false otherwise
	 */
	bool CheckProcess();


	/**
	 * Method: TShell::GetWorkingDirectory
	 * Purpose: Reports the current working directory of the shell in TString form
	 * Parameters: void
	 * Returns: TString - the working directory of this shell
	 */
	TString GetWorkingDirectory();

private:

	/**
	 * Method: TShell::Process_pwd
	 * Purpose: Sets the working directory to be the output of the shell object
	 * Parameters: TString& command - the pwd command to process
	 * Returns: void
	 */
	void Process_pwd(TString& command);


	/**
	 * Method: TShell::Process_cd
	 * Purpose: Processes the "cd" command, since the working directory is maintained by this object, not by the underlying shell program
	 * Parameters: TString& command - the cd command to process
	 * Returns: void
	 */
	void Process_cd(TString& command);



	/**
	 * Method: TShell::ProcessBackgroundProcess
	 * Purpose: Causes the shell to launch a process in the background with the given command. Once launched,
	 *		TShell will assume no responsibility for this particular process
	 * Parameters: TString& command - the command to process
	 * Returns: void
	 */
	void ProcessBackgroundProcess(TString& command);

	/**
	 * Method: TShell::ProcessFrontCommand
	 * Purpose: Causes the Shell to launch a new process with the provided command. This will put the shell on hold
	 *		until the process is terminated
	 * Parameters: TString& command - the command to process
	 * Returns: void
	 */
	void ProcessFrontCommand(TString& command);

	/**
	 * The working Directory of the shell
	 */
	TString workingDirectory;

	/**
	 * output: the output captured by the shell
	 * standardError: the standard Error captured by the shell
	 */
	TString output, standardError;

	/**
	 * stdOutRd: handle to pipe used by the shell used for reading standard output
	 * stdInRd: handle to pipe to be passed to the process being created
	 * stdErrRd: handle to pipe used by the shell used for reading standard error
	 */
	HANDLE stdOutRd, stdInRd, stdErrRd;

	/**
	 * stdOutWt: handle to pipe passed to the process in order to write out that this object can catch
	 * stdInWt: handle to pipe used so that the shell object sand send input to the process
	 * stdErrWt: handle to pipe passed to the process in order to write error that this object can catch
	 */
	HANDLE stdOutWt, stdInWt, stdErrWt;

	/**
	 * Holds information about a process being run, used by the Create Process function Windows provides
	 */
	PROCESS_INFORMATION processInfo;

	/**
	 * location of the program shell Anagame relies upon
	 */
	TString programShell;

	/**
	 * 
	 */
	ULONGLONG outputLoc;
};

