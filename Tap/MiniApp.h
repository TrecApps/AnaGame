#pragma once
#include <TObject.h>
#include "HandlerMessage.h"
#include "TIdeWindow.h"

/**
 * Class: MiniApp
 * Purpose: Holds a collection of Pages needed to support a "miniture" application within the confines of an IDE Window
 *
 * Note: Abstract to allow a wide variety of Apps to be supported
 */
class _TAP_DLL MiniApp : public TObject
{
public:

	/**
	 * Method: MiniApp::MiniApp
	 * Purpose: Constructor
	 * Parameters: TrecSubPointer<TWindow, TIdeWindow> win - the Window Managing this MiniApp
	 * Returns: New MiniApp Object
	 */
	MiniApp(TrecSubPointer<TWindow, TIdeWindow> win);

	/**
	 * Method: MiniApp::DispatchAnagameMessage
	 * Purpose: Enables Handlers to dispatch a Message through the App instead of the TInstance
	 * Parameters: TrecPointer<HandlerMessage> message -  the message to send
	 * Returns: void
	 */
	virtual void DispatchAnagameMessage(TrecPointer<HandlerMessage> message) = 0;

	/**
	 * Method: MiniApp::Initialize
	 * Purpose: Allows the MiniApp to decide which handlers and Pages it needs to complete itself
	 * Parameters: void
	 * Returns: UINT - error code, 0 for success
	 */
	virtual UINT Initialize() = 0;

	/**
	 * Method: MiniApp::ShouldDestroy
	 * Purpose: Reports whether the MiniApp is prepared to be destroyed
	 * Parameters: void
	 * Returns: bool - whether the MiniApp can be destroyed, by default whather the Main Page is present or not
	 */
	virtual bool ShouldDestroy();

	/**
	 * Method: MiniApp::SetSelf
	 * Purpose: Allows TrecPointerKey to set the "self" of this object
	 * Parameters: TrecPointer<MiniApp> s -  the TrecPointer refering to itself
	 * Returns: void
	 */
	void SetSelf(TrecPointer<MiniApp> s);

	/**
	 * Method: MiniApp::OnSave
	 * Purpose: Allows MiniApp to "Save" work done with it
	 * Parameters: void
	 * Returns: void
	 */
	virtual void OnSave() = 0;

protected:
	/**
	 * the Window holding this MniApp
	 */
	TrecSubPointer<TWindow, TIdeWindow> win;

	/**
	 * the Main page and therefore reson for the MiniApp object to be created
	 */
	TrecSubPointer<Page, IDEPage> mainPage;

	/**
	 * The TInstance underwhich this miniApp is created
	 */
	TrecPointerSoft<TInstance> instance;
	
	/**
	 * The Self reference that allows the MiniApp to provide references to itself
	 */
	TrecPointerSoft<MiniApp> self;
};

