#pragma once
#include "Tap_dll.h"
#include "HandlerMessage.h"
#include <TPage.h>
#include "TWindow.h"
#include <TFileShell.h>
#include "MiniApp.h"

/**
 * Class: EventHandler
 * Purpose: Base Class for Handling Events generated by a TControl
 * 
 * SuperClass: TObject
 */
class _TAP_DLL TapEventHandler: public TPage::EventHandler
{
	friend class TInstance;
public:

	/**
	 * Method: EventHandler::EventHandler
	 * Purpose: Constructor
	 * Parameters: TrecPointer<TProcess> instance - instance associated with this handler
	 * Returns: New EventHandler Object
	 */
    TapEventHandler(TrecPointer<TProcess> instance);
	/**
	 * Method: EventHandler::EventHandler
	 * Purpose: Constructor
	 * Parameters: TrecPointer<TProcess> instance - instance associated with this handler
	 *				const TString& name - the Name to give this handler
	 * Returns: New EventHandler Object
	 */
	TapEventHandler(TrecPointer<TProcess> instance, const TString& name);

	/**
	 * Method: TapEventHandler::SetWindow
	 * Purpose: Sets the Window of this Handler
	 * Parameters: TrecPointer<TWindow> window - the window to set
	 * Returns: void
	 */
	virtual void SetWindow(TrecPointer<TWindow> window);


	/**
	 * Method: EventHandler::~EventHandler
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
    virtual ~TapEventHandler();

	/**
	 * Method: EventHandler::GetType
	 * Purpose: Returns a String Representation of the object type
	 * Parameters: void
	 * Returns: TString - representation of the object type
	 * 
	 * Attributes: override
	 */
	virtual TString GetType()override;


	/**
	 * Method: EventHandler::OnFirstDraw
	 * Purpose: Allows Handlers to perform some task after the first draw
	 * Parameter: void
	 * Returns: void
	 * 
	 * Attributes: virtual
	 */
	virtual void OnFirstDraw();

	/**
	 * Method: EventHandler::ShouldProcessMessage
	 * Purpose: Reports whether the Message is to be processed by this handler
	 * Parameters: TrecPointer<HandlerMessage> message - the message to process
	 * Returns: bool - whether this is the handler for the submitted message
	 */
	bool ShouldProcessMessage(TrecPointer<HandlerMessage> message);

	/**
	 * Method: EventHandler::ProcessMessage
	 * Purpose: Processes the message sent to the handler
	 * Parameters: TrecPointer<HandlerMessage> message - the message to recieve and Process
	 * Returns: void
	 * 
	 * Attributes: abstract
	 */
	virtual void ProcessMessage(TrecPointer<HandlerMessage> message) = 0;


	/**
	 * Method: EventHandler::OnDestroy
	 * Purpose: Reports whether the Handler is ready to be destroyed
	 * Parameters: void
	 * Returns: bool - true
	 * 
	 * Attributes: virtual
	 */
    virtual bool OnDestroy();

	/**
	 * Method: EventHandler::SetSelf
	 * Purpose: Allows the Event Handler to supply a reference to itself
	 * Parameters: TrecPointer<EventHandler> handleSelf - the reference generated by the TrecPointerKey
	 * Returns: void
	 */
	virtual void SetSelf(TrecPointer<EventHandler> handleSelf);
	/**
	 * Method: EventHandler::GetId
	 * Purpose: Retirves the ID of this Handler
	 * Parameters: void
	 * Returns: UINT - the id of the Handler
	 *
	 * Note: id is not currently set, so this is a redundant method for the time being
	 */
	UINT GetId();

	/**
	 * Method: EventHandler::GetPage
	 * Purpose: Retrieves the Page associated with this Handler
	 * Parameters: void 
	 * Returns: TrecPointer<Page> - the page provided when Initialize was called
	 */
	virtual TrecPointer<TPage> GetPage() override;

	virtual bool OnChar(bool fromChar, UINT nChar, UINT nRepCnt, UINT nFlags, message_output* mOut)override;

	// Mini App related methods


	/**
	 * Method: EventHandler::SetMiniApp
	 * Purpose: Sets the MiniApp associted with this Handler
	 * Parameters: TrecPointer<MiniApp> mApp - the miniApp that called for this Handler
	 * Returns: void
	 */
	void SetMiniApp(TrecPointer<MiniApp> mApp);
	/**
	 * Method: EventHandler::OnFocus
	 * Purpose: Lets the handler know that this is the current focus of the User. Used to Set the Main MiniApp of the
	 *		IDE Window, if one is set
	 * Parameters: void
	 * Returns: void
	 */
	void OnFocus();

