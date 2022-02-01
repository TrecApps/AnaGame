#pragma once
#include "DocumentHandler.h"
#include <TTextInput.h>

/**
 * Class: TCodeHandler
 * Purpose: Extends the Handler to hold a specific code file
 * 
 * SuperClass: DocumentHandler
 */
class _TAP_DLL TCodeHandler : public DocumentHandler
{
public:

	/**
	 * Method: TCodeHandler::TCodeHandler
	 * Purpose: Constructor
	 * Parameters: TrecPointer<TInstance> instance - instance associated with this handler
	 * Returns: New EventHandler Object
	 */
	TCodeHandler(TrecPointer<TProcess> in);

	/**
	 * Method: TCodeHandler::TCodeHandler
	 * Purpose: Constructor
	 * Parameters: TrecPointer<TInstance> instance - instance associated with this handler
	 *				const TString& name - the Name to give this handler
	 * Returns: New EventHandler Object
	 */
	TCodeHandler(TrecPointer<TProcess> in, const TString& name);


	/**
	 * Method: TCodeHandler::GetType
	 * Purpose: Returns a String Representation of the object type
	 * Parameters: void
	 * Returns: TString - representation of the object type
	 */
	virtual TString GetType()override;


	/**
	 * Method: TCodeHandler::Initialize
	 * Purpose: Initializes the Handler so that it has direct Access to certain Controls held by the page
	 * Parameters: TrecPointer<Page> page - page that holds the Controls to latch on to
	 * Returns: void
	 * 
	 * Attributes: override
	 */
	virtual void Initialize(TrecPointer<TPage> page) override;

	/**
	 * Method: TCodeHandler::HandleEvents
	 * Purpose: Handles Events produced from the set of TControls
	 * Parameters: TDataArray<EventID_Cred>& eventAr - list of events to process
	 * Returns: void
	 * 
	 * Attributes: override
	 */
	virtual void HandleEvents(TDataArray<TPage::EventID_Cred>& eventAr) override;


	/**
	 * Method: TCodeHandler::ProcessMessage
	 * Purpose: Processes the message sent to the handler
	 * Parameters: TrecPointer<HandlerMessage> message - the message to recieve and Process
	 * Returns: void
	 * 
	 * Attributes: override
	 */
	virtual void ProcessMessage(TrecPointer<HandlerMessage> message) override;


	/**
	 * Method: TCodeHandler::OnSave
	 * Purpose: Provides a Save method in case there is a way to save
	 * Parameters: void
	 * Returns: void
	 * 
	 * Attributes: override
	 */
	virtual void OnSave()override;

	/**
	 * Method: EventHandler::SetSaveFile
	 * Purpose: Sets up the file to save if OnSave is called
	 * Parameters: TrecPointer<TFileShell> file - the file to focus on
	 * Returns: void
	 */
	virtual void SetSaveFile(TrecPointer<TFileShell> file) override;


	/**
	 * Method: TapEventHandler::ReportHelperPages
	 * Purpose: Reports the Helper Page types that this Handler will use to hep supplement control
	 * Parameters: TDataArray<TString>& pages - the pages this handler will use
	 * Returns: void
	 *
	 * Attributes: virtual
	 *
	 * Notes: Each Entry should be in the form of '[type]:[name]' where 'type' is eiher 'ribbon', 'singular', or 'nultiple', Anagame will then check the attached Environments
	 *		and provide codes for these pages if available in the 'SetSupPageCodes' method
	 */
	virtual void ReportHelperPages(TDataArray<TString>& pages);

protected:

	void PrepCodeText();


	/**
	 * Method: TCodeHandler::ShouldProcessMessageByType
	 * Purpose: Reports whether this Object is of the correct type to recieve the message
	 * Parameters: TrecPointer<HandlerMessage> message - the message to scan
	 * Returns: bool - true if the type matches, false oherwise
	 * 
	 * Attributes: override
	 */
	virtual bool ShouldProcessMessageByType(TrecPointer<HandlerMessage> message) override;

	TrecSubPointer<TPage, TTextInput> code, lines;


	/**
	 * Method: TCodeHandler::OnSave
	 * Purpose: Writes the contents of the code file to the file
	 * Parameters: TFile& file - the file to write to
	 * Returns: void
	 * 
	 * Attributes: override
	 */
	void OnSave(TFile& file)override;

	/**
	 * Method: TCodeHandler::OnLoad
	 * Purpose: Intended to read from a provided file
	 * Parameters: TFile& the file to read from
	 * Returns: void
	 * 
	 * Attributes: override
	 */
	virtual void OnLoad(TFile&)override;
};

