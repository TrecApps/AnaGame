
#include "Page.h"
#include <AnafaceParser.h>
#include <TML_Reader_.h>
#include "TInstance.h"
#include "TWindow.h"
#include "MiniHandler.h"

/**
 * Method: Page::Page
 * Purpose: Constructor
 * Parameters: TrecPointer<DrawingBoard> -  the Drawing Board for All Drawing operations
 * Returns: new Page object
 */
Page::Page(TrecPointer<DrawingBoard> board)
{
	drawingBoard = board;
	deviceH = nullptr;
	adjustMatrix = D2D1::Matrix3x2F::Identity();
	area = D2D1_RECT_F{ 0,0,0,0 };
	scale = 1.0f;
}

/**
 * Method: Page::GetAnimationByName
 * Purpose: Retrieves the Animation Bulder by name
 * Parameters: TString& name - name of the Animation Builder
 * Returns: TrecPointer<AnimationBuilder> - the builder requested, null if not found
 */
TrecPointer<AnimationBuilder> Page::GetAnimationByName(TString& name)
{
	for (UINT Rust = 0; Rust < animations.Size(); Rust++)
	{
		if (!animations[Rust].Get())
			continue;
		if (!animations[Rust]->GetName().Compare(name))
			return animations[Rust];
	}
	return TrecPointer<AnimationBuilder>();
}

/**
 * Method: Page::SwitchChildControl
 * Purpose: Allows the root control to insert a new control (most likely a TScollerControl) between the page and itself
 * Parameters: TrecPointer<TControl> newRoot - the control to set as the new root
 * Returns: void
 */
void Page::SwitchChildControl(TrecPointer<TControl> newRoot)
{
	if (newRoot.Get())
	{
		rootControl = newRoot;
		rootControl->setParent(selfHolder);
	}
}

/**
 * Method: Page::PrepAnimations
 * Purpose: Allows the Page to Prepare Animations requiested by the TControls it has generated
 * Parameters: TAnimationManager& aManager - the Animation Manager that oversees all Story Board within a Window
 * Returns: void
 */
void Page::PrepAnimations(TAnimationManager& aManager)
{
	if (!rootControl.Get())
		return;

	aManager.CleanBegin();

	TDataArray<TrecPointer<AnimationData>> aData;
	rootControl->RegisterAnimations(aData);

	TMap<TStoryBoard> storyBoards;
	TMap<TStoryBoard> persistentStoryBoards;

	assert(windowHandle.Get());

	for (UINT Rust = 0; Rust < basicStoryBoards.Size(); Rust++)
	{
		TrecPointer<TStoryBoard> story = TrecPointerKey::GetNewTrecPointer<TStoryBoard>();
		storyBoards.addEntry(basicStoryBoards[Rust], story);
		story->SetWindow(TrecPointerKey::GetTrecPointerFromSoft<TWindow>(windowHandle));
	}

	for (UINT Rust = 0; Rust < this->persistentStoryBoards.Size(); Rust++)
	{
		TrecPointer<TStoryBoard> story = TrecPointerKey::GetNewTrecPointer<TStoryBoard>();
		persistentStoryBoards.addEntry(this->persistentStoryBoards[Rust], story);
		story->SetPersistant();
		story->SetWindow(TrecPointerKey::GetTrecPointerFromSoft<TWindow>(windowHandle));
	}

	for (UINT Rust = 0; Rust < aData.Size(); Rust++)
	{
		if (aData[Rust].Get())
		{
			auto builder = GetAnimationByName(aData[Rust]->name);
			if (!builder.Get())
				continue;
			TrecPointer<Animation> ani = builder->Build();
			if (!ani.Get())
				continue;
			ani->SetControl(TrecPointerKey::GetTrecPointerFromSoft<TControl>(aData[Rust]->control));
			if (aData[Rust]->brush.Get())
				ani->SetComponent(aData[Rust]->brush);
			TrecPointer<TStoryBoard> sb = storyBoards.retrieveEntry(aData[Rust]->storyName);
			if(!sb.Get())
				sb = persistentStoryBoards.retrieveEntry(aData[Rust]->storyName);
			if (aData[Rust]->storyName.GetSize() && sb.Get())
			{
				sb->AddAnimation(ani);
			}
			else
				aManager.AddAnimationBegin(ani);
		}
	}

	for (UINT Rust = 0; Rust < storyBoards.count(); Rust++)
	{
		auto entry = storyBoards.GetEntryAt(Rust);
		if (!entry.Get() || !entry->object.Get())
			continue;

		aManager.AddStoryBoard(entry->key, entry->object);
	}

	for (UINT Rust = 0; Rust < persistentStoryBoards.count(); Rust++)
	{
		auto entry = persistentStoryBoards.GetEntryAt(Rust);
		if (!entry.Get() || !entry->object.Get())
			continue;

		aManager.AddStoryBoard(entry->key, entry->object);
	}
}


