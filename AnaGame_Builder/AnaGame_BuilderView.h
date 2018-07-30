
// AnaGame_BuilderView.h : interface of the CAnaGameBuilderView class
//

#pragma once


class CAnaGameBuilderView : public CView
{
protected: // create from serialization only
	CAnaGameBuilderView();
	DECLARE_DYNCREATE(CAnaGameBuilderView)

// Attributes
public:
	CAnaGameBuilderDoc* GetDocument() const;

// Operations
public:



// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CAnaGameBuilderView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	TDataArray<EventID_Cred> cred;

// Generated message map functions
protected:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSize(UINT nType,	int cx,	int cy);
	DECLARE_MESSAGE_MAP()



	void doUpdate(messageOutput*);
};

#ifndef _DEBUG  // debug version in AnaGame_BuilderView.cpp
inline CAnaGameBuilderDoc* CAnaGameBuilderView::GetDocument() const
   { return reinterpret_cast<CAnaGameBuilderDoc*>(m_pDocument); }
#endif

