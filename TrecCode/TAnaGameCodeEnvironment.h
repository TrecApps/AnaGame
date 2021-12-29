#pragma once
#include <TPage.h>
#include <TDirectory.h>
#include "TLanguage.h"
#include <EventHandler.h>

typedef enum class CompileErrorHandling
{
	ceh_stop,
	ceh_contain_to_block,
	ceh_document_errors
}CompileErrorHandling;

typedef enum class TargetAnagameMachine
{
	tam_object_register,
	tam_object_stack,
	tam_binary_register,
	tam_binary_stack
}TargetAnagameMachine;




class TAnaGameCodeEnvironment :
	public TPageEnvironment
{
public:
	TAnaGameCodeEnvironment(TrecPointer<TFileShell> shell);


	/**
	 * Method: TAnaGameCodeEnvironment::GetType
	 * Purpose: Returns a String Representation of the object type
	 * Parameters: void
	 * Returns: TString - representation of the object type
	 */
	virtual TString GetType()override;

	void PreProcessSingleFile(TrecPointer<TFile> file);

	virtual UINT RunTask(TString& task)override;
	virtual UINT SetUpEnv()override;

	virtual void Compile()override;
	virtual void Compile(TrecPointer<TFile> logFile)override;

	virtual void Log()override;

	virtual void Run()override;
	virtual void Run(TrecPointer<TFileShell> file)override;

	virtual TrecPointer<TObjectNode> GetBrowsingNode() override;

	virtual bool SupportsFileExt(const TString& ext) override;
	virtual void SupportsFileExt(TDataArray<TString>& ext) override;

	/**
	 * Method: TEnvironment::Print
	 * Purpose: Allows external code to manually add something to print out
	 * Parameters: TString& input - the command to enter
	 * Returns: bool - whether a prompt was set or not
	 */
	virtual bool Print(const TString& input)override;

	/**
	 * Method: TEnvironment::PrintLine
	 * Purpose: Allows external code to manually add something to print out, adding an extra new line at the end
	 * Parameters: TString& input - the command to enter
	 * Returns: void
	 */
	virtual bool PrintLine(const TString& input)override;

	/**
	 * Method: TEnvironment::SaveEnv
	 * Purpose: Saves the Current Status of the Environment itself
	 * Parameters: void
	 * Returns: UINT - error code (0 for no error)
	 */
	virtual UINT SaveEnv() override;


	/**
	 * Method: TEnvironment::GetProjectLayout
	 * Purpose: Reports the Layout of the Project
	 * Parameters: void
	 * Returns: TrecPointer<TObjectNode> nodes - the nodes that represent the layout of the Project
	 *
	 * Attributes: abstract
	 */
	virtual TrecPointer<TObjectNode> GetProjectLyout() override;


	/**
	 * Method: TEnvironment::AddResource
	 * Purpose: Adds a file Resource to the Environment
	 * Parameters: TrecPointer<TFileShell> fileResorce - the file to add
	 * Returns: void
	 *
	 * Attributes: abstract
	 */
	virtual void AddResource(TrecPointer<TFileShell> fileResource) override;


	/**
	 * Method: TEnvironment::SetLoadFile
	 * Purpose: Allows Environment to load itself
	 * Parameters: TrecPointer<TFileShell> file - the file to load from
	 * Returns: TString - error information (blank means success)
	 *
	 * Attributes: abstract
	 */
	virtual TString SetLoadFile(TrecPointer<TFileShell> file) override;

protected:


	/**
	 * Method: TPageEnvironment::GetPageAndHandler
	 * Purpose: Retrieves the Pages by the Specified name
	 * Parameters: const TString& name, TrecPointer<TPage> page, TrecPointer<TPage::EventHandler> handler
	 * Returns: void
	 *
	 * Note: If successful, the page and handler params should be assigned once the method returns
	 */
	virtual void GetPageAndHandler_(const TString& name, TrecPointer<TPage>& page, TrecPointer<TPage::EventHandler>& handler, TrecPointer<DrawingBoard> board, TrecPointer<TProcess> proc) override;

	/**
	 * Method: TPageEnvironment::GetPageList
	 * Purpose: Retrieves the available Page Names by extension
	 * Parameters: const TString& ext, TDataArray<TString>& extensions
	 * Returns: void
	 *
	 * Note: if ext is an empty String, then return all that the Environment has to offer
	 */
	virtual void GetPageList_(const TString& ext, TDataArray<TString>& extensions) override;

	TString targetExtensions;	// Extensions to target for compilation

	CompileErrorHandling compileErrorHandling;
	TargetAnagameMachine targetMachine;
	TString endEnvironment;

	TMap<TLanguage> languages;

	TDataArray<TrecPointer<TFileShell>> files;
	TrecPointer<TFileShell> mainFile;

	TrecPointer<TVariable> currentRunner;

};