/**
 * Method: Page::~Page
 * Purpose: Destructor
 * Parameters: void
 * Returns: void
 */
Page::~Page()
{
	rootControl.Delete();
	// 3D mixing with 2D Resources
	solidBrush.Delete();
	if (instance.Get())
	{
		TrecPointerKey::GetTrecPointerFromSoft<TInstance>(instance)->UnregisterHandler(handler);
	}
}


/**
 * Method: static Page::GetWindowPage
 * Purpose: Proides a new page set for just 2D drawing
 * Parameters: TrecPointer<TInstance> - instance under which this page is created
 *				TrecPointer<TWindow> - the WIndow to hold this page
 *				TrecPointer<EventHandler> - the Event Handler to work with this page
 * Returns: TrecPointer<Page> - the page object requested
 */
TrecPointer<Page> Page::GetWindowPage(TrecPointer<TInstance> in , TrecPointer<TWindow> window, TrecPointer<EventHandler> eh)
{
	if (!in.Get())
		throw L"Error! Instance Object MUST ne initialized";

	RECT area;

	GetClientRect(window->GetWindowHandle(), &area);

	TrecPointer<Page> ret = TrecPointerKey::GetNewSelfTrecPointer<Page>(window->GetDrawingBoard());

	convertCRectToD2DRect(&area, &ret->area);
	ret->instance = TrecPointerKey::GetSoftPointerFromTrec<TInstance>( in);
	ret->handler = eh;
	ret->windowHandle = TrecPointerKey::GetSoftPointerFromTrec<TWindow>(window);
	ret->deviceH = GetWindowDC(window->GetWindowHandle());

	if (ret->handler.Get() && in.Get())
		in->RegisterHandler(eh);

	return ret;

}


/**
 * Method: Page::
 * Purpose:
 * Parameters:
 * Returns:
 *
 * Note: DEPRICATED - in favor of the IDE Page in the TIdeWindow
 */
TrecPointer<Page> Page::GetSmallPage(TrecPointer<TInstance> in, TrecPointer<TWindow> window, D2D1_RECT_F area)
{
	TrecPointer<Page> ret = TrecPointerKey::GetNewSelfTrecPointer<Page>(window->GetDrawingBoard());

	ret->area = area;


	ret->instance = TrecPointerKey::GetSoftPointerFromTrec<TInstance>(in);
	ret->windowHandle = TrecPointerKey::GetSoftPointerFromTrec<TWindow>(window);
	ret->deviceH = GetWindowDC(window->GetWindowHandle());
	return ret;
}

/**
 * Method: Page::SetAnaface
 * Purpose: Sets up the Anaface of this Page as well as the Handler
 * Parameters: TrecPointer<TFile> file - the file holding the TML file
 *				TrecPointer<EventHandler> eh -  the handler that goes with this TML Anaface
 * Returns: int error code (0 for success)
 *
 * Note: the file is expected to already be open
 */
int Page::SetAnaface(TrecPointer<TFile> file, TrecPointer<EventHandler> eh)
{
	if (!file.Get())
		return -1;
	if (!file->IsOpen())
		return -2;

	assert(windowHandle.Get());

	TrecPointer<Parser_> parser = TrecPointerKey::GetNewTrecPointerAlt<Parser_, AnafaceParser>(drawingBoard, TrecPointerKey::GetTrecPointerFromSoft<TWindow>(windowHandle)->GetWindowHandle(), file->GetFileDirectory());

	if (eh.Get())
		dynamic_cast<AnafaceParser*>(parser.Get())->setEventSystem(eh->GetEventNameList());

	TML_Reader_ reader(file, parser);
	int result = 0;
	if (!reader.read(&result))
		throw result;

	rootControl = dynamic_cast<AnafaceParser*>(parser.Get())->getRootControl();
	if (rootControl.Get())
	{
		rootControl->onCreate(area, TrecPointerKey::GetTrecPointerFromSoft<TWindow>(windowHandle)->GetWindowEngine());
		rootControl->setParent(selfHolder);
	}
	persistentStoryBoards = dynamic_cast<AnafaceParser*>(parser.Get())->GetPersistentStoryBoards();
	basicStoryBoards = dynamic_cast<AnafaceParser*>(parser.Get())->GetStoryBoards();
	animations = dynamic_cast<AnafaceParser*>(parser.Get())->GetAnimations();

	if(handler.Get())
		handler->Initialize(TrecPointerKey::GetTrecPointerFromSoft<Page>(self));
	

	return 0;
}

