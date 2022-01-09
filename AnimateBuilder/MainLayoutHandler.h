#pragma once
#include <EventHandler.h>
#include <MiniApp.h>
#include <TTextInput.h>

class MainLayoutHandler;

typedef void (MainLayoutHandler::* LayoutCall)(TrecPointer<TPage> tc, EventArgs ea);

class MainLayoutHandler : public TapEventHandler
{
public:
	MainLayoutHandler(TrecPointer<TProcess> ins);
	~MainLayoutHandler();


	void Initialize(TrecPointer<TPage> page)override;
	void HandleEvents(TDataArray<TPage::EventID_Cred>& eventAr);

	void Draw();
	void OnSwitchTab(TrecPointer<TPage> tc, EventArgs ea);


	virtual void ProcessMessage(TrecPointer<HandlerMessage> message)override;


	virtual void OnFirstDraw()override;

protected:
	TDataArray<LayoutCall> calls;

	// Pointers to frequently access controls in the main layout
	TrecPointer<TPage> rootControl;
	bool init;
	TString fileAnaface;
	TDataMap<UINT> events;

	D2D1_COLOR_F backColor;

	TrecSubPointer<TPage, TTextInput> input;

	TrecPointer<TControl> ribbon;					// AnafaceUI

	// The first Ribbon Panel
	TrecPointer<TControl> ribbon1;					// TLayout
	TrecPointer<TControl> solutionName;			// TTextField

	// The Fourth Ribbon Panel (Arena)
	TrecPointer<TControl> ribbon4;					// TLayout
	TrecPointer<TControl> arenaStack1, arenaStack3;	// TLayout

	// The Fifth Ribbon Panel (Document/Source)
	TrecPointer<TControl> ribbon5;					// TLayout
	TrecPointer<TControl> docStack2;					// TLayout

	// New TPage Resources
	TrecPointer<TEnvironment> environment;

	
	//TrecPointer<BuilderApp> currentDocument;

	// Found on the Home Tab
	void OnLoadNewSolution(TrecPointer<TPage> tc, EventArgs ea);
	void OnSaveFile(TrecPointer<TPage> tc, EventArgs ea);
	void OnSaveAllFiles(TrecPointer<TPage> tc, EventArgs ea);
	void OnNewResource(TrecPointer<TPage> tc, EventArgs ea);
	void OnImportFile(TrecPointer<TPage> tc, EventArgs ea);
	void OnPrint(TrecPointer<TPage> tc, EventArgs ea);

	// Found on the Arena Tab
	void OnNewArena(TrecPointer<TPage> tc, EventArgs ea);
	void OnUpdateClearColor(TrecPointer<TPage> tc, EventArgs ea);
	void OnNewModel(TrecPointer<TPage> tc, EventArgs ea);

	// Found on the Document (Source Code) Tab
	void OnNewCodeFile(TrecPointer<TPage> tc, EventArgs ea);
	void OnImportCode(TrecPointer<TPage> tc, EventArgs ea);
	void OnProcessCode(TrecPointer<TPage> tc, EventArgs ea);


	/// Now to host the various mini apps that the user g=could launch on the builder
	TDataArray<TrecPointer<MiniApp>> ActiveDocuments;
	TrecPointer<MiniApp> currentDocument;

	TrecSubPointer<TWindow, TIdeWindow> ideWindow;

	virtual bool ShouldProcessMessageByType(TrecPointer<HandlerMessage> message) override;
};

