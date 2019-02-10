
// AnaGame_BuilderDoc.h : interface of the CAnaGameBuilderDoc class
//
#include <Page.h>
#include <AnafaceParser.h>
#include "Solution.h"
#include "BuilderApp.h"

#define ANAGAME_IDE_METHOD_COUNT 10

#pragma once

class CAnaGameBuilderDoc;

typedef void (CAnaGameBuilderDoc::*IDEMethods)(TControl* tc, EventArgs ea);

class CAnaGameBuilderDoc : public CDocument, public Page
{
	friend class CAnaGameBuilderView;
protected: // create from serialization only
	CAnaGameBuilderDoc();
	DECLARE_DYNCREATE(CAnaGameBuilderDoc)

// Attributes
public:

// Operations
public:
	//void SetRightClick(CPoint p);
// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CAnaGameBuilderDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	void OnDraw(CDC* pDC);

	void ShowLanguagePanel(bool show);

protected:
	void InitializeBuilderView(CDC* pDC);
	TrecPointer<TControl> rootControl;
	bool init;
	TString fileAnaface;
	TDataArray<eventNameID> idMatch;

	D2D1::ColorF backColor;

	TrecPointer<Solution> currentSolution;

	TrecPointer<AnafaceUI> ribbon;
	TrecPointer<AnafaceUI> body;
	TrecPointer<AnafaceUI> classUI;
	TrecPointer<AnafaceUI> outputPanel;

	// The first Ribbon Panel
	TrecPointer<TLayout> ribbon1;
	TrecPointer<TTextField> solutionName;

	// The Fourth Ribbon Panel (Arena)
	TrecPointer<TLayout> ribbon4;
	TrecPointer<TLayout> arenaStack1, arenaStack3;

	// The Fifth Ribbon Panel (Document/Source)
	TrecPointer<TLayout> ribbon5;
	TrecPointer<TLayout> docStack2;

	TArray<BuilderApp> ActiveDocuments;
	TrecPointer<BuilderApp> currentDocument;


	void InitilaizeControls();

	void setCurrentDocument(BuilderApp* ba);
	void setCurrentDocument(TrecPointer<BuilderApp> ba);

// Generated message map functions
protected:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point, messageOutput& mo);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point, messageOutput& mo);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint, messageOutput& mo);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point, messageOutput& mo);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point, messageOutput& mo);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point, messageOutput& mo);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput& mo);
	afx_msg void OnSize(UINT nType,	int cx,	int cy);
	TDataArray<EventID_Cred> cred;
	void MessageHandler();

	// Action Listeners working at the AnaGame level
	IDEMethods ideListeners[ANAGAME_IDE_METHOD_COUNT];

	// Handlers for Home Tab
	void OnNewProject(TControl* tc, EventArgs ea);
	void OnNewFile(TControl* tc, EventArgs ea);

	// Handlers for the Arena Tab
	void OnNewArena(TControl* tc, EventArgs ea);
	void OnUpdateClearColor(TControl* tc, EventArgs ea);
	void OnNewModel(TControl* tc, EventArgs);

	// Handlers for the Document (Source Code) Tab
	void OnNewCodeFile(TControl* tc, EventArgs ea);
	void OnImportCodeFile(TControl* tc, EventArgs ea);
	void OnProcessCode(TControl* tc, EventArgs ea);

	// Base Handlers
	void OnSaveAll(TControl* tc, EventArgs ea);
	void OnSave(TControl* tc, EventArgs ea);
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