/**
 * Method: Page::SetAnaface
 * Purpose:
 * Parameters:
 * Returns:
 *
 * Note: Might be marked for Deprication
 */
int Page::SetAnaface(TrecPointer<TFile> file, TDataArray<eventNameID>& id)
{
	if (!file.Get())
		return -1;
	if (!file->IsOpen())
		return -2;
	assert(windowHandle.Get());
	TrecPointer<Parser_> parser = TrecPointerKey::GetNewTrecPointerAlt<Parser_, AnafaceParser>(drawingBoard, TrecPointerKey::GetTrecPointerFromSoft<TWindow>(windowHandle)->GetWindowHandle(), file->GetFileDirectory());

	dynamic_cast<AnafaceParser*>(parser.Get())->setEventSystem(id);

	TML_Reader_ reader(file, parser);
	int result = 0;
	if (!reader.read(&result))
		throw result;

	rootControl = dynamic_cast<AnafaceParser*>(parser.Get())->getRootControl();
	if (rootControl.Get())
	{
		rootControl->onCreate(area, TrecPointerKey::GetTrecPointerFromSoft<TWindow>(windowHandle)->GetWindowEngine());
		rootControl->setParent(selfHolder);
	}
	if(handler.Get())
		handler->Initialize(TrecPointerKey::GetTrecPointerFromSoft<Page>(self));

	persistentStoryBoards = dynamic_cast<AnafaceParser*>(parser.Get())->GetPersistentStoryBoards();
	basicStoryBoards = dynamic_cast<AnafaceParser*>(parser.Get())->GetStoryBoards();
	animations = dynamic_cast<AnafaceParser*>(parser.Get())->GetAnimations();

	return 0;
}

/**
 * Method: Page::
 * Purpose:
 * Parameters:
 * Returns:
 */
void Page::SetAnaface(TrecPointer<TControl> newRoot)
{
	rootControl = newRoot;
	if(rootControl.Get())
	rootControl->setParent(selfHolder);
}


/**
 * Method: Page::GetRootControl
 * Purpose: Retrieves the Root Control of this page
 * Parameters: void
 * Returns: TrecPointer<TControl> - the Root control of this Page
 */
TrecPointer<TControl> Page::GetRootControl()
{
	return rootControl;
}

/**
 * Method: Page::ExtractRootControl
 * Purpose: Retrieves the Root Control of this page, while also removing it from the Page
 * Parameters: void
 * Returns: TrecPointer<TControl> - the (now former) Root control of this Page
 */
TrecPointer<TControl> Page::ExtractRootControl()
{
	TrecPointer<TControl> ret(rootControl);
	rootControl.Nullify();
	return ret;
}

/**
 * Method: Page::GetWindowHandle
 * Purpose: Retrieves the TWindow holding this page
 * Parameters: void
 * Returns: TrecPointer<TWindow> - the TWindow associated with this page
 */
TrecPointer<TWindow> Page::GetWindowHandle()
{
	return TrecPointerKey::GetTrecPointerFromSoft<TWindow>(windowHandle);
}

/**
 * Method: Page::GetHandler
 * Purpose: Retrieves the Handler held by this Page
 * Parameters: void
 * Returns: TrecPointer<EventHandler> - the Event Handler responding to Events for this page
 */
TrecPointer<EventHandler> Page::GetHandler()
{
	return handler;
}

/**
 * Method: Page::SetHandler
 * Purpose: Sets the Handler for this Page
 * Parameters: TrecPointer<EventHandler> eh - the Handler to set to
 * Returns: void
 */
void Page::SetHandler(TrecPointer <EventHandler> eh)
{
	handler = eh;
	if (handler.Get() && instance.Get())
		TrecPointerKey::GetTrecPointerFromSoft<TInstance>(instance)->RegisterHandler(handler);
}

/**
 * Method: Page::OnRButtonUp
 * Purpose: Responds to the Right Button Up Message
 * Parameters: UINT nFlags - flags associated with the message
 *				TPoint point - the point included in the message
 *				messageOutput* mOut -  the result of the message
 * Returns: void
 */
