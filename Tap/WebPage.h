#pragma once
#include "Page.h"
#include <TEnvironment.h>

// Remove this once the library managng HTML is created and provides the TWebNode
class TWebNode;


/**
 * Class: WebPageHolder
 * Purpose: Represents the 'tab' in the Web-Browser, holding the actual web-page
 */
class WebPageHolder
{
	friend class WebPage;
	friend class WebPageHolder;
public:

	/**
	 * Method: WebPageHolder::WebPageHolder
	 * Purpose: Constructor
	 * Parameters: TString name, - name to present of the Tab
	 *				TrecPointer<DrawingBoard> rt - the Drawing Board to draw with
	 *				UINT barSpace - the size of the bar
	 *				TrecPointer<EventHandler> handler - the handler to the new page
	 *				TrecPointer<TWindow> win - the Window to draw against
	 *				D2D1_RECT_F initLoc -  the initial location available (might be smaller than what's provided here)
	 * Returns: New Page Holder Object
	 */
	WebPageHolder(TString name, TrecPointer<DrawingBoard> rt, UINT barSpace, TrecPointer<EventHandler> handler, TrecPointer<TWindow> win, D2D1_RECT_F initLoc);


	/**
	 * Method: WebPageHolder::~WebPageHolder
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	virtual ~WebPageHolder();

	/**
	 * Method: WebPageHolder::SetImage
	 * Purpose: Sets up the image for the Tab to draw
	 * Parameters: TrecPointer<TFileShell> file - reference to the file to call upon
	 * Returns: bool - true if the file was established, false if an error occured
	 */
	bool SetImage(TrecPointer<TFileShell> file);


	/**
	 * Method: WebPageHolder::GetBasePage
	 * Purpose: Retrieves the Base Reference to the Page it is holding
	 * Parameters: void
	 * Returns: TrecPointer<Page> - the Base Page reference
	 */
	TrecPointer<Page> GetBasePage();

	/**
	 * Method: WebPageHolder::GetName
	 * Purpose: Retrievs the Name printed on the Tab
	 * Parameters: void
	 * Returns: TString - the Name held by the holder
	 */
	TString GetName();

	/**
	 * Method: WebPageHolder::GetLocation
	 * Purpose: Retrieves the Location of the Tab
	 * Parameters: void
	 * Returns: D2D1_RECT_F - the Rectable occupied by the Tab
	 */
	D2D1_RECT_F GetLocation();
	/**
	 * Method: WebPageHolder::SetLocation
	 * Purpose: Sets the boundaries of the Tab
	 * Parameters: const D2D1_RECT_F& newLoc - reference to the space this tab is being allocated
	 * Returns: D2D1_RECT_F - the Rectable occupied by the Tab
	 */
	D2D1_RECT_F SetLocation(const D2D1_RECT_F& newLoc);
	/**
	 * Method: WebPageHolder::Draw
	 * Purpose: Draws the Tab
	 * Parameters: void
	 * Returns: void
	 */
	void Draw();
	/**
	 * Method: WebPageHolder::Move
	 * Purpose: Moves the Tab by the specifed amount
	 * Parameters: TPoint& moveBy - the amount to move by
	 * Returns: void
	 */
	void Move(TPoint& moveBy);

	/**
	 * Method: WebPageHolder::SetCurrentPoint
	 * Purpose: Sets the Current Point of the Tab
	 * Parameters: TPoint& p - the starting position of the tab
	 * Returns: void
	 */
	void SetCurrentPoint(TPoint& p);

	/**
	 * Method: WebPageHolder::SetPage
	 * Purpose:sets the Page for this Tab
	 * Parameters: TrecSubPointer<Page, WebPage> p - the page to set this to
	 * Returns: void
	 */
	void SetPage(TrecPointer<Page> p);
protected:
	/**
	 * Image to draw on the Tab
	 */
	TrecSubPointer<TBrush, TBitmapBrush> image;

	/**
	 * the page this tab is holding on to
	 */
	TrecPointer<Page> page;

	/**
	 * The Text to print out
	 */
	TrecPointer<TText> text;

	/**
	 * the location this tab occupies
	 */
	D2D1_RECT_F location;

	/**
	 * Used for moving
	 */
	TPoint curPoint;

	/**
	 * Holds the Drawing Board
	 */
	TrecPointer<DrawingBoard> board;
};


/**
 * Class: WebPage
 * Purpose: Extends Page to Support Web-oriented Content
 * 
 * SuperClass: Page
 */
class WebPage : public Page
{
	friend class TWebWindow;
public:

	/**
	 * Method: WebPage::GetType
	 * Purpose: Returns a String Representation of the object type
	 * Parameters: void
	 * Returns: TString - representation of the object type
	 */
	virtual TString GetType() override;


	/**
	 * Method: WebPage::~WebPage
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	virtual ~WebPage();

	/**
	 * Method: WebPage::SetAnaface
	 * Purpose: Sets up the Anaface of this Page as well as the Handler
	 * Parameters: TrecPointer<TFile> file - the file holding the TML file
	 *				TrecPointer<EventHandler> eh -  the handler that goes with this TML Anaface
	 * Returns: int error code (0 for success)
	 *
	 * Note: the file is expected to already be open
	 */
	int SetAnaface(TrecPointer<TFile> file, TrecPointer<EventHandler> eh);


	/**
	 * Method: WebPage::Draw
	 * Purpose: Draws the Page to the Window
	 * Parameters: TWindowEngine* twe - the 3D Window Engine (not sure why this parameter is here)
	 * Returns: void
	 */
	virtual void Draw(TWindowEngine* twe = nullptr);


	// Because this Object more or less represents Anagame's version of the JavaScript Web DOM, here are methods
	// Supporting that API, which need to be wrapped around by TNativeInterpretor functions from the TcDataStruct library

	/**
	 * Method: WebPage::GetElementById
	 * Purpose: Internal Anagame implementation of the DOM.getElementById method
	 * Parameters: const TString& id - the ID of the requested HTML node
	 * Returns: TrecPointer<TWebNode> - the Requested node
	 */
	TrecPointer<TWebNode> GetElementById(const TString& id);

	/**
	 * Method: WebPage::GetElementsByTag
	 * Purpose: Internal Anagame implementation of the DOM.getElementsByTagName method
	 * Parameters: const TString& id - the tag name of the requested HTML nodes
	 * Returns: TDataArray<TrecPointer<TWebNode>> - the list of Requested nodes
	 */
	TDataArray<TrecPointer<TWebNode>> GetElementsByTag(const TString& id);

	/**
	 * Method: WebPage::GetElementsByClass
	 * Purpose: Internal Anagame implementation of the DOM.getElementsByClassName method
	 * Parameters: const TString& id - the class name of the requested HTML nodes
	 * Returns: TDataArray<TrecPointer<TWebNode>> - the list of Requested nodes
	 */
	TDataArray<TrecPointer<TWebNode>> GetElementsByClass(const TString& id);

	void Open();

	void Write(const TString& markup);

	void Close();


protected:

	TrecPointer<TEnvironment> environment;

	TrecPointer<TWebNode> rootNode;

	TrecPointer<WebPageHolder> tab;
};
