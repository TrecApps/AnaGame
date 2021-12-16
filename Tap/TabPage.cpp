#include "TabPage.h"

bool IsSnipZero(D2D1_RECT_F& loc)
{
	return !loc.bottom && !loc.left
		&& !loc.right && !loc.top;
}


D2D1_RECT_F TabPage::GetChildSpace()
{
	D2D1_RECT_F ret = area;
	ret.top += tabHeight;
	return ret;
}

TabPage::TabPage(ide_page_type t,TrecPointer<DrawingBoard> board, UCHAR tabHeight): TPage(board), tabBar(board)
{
	this->tabHeight = tabHeight;
	type = t;
	moveMode = tab_page_move_mode::normal;
	if (type == ide_page_type::ide_page_type_main)
	{
		tabBar.SetConfigure(false, false);
	}
}

bool TabPage::HandlesEvents()
{
	return false;
}

void TabPage::Draw(TrecPointer<TVariable> object)
{
	TrecPointer<TVariable> v;
	tabBar.Draw(v);
	if (currentPage.Get())
		currentPage->Draw(v);
}

ag_msg void TabPage::OnRButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>& cred)
{
	if (currentPage.Get())
		currentPage->OnRButtonUp(nFlags, point, mOut, cred);
}

ag_msg void TabPage::OnRButtonDown(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>& cred)
{
	if (currentPage.Get())
		currentPage->OnRButtonDown(nFlags, point, mOut, cred);
}

ag_msg void TabPage::OnLButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>& cred)
{
	tabBar.OnLButtonUp(nFlags, point, mOut, cred);
	if (currentPage.Get())
		currentPage->OnLButtonUp(nFlags, point, mOut, cred);
}

ag_msg void TabPage::OnLButtonDown(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>& cred)
{
	tabBar.OnLButtonDown(nFlags, point, mOut, cred);
	if (currentPage.Get())
		currentPage->OnLButtonDown(nFlags, point, mOut, cred);
}

ag_msg void TabPage::OnMouseMove(UINT nFlags, TPoint point, message_output& mOut, TDataArray<EventID_Cred>& cred)
{
	tabBar.OnMouseMove(nFlags, point, mOut, cred);
	if (currentPage.Get())
		currentPage->OnMouseMove(nFlags, point, mOut, cred);
}

ag_msg void TabPage::OnLButtonDblClk(UINT nFlags, TPoint point, message_output& mOut, TDataArray<EventID_Cred>& cred)
{
	if (currentPage.Get())
		currentPage->OnLButtonDblClk(nFlags, point, mOut, cred);
}

ag_msg void TabPage::OnResize(D2D1_RECT_F& newLoc, UINT nFlags, TDataArray<EventID_Cred>& eventAr)
{
	D2D1_RECT_F tabSpace = newLoc, currentSpace = newLoc;
	if (tabHeight)
	{
		tabSpace.bottom = tabSpace.top + tabHeight;
		currentSpace.top = tabSpace.bottom;
		tabBar.OnResize(tabSpace, nFlags, eventAr);
	}
	if (currentPage.Get())
		currentPage->OnResize(currentSpace, nFlags, eventAr);
}

ag_msg bool TabPage::OnDestroy()
{
	return true;
}

ag_msg bool TabPage::OnScroll(bool fromBars, const TPoint& point, const TPoint& direction, TDataArray<EventID_Cred>& args)
{
	if (currentPage.Get())
		return OnScroll(false, point, direction, args);
	return false;
}

void TabPage::SetView(TrecPointer<TPage> page)
{
	bool shown = false;
	if (page.Get())
	{
		shown = true;
		currentPage = page;
		previousPages.push_back(page);
		
	}
	else if (previousPages.Size())
	{
		shown = true;
		currentPage = previousPages[previousPages.Size() - 1];
	}
	
	if (!shown)
	{
		// To-Do
	}
}