void Page::OnRButtonUp(UINT nFlags, TPoint point, messageOutput* mOut)
{
	TDataArray<EventID_Cred> eventAr;
	OnRButtonUp(nFlags, point, mOut, eventAr);
	
	if(handler.Get())
		handler->HandleEvents(eventAr);

	if (miniHandler.Get())
		miniHandler->OnRButtonUp(nFlags, point, mOut);

	if( *mOut == messageOutput::negativeUpdate || *mOut == messageOutput::positiveContinueUpdate || *mOut == messageOutput::positiveOverrideUpdate)
		if (windowHandle.Get())TrecPointerKey::GetTrecPointerFromSoft<TWindow>(windowHandle)->Draw(); else return;
}

/**
 * Method: Page::OnLButtonDown
 * Purpose: Responds to the Left Button Down Message
 * Parameters: UINT nFlags - flags associated with the message
 *				TPoint point - the point included in the message
 *				messageOutput* mOut -  the result of the message
 * Returns: void
 */
void Page::OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TrecPointer<TFlyout> fly)
{
	TDataArray<EventID_Cred> eventAr;
	OnLButtonDown(nFlags, point, mOut, eventAr, fly);
	assert(windowHandle.Get());
	for (UINT Rust = 0; Rust < eventAr.Size(); Rust++)
	{
		if (eventAr[Rust].eventType == R_Message_Type::On_Select_Scroller && eventAr[Rust].scroll.Get() && windowHandle.Get())
		{
			TrecPointerKey::GetTrecPointerFromSoft<TWindow>(windowHandle)->currentScrollBar = eventAr[Rust].scroll;
			break;
		}
	}
	
	if(handler.Get())
		handler->HandleEvents(eventAr);

	if (miniHandler.Get())
		miniHandler->OnLButtonDown(nFlags, point, mOut);

	if( *mOut == messageOutput::negativeUpdate || *mOut == messageOutput::positiveContinueUpdate || *mOut == messageOutput::positiveOverrideUpdate)
		if (windowHandle.Get())TrecPointerKey::GetTrecPointerFromSoft<TWindow>(windowHandle)->Draw(); else return;
}

/**
 * Method: Page::OnRButtonDown
 * Purpose: Responds to the Right Button Down Message
 * Parameters: UINT nFlags - flags associated with the message
 *				TPoint point - the point included in the message
 *				messageOutput* mOut -  the result of the message
 * Returns: void
 */
void Page::OnRButtonDown(UINT nFlags, TPoint point, messageOutput* mOut)
{
	TDataArray<EventID_Cred> eventAr;
	OnRButtonDown(nFlags, point, mOut, eventAr);
	
	if (*mOut == messageOutput::positiveScroll && eventAr.Size() && windowHandle.Get())
	{
		for (UINT Rust = 0; Rust < eventAr.Size(); Rust++)
		{
			if (eventAr[Rust].scroll.Get())
			{
				TrecPointerKey::GetTrecPointerFromSoft<TWindow>(windowHandle)->currentScrollBar = eventAr[Rust].scroll;
				break;
			}
		}
	}


	if(handler.Get())
		handler->HandleEvents(eventAr);

	if (miniHandler.Get())
		miniHandler->OnRButtonDown(nFlags, point, mOut);
	if (*mOut == messageOutput::negativeUpdate || *mOut == messageOutput::positiveContinueUpdate || *mOut == messageOutput::positiveOverrideUpdate)
		if (windowHandle.Get())TrecPointerKey::GetTrecPointerFromSoft<TWindow>(windowHandle)->Draw(); else return;
}

/**
 * Method: Page::OnMouseMove
 * Purpose: Responds to the Mouse Move Message
 * Parameters: UINT nFlags - flags associated with the message
 *				TPoint point - the point included in the message
 *				messageOutput* mOut -  the result of the message
 * Returns: void
 */
void Page::OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TrecPointer<TFlyout> fly)
{
	TDataArray<EventID_Cred> eventAr;
	OnMouseMove(nFlags, point, mOut, eventAr, fly);
	
	if(handler.Get())
		handler->HandleEvents(eventAr);

	if (miniHandler.Get())
		miniHandler->OnMouseMove(nFlags, point, mOut);
	if (*mOut == messageOutput::negativeUpdate || *mOut == messageOutput::positiveContinueUpdate || *mOut == messageOutput::positiveOverrideUpdate)
		if (windowHandle.Get())TrecPointerKey::GetTrecPointerFromSoft<TWindow>(windowHandle)->Draw(); else return;
}

/**
 * Method: Page::OnLButtonDblClk
 * Purpose: Responds to the Left Button Double CLick Message
 * Parameters: UINT nFlags - flags associated with the message
 *				TPoint point - the point included in the message
 *				messageOutput* mOut -  the result of the message
 * Returns: void
 */
