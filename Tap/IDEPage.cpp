#include "IDEPage.h"
#include "TInstance.h"
#include "TIdeWindow.h"


/**
 * Method: IDEPage::IDEPage
 * Purpose: Constructor
 * Parameters: ide_page_type type - the type of Page this will be
 *				UINT barSpace - how much space to give the Tab Bar
 *				TrecPointer<DrawingBoard> board - The Drawing Board to draw on
 * Returns: New IDEPage object
 */
IDEPage::IDEPage(ide_page_type type, UINT barSpace, TrecPointer<DrawingBoard> board): Page(board)
{
	this->type = type;
	this->barSpace = barSpace;

	moveMode = type == ide_page_type::ide_page_type_drag ? page_move_mode::page_move_mode_drag : page_move_mode::page_move_mode_normal;
	curPoint.x = curPoint.y = 0.0f;
	draw = true;



}

IDEPage::~IDEPage()
{
	for (UINT Rust = 0; Rust < pages.Size(); Rust++)
	{
		if (pages[Rust].Get())
			pages[Rust].Delete();
	}
}

/**
 * Method: IDEPage::SetResources
 * Purpose: Sets up resources for this Page to be a 2D Page
 * Parameters: TrecPointer<TInstance> in, TrecPointer<TWindow> window
 * Returns: void
 */
void IDEPage::SetResources(TrecPointer<TInstance> in, TrecPointer<TWindow> window)
{
	this->windowHandle = TrecPointerKey::GetSoftPointerFromTrec<TWindow>( window);
	instance = TrecPointerKey::GetSoftPointerFromTrec<TInstance>(in);


	deviceH = GetWindowDC(window->GetWindowHandle());
	
}

/**
 * Method: IDEPage::SetResources
 * Purpose: Sets up resources for 3D Support
 * Parameters: TrecPointer<TInstance> in - instance associate with this Page
 *				TrecPointer<TWindow> window - the Window this will be added to
 *				TrecPointer<TWindowEngine> engine - the Engine to provide 3D support
 * Returns: void
 */
void IDEPage::SetResources(TrecPointer<TInstance> in, TrecPointer<TWindow> window, TrecPointer<TWindowEngine> engine)
{
	SetResources(in, window);
}

/**
 * Method: IDEPage::MoveBorder
 * Purpose: Shofts the border when called upon by a neighboring page
 * Parameters: float& magnitude - the amount to shift the border by
 *				page_move_mode mode - which border to shift
 * Returns: void
 */
void IDEPage::MoveBorder(float& magnitude, page_move_mode mode)
{
	if (isSnipZero(area))
		return;
	switch (mode)
	{
	case page_move_mode::page_move_mode_bottom:
		area.bottom += magnitude;
		if (area.bottom < area.top)
		{
			magnitude -= (area.top - area.bottom);
			area.bottom = area.top;
		}
		break;
	case page_move_mode::page_move_mode_left:
		area.left += magnitude;
		if (area.right < area.left)
		{
			magnitude -= (area.right - area.left);
			area.left = area.right;
		}
		break;
	case page_move_mode::page_move_mode_right:
		area.right += magnitude;
		if (area.right < area.left)
		{
			magnitude -= (area.right - area.left);
			area.right = area.left;
		}
		break;
	case page_move_mode::page_move_mode_top:
		area.top += magnitude;
		if (area.bottom < area.top)
		{
			magnitude -= (area.top - area.bottom);
			area.top = area.bottom;
		}
	}

	if (area.left == area.right || area.top == area.bottom)
		draw = false;
	else
		draw = true;

	D2D1_RECT_F topBorder = area;
	topBorder.top = topBorder.top + barSpace;

	if (currentPage.Get())
	{
		for (UINT Rust = 0; Rust < pages.Size(); Rust++)
			if (pages[Rust].Get() && pages[Rust]->GetPage().Get())
				pages[Rust]->GetPage()->SetArea(topBorder);
	}
	else if (rootControl.Get())
		rootControl->Resize(topBorder);
}

/**
 * Method: IDEPage::OnRButtonUp
 * Purpose: Responds to the Right Button Up Message
 * Parameters: UINT nFlags - flags associated with the message
 *				TPoint point - the point included in the message
 *				messageOutput* mOut -  the result of the message
 * Returns: void
 */
void IDEPage::OnRButtonUp(UINT nFlags, TPoint point, messageOutput* mOut)
{
	if (currentPage.Get())
		currentPage->OnRButtonUp(nFlags, point, mOut);
	else
		Page::OnRButtonUp(nFlags, point, mOut);
}

/**
 * Method: IDEPage::OnLButtonDown
 * Purpose: Responds to the Left Button Down Message
 * Parameters: UINT nFlags - flags associated with the message
 *				TPoint point - the point included in the message
 *				messageOutput* mOut -  the result of the message
 * Returns: void
 */
void IDEPage::OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TrecPointer<TFlyout> fly)
{
	focusPage = GetFocusPage(point);

	if (focusPage.Get() && parentWindow.Get())
	{
		TrecPointerKey::GetSubPointerFromSoft<TWindow, TIdeWindow>(parentWindow)->SetCurrentHolder(focusPage);
		focusPage->curPoint = point;
	}

	if (isContained(point, area))
	{
		if (focusPage.Get())
		{
			auto targetPage = focusPage->GetPage();

			auto hand = targetPage->GetHandler();

			if (hand.Get())
				hand->OnFocus();
		}
		else if (handler.Get())
			handler->OnFocus();
	}

	if (currentPage.Get())
		currentPage->OnLButtonDown(nFlags, point, mOut, fly);
	else
		Page::OnLButtonDown(nFlags, point, mOut, fly);
}

/**
 * Method: IDEPage::OnRButtonDown
 * Purpose: Responds to the Right Button Down Message
 * Parameters: UINT nFlags - flags associated with the message
 *				TPoint point - the point included in the message
 *				messageOutput* mOut -  the result of the message
 * Returns: void
 */
