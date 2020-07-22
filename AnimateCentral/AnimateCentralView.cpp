
// AnimateCentralView.cpp : implementation of the CAnimateCentralView class
//

#include "stdafx.h"

#include <TScrollBar.h> // Accelerated access to scrollBar functions

// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "AnimateCentral.h"
#endif

#include "AnimateCentralDoc.h"
#include "AnimateCentralView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAnimateCentralView

IMPLEMENT_DYNCREATE(CAnimateCentralView, CView)

BEGIN_MESSAGE_MAP(CAnimateCentralView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()	
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CAnimateCentralView construction/destruction

CAnimateCentralView::CAnimateCentralView()
{
	// TODO: add construction code here
	firstDraw = true;
}

CAnimateCentralView::~CAnimateCentralView()
{
}

BOOL CAnimateCentralView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CAnimateCentralView drawing

void CAnimateCentralView::OnDraw(CDC* pDC)
{
	CAnimateCentralDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;



	// TODO: add draw code for native data here
//	if (firstDraw)
//	{
		pDoc->setUpTControl(pDC);
//		firstDraw = false;
//	}
	pDoc->onDraw();

}

void CAnimateCentralView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	/*
	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetSizes(MM_TEXT, sizeTotal);
*/

}

void CAnimateCentralView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	//ClientToScreen(&point);
	OnContextMenu(this, point);
	//TControl* messageControl = thisDoc->rootControl;
}

void CAnimateCentralView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CAnimateCentralDoc* thisDoc = GetDocument();
	TrecPointer<TControl> messageControl = thisDoc->rootControl;


	messageOutput mo = negative;

	ScrollLButtonDown(nFlags, point, &mo);
	if (mo != negative)
	{
		doUpdate(&mo);
		return;
	}





	//ClientToScreen(&point);
	mo = negative;
	if (messageControl.Get())
		messageControl->OnLButtonDown(nFlags, point, &mo,cred);

	ResetComboBoxes();
	doUpdate(&mo);
}

void CAnimateCentralView::OnRButtonDown(UINT nFlags, CPoint point)
{
	CAnimateCentralDoc* thisDoc = GetDocument();
	thisDoc->SetRightClick(point);
	TrecPointer<TControl> messageControl = thisDoc->rootControl;
	//ClientToScreen(&point);
	messageOutput mo = positiveContinue;
	if (messageControl.Get())
		messageControl->OnRButtonDown(nFlags, point, &mo,cred);
	doUpdate(&mo);
}

void CAnimateCentralView::OnMouseMove(UINT nFlags, CPoint point)
{
	CAnimateCentralDoc* thisDoc = GetDocument();
	TrecPointer<TControl> messageControl = thisDoc->rootControl;


	messageOutput mo = negative;

	ScrollMouseMove(nFlags, point, &mo);
	if (mo != negative)
	{	
		doUpdate(&mo);
		return;
	}

	TFlyout* fly = TFlyout::DoDraw();
	if (fly)
	{
		fly->OnMouseMove(nFlags, point, &mo, cred);
	}

	if (messageControl.Get())
		messageControl->OnMouseMove(nFlags, point, &mo,cred);
	doUpdate(&mo);
}

void CAnimateCentralView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
//	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

void CAnimateCentralView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CAnimateCentralDoc* thisDoc = GetDocument();
	TrecPointer<TControl> messageControl = thisDoc->rootControl;
	//ClientToScreen(&point);
	messageOutput mo = positiveContinue;
	if (messageControl.Get())
		messageControl->OnLButtonDblClk(nFlags, point, &mo,cred);
	doUpdate(&mo);
}

void CAnimateCentralView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CAnimateCentralDoc* thisDoc = GetDocument();
	TrecPointer<TControl> messageControl = thisDoc->rootControl;
	//ClientToScreen(&point);
	messageOutput mo = negative;

	ScrollLButtonUp(nFlags, point, &mo);
	if (mo != negative)
	{
		doUpdate(&mo);
		return;
	}
	mo = positiveContinue;
	TFlyout* fly = TFlyout::DoDraw();
	if (fly)
	{
		fly->OnLButtonUp(nFlags, point, &mo, cred);
		if (mo == negative || mo == negativeUpdate)
		{
			fly->Hide();
			thisDoc->SetRightClick(CPoint(0, 0));

			
		}
	}

	if (messageControl.Get())
		messageControl->OnLButtonUp(nFlags, point, &mo,cred);
	doUpdate(&mo);

}

void CAnimateCentralView::doUpdate(messageOutput *mo)
{
	if( *mo == positiveContinueUpdate || *mo == positiveOverrideUpdate || *mo == negativeUpdate)
		GetDocument()->UpdateAllViews(NULL);
}

void CAnimateCentralView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CAnimateCentralDoc* thisDoc = GetDocument();
	TrecPointer<TControl> messageControl = thisDoc->rootControl;
	//ClientToScreen(&point);
	messageOutput mo = positiveContinue;
	if (messageControl.Get())
		messageControl->OnChar(true, nChar,nRepCnt,nFlags,&mo,cred);
	doUpdate(&mo);
}

void CAnimateCentralView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case VK_LEFT:
	case VK_RIGHT:
	case VK_ESCAPE:
	case VK_HOME:
	case VK_RETURN:
	case VK_END:
		CAnimateCentralDoc* thisDoc = GetDocument();
		TrecPointer<TControl> messageControl = thisDoc->rootControl;
		//ClientToScreen(&point);
		messageOutput mo = positiveContinue;
		if (messageControl.Get())
			messageControl->OnChar(false, nChar, nRepCnt, nFlags, &mo,cred);
		doUpdate(&mo);
	}
}

// CAnimateCentralView diagnostics

#ifdef _DEBUG
void CAnimateCentralView::AssertValid() const
{
	CView::AssertValid();
}

void CAnimateCentralView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAnimateCentralDoc* CAnimateCentralView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAnimateCentralDoc)));
	return (CAnimateCentralDoc*)m_pDocument;
}
#endif //_DEBUG


// CAnimateCentralView message handlers