void Page::OnLButtonDblClk(UINT nFlags, TPoint point, messageOutput* mOut)
{
	TDataArray<EventID_Cred> eventAr;
	OnLButtonDblClk(nFlags, point, mOut, eventAr);
	
	if(handler.Get())
		handler->HandleEvents(eventAr);

	if (miniHandler.Get())
		miniHandler->OnLButtonDblClk(nFlags, point, mOut);

	if( *mOut == messageOutput::negativeUpdate || *mOut == messageOutput::positiveContinueUpdate || *mOut == messageOutput::positiveOverrideUpdate)
		if (windowHandle.Get())TrecPointerKey::GetTrecPointerFromSoft<TWindow>(windowHandle)->Draw(); else return;
}

/**
 * Method: Page::OnLButtonUp
 * Purpose: Responds to the Left Button Up Message
 * Parameters: UINT nFlags - flags associated with the message
 *				TPoint point - the point included in the message
 *				messageOutput* mOut -  the result of the message
 * Returns: void
 */
void Page::OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TrecPointer<TFlyout> fly)
{
	TDataArray<EventID_Cred> eventAr;
	OnLButtonUp(nFlags, point, mOut, eventAr, fly);

	assert(windowHandle.Get());

	// First, check to see if there is a new flyout to display. If there is, send it to the TWindow
	for (UINT Rust = 0; Rust < eventAr.Size(); Rust++)
	{
		if (eventAr[Rust].eventType == R_Message_Type::On_Flyout && eventAr[Rust].flyout.Get() && windowHandle.Get())
		{
			TrecPointerKey::GetTrecPointerFromSoft<TWindow>(windowHandle)->SetFlyout(eventAr[Rust].flyout);
			break;
		}
	}

	if (handler.Get())
		handler->HandleEvents(eventAr);

	if (miniHandler.Get())
		miniHandler->OnLButtonUp(nFlags, point, mOut);

	if (*mOut == messageOutput::negativeUpdate || *mOut == messageOutput::positiveContinueUpdate || *mOut == messageOutput::positiveOverrideUpdate)
		if (windowHandle.Get())TrecPointerKey::GetTrecPointerFromSoft<TWindow>(windowHandle)->Draw(); else return;
}

/**
 * Method: Page::OnChar
 * Purpose: Responds to the Char Message
 * Parameters: bool fromChar - whether this is from on Char at the Windows level (assume "Key Down" if false)
 *				UINT nChar - the Character provided
 *				UINT nRepCnt - the number of times to add it
 *				UINT nFlags - flags associated with the message
 *				messageOutput* mOut - the result of the event
 * Returns: bool
 */
bool Page::OnChar(bool fromChar,UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput *mOut)
{
	TDataArray<EventID_Cred> eventAr;
	bool returnable = false;
	if (miniHandler.Get())
		returnable = miniHandler->OnChar(fromChar, nChar, nRepCnt, nFlags, mOut);
	if (handler.Get())
		handler->HandleEvents(eventAr);

	if(!returnable)
		returnable = OnChar(fromChar, nChar, nRepCnt, nFlags, mOut, eventAr);

	if (*mOut == messageOutput::negativeUpdate || *mOut == messageOutput::positiveContinueUpdate || *mOut == messageOutput::positiveOverrideUpdate)
		if (windowHandle.Get())TrecPointerKey::GetTrecPointerFromSoft<TWindow>(windowHandle)->Draw(); else return false;
	return returnable;
}

/**
 * Method: Page::OnResize
 * Purpose: Resizes the Page
 * Parameters: D2D1_RECT_F& newLoc - the new regoin of the Page
 *				UINT nFlags - flags associated with the move
 *				TrecPointer<TWindowEngine> - the 3D Engine to work with
 * Returns: void
 */
void Page::OnResize(D2D1_RECT_F& newLoc, UINT nFlags, TrecPointer<TWindowEngine> engine)
{
	// Only Resize if basic resources are in order
	if (!instance.Get() || !windowHandle.Get())
		return;

	area = newLoc;

	if (rootControl.Get())
	{
		rootControl->Resize(area);
	}

	if (miniHandler.Get())
		miniHandler->OnResize(area);
}

/**
 * Method: Page::OnRButtonUp
 * Purpose: Responds to the Right Button Up Message
 * Parameters: UINT nFlags - flags associated with the message
 *				TPoint point - the point included in the message
 *				messageOutput* mOut -  the result of the message
 *				TDataArray<EventID_Cred>& eventAr - list of events
 * Returns: void
 *
 * Note: May be Deprecated soon once the MiniHandler is removed from the library
 */