void IDEPage::OnRButtonDown(UINT nFlags, TPoint point, messageOutput* mOut)
{
	if (currentPage.Get())
		currentPage->OnRButtonDown(nFlags, point, mOut);
	else
		Page::OnRButtonDown(nFlags, point, mOut);
}

/**
 * Method: IDEPage::OnMouseMove
 * Purpose: Responds to the Mouse Move Message
 * Parameters: UINT nFlags - flags associated with the message
 *				TPoint point - the point included in the message
 *				messageOutput* mOut -  the result of the message
 * Returns: void
 */
void IDEPage::OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TrecPointer<TFlyout> fly)
{
	if (currentPage.Get())
		currentPage->OnMouseMove(nFlags, point, mOut, fly);
	else
		Page::OnMouseMove(nFlags, point, mOut, fly);
}

/**
 * Method: IDEPage::OnLButtonDblClk
 * Purpose: Responds to the Left Button Double CLick Message
 * Parameters: UINT nFlags - flags associated with the message
 *				TPoint point - the point included in the message
 *				messageOutput* mOut -  the result of the message
 * Returns: void
 */
void IDEPage::OnLButtonDblClk(UINT nFlags, TPoint point, messageOutput* mOut)
{
	if (currentPage.Get())
		currentPage->OnLButtonDblClk(nFlags, point, mOut);
	else
		Page::OnLButtonDblClk(nFlags, point, mOut);
}

/**
 * Method: IDEPage::OnLButtonUp
 * Purpose: Responds to the Left Button Up Message
 * Parameters: UINT nFlags - flags associated with the message
 *				TPoint point - the point included in the message
 *				messageOutput* mOut -  the result of the message
 * Returns: void
 */
void IDEPage::OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TrecPointer<TFlyout> fly)
{
	auto upPage = GetFocusPage(point);
	if (focusPage.Get() && focusPage.Get() == upPage.Get())
	{
		currentPage = focusPage->GetBasePage();
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
	focusPage.Nullify();

	if (currentPage.Get())
		currentPage->OnLButtonUp(nFlags, point, mOut, fly);
	else
		Page::OnLButtonUp(nFlags, point, mOut, fly);
}

/**
 * Method: IDEPage::OnChar
 * Purpose: Responds to the Char Message
 * Parameters: bool fromChar - whether this is from on Char at the Windows level (assume "Key Down" if false)
 *				UINT nChar - the Character provided
 *				UINT nRepCnt - the number of times to add it
 *				UINT nFlags - flags associated with the message
 *				messageOutput* mOut - the result of the event
 * Returns:
 */
bool IDEPage::OnChar(bool fromChar, UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput* mOut)
{
	if (currentPage.Get())
		return currentPage->OnChar(fromChar, nChar, nRepCnt, nFlags, mOut);
	else
		return Page::OnChar(fromChar, nChar, nRepCnt, nFlags, mOut);
}

/**
 * Method: IDEPage::OnResize
 * Purpose: Resizes the Page
 * Parameters: D2D1_RECT_F& newLoc - the new regoin of the Page
 *				UINT nFlags - flags associated with the move
 *				TrecPointer<TWindowEngine> - the 3D Engine to work with
 * Returns: void
 */
void IDEPage::OnResize(D2D1_RECT_F& newLoc, UINT nFlags, TrecPointer<TWindowEngine> e)
{
	if (currentPage.Get())
		currentPage->OnResize(newLoc, nFlags, e);
	else
		Page::OnResize(newLoc, nFlags, e);
}

/**
 * Method: IDEPage::OnLButtonDown
 * Purpose: Captures the message and sees if it needs to prepare to shift borders before processing the message normally
 * Parameters: UINT nFlags - flags associated with the click
 *				TPoint point - Location of the mouse
 *				messageOutput* mOut - result of the click
 *				TDataArray<EventID_Cred>& eventAr - list of events to feed the Handler if applicable
 * Returns: void
 */
void IDEPage::OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TrecPointer<TFlyout> fly)
{
	if (isContained(point, area))
	{
		D2D1_RECT_F topBorder = area;
		topBorder.bottom = topBorder.top + barSpace;
		curPoint = point;


		if (moveMode == page_move_mode::page_move_mode_drag)
			goto regular_click_mode;


		


		if (isContained(point, topBorder))
		{
			//moveMode = page_move_mode::page_move_mode_top;
			// To-Do: Set Cursor

			return;
		}

		if (area.bottom - point.y < 1.5)
		{
			moveMode = page_move_mode::page_move_mode_bottom;
			// To-Do: Set Cursor

			return;
		}

		if (area.right - point.x < 1.5)
		{
			moveMode = page_move_mode::page_move_mode_right;
			// To-Do: Set Cursor

			return;
		}

		if (point.x - area.left < 1.5)
		{
			moveMode = page_move_mode::page_move_mode_left;
			// To-Do: Set Cursor
			
			return;
		}

		regular_click_mode:
		Page::OnLButtonDown(nFlags, point, mOut, eventAr, fly);
	}
}

/**
 * Method: IDEPage::OnMouseMove
 * Purpose: Captures the message and sees if it needs to shift borders before processing the message normally
 * Parameters: UINT nFlags - flags associated with the movement
 *				TPoint point - Location of the mouse
 *				messageOutput* mOut - result of the movement
 *				TDataArray<EventID_Cred>& eventAr - list of events to feed the Handler if applicable
 * Returns: void
 */