void TabPage::RemovePage(TrecPointer<TPage> page)
{
	if (!page.Get())
		return;
	for (UINT Rust = previousPages.Size() -1; Rust < previousPages.Size(); Rust--)
	{
		if (previousPages[Rust].Get() == page.Get())
			previousPages.RemoveAt(Rust);
	}

	if (previousPages.Size())
	{
		currentPage = previousPages[previousPages.Size() - 1];
	}
	else
	{
		// To-Do:
	}
}

void TabPage::SetView(TrecPointer<TPage> page, const TString& name)
{
	int e = 0;
}


/**
 * Method: IDEPage::IsDrawing
 * Purpose: Reports whether the Page is currently able to draw
 * Parameters: void
 * Returns: bool - if the region is not zeroed
 */
bool TabPage::IsDrawing()
{
	ThreadLock();
	bool ret = !IsSnipZero(area);
	ThreadRelease();
	return ret;
}

/**
 * Method: TabPage::MouseMoveBody
 * Purpose: Send move commands to its neighbors, assuming that this IDE Page is the Body Page
 * Parameters: TPoint& diff - the amount of units to move by
 * Returns: void
 */
void TabPage::MouseMoveBody(TPoint& diff)
{
	TPoint before = diff;
	float yDiff = 0.0f, xDiff = 0.0f;
	D2D1_RECT_F curRect = { 0,0,0,0 };
	ThreadLock();

	TDataArray<EventID_Cred> cred;

	switch (moveMode)
	{
	case tab_page_move_mode::bottom:
		if (!dynamic_cast<TabPage*>(basicConsole.Get())->IsDrawing())
			dynamic_cast<TabPage*>(deepConsole.Get())->MoveBorder(diff.y, tab_page_move_mode::top);
		else
		{
			dynamic_cast<TabPage*>(basicConsole.Get())->MoveBorder(diff.y, tab_page_move_mode::top);
			if (before.y != diff.y)
			{
				// In this case, we went down and tried to push it down. We should still attempt to push the deep console down
				yDiff = before.y - diff.y;

				dynamic_cast<TabPage*>(deepConsole.Get())->MoveBorder(yDiff, tab_page_move_mode::top);
				curRect = basicConsole.Get()->GetArea();
				curRect.bottom += yDiff;
				curRect.top += yDiff;
				basicConsole.Get()->OnResize(curRect, 0, cred);


			}
		}
		// Now do the left side
		if (!dynamic_cast<TabPage*>(lowerLeft.Get())->IsDrawing())
			dynamic_cast<TabPage*>(upperLeft.Get())->MoveBorder(diff.y, tab_page_move_mode::bottom);
		else
		{
			dynamic_cast<TabPage*>(lowerLeft.Get())->MoveBorder(diff.y, tab_page_move_mode::bottom);
			if (yDiff != diff.y)
			{
				float newDiff = yDiff - diff.y;
				dynamic_cast<TabPage*>(upperLeft.Get())->MoveBorder(newDiff, tab_page_move_mode::bottom);
				curRect = basicConsole.Get()->GetArea();
				curRect.bottom += newDiff;
				curRect.top += newDiff;
				basicConsole.Get()->OnResize(curRect, 0, cred);
				diff.y = yDiff;
			}
		}

		// Finally, the right side
		if (!dynamic_cast<TabPage*>(lowerRight.Get())->IsDrawing())
			dynamic_cast<TabPage*>(upperRight.Get())->MoveBorder(diff.y, tab_page_move_mode::bottom);
		else
		{
			dynamic_cast<TabPage*>(lowerRight.Get())->MoveBorder(diff.y, tab_page_move_mode::bottom);
			if (yDiff != diff.y)
			{
				float newDiff = yDiff - diff.y;
				dynamic_cast<TabPage*>(upperRight.Get())->MoveBorder(newDiff, tab_page_move_mode::bottom);
				curRect = basicConsole.Get()->GetArea();
				curRect.bottom += newDiff;
				curRect.top += newDiff;
				basicConsole.Get()->OnResize(curRect, 0, cred);
				diff.y = yDiff;
			}
		}
		MoveBorder(diff.y, moveMode);
		break;
	case tab_page_move_mode::left:
		dynamic_cast<TabPage*>(basicConsole.Get())->MoveBorder(diff.x, tab_page_move_mode::right);
		dynamic_cast<TabPage*>(lowerLeft.Get())->MoveBorder(diff.x, tab_page_move_mode::right);
		dynamic_cast<TabPage*>(upperLeft.Get())->MoveBorder(diff.x, tab_page_move_mode::right);
		MoveBorder(diff.x, moveMode);
		break;
	case tab_page_move_mode::right:
		dynamic_cast<TabPage*>(basicConsole.Get())->MoveBorder(diff.x, tab_page_move_mode::left);
		dynamic_cast<TabPage*>(lowerRight.Get())->MoveBorder(diff.x, tab_page_move_mode::left);
		dynamic_cast<TabPage*>(upperRight.Get())->MoveBorder(diff.x, tab_page_move_mode::left);
		MoveBorder(diff.x, moveMode);
		break;
		//default:

	}
	ThreadRelease();
}