void Page::OnRButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	if (!isContained(&point, &area))
		return;


	if (rootControl.Get())
		rootControl->OnRButtonUp(nFlags, point, mOut, eventAr);


	for (UINT Rust = 0; Rust < eventAr.Size(); Rust++)
	{
		auto ev = eventAr[Rust];
		for (UINT C = 0; C < rClickedControl.Size(); C++)
		{
			if (rClickedControl[C] == ev.control.Get())
			{
				rClickedControl.RemoveAt(C);
				break;
			}
		}
	}

	UINT curSize = rClickedControl.Size();
	for (UINT c = 0; c < rClickedControl.Size(); c++)
	{
		if (rClickedControl[c])
			rClickedControl[c]->SetNormalMouseState();
	}
	rClickedControl.RemoveAll();
	if (curSize)
	{
		switch (*mOut)
		{
		case messageOutput::negative:
			*mOut = messageOutput::negativeUpdate;
			break;
		case messageOutput::positiveContinue:
			*mOut = messageOutput::positiveContinueUpdate;
			break;
		case messageOutput::positiveOverride:
			*mOut = messageOutput::positiveOverrideUpdate;
		}
	}
}

/**
 * Method: Page::OnLButtonDown
 * Purpose: Responds to the Left Button Down Message
 * Parameters: UINT nFlags - flags associated with the message
 *				TPoint point - the point included in the message
 *				messageOutput* mOut -  the result of the message
 *				TDataArray<EventID_Cred>& eventAr - list of events
 * Returns: void
 *
 * Note: May be Deprecated soon once the MiniHandler is removed from the library
 */
void Page::OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TrecPointer<TFlyout> fly)
{
	if (!isContained(&point, &area))
		return;

	if (fly.Get())
	{
		fly->OnLButtonDown(nFlags, point, mOut, eventAr, clickedControl);
		if (*mOut != messageOutput::negative && *mOut != messageOutput::negativeUpdate)
			return;
	}

	if (rootControl.Get())
		rootControl->OnLButtonDown(nFlags, point, mOut, eventAr, clickedControl);
}

/**
 * Method: Page::OnRButtonDown
 * Purpose: Responds to the Right Button Down Message
 * Parameters: UINT nFlags - flags associated with the message
 *				TPoint point - the point included in the message
 *				messageOutput* mOut -  the result of the message
 *				TDataArray<EventID_Cred>& eventAr - list of events
 * Returns: void
 *
 * Note: May be Deprecated soon once the MiniHandler is removed from the library
 */
void Page::OnRButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	if (!isContained(&point, &area))
		return;

	if (rootControl.Get())
		rootControl->OnRButtonDown(nFlags, point, mOut, eventAr, rClickedControl);
}

/**
 * Method: Page::OnMouseMove
 * Purpose: Responds to the Mouse Move Message
 * Parameters: UINT nFlags - flags associated with the message
 *				TPoint point - the point included in the message
 *				messageOutput* mOut -  the result of the message
 *				TDataArray<EventID_Cred>& eventAr - list of events
 * Returns: void
 *
 * Note: May be Deprecated soon once the MiniHandler is removed from the library
 */
void Page::OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TrecPointer<TFlyout> fly)
{
	if (!isContained(&point, &area))
		return;

	if (fly.Get())
	{
		fly->OnMouseMove(nFlags, point, mOut, eventAr, mouseFocusControl);
		if (*mOut != messageOutput::negative && *mOut != messageOutput::negativeUpdate)
			return;
	}

	if (rootControl.Get())
		rootControl->OnMouseMove(nFlags, point, mOut, eventAr, mouseFocusControl);
}

/**
 * Method: Page::OnLButtonDblClk
 * Purpose: Responds to the Left Button Double CLick Message
 * Parameters: UINT nFlags - flags associated with the message
 *				TPoint point - the point included in the message
 *				messageOutput* mOut -  the result of the message
 *				TDataArray<EventID_Cred>& eventAr - list of events
 * Returns: void
 *
 * Note: May be Deprecated soon once the MiniHandler is removed from the library
 */
void Page::OnLButtonDblClk(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	if (!isContained(&point, &area))
		return;

	if (rootControl.Get())
		rootControl->OnLButtonDblClk(nFlags, point, mOut, eventAr);
}