void IDEPage::OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TrecPointer<TFlyout> fly)
{
	if (moveMode == page_move_mode::page_move_mode_normal)
		return Page::OnMouseMove(nFlags, point, mOut, eventAr, fly);

	

	TPoint diff(static_cast<int>(point.x) - static_cast<int>(curPoint.x),
		static_cast<int>(point.y) - static_cast<int>(curPoint.y));

	switch (type)
	{
	case ide_page_type::ide_page_type_body:
		MouseMoveBody(diff);
		break;
	case ide_page_type::ide_page_type_basic_console:
		MouseMoveBasicConsole(diff);
		break;
	case ide_page_type::ide_page_type_deep_console:
		MouseMoveDeepConsole(diff);
		break;
	case ide_page_type::ide_page_type_lower_left:
		MouseMoveLowerLeft(diff);
		break;
	case ide_page_type::ide_page_type_lower_right:
		MouseMoveLowerRight(diff);
		break;
	case ide_page_type::ide_page_type_upper_left:
		MouseMoveUpperLeft(diff);
		break;
	case ide_page_type::ide_page_type_upper_right:
		MouseMoveUpperRight(diff);

	}

	curPoint.x = point.x;
	curPoint.y = point.y;
	*mOut = messageOutput::positiveOverrideUpdate;
}

/**
 * Method: IDEPage::OnLButtonUp
 * Purpose: Used to confirm whether a proper click was done
 * Parameters: TPoint& point - location where the mouse is when the user releases
 * Returns: bool - whether the mouse was in the Page's region
 */
bool IDEPage::OnLButtonUp(TPoint& point)
{
	curPoint.x = curPoint.y = 0.0f;
	moveMode = page_move_mode::page_move_mode_normal;

	return isContained(point, area);
}

/**
 * Method: IDEPage::Draw
 * Purpose: Draws the Tab Bar and then proceeds to draw its contents
 * Parameters: TrecPointer<TBrush> color - the Brush to draw the TabBar with
 *				TWindowEngine* twe = nullptr -  the TWindowEngine to feed the COntrols
 * Returns: void
 */
void IDEPage::Draw(TrecPointer<TBrush> color, TWindowEngine* twe)
{
	D2D1_RECT_F topBorder = area;
	topBorder.bottom = topBorder.top + barSpace;

	if (color.Get())
		color->FillRectangle(topBorder);
	if (type != ide_page_type::ide_page_type_drag)
	{
		for (UINT C = 0; C < pages.Size(); C++)
		{
			if (pages[C].Get())
			{
				pages[C]->Draw();
			}
		}


		if (dynamic_cast<IDEPage*>(currentPage.Get()))
			dynamic_cast<IDEPage*>(currentPage.Get())->Draw(color, twe);
	}
	else
		Page::Draw(twe);
	if (color.Get())
		color->DrawRectangle(area, 1.5F);
}


/**
 * Method: IDEPage::SetLink
 * Purpose: Allows the IDE Window to link up it's pages so that they can directly communicate with each other
 * Parameters: TrecSubPointer<Page, IDEPage> p - The Page to link to
 *				ide_page_type t - the Specific purpose of the Page
 * Returns: void
 */
void IDEPage::SetLink(TrecSubPointer<Page, IDEPage> p, ide_page_type t)
{
	switch (t)
	{
	case ide_page_type::ide_page_type_body:
		body = p;
		break;
	case ide_page_type::ide_page_type_basic_console:
		basicConsole = p;
		break;
	case ide_page_type::ide_page_type_deep_console:
		deepConsole = p;
		break;
	case ide_page_type::ide_page_type_upper_right:
		upperRight = p;
		break;
	case ide_page_type::ide_page_type_lower_right:
		lowerRight = p;
		break;
	case ide_page_type::ide_page_type_upper_left:
		upperLeft = p;
		break;
	case ide_page_type::ide_page_type_lower_left:
		lowerLeft = p;
		break;

	}
}

/**
 * Method: IDEPage::GetName
 * Purpose: Retireves the Name associated with this page
 * Parameters: void
 * Returns: TSTring - name held by the page
 */
TString IDEPage::GetName()
{
	return name;
}

/**
 * Method: IDEPage::SetNewParentPage
 * Purpose: Sets the Parent of this Page (assumes this is a Drag Page)
 * Parameters: TrecPointer<Page> p -  the Page that holds this page
 * Returns: void
 */
void IDEPage::SetNewParentPage(TrecPointer<Page> p)
{
	currentPage = p;
}

/**
 * Method: IDEPage::AddNewPage
 * Purpose: Add an Existing Page holder to THIS page
 * Parameters: TrecPointer<IDEPageHolder> pageHolder - The Tab and Page to add to
 * Returns: void
 */
void IDEPage::AddNewPage(TrecPointer<IDEPageHolder> pageHolder)
{
	if (!pageHolder.Get() || !pageHolder->GetPage().Get()) return;
	for (UINT c = 0; c < pages.Size(); c++)
	{
		if (pages[c].Get() == pageHolder.Get())
		{
			// First, return the location if need be
			D2D1_RECT_F loc{ 0.0f,0.0f,0.0f,0.0f };
			loc.top = area.top;
			loc.bottom = area.top + barSpace;

			if (c == 0)
				loc.left = area.left;
			else
				loc.left = pages[c - 1]->GetLocation().right;

			if (c + 1 == pages.Size())
			{
				loc.right = area.right;
			}
			else
				loc.right = pages[c + 1]->GetLocation().left;

			pageHolder->SetLocation(loc);

			return;
		}
	}

	D2D1_RECT_F loc{ 0.0f,0.0f,0.0f,0.0f };
	if (pages.Size())
	{
		loc = pages[pages.Size() - 1]->GetLocation();
		if (loc.right > 0.0f)
		{
			D2D1_RECT_F l = pageHolder->GetLocation();
			float leftBounds = loc.right + (l.right - l.left);
			if (leftBounds > area.right)
				loc.bottom = loc.left = loc.right = loc.top = 0.0f;
			else
			{
				loc.left = loc.right;
				loc.right = area.right;
			}
		}
		else 
			loc.bottom = loc.left = loc.right = loc.top = 0.0f;
	}
	else
	{
		loc = area;
		loc.bottom = loc.top + barSpace;
	}

	pageHolder->SetLocation(loc);
	pages.push_back(pageHolder);

	loc = area;
	loc.top += barSpace;
	pageHolder->GetPage()->SetArea(loc);
}

