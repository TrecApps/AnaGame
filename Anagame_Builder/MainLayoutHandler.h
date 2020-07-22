#pragma once
#include <EventHandler.h>
#include <MiniHandler.h>
#include <TEnvironment.h>

class MainLayoutHandler;

typedef void (MainLayoutHandler::* LayoutCall)(TrecPointer<TControl> tc, EventArgs ea);

class MainLayoutHandler : public EventHandler
{
public:
	MainLayoutHandler(TrecPointer<TInstance> ins);
	~MainLayoutHandler();

	afx_msg void OnRButtonUp(UINT nFlags, TPoint point, messageOutput* mOut) override;
	afx_msg void OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut) override;
	afx_msg void OnRButtonDown(UINT nFlags, TPoint, messageOutput* mOut) override;
	afx_msg void OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut) override;
	afx_msg void OnLButtonDblClk(UINT nFlags, TPoint point, messageOutput* mOut) override;
	afx_msg void OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut) override;
	afx_msg bool OnChar(bool fromChar, UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput* mOut) override;

	void Initialize(TrecPointer<Page> page)override;
	void HandleEvents(TDataArray<EventID_Cred>& eventAr);

	void Draw();
	void OnSwitchTab(TrecPointer<TControl> tc, EventArgs ea);

	virtual void ProcessMessage(TrecPointer<HandlerMessage> message)override;
protected:
	TDataArray<LayoutCall> calls;

	// Pointers to frequently access controls in the main layout
	TrecPointer<TControl> rootControl;
	bool init;
	TString fileAnaface;
	TDataArray<eventNameID> idMatch;

	D2D1_COLOR_F backColor;

	TrecPointer<TControl> ribbon;					// AnafaceUI
	TrecPointer<TControl> body;						// AnafaceUI
	TrecPointer<TControl> classUI;					// AnafaceUI
	TrecPointer<TControl> outputPanel;				// AnafaceUI

	// The first Ribbon Panel
	TrecPointer<TControl> ribbon1;					// TLayout
	TrecPointer<TControl> solutionName;			// TTextField

	// The Fourth Ribbon Panel (Arena)
	TrecPointer<TControl> ribbon4;					// TLayout
	TrecPointer<TControl> arenaStack1, arenaStack3;	// TLayout

	// The Fifth Ribbon Panel (Document/Source)
	TrecPointer<TControl> ribbon5;					// TLayout
	TrecPointer<TControl> docStack2;					// TLayout

	
	//TrecPointer<BuilderApp> currentDocument;

	// Found on the Home Tab
	void OnLoadNewSolution(TrecPointer<TControl> tc, EventArgs ea);
	void OnSaveFile(TrecPointer<TControl> tc, EventArgs ea);
	void OnSaveAllFiles(TrecPointer<TControl> tc, EventArgs ea);
	void OnNewFile(TrecPointer<TControl> tc, EventArgs ea);
	void OnImportFile(TrecPointer<TControl> tc, EventArgs ea);
	void OnPrint(TrecPointer<TControl> tc, EventArgs ea);

	// Found on the Arena Tab
	void OnNewArena(TrecPointer<TControl> tc, EventArgs ea);
	void OnUpdateClearColor(TrecPointer<TControl> tc, EventArgs ea);
	void OnNewModel(TrecPointer<TControl> tc, EventArgs ea);

	// Found on the Document (Source Code) Tab
	void OnNewCodeFile(TrecPointer<TControl> tc, EventArgs ea);
	void OnImportCode(TrecPointer<TControl> tc, EventArgs ea);
	void OnProcessCode(TrecPointer<TControl> tc, EventArgs ea);

	TrecPointer<TEnvironment> environment;


	/// Now to host the various mini apps that the user g=could launch on the builder
	TDataArray<TrecSubPointer<EventHandler, MiniHandler>> ActiveDocuments;
	TrecSubPointer<EventHandler, MiniHandler> currentDocument;


	virtual bool ShouldProcessMessageByType(TrecPointer<HandlerMessage> message) override;
};

