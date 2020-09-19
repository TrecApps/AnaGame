#pragma once
#include "EventHandler.h"
class _TAP_DLL DocumentHandler :
	public EventHandler
{
public:
	DocumentHandler(TrecPointer<TInstance> in);
	DocumentHandler(TrecPointer<TInstance> in, const TString& name);


	/**
	 * Method: DocumentHandler::GetType
	 * Purpose: Returns a String Representation of the object type
	 * Parameters: void
	 * Returns: TString - representation of the object type
	 */
	virtual TString GetType()override;

	virtual void OnSave();
	virtual void OnLoad();

	virtual void OnShow();
	virtual void onHide();
	TString GetFilePath();
	TrecPointer<TFileShell> GetFile();

protected:

	virtual void OnSave(TFile&) = 0;
	virtual void OnLoad(TFile&) = 0;

	TrecPointer<TFileShell> file;
};