/**
 * Method: IDEPage::AddNewPage
 * Purpose: Submits a new Page to the IDE Page to be tabbed
 * Parameters: TrecPointer<TInstance> ins - the instance associated with this operation
 *				TrecPointer<TWindow> win - The Window this Page is in
 *				TString name - Te Name to present of the Tab
 *				TrecPointer<EventHandler> h - the EventHandler associated with the Page
 * Returns: TrecPointer<Page> - the page generated
 */
TrecPointer<Page> IDEPage::AddNewPage(TrecPointer<TInstance> ins, TrecPointer<TWindow> win, TString name, TrecPointer<EventHandler> h)
{
	TrecSubPointer<Page, IDEPage> newPage = TrecPointerKey::GetNewSelfTrecSubPointer<Page, IDEPage>(ide_page_type::ide_page_type_drag, 0, this->drawingBoard);

	newPage->SetResources(ins, win, win->GetWindowEngine());
	D2D1_RECT_F curArea = area;
	curArea.top += barSpace;
	newPage->SetArea(curArea);

	curArea = area;
	curArea.bottom = curArea.top + barSpace;


	for (UINT c = pages.Size() - 1; c < pages.Size(); c--)
	{
		if (pages[c].Get() && pages[c]->GetLocation().right)
		{
			curArea.left = pages[c]->GetLocation().right;
			break;
		}
	}
	

	TrecPointer<IDEPageHolder> newHolder = TrecPointerKey::GetNewTrecPointer<IDEPageHolder>(name, drawingBoard, barSpace, h, win, curArea);
	newHolder->SetPage(newPage);
	pages.push_back(newHolder);
	currentPage = TrecPointerKey::GetTrecPointerFromSub<Page, IDEPage>(newPage);
	return newHolder->GetBasePage();
}

/**
 * Method: IDEPage::RemovePage
 * Purpose: Removes the Page Holder and page from this IDE Page
 * Parameters: TrecPointer<IDEPageHolder> pageHolder - The Page Holder to Remove
 * Returns: void
 */
void IDEPage::RemovePage(TrecPointer<IDEPageHolder> pageHolder)
{
	int index = -1;
	for (UINT C = 0; C < pages.Size(); C++)
	{
		if (pageHolder.Get() == pages[C].Get())
		{
			index = C;
			break;
		}
	}

	if (index == -1)
		return;

	D2D1_RECT_F holdLoc = pageHolder->GetLocation();
	float width = holdLoc.right - holdLoc.left;

	for (UINT i = index + 1; i < pages.Size(); i++)
	{
		holdLoc = pages[i]->GetLocation();
		holdLoc.left -= width;
		holdLoc.right -= width;
		pages[i]->SetLocation(holdLoc);
	}

	pages.RemoveAt(index);
}

/**
 * Method: IDEPage::IsDrawing
 * Purpose: Reports whether the Page is currently able to draw
 * Parameters: void
 * Returns: bool - if the region is not zeroed
 */
bool IDEPage::IsDrawing()
{
	return draw = !isSnipZero(area);
}

/**
 * Method: IDEPage::GetFocusPage
 * Purpose: Retrieves the Page to set the focus to if the User Clicks on the Tab Bar
 * Parameters:TPoint& point
 * Returns: TrecPointer<IDEPageHolder> - the Holder referenced by the Tab
 */
TrecPointer<IDEPageHolder> IDEPage::GetFocusPage(TPoint& point)
{
	for (UINT c = 0; c < pages.Size(); c++)
	{
		if (pages[c].Get() && isContained(point, pages[c]->GetLocation()))
		{


			return pages[c];
		}
	}
	return TrecPointer<IDEPageHolder>();
}

/**
 * Method: IDEPage::MouseMoveBody
 * Purpose: Send move commands to its neighbors, assuming that this IDE Page is the Body Page
 * Parameters: TPoint& diff - the amount of units to move by
 * Returns: void
 */
void IDEPage::MouseMoveBody(TPoint& diff)
{
	TPoint before = diff;
	float yDiff = 0.0f, xDiff = 0.0f;
	D2D1_RECT_F curRect = { 0,0,0,0 };
	switch (moveMode)
	{
	case page_move_mode::page_move_mode_bottom:
		if(!dynamic_cast<IDEPage*>(basicConsole.Get())->IsDrawing())
			dynamic_cast<IDEPage*>(deepConsole.Get())->MoveBorder(diff.y, page_move_mode::page_move_mode_top);
		else
		{
			dynamic_cast<IDEPage*>(basicConsole.Get())->MoveBorder(diff.y, page_move_mode::page_move_mode_top);
			if (before.y != diff.y)
			{
				// In this case, we went down and tried to push it down. We should still attempt to push the deep console down
				yDiff = before.y - diff.y;

				dynamic_cast<IDEPage*>(deepConsole.Get())->MoveBorder(yDiff, page_move_mode::page_move_mode_top);
				curRect = basicConsole.Get()->GetArea();
				curRect.bottom += yDiff;
				curRect.top += yDiff;
				basicConsole.Get()->SetArea(curRect);

				
			}
		}
		// Now do the left side
		if (!dynamic_cast<IDEPage*>(lowerLeft.Get())->IsDrawing())
			dynamic_cast<IDEPage*>(upperLeft.Get())->MoveBorder(diff.y, page_move_mode::page_move_mode_bottom);
		else
		{
			dynamic_cast<IDEPage*>(lowerLeft.Get())->MoveBorder(diff.y, page_move_mode::page_move_mode_bottom);
			if (yDiff != diff.y)
			{
				float newDiff = yDiff - diff.y;
				dynamic_cast<IDEPage*>(upperLeft.Get())->MoveBorder(newDiff, page_move_mode::page_move_mode_bottom);
				curRect = basicConsole.Get()->GetArea();
				curRect.bottom += newDiff;
				curRect.top += newDiff;
				basicConsole.Get()->SetArea(curRect);
				diff.y = yDiff;
			}
		}

		// Finally, the right side
		if (!dynamic_cast<IDEPage*>(lowerRight.Get())->IsDrawing())
			dynamic_cast<IDEPage*>(upperRight.Get())->MoveBorder(diff.y, page_move_mode::page_move_mode_bottom);
		else
		{
			dynamic_cast<IDEPage*>(lowerRight.Get())->MoveBorder(diff.y, page_move_mode::page_move_mode_bottom);
			if (yDiff != diff.y)
			{
				float newDiff = yDiff - diff.y;
				dynamic_cast<IDEPage*>(upperRight.Get())->MoveBorder(newDiff, page_move_mode::page_move_mode_bottom);
				curRect = basicConsole.Get()->GetArea();
				curRect.bottom += newDiff;
				curRect.top += newDiff;
				basicConsole.Get()->SetArea(curRect);
				diff.y = yDiff;
			}
		}
		MoveBorder(diff.y, moveMode);
		break;
	case page_move_mode::page_move_mode_left:
		dynamic_cast<IDEPage*>(basicConsole.Get())->MoveBorder(diff.x, page_move_mode::page_move_mode_right);
		dynamic_cast<IDEPage*>(lowerLeft.Get())->MoveBorder(diff.x, page_move_mode::page_move_mode_right);
		dynamic_cast<IDEPage*>(upperLeft.Get())->MoveBorder(diff.x, page_move_mode::page_move_mode_right);
		MoveBorder(diff.x, moveMode);
		break;
	case page_move_mode::page_move_mode_right:
		dynamic_cast<IDEPage*>(basicConsole.Get())->MoveBorder(diff.x, page_move_mode::page_move_mode_left);
		dynamic_cast<IDEPage*>(lowerRight.Get())->MoveBorder(diff.x, page_move_mode::page_move_mode_left);
		dynamic_cast<IDEPage*>(upperRight.Get())->MoveBorder(diff.x, page_move_mode::page_move_mode_left);
		MoveBorder(diff.x, moveMode);
		break;
	//default:
		
	}
}