/**
 * Method: TabPage::MouseMoveBasicConsole
 * Purpose: Send move commands to its neighbors, assuming that this IDE Page is the Basic-Console Page
 * Parameters: TPoint& diff - the amount of units to move by
 * Returns: void
 */
void TabPage::MouseMoveBasicConsole(TPoint& diff)
{
	ThreadLock();
	switch (moveMode)
	{
	case tab_page_move_mode::bottom:
		dynamic_cast<TabPage*>(deepConsole.Get())->MoveBorder(diff.y, tab_page_move_mode::top);
		dynamic_cast<TabPage*>(lowerLeft.Get())->MoveBorder(diff.y, tab_page_move_mode::bottom);
		dynamic_cast<TabPage*>(lowerRight.Get())->MoveBorder(diff.y, tab_page_move_mode::bottom);
		MoveBorder(diff.y, moveMode);
		break;
	case tab_page_move_mode::top:
		dynamic_cast<TabPage*>(body.Get())->MoveBorder(diff.y, tab_page_move_mode::bottom);
		MoveBorder(diff.y, moveMode);
		break;
	case tab_page_move_mode::left:
		dynamic_cast<TabPage*>(body.Get())->MoveBorder(diff.x, tab_page_move_mode::right);
		dynamic_cast<TabPage*>(lowerLeft.Get())->MoveBorder(diff.x, tab_page_move_mode::right);
		dynamic_cast<TabPage*>(upperLeft.Get())->MoveBorder(diff.x, tab_page_move_mode::right);
		MoveBorder(diff.x, moveMode);
		break;
	case tab_page_move_mode::right:
		dynamic_cast<TabPage*>(body.Get())->MoveBorder(diff.x, tab_page_move_mode::left);
		dynamic_cast<TabPage*>(lowerRight.Get())->MoveBorder(diff.x, tab_page_move_mode::left);
		dynamic_cast<TabPage*>(upperRight.Get())->MoveBorder(diff.x, tab_page_move_mode::left);
		MoveBorder(diff.x, moveMode);
		break;
	}
	ThreadRelease();
}

/**
 * Method: TabPage::MouseMoveDeepConsole
 * Purpose: Send move commands to its neighbors, assuming that this IDE Page is the Deep-Console Page
 * Parameters: TPoint& diff - the amount of units to move by
 * Returns: void
 */