/**
 * Method: Page::OnLButtonUp
 * Purpose: Responds to the Left Button Up Message
 * Parameters: UINT nFlags - flags associated with the message
 *				TPoint point - the point included in the message
 *				messageOutput* mOut -  the result of the message
 *				TDataArray<EventID_Cred>& eventAr - list of events
 * Returns: void
 *
 * Note: May be Deprecated soon once the MiniHandler is removed from the library
 */
void Page::OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TrecPointer<TFlyout> fly)
{
	if (!isContained(&point, &area))
		return;

	if (fly.Get())
	{
		fly->OnLButtonUp(nFlags, point, mOut, eventAr);
		if (*mOut != messageOutput::negative && *mOut != messageOutput::negativeUpdate)
			return;
	}

	if (rootControl.Get())
		rootControl->OnLButtonUp(nFlags, point, mOut, eventAr);
	UINT curSize = clickedControl.Size();
	for (UINT c = 0; c < clickedControl.Size(); c++)
	{
		if (clickedControl[c])
			clickedControl[c]->SetNormalMouseState();
	}
	clickedControl.RemoveAll();
	if (curSize)
	{
		switch (*mOut)
		{
		case messageOutput::negative:
			*mOut = messageOutput::negativeUpdate;
			break;
		case messageOutput::positiveContinue:
			*mOut = messageOutput::positiveContinueUpdate;
			break;
		case messageOutput::positiveOverride:
			*mOut = messageOutput::positiveOverrideUpdate;
		}
	}
}

/**
 * Method: Page::OnChar
 * Purpose: Responds to the Char Message
 * Parameters: bool fromChar - whether this is from on Char at the Windows level (assume "Key Down" if false)
 *				UINT nChar - the Character provided
 *				UINT nRepCnt - the number of times to add it
 *				UINT nFlags - flags associated with the message
 *				messageOutput* mOut - the result of the event
 *				TDataArray<EventID_Cred>& eventAr - list of events
 * Returns: bool
 *
 * Note: May be Deprecated soon once the MiniHandler is removed from the library
 */
bool Page::OnChar(bool fromChar, UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	if (rootControl.Get())
		return rootControl->OnChar(fromChar, nChar, nRepCnt, nFlags, mOut, eventAr);

	return false;
}

/**
 * Method: Page::OnResize
 * Purpose: Resizes the Page
 * Parameters: D2D1_RECT_F& newLoc - the new regoin of the Page
 *				UINT nFlags - flags associated with the move
 *				TrecPointer<TWindowEngine> - the 3D Engine to work with
 *				TDataArray<EventID_Cred>& eventAr - list of events
 * Returns: void
 *
 * Note: May be Deprecated soon once the MiniHandler is removed from the library
 */
void Page::OnResize(D2D1_RECT_F& newLoc, UINT nFlags, TDataArray<EventID_Cred>& eventAr)
{
}

/**
 * Method: Page::OnDestroy
 * Purpose: Reports whether the Page is ready to be destroyed
 * Parameters: void
 * Returns: bool - true if the Page doesn't have a handler or that handler is ready to be destroyed
 */
bool Page::OnDestroy()
{
	if(handler.Get())
		return handler->OnDestroy();
	return true;
}

/**
 * Method: Page::SetSelf
 * Purpose: Allows TrecPointerKey to set a Trec Reference to itself
 * Parameters: TrecPointer<Page> - self reference to the page
 * Returns: void
 */
void Page::SetSelf(TrecPointer<Page> s)
{
	if (!s.Get() || s.Get() != this)
		throw L"Error! Must Set sef with a valid pointer!";
	self = TrecPointerKey::GetSoftPointerFromTrec<Page>(s);

	selfHolder = TrecPointerKey::GetNewTrecPointerAlt<TParentHolder, TPageParentHolder>(self);
}

/**
 * Method: Page::GetInstance
 * Purpose: Retrieves the TInstance associiated with this page
 * Parameters: void
 * Returns: TrecPointer<TInstance> - the TInstance under which this page was created
 */
TrecPointer<TInstance> Page::GetInstance()
{
	return TrecPointerKey::GetTrecPointerFromSoft<TInstance>(instance);
}



/**
 * Method: Page::SetMiniHandler
 * Purpose: Sets the Mini Handler for this Page
 * Parameters: TrecSubPointer<EventHandler, MiniHandler> mh - the miniHandler to set
 * Returns: void
 *
 * Note: DEPRICATED - MiniHandler class is depricated so this method is as well
 */
void Page::SetMiniHandler(TrecSubPointer<EventHandler, MiniHandler> mh)
{
	miniHandler = mh;
	if (miniHandler.Get() && instance.Get())
	{
		TrecPointerKey::GetTrecPointerFromSoft<TInstance>(instance)->RegisterHandler(TrecPointerKey::GetTrecPointerFromSub<EventHandler, MiniHandler>(miniHandler));
	}
}