/**
 * Method: IDEPage::MouseMoveBasicConsole
 * Purpose: Send move commands to its neighbors, assuming that this IDE Page is the Basic-Console Page
 * Parameters: TPoint& diff - the amount of units to move by
 * Returns: void
 */
void IDEPage::MouseMoveBasicConsole(TPoint& diff)
{
	switch (moveMode)
	{
	case page_move_mode::page_move_mode_bottom:
		dynamic_cast<IDEPage*>(deepConsole.Get())->MoveBorder(diff.y, page_move_mode::page_move_mode_top);
		dynamic_cast<IDEPage*>(lowerLeft.Get())->MoveBorder(diff.y, page_move_mode::page_move_mode_bottom);
		dynamic_cast<IDEPage*>(lowerRight.Get())->MoveBorder(diff.y, page_move_mode::page_move_mode_bottom);
		MoveBorder(diff.y, moveMode);
		break;
	case page_move_mode::page_move_mode_top:
		dynamic_cast<IDEPage*>(body.Get())->MoveBorder(diff.y, page_move_mode::page_move_mode_bottom);
		MoveBorder(diff.y, moveMode);
		break;
	case page_move_mode::page_move_mode_left:
		dynamic_cast<IDEPage*>(body.Get())->MoveBorder(diff.x, page_move_mode::page_move_mode_right);
		dynamic_cast<IDEPage*>(lowerLeft.Get())->MoveBorder(diff.x, page_move_mode::page_move_mode_right);
		dynamic_cast<IDEPage*>(upperLeft.Get())->MoveBorder(diff.x, page_move_mode::page_move_mode_right);
		MoveBorder(diff.x, moveMode);
		break;
	case page_move_mode::page_move_mode_right:
		dynamic_cast<IDEPage*>(body.Get())->MoveBorder(diff.x, page_move_mode::page_move_mode_left);
		dynamic_cast<IDEPage*>(lowerRight.Get())->MoveBorder(diff.x, page_move_mode::page_move_mode_left);
		dynamic_cast<IDEPage*>(upperRight.Get())->MoveBorder(diff.x, page_move_mode::page_move_mode_left);
		MoveBorder(diff.x, moveMode);
		break;
	}
}

/**
 * Method: IDEPage::MouseMoveDeepConsole
 * Purpose: Send move commands to its neighbors, assuming that this IDE Page is the Deep-Console Page
 * Parameters: TPoint& diff - the amount of units to move by
 * Returns: void
 */
void IDEPage::MouseMoveDeepConsole(TPoint& diff)
{
	if (moveMode == page_move_mode::page_move_mode_top)
	{
		D2D1_RECT_F curRect = { 0,0,0,0 };
		float yDiff = diff.y;

		// First, tackle the basic console and the body if necessary
		if(!dynamic_cast<IDEPage*>(basicConsole.Get())->IsDrawing())
			dynamic_cast<IDEPage*>(body.Get())->MoveBorder(diff.y, page_move_mode::page_move_mode_bottom);
		else
		{
			dynamic_cast<IDEPage*>(basicConsole.Get())->MoveBorder(diff.y, page_move_mode::page_move_mode_bottom);
			if (yDiff != diff.y)
			{
				float newDiff = yDiff - diff.y;
				dynamic_cast<IDEPage*>(body.Get())->MoveBorder(newDiff, page_move_mode::page_move_mode_bottom);
				curRect = basicConsole.Get()->GetArea();
				curRect.bottom += newDiff;
				curRect.top += newDiff;
				basicConsole.Get()->SetArea(curRect);
				diff.y = yDiff;
			}
		}


		// Now do the left side
		if(!dynamic_cast<IDEPage*>(lowerLeft.Get())->IsDrawing())
			dynamic_cast<IDEPage*>(upperLeft.Get())->MoveBorder(diff.y, page_move_mode::page_move_mode_bottom);
		else
		{
			dynamic_cast<IDEPage*>(lowerLeft.Get())->MoveBorder(diff.y, page_move_mode::page_move_mode_bottom);
			if (yDiff != diff.y)
			{
				float newDiff = yDiff - diff.y;
				dynamic_cast<IDEPage*>(upperLeft.Get())->MoveBorder(newDiff, page_move_mode::page_move_mode_bottom);
				curRect = basicConsole.Get()->GetArea();
				curRect.bottom += newDiff;
				curRect.top += newDiff;
				basicConsole.Get()->SetArea(curRect);
				diff.y = yDiff;
			}
		}

		// Finally, the right side
		if(dynamic_cast<IDEPage*>(lowerRight.Get())->IsDrawing())
			dynamic_cast<IDEPage*>(upperRight.Get())->MoveBorder(diff.y, page_move_mode::page_move_mode_bottom);
		else
		{
			dynamic_cast<IDEPage*>(lowerRight.Get())->MoveBorder(diff.y, page_move_mode::page_move_mode_bottom);
			if (yDiff != diff.y)
			{
				float newDiff = yDiff - diff.y;
				dynamic_cast<IDEPage*>(upperRight.Get())->MoveBorder(newDiff, page_move_mode::page_move_mode_bottom);
				curRect = basicConsole.Get()->GetArea();
				curRect.bottom += newDiff;
				curRect.top += newDiff;
				basicConsole.Get()->SetArea(curRect);
				diff.y = yDiff;
			}
		}
		MoveBorder(diff.y, moveMode);
	}
}