void TabPage::MouseMoveDeepConsole(TPoint& diff)
{
	ThreadLock();
	if (moveMode == tab_page_move_mode::top)
	{

		TDataArray<EventID_Cred> cred;
		D2D1_RECT_F curRect = { 0,0,0,0 };
		float yDiff = diff.y;

		// First, tackle the basic console and the body if necessary
		if (!dynamic_cast<TabPage*>(basicConsole.Get())->IsDrawing())
			dynamic_cast<TabPage*>(body.Get())->MoveBorder(diff.y, tab_page_move_mode::bottom);
		else
		{
			dynamic_cast<TabPage*>(basicConsole.Get())->MoveBorder(diff.y, tab_page_move_mode::bottom);
			if (yDiff != diff.y)
			{
				float newDiff = yDiff - diff.y;
				dynamic_cast<TabPage*>(body.Get())->MoveBorder(newDiff, tab_page_move_mode::bottom);
				curRect = basicConsole.Get()->GetArea();
				curRect.bottom += newDiff;
				curRect.top += newDiff;
				basicConsole.Get()->OnResize(curRect, 0, cred);
				diff.y = yDiff;
			}
		}


		// Now do the left side
		if (!dynamic_cast<TabPage*>(lowerLeft.Get())->IsDrawing())
			dynamic_cast<TabPage*>(upperLeft.Get())->MoveBorder(diff.y, tab_page_move_mode::bottom);
		else
		{
			dynamic_cast<TabPage*>(lowerLeft.Get())->MoveBorder(diff.y, tab_page_move_mode::bottom);
			if (yDiff != diff.y)
			{
				float newDiff = yDiff - diff.y;
				dynamic_cast<TabPage*>(upperLeft.Get())->MoveBorder(newDiff, tab_page_move_mode::bottom);
				curRect = basicConsole.Get()->GetArea();
				curRect.bottom += newDiff;
				curRect.top += newDiff;
				basicConsole.Get()->OnResize(curRect, 0, cred);
				diff.y = yDiff;
			}
		}

		// Finally, the right side
		if (dynamic_cast<TabPage*>(lowerRight.Get())->IsDrawing())
			dynamic_cast<TabPage*>(upperRight.Get())->MoveBorder(diff.y, tab_page_move_mode::bottom);
		else
		{
			dynamic_cast<TabPage*>(lowerRight.Get())->MoveBorder(diff.y, tab_page_move_mode::bottom);
			if (yDiff != diff.y)
			{
				float newDiff = yDiff - diff.y;
				dynamic_cast<TabPage*>(upperRight.Get())->MoveBorder(newDiff, tab_page_move_mode::bottom);
				curRect = basicConsole.Get()->GetArea();
				curRect.bottom += newDiff;
				curRect.top += newDiff;
				basicConsole.Get()->OnResize(curRect, 0, cred);
				diff.y = yDiff;
			}
		}
		MoveBorder(diff.y, moveMode);
	}
	ThreadRelease();
}

/**
 * Method: TabPage::MouseMoveUpperRight
 * Purpose: Send move commands to its neighbors, assuming that this IDE Page is the Upper-Right Page
 * Parameters: TPoint& diff - the amount of units to move by
 * Returns: void
 */
void TabPage::MouseMoveUpperRight(TPoint& diff)
{
	TPoint before = diff;
	float yDiff = 0.0f, xDiff = 0.0f;
	D2D1_RECT_F curRect = { 0,0,0,0 };
	ThreadLock();

	TDataArray<EventID_Cred> cred;
	switch (moveMode)
	{
	case tab_page_move_mode::bottom:
		dynamic_cast<TabPage*>(lowerRight.Get())->MoveBorder(diff.y, tab_page_move_mode::top);
		if (before.y != diff.y)
		{
			// In this case, we went down and tried to push it down. We should still attempt to push the deep console down
			yDiff = before.y - diff.y;

			dynamic_cast<TabPage*>(deepConsole.Get())->MoveBorder(yDiff, tab_page_move_mode::top);
			curRect = basicConsole.Get()->GetArea();
			curRect.bottom += yDiff;
			curRect.top += yDiff;
			basicConsole.Get()->OnResize(curRect, 0, cred);

			// Since the deep Console moved, we need to send the message to the lower side panels
			dynamic_cast<TabPage*>(lowerLeft.Get())->MoveBorder(yDiff, tab_page_move_mode::bottom);
		}
		MoveBorder(diff.y, moveMode);
		break;
	case tab_page_move_mode::left:
		dynamic_cast<TabPage*>(body.Get())->MoveBorder(diff.x, tab_page_move_mode::right);
		dynamic_cast<TabPage*>(basicConsole.Get())->MoveBorder(diff.x, tab_page_move_mode::right);
		if (before.x != diff.x)
		{
			// In this case, we went down and tried to push it down. We should still attempt to push the left panels left
			yDiff = before.x - diff.x;

			dynamic_cast<TabPage*>(upperLeft.Get())->MoveBorder(yDiff, tab_page_move_mode::right);
			curRect = body.Get()->GetArea();
			curRect.left += yDiff;
			curRect.right += yDiff;
			body.Get()->OnResize(curRect, 0, cred);

			dynamic_cast<TabPage*>(lowerLeft.Get())->MoveBorder(yDiff, tab_page_move_mode::right);
			curRect = basicConsole.Get()->GetArea();
			curRect.left += yDiff;
			curRect.right += yDiff;
			basicConsole.Get()->OnResize(curRect, 0, cred);

			// Since the deep Console moved, we need to send the message to the lower side panels
			//dynamic_cast<TabPage*>(lowerLeft.Get())->MoveBorder(yDiff, tab_page_move_mode::bottom);
		}
		dynamic_cast<TabPage*>(lowerRight.Get())->MoveBorder(diff.x, tab_page_move_mode::left);
		MoveBorder(diff.x, moveMode);
		break;
	}
	ThreadRelease();
}