	/**
	 * Method: EventHandler::OnSave
	 * Purpose: Provides a Save method in case there is a way to save
	 * Parameters: void
	 * Returns: void
	 * 
	 * Attributes: virtual
	 */
	virtual void OnSave();


	/**
	 * Method: EventHandler::SetSaveFile
	 * Purpose: Sets up the file to save if OnSave is called
	 * Parameters: TrecPointer<TFileShell> file - the file to focus on
	 * Returns: void
	 */
	void SetSaveFile(TrecPointer<TFileShell> file);

	TrecPointer<TFileShell> GetFilePointer();

	virtual TrecPointer<TTextIntercepter> GetTextIntercepter();

	virtual void HandleEvents(TDataArray<TPage::EventID_Cred>& eventAr);


protected:
	/**
	 * Reference to "this" Handler
	 */
	TrecPointerSoft<EventHandler> hSelf;

	TString onFocusString;

	TrecPointer<TTextIntercepter> textIntercepter;

	/**
	 * Method: EventHandler::ShouldProcessMessageByType
	 * Purpose: Reports whether this Object is of the correct type to recieve the message
	 * Parameters: TrecPointer<HandlerMessage> message - the message to scan
	 * Returns: bool - true if the type matches, false oherwise
	 * 
	 * Attributes: abstract
	 */
	virtual bool ShouldProcessMessageByType(TrecPointer<HandlerMessage> message) = 0;

	/**
	 * The id of the Handler (not used yet)
	 */
	UINT id;

	/**
	 * The Name of the Handler for identification
	 */
	TString name;

	/**
	 * The instance associated with the Handler
	 */
	TrecPointerSoft<TProcess> app;

	/**
	 * The Page that the Handler is attached to
	 */
	TrecPointer<TPage> page;

	/**
	 * The Window this is pointing to
	 */
	TrecPointer<TWindow> window;

	/**
	 * The MiniApp that generated this handler (not always used
	 */
	TrecPointer<MiniApp> miniApp;

	// Resources for Saving

	/**
	 * The File attached to this Handler (if applicable)
	 */
	TrecPointer<TFileShell> filePointer;


	/**
	 * Method: EventHandler::SetSaveFile
	 * Purpose: Sets up the file to save if OnSave is called
	 * Parameters: void
	 * Returns: void
	 */
	void SetSaveFile();
};

/**
 * Class: TPageEnvironment
 * Purpose: Extends the TEnvironment to Support TPage and Handler Generation
 */
class _VIDEO_GRAPHICS TPageEnvironment : public TEnvironment
{
public:

	typedef enum class handler_type {
		ht_singular,
		ht_multiple,
		ht_ribbon,
		ht_file
	}handler_type;


	TPageEnvironment(TrecPointer<TFileShell> shell);

	/**
	 * Method: TPageEnvironment::GetPageAndHandler
	 * Purpose: Retrieves the Pages by the Specified name
	 * Parameters: const TString& name, TrecPointer<TPage> page, TrecPointer<TPage::EventHandler> handler
	 * Returns: void
	 *
	 * Note: If successful, the page and handler params should be assigned once the method returns
	 */
	void GetPageAndHandler(handler_type hType, const TString& name, TrecPointer<TPage>& page, TrecPointer<TPage::EventHandler>& handler, TrecPointer<DrawingBoard> board, TrecPointer<TProcess> proc);

	/**
	 * Method: TPageEnvironment::GetPageList
	 * Purpose: Retrieves the available Page Names by extension
	 * Parameters: const TString& ext, TDataArray<TString>& extensions
	 * Returns: void
	 *
	 * Note: if ext is an empty String, then return all that the Environment has to offer
	 */
	void GetPageList(handler_type hType, const TString& ext, TDataArray<TString>& extensions);

protected:


	/**
	 * Method: TPageEnvironment::GetPageAndHandler
	 * Purpose: Retrieves the Pages by the Specified name
	 * Parameters: const TString& name, TrecPointer<TPage> page, TrecPointer<TPage::EventHandler> handler
	 * Returns: void
	 *
	 * Note: If successful, the page and handler params should be assigned once the method returns
	 */
	virtual void GetPageAndHandler_(handler_type hType, const TString& name, TrecPointer<TPage>& page, TrecPointer<TPage::EventHandler>& handler, TrecPointer<DrawingBoard> board, TrecPointer<TProcess> proc) = 0;

	/**
	 * Method: TPageEnvironment::GetPageList
	 * Purpose: Retrieves the available Page Names by extension
	 * Parameters: const TString& ext, TDataArray<TString>& extensions
	 * Returns: void
	 *
	 * Note: if ext is an empty String, then return all that the Environment has to offer
	 */
	virtual void GetPageList_(handler_type hType, const TString& ext, TDataArray<TString>& extensions) = 0;
};