/**
 * Method: IDEPage::MouseMoveUpperRight
 * Purpose: Send move commands to its neighbors, assuming that this IDE Page is the Upper-Right Page
 * Parameters: TPoint& diff - the amount of units to move by
 * Returns: void
 */
void IDEPage::MouseMoveUpperRight(TPoint& diff)
{
	TPoint before = diff;
	float yDiff = 0.0f, xDiff = 0.0f;
	D2D1_RECT_F curRect = { 0,0,0,0 };
	switch (moveMode)
	{
	case page_move_mode::page_move_mode_bottom:
		dynamic_cast<IDEPage*>(lowerRight.Get())->MoveBorder(diff.y, page_move_mode::page_move_mode_top);
		if (before.y != diff.y)
		{
			// In this case, we went down and tried to push it down. We should still attempt to push the deep console down
			yDiff = before.y - diff.y;

			dynamic_cast<IDEPage*>(deepConsole.Get())->MoveBorder(yDiff, page_move_mode::page_move_mode_top);
			curRect = basicConsole.Get()->GetArea();
			curRect.bottom += yDiff;
			curRect.top += yDiff;
			basicConsole.Get()->SetArea(curRect);

			// Since the deep Console moved, we need to send the message to the lower side panels
			dynamic_cast<IDEPage*>(lowerLeft.Get())->MoveBorder(yDiff, page_move_mode::page_move_mode_bottom);
		}
		MoveBorder(diff.y, moveMode);
		break;
	case page_move_mode::page_move_mode_left:
		dynamic_cast<IDEPage*>(body.Get())->MoveBorder(diff.x, page_move_mode::page_move_mode_right);
		dynamic_cast<IDEPage*>(basicConsole.Get())->MoveBorder(diff.x, page_move_mode::page_move_mode_right);
		if (before.x != diff.x)
		{
			// In this case, we went down and tried to push it down. We should still attempt to push the left panels left
			yDiff = before.x - diff.x;

			dynamic_cast<IDEPage*>(upperLeft.Get())->MoveBorder(yDiff, page_move_mode::page_move_mode_right);
			curRect = body.Get()->GetArea();
			curRect.left += yDiff;
			curRect.right += yDiff;
			body.Get()->SetArea(curRect);

			dynamic_cast<IDEPage*>(lowerLeft.Get())->MoveBorder(yDiff, page_move_mode::page_move_mode_right);
			curRect = basicConsole.Get()->GetArea();
			curRect.left += yDiff;
			curRect.right += yDiff;
			basicConsole.Get()->SetArea(curRect);

			// Since the deep Console moved, we need to send the message to the lower side panels
			//dynamic_cast<IDEPage*>(lowerLeft.Get())->MoveBorder(yDiff, page_move_mode::page_move_mode_bottom);
		}
		dynamic_cast<IDEPage*>(lowerRight.Get())->MoveBorder(diff.x, page_move_mode::page_move_mode_left);
		MoveBorder(diff.x, moveMode);
		break;
	}
}

/**
 * Method: IDEPage::MouseMoveLowerRight
 * Purpose: Send move commands to its neighbors, assuming that this IDE Page is the Lower-Right Page
 * Parameters: TPoint& diff - the amount of units to move by
 * Returns: void
 */
void IDEPage::MouseMoveLowerRight(TPoint& diff)
{
	TPoint before = diff;
	float yDiff = 0.0f, xDiff = 0.0f;
	D2D1_RECT_F curRect = { 0,0,0,0 };
	switch (moveMode)
	{
	case page_move_mode::page_move_mode_top:
		dynamic_cast<IDEPage*>(upperRight.Get())->MoveBorder(diff.y, page_move_mode::page_move_mode_bottom);
		MoveBorder(diff.y, moveMode);
		break;
	case page_move_mode::page_move_mode_left:
		dynamic_cast<IDEPage*>(body.Get())->MoveBorder(diff.x, page_move_mode::page_move_mode_right);
		dynamic_cast<IDEPage*>(basicConsole.Get())->MoveBorder(diff.x, page_move_mode::page_move_mode_right);
		if (before.x != diff.x)
		{
			// In this case, we went down and tried to push it down. We should still attempt to push the left panels left
			yDiff = before.x - diff.x;

			dynamic_cast<IDEPage*>(upperLeft.Get())->MoveBorder(yDiff, page_move_mode::page_move_mode_right);
			curRect = body.Get()->GetArea();
			curRect.left += yDiff;
			curRect.right += yDiff;
			body.Get()->SetArea(curRect);

			dynamic_cast<IDEPage*>(lowerLeft.Get())->MoveBorder(yDiff, page_move_mode::page_move_mode_right);
			curRect = basicConsole.Get()->GetArea();
			curRect.left += yDiff;
			curRect.right += yDiff;
			basicConsole.Get()->SetArea(curRect);

			// Since the deep Console moved, we need to send the message to the lower side panels
			//dynamic_cast<IDEPage*>(lowerLeft.Get())->MoveBorder(yDiff, page_move_mode::page_move_mode_bottom);
		}
		dynamic_cast<IDEPage*>(upperRight.Get())->MoveBorder(diff.x, page_move_mode::page_move_mode_left);
		MoveBorder(diff.x, moveMode);
		break;
	case page_move_mode::page_move_mode_bottom:
		dynamic_cast<IDEPage*>(deepConsole.Get())->MoveBorder(diff.y, page_move_mode::page_move_mode_top);
		dynamic_cast<IDEPage*>(basicConsole.Get())->MoveBorder(diff.y, page_move_mode::page_move_mode_bottom);
		dynamic_cast<IDEPage*>(lowerLeft.Get())->MoveBorder(diff.y, page_move_mode::page_move_mode_bottom);
		MoveBorder(diff.y, moveMode);
		break;
	}
}