/**
 * Method: TabPage::MouseMoveLowerRight
 * Purpose: Send move commands to its neighbors, assuming that this IDE Page is the Lower-Right Page
 * Parameters: TPoint& diff - the amount of units to move by
 * Returns: void
 */
void TabPage::MouseMoveLowerRight(TPoint& diff)
{
	ThreadLock();
	TPoint before = diff;
	float yDiff = 0.0f, xDiff = 0.0f;
	D2D1_RECT_F curRect = { 0,0,0,0 };

	TDataArray<EventID_Cred> cred;
	switch (moveMode)
	{
	case tab_page_move_mode::top:
		dynamic_cast<TabPage*>(upperRight.Get())->MoveBorder(diff.y, tab_page_move_mode::bottom);
		MoveBorder(diff.y, moveMode);
		break;
	case tab_page_move_mode::left:
		dynamic_cast<TabPage*>(body.Get())->MoveBorder(diff.x, tab_page_move_mode::right);
		dynamic_cast<TabPage*>(basicConsole.Get())->MoveBorder(diff.x, tab_page_move_mode::right);
		if (before.x != diff.x)
		{
			// In this case, we went down and tried to push it down. We should still attempt to push the left panels left
			yDiff = before.x - diff.x;

			dynamic_cast<TabPage*>(upperLeft.Get())->MoveBorder(yDiff, tab_page_move_mode::right);
			curRect = body.Get()->GetArea();
			curRect.left += yDiff;
			curRect.right += yDiff;
			body.Get()->OnResize(curRect, 0, cred);

			dynamic_cast<TabPage*>(lowerLeft.Get())->MoveBorder(yDiff, tab_page_move_mode::right);
			curRect = basicConsole.Get()->GetArea();
			curRect.left += yDiff;
			curRect.right += yDiff;
			basicConsole.Get()->OnResize(curRect, 0, cred);

			// Since the deep Console moved, we need to send the message to the lower side panels
			//dynamic_cast<TabPage*>(lowerLeft.Get())->MoveBorder(yDiff, tab_page_move_mode::bottom);
		}
		dynamic_cast<TabPage*>(upperRight.Get())->MoveBorder(diff.x, tab_page_move_mode::left);
		MoveBorder(diff.x, moveMode);
		break;
	case tab_page_move_mode::bottom:
		dynamic_cast<TabPage*>(deepConsole.Get())->MoveBorder(diff.y, tab_page_move_mode::top);
		dynamic_cast<TabPage*>(basicConsole.Get())->MoveBorder(diff.y, tab_page_move_mode::bottom);
		dynamic_cast<TabPage*>(lowerLeft.Get())->MoveBorder(diff.y, tab_page_move_mode::bottom);
		MoveBorder(diff.y, moveMode);
		break;
	}
	ThreadRelease();
}