/**
 * Method: Page::AddStory
 * Purpose: Adds to the list of StoryBoards to manage
 * Parameters: TString& story - the name of the Story Board
 *				bool persistent - whether this Story Board should be "persistent" or not
 * Returns:
 */
void Page::AddStory(TString& story, bool persistent)
{
	if (persistent)
		persistentStoryBoards.push_back(story);
	else
		basicStoryBoards.push_back(story);
}

/**
 * Method: Page::AddAnimations
 * Purpose: Enables the Page to Create an Animation with an Animation Builder
 * Parameters: TrecPointer<AnimationBuilder> builder - the Builder for the Animation
 * Returns: void
 */
void Page::AddAnimations(TrecPointer<AnimationBuilder> builder)
{
	if (builder.Get())
		animations.push_back(builder);
}

/**
 * Method: Page::OnFocus
 * Purpose: Lets the Page know that User is now focusing on it
 * Parameters: void
 * Returns: void
 */
void Page::OnFocus()
{
	if (handler.Get())
		handler->OnFocus();
}




UCHAR * Page::GetAnaGameType()
{
	return nullptr;
}

/**
 * Method: Page::
 * Purpose:
 * Parameters:
 * Returns:
 *
 * Note: DEPRICATED - Use Resize or SetArea instead
 */
void Page::OnSize(UINT nType, int cx, int cy)
{


	area.top = area.left = 0.0f;
	area.bottom = cy;
	area.right = cx;



}

/**
 * Method: Page::GetArenaEngine
 * Purpose: Gets the Arena Engine Used by this Page
 * Parameters: void
 * Returns: TrecPointer<TArenaEngine> - the Arena Engine used by one of this Page's Controls
 */
TrecPointer<TArenaEngine> Page::GetArenaEngine()
{
	return engine;
}

/**
 * Method: Page::CreateLayout
 * Purpose: Calls OnCreate for the Root Control
 * Parameters: void
 * Returns: void
 */
void Page::CreateLayout()
{
	if (rootControl.Get() && windowHandle.Get())
	{
		rootControl->onCreate(area, TrecPointerKey::GetTrecPointerFromSoft<TWindow>(windowHandle)->GetWindowEngine());
	}
}

/**
 * Method: Page::Draw
 * Purpose: Draws the Page to the Window
 * Parameters: TWindowEngine* twe - the 3D Window Engine (not sure why this parameter is here)
 * Returns: void
 */
void Page::Draw(TWindowEngine* twe)
{
	if (!rootControl.Get() || !drawingBoard.Get()) return;
	
	//drawingBoard->SetTransform(adjustMatrix);
	rootControl->onDraw();
	//regRenderTarget->EndDraw();

	if (handler.Get())
		handler->Draw();
}

/**
 * Method: Page::GetArea
 * Purpose: Retrieves the Area held by the Page
 * Parameters: void
 * Returns: D2D1_RECT_F -  the Region the Page claims it is drawing to
 */
D2D1_RECT_F Page::GetArea()
{
	return area;
}

/**
 * Method: Page::SetArea
 * Purpose: Sets the Region of the Page
 * Parameters: const D2D1_RECT_F& loc -  the location to provide this Page
 * Returns: void
 */
void Page::SetArea(const D2D1_RECT_F& loc)
{
	area = loc;
	if (rootControl.Get())
		rootControl->Resize(area);
}

/**
 * Method: TPageParentHolder::TPageParentHolder
 * Purpose: Constructor
 * Parameters: TrecPointerSoft<class Page> page -  the Page to serve as the Parent
 * Returns: New Page Holder Object
 */
TPageParentHolder::TPageParentHolder(TrecPointerSoft<class Page> page)
{
	parent = page;
}

/**
 * Method: TPageParentHolder::SwitchChildControl
 * Purpose: Instructs the Page to treat the provided TControl as the new Root Control
 * Parameters: TrecPointerSoft<TControl> cur - the current root control
 *				TrecPointer<TControl> newTControl - the new control to set as root
 * Returns: void
 */
void TPageParentHolder::SwitchChildControl(TrecPointerSoft<TControl> cur, TrecPointer<TControl> newTControl)
{
	auto tPage = TrecPointerKey::GetTrecPointerFromSoft<Page>(parent);
	if (tPage.Get())
	{
		tPage->SwitchChildControl(newTControl);
	}
}