/**
 * Method: IDEPage::MouseMoveUpperLeft
 * Purpose: Send move commands to its neighbors, assuming that this IDE Page is the Upper-Left Page
 * Parameters: TPoint& diff - the amount of units to move by
 * Returns: void
 */
void IDEPage::MouseMoveUpperLeft(TPoint& diff)
{
	TPoint before = diff;
	float yDiff = 0.0f, xDiff = 0.0f;
	D2D1_RECT_F curRect = { 0,0,0,0 };
	switch (moveMode)
	{
	case page_move_mode::page_move_mode_bottom:
		dynamic_cast<IDEPage*>(lowerLeft.Get())->MoveBorder(diff.y, page_move_mode::page_move_mode_top);
		if (before.y != diff.y)
		{
			// In this case, we went down and tried to push it down. We should still attempt to push the deep console down
			yDiff = before.y - diff.y;

			dynamic_cast<IDEPage*>(deepConsole.Get())->MoveBorder(yDiff, page_move_mode::page_move_mode_top);
			curRect = basicConsole.Get()->GetArea();
			curRect.bottom += yDiff;
			curRect.top += yDiff;
			basicConsole.Get()->SetArea(curRect);

			// Since the deep Console moved, we need to send the message to the lower side panels
			dynamic_cast<IDEPage*>(lowerRight.Get())->MoveBorder(yDiff, page_move_mode::page_move_mode_bottom);
		}
		MoveBorder(diff.y, moveMode);
		break;
	case page_move_mode::page_move_mode_left:
		dynamic_cast<IDEPage*>(body.Get())->MoveBorder(diff.x, page_move_mode::page_move_mode_left);
		dynamic_cast<IDEPage*>(basicConsole.Get())->MoveBorder(diff.x, page_move_mode::page_move_mode_left);
		if (before.x != diff.x)
		{
			// In this case, we went down and tried to push it down. We should still attempt to push the left panels left
			yDiff = before.x - diff.x;

			dynamic_cast<IDEPage*>(upperRight.Get())->MoveBorder(yDiff, page_move_mode::page_move_mode_left);
			curRect = body.Get()->GetArea();
			curRect.right += yDiff;
			curRect.left += yDiff;
			body.Get()->SetArea(curRect);

			dynamic_cast<IDEPage*>(lowerRight.Get())->MoveBorder(yDiff, page_move_mode::page_move_mode_left);
			curRect = basicConsole.Get()->GetArea();
			curRect.right += yDiff;
			curRect.left += yDiff;
			basicConsole.Get()->SetArea(curRect);

			// Since the deep Console moved, we need to send the message to the lower side panels
			//dynamic_cast<IDEPage*>(lowerLeft.Get())->MoveBorder(yDiff, page_move_mode::page_move_mode_bottom);
		}
		dynamic_cast<IDEPage*>(lowerLeft.Get())->MoveBorder(diff.x, page_move_mode::page_move_mode_right);
		MoveBorder(diff.x, moveMode);
		break;
	}
}

/**
 * Method: IDEPage::MouseMoveLowerLeft
 * Purpose: Send move commands to its neighbors, assuming that this IDE Page is the Lower-Left Page
 * Parameters: TPoint& diff - the amount of units to move by
 * Returns: void
 */
void IDEPage::MouseMoveLowerLeft(TPoint& diff)
{
	TPoint before = diff;
	float yDiff = 0.0f, xDiff = 0.0f;
	D2D1_RECT_F curRect = { 0,0,0,0 };
	switch (moveMode)
	{
	case page_move_mode::page_move_mode_top:
		dynamic_cast<IDEPage*>(upperLeft.Get())->MoveBorder(diff.y, page_move_mode::page_move_mode_bottom);
		MoveBorder(diff.y, moveMode);
		break;
	case page_move_mode::page_move_mode_right:
		dynamic_cast<IDEPage*>(body.Get())->MoveBorder(diff.x, page_move_mode::page_move_mode_left);
		dynamic_cast<IDEPage*>(basicConsole.Get())->MoveBorder(diff.x, page_move_mode::page_move_mode_left);
		if (before.x != diff.x)
		{
			// In this case, we went down and tried to push it down. We should still attempt to push the left panels left
			yDiff = before.x - diff.x;

			dynamic_cast<IDEPage*>(upperRight.Get())->MoveBorder(yDiff, page_move_mode::page_move_mode_left);
			curRect = body.Get()->GetArea();
			curRect.left += yDiff;
			curRect.right += yDiff;
			body.Get()->SetArea(curRect);

			dynamic_cast<IDEPage*>(lowerRight.Get())->MoveBorder(yDiff, page_move_mode::page_move_mode_left);
			curRect = basicConsole.Get()->GetArea();
			curRect.right += yDiff;
			curRect.left += yDiff;
			basicConsole.Get()->SetArea(curRect);

			// Since the deep Console moved, we need to send the message to the lower side panels
			//dynamic_cast<IDEPage*>(lowerLeft.Get())->MoveBorder(yDiff, page_move_mode::page_move_mode_bottom);
		}
		dynamic_cast<IDEPage*>(upperLeft.Get())->MoveBorder(diff.x, page_move_mode::page_move_mode_left);
		MoveBorder(diff.x, moveMode);
		break;
	case page_move_mode::page_move_mode_bottom:
		dynamic_cast<IDEPage*>(deepConsole.Get())->MoveBorder(diff.y, page_move_mode::page_move_mode_top);
		dynamic_cast<IDEPage*>(basicConsole.Get())->MoveBorder(diff.y, page_move_mode::page_move_mode_bottom);
		dynamic_cast<IDEPage*>(lowerRight.Get())->MoveBorder(diff.y, page_move_mode::page_move_mode_bottom);
		MoveBorder(diff.y, moveMode);
		break;
	}
}