/**
 * Method: TabPage::MouseMoveUpperLeft
 * Purpose: Send move commands to its neighbors, assuming that this IDE Page is the Upper-Left Page
 * Parameters: TPoint& diff - the amount of units to move by
 * Returns: void
 */
void TabPage::MouseMoveUpperLeft(TPoint& diff)
{
	TPoint before = diff;
	float yDiff = 0.0f, xDiff = 0.0f;
	D2D1_RECT_F curRect = { 0,0,0,0 };
	ThreadLock();

	TDataArray<EventID_Cred> cred;
	switch (moveMode)
	{
	case tab_page_move_mode::bottom:
		dynamic_cast<TabPage*>(lowerLeft.Get())->MoveBorder(diff.y, tab_page_move_mode::top);
		if (before.y != diff.y)
		{
			// In this case, we went down and tried to push it down. We should still attempt to push the deep console down
			yDiff = before.y - diff.y;

			dynamic_cast<TabPage*>(deepConsole.Get())->MoveBorder(yDiff, tab_page_move_mode::top);
			curRect = basicConsole.Get()->GetArea();
			curRect.bottom += yDiff;
			curRect.top += yDiff;
			basicConsole.Get()->OnResize(curRect, 0, cred);

			// Since the deep Console moved, we need to send the message to the lower side panels
			dynamic_cast<TabPage*>(lowerRight.Get())->MoveBorder(yDiff, tab_page_move_mode::bottom);
		}
		MoveBorder(diff.y, moveMode);
		break;
	case tab_page_move_mode::left:
		dynamic_cast<TabPage*>(body.Get())->MoveBorder(diff.x, tab_page_move_mode::left);
		dynamic_cast<TabPage*>(basicConsole.Get())->MoveBorder(diff.x, tab_page_move_mode::left);
		if (before.x != diff.x)
		{
			// In this case, we went down and tried to push it down. We should still attempt to push the left panels left
			yDiff = before.x - diff.x;

			dynamic_cast<TabPage*>(upperRight.Get())->MoveBorder(yDiff, tab_page_move_mode::left);
			curRect = body.Get()->GetArea();
			curRect.right += yDiff;
			curRect.left += yDiff;
			body.Get()->OnResize(curRect, 0, cred);

			dynamic_cast<TabPage*>(lowerRight.Get())->MoveBorder(yDiff, tab_page_move_mode::left);
			curRect = basicConsole.Get()->GetArea();
			curRect.right += yDiff;
			curRect.left += yDiff;
			basicConsole.Get()->OnResize(curRect, 0, cred);

			// Since the deep Console moved, we need to send the message to the lower side panels
			//dynamic_cast<TabPage*>(lowerLeft.Get())->MoveBorder(yDiff, tab_page_move_mode::bottom);
		}
		dynamic_cast<TabPage*>(lowerLeft.Get())->MoveBorder(diff.x, tab_page_move_mode::right);
		MoveBorder(diff.x, moveMode);
		break;
	}
	ThreadRelease();
}

/**
 * Method: TabPage::MouseMoveLowerLeft
 * Purpose: Send move commands to its neighbors, assuming that this IDE Page is the Lower-Left Page
 * Parameters: TPoint& diff - the amount of units to move by
 * Returns: void
 */
