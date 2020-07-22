
// AnimateCentralDoc.h : interface of the CAnimateCentralDoc class
//


#pragma once
#include <AnafaceUI.h>

class _declspec(dllimport) TControl;

typedef struct Tab
{
	TrecPointer<TControl> page;
	TString title;

}Tab;

class CAnimateCentralDoc : public CDocument
{
protected: // create from serialization only
	CAnimateCentralDoc();
	DECLARE_DYNCREATE(CAnimateCentralDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	void setView(CView*);

#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CAnimateCentralDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	
	void SetRightClick(CPoint p);

	TrecPointer<TControl> rootControl;
	TrecPointer<TArray<styleTable>> table;

	void setUpTControl(CDC*);
	void onDraw();
	bool getFirstDraw();
	void setDrawn();
	RECT getLocation();
	TFlyout* drawFlyout;
protected:
	//CDC* contextPointer;
	RECT area;
	CView* attachedView;
	TrecComPointer<ID2D1RenderTarget> rt;
	TrecComPointer<ID2D1Factory> factory;

	HWND windowHandle;
	bool WindowSet;

	bool firstDraw;

	CArray<EventID_Cred> cred;


	CPoint rightClickPoint;

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnSetacontroltest1();

	afx_msg void OnSetuptcontrol1();
	afx_msg void OnSetuptcontrol2();
	afx_msg void OnSetuptcontrol3();

	afx_msg void OnSetuptcontrol4();
	afx_msg void OnSetuptcontrol5();
	afx_msg void OnSetuptcontrol6();
	afx_msg void OnSetuptcontrolLayeredtlayout();
	afx_msg void OnSetuptcontrolLayoutGadget();
	afx_msg void OnSetuptcontrolTextbox();
	afx_msg void OnSetuptcontrolCombobox();
	afx_msg void OnSetuptcontrolScrolltest();
};