/**
 * Method: IDEPageHolder::IDEPageHolder
 * Purpose: Constructor
 * Parameters: TString name, - name to present of the Tab
 *				TrecPointer<DrawingBoard> rt - the Drawing Board to draw with
 *				UINT barSpace - the size of the bar
 *				TrecPointer<EventHandler> handler - the handler to the new page
 *				TrecPointer<TWindow> win - the Window to draw against
 *				D2D1_RECT_F initLoc -  the initial location available (might be smaller than what's provided here)
 * Returns: New Page Holder Object
 */
IDEPageHolder::IDEPageHolder(TString name, TrecPointer<DrawingBoard> rt, UINT barSpace, TrecPointer<EventHandler> handler, TrecPointer<TWindow> win, D2D1_RECT_F initLoc)
{
	text = TrecPointerKey::GetNewTrecPointer<TText>(rt, nullptr);
	text->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
	text->setNewFontSize(12.0f);
	text->setCaption(name);
	text->setNewHorizontalAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	text->setNewVerticalAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	text->onCreate(initLoc);
	bool w;
	float minWidth = text->GetMinWidth(w);
	if (w)
	{
		initLoc.right = initLoc.left + minWidth;
		text->setNewLocation(convertD2DRectToRECT(initLoc));
	}
	location = initLoc;

	page = TrecPointerKey::GetNewSelfTrecSubPointer<Page, IDEPage>(ide_page_type::ide_page_type_drag, 0, rt);
	page->SetHandler(handler);
}

IDEPageHolder::~IDEPageHolder()
{
	page.Delete();
	text.Delete();
}

/**
 * Method: IDEPageHolder::GetBasePage
 * Purpose: Retrieves the Base Reference to the Page it is holding
 * Parameters: void
 * Returns: TrecPointer<Page> - the Base Page reference
 */
TrecPointer<Page> IDEPageHolder::GetBasePage()
{
	return TrecPointerKey::GetTrecPointerFromSub<Page, IDEPage>(page);
}

/**
 * Method: IDEPageHolder::GetPage
 * Purpose: Retrieves the IDE Page Reference this object is holding
 * Parameters: void
 * Returns: TrecSubPointer<Page, IDEPage> - the IDE Page reference
 */
TrecSubPointer<Page, IDEPage> IDEPageHolder::GetPage()
{
	return page;
}

/**
 * Method: IDEPageHolder::GetName
 * Purpose: Retrievs the Name printed on the Tab
 * Parameters: void
 * Returns: TString - the Name held by the holder
 */
TString IDEPageHolder::GetName()
{
	if(!text.Get())
	return TString();
	return text->getCaption();
}

/**
 * Method: IDEPageHolder::GetLocation
 * Purpose: Retrieves the Location of the Tab
 * Parameters: void
 * Returns: D2D1_RECT_F - the Rectable occupied by the Tab
 */
D2D1_RECT_F IDEPageHolder::GetLocation()
{
	return location;
}

/**
 * Method: IDEPageHolder::SetLocation
 * Purpose: Sets the boundaries of the Tab
 * Parameters: const D2D1_RECT_F& newLoc - reference to the space this tab is being allocated
 * Returns: D2D1_RECT_F - the Rectable occupied by the Tab
 */
D2D1_RECT_F IDEPageHolder::SetLocation(const D2D1_RECT_F& newLoc)
{
	location = newLoc;
	if (text.Get())
	{
		text->setNewLocation(convertD2DRectToRECT( newLoc));
		bool w;
		float width = text->GetMinWidth(w);
		location.right = location.left + width;

		text->setNewLocation(convertD2DRectToRECT(location));

	}
	return location;
}

/**
 * Method: IDEPageHolder::Draw
 * Purpose: Draws the Tab
 * Parameters: void
 * Returns: void
 */
void IDEPageHolder::Draw()
{
	if (text.Get())
		text->onDraw(location);
}



/**
 * Method: IDEPageHolder::Move
 * Purpose: Moves the Tab by the specifed amount
 * Parameters: TPoint& moveBy - the amount to move by
 * Returns: void
 */
void IDEPageHolder::Move(TPoint& moveBy)
{
	auto tempPoint = moveBy;
	moveBy.x -= curPoint.x;
	moveBy.y -= curPoint.y;

	location.bottom += moveBy.y;
	location.top += moveBy.y;
	location.left += moveBy.x;
	location.right += moveBy.x;

	curPoint = tempPoint;

	if (text.Get())
		text->setNewLocation(convertD2DRectToRECT(location));
}

/**
 * Method: IDEPageHolder::SetCurrentPoint
 * Purpose: Sets the Current Point of the Tab
 * Parameters: TPoint& p - the starting position of the tab
 * Returns: void
 */
void IDEPageHolder::SetCurrentPoint(TPoint& p)
{
	curPoint = p;
}

/**
 * Method: IDEPageHolder::SetPage
 * Purpose:sets the Page for this Tab
 * Parameters: TrecSubPointer<Page, IDEPage> p - the page to set this to
 * Returns: void
 */
void IDEPageHolder::SetPage(TrecSubPointer<Page, IDEPage> p)
{
	page = p;
}