void TabPage::MouseMoveLowerLeft(TPoint& diff)
{
	TPoint before = diff;
	float yDiff = 0.0f, xDiff = 0.0f;
	D2D1_RECT_F curRect = { 0,0,0,0 };
	ThreadLock();

	TDataArray<EventID_Cred> cred;
	switch (moveMode)
	{
	case tab_page_move_mode::top:
		dynamic_cast<TabPage*>(upperLeft.Get())->MoveBorder(diff.y, tab_page_move_mode::bottom);
		MoveBorder(diff.y, moveMode);
		break;
	case tab_page_move_mode::right:
		dynamic_cast<TabPage*>(body.Get())->MoveBorder(diff.x, tab_page_move_mode::left);
		dynamic_cast<TabPage*>(basicConsole.Get())->MoveBorder(diff.x, tab_page_move_mode::left);
		if (before.x != diff.x)
		{
			// In this case, we went down and tried to push it down. We should still attempt to push the left panels left
			yDiff = before.x - diff.x;

			dynamic_cast<TabPage*>(upperRight.Get())->MoveBorder(yDiff, tab_page_move_mode::left);
			curRect = body.Get()->GetArea();
			curRect.left += yDiff;
			curRect.right += yDiff;
			body.Get()->OnResize(curRect, 0, cred);

			dynamic_cast<TabPage*>(lowerRight.Get())->MoveBorder(yDiff, tab_page_move_mode::left);
			curRect = basicConsole.Get()->GetArea();
			curRect.right += yDiff;
			curRect.left += yDiff;
			basicConsole.Get()->OnResize(curRect, 0, cred);

			// Since the deep Console moved, we need to send the message to the lower side panels
			//dynamic_cast<TabPage*>(lowerLeft.Get())->MoveBorder(yDiff, tab_page_move_mode::bottom);
		}
		dynamic_cast<TabPage*>(upperLeft.Get())->MoveBorder(diff.x, tab_page_move_mode::left);
		MoveBorder(diff.x, moveMode);
		break;
	case tab_page_move_mode::bottom:
		dynamic_cast<TabPage*>(deepConsole.Get())->MoveBorder(diff.y, tab_page_move_mode::top);
		dynamic_cast<TabPage*>(basicConsole.Get())->MoveBorder(diff.y, tab_page_move_mode::bottom);
		dynamic_cast<TabPage*>(lowerRight.Get())->MoveBorder(diff.y, tab_page_move_mode::bottom);
		MoveBorder(diff.y, moveMode);
		break;
	}
	ThreadRelease();
}


/**
 * Method: TabPage::MoveBorder
 * Purpose: Shofts the border when called upon by a neighboring page
 * Parameters: float& magnitude - the amount to shift the border by
 *				page_move_mode mode - which border to shift
 * Returns: void
 */
void TabPage::MoveBorder(float& magnitude, tab_page_move_mode mode)
{
	ThreadLock();
	if (IsSnipZero(area))
	{
		ThreadRelease();
		return;
	}

	auto newArea = area;

	switch (mode)
	{
	case tab_page_move_mode::bottom:
		newArea.bottom += magnitude;
		if (newArea.bottom < newArea.top)
		{
			magnitude -= (newArea.top - newArea.bottom);
			newArea.bottom = newArea.top;
		}
		break;
	case tab_page_move_mode::left:
		newArea.left += magnitude;
		if (newArea.right < newArea.left)
		{
			magnitude -= (newArea.right - newArea.left);
			newArea.left = newArea.right;
		}
		break;
	case tab_page_move_mode::right:
		newArea.right += magnitude;
		if (newArea.right < newArea.left)
		{
			magnitude -= (newArea.right - newArea.left);
			newArea.right = newArea.left;
		}
		break;
	case tab_page_move_mode::top:
		newArea.top += magnitude;
		if (newArea.bottom < newArea.top)
		{
			magnitude -= (newArea.top - newArea.bottom);
			newArea.top = newArea.bottom;
		}
	}

	TDataArray<EventID_Cred> cred;
	OnResize(newArea, 0, cred);
	ThreadRelease();
}

/**
 * Method: TabPage::SetLink
 * Purpose: Allows the IDE Window to link up it's pages so that they can directly communicate with each other
 * Parameters: TrecSubPointer<Page, IDEPage> p - The Page to link to
 *				ide_page_type t - the Specific purpose of the Page
 * Returns: void
 */
void TabPage::SetLink(TrecSubPointer<TPage, TabPage> p, ide_page_type t)
{
	ThreadLock();
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
	ThreadRelease();
}