#pragma once
#include <MiniHandler.h>
#include <TEnvironment.h>
class SourceCodeApp;

typedef void (SourceCodeApp::* SCBuilderEvents)(TControl* tc, EventArgs ea);

class SourceCodeApp :
	public MiniHandler
{
public:
	SourceCodeApp(TrecPointer<TControl> m, TrecPointer<TControl>o, TrecPointer<TControl> e, TrecPointer<TInstance> r);
	~SourceCodeApp();

	bool InitializeControls() override;

	void SetContents(TFile& content);
	//virtual void OnSave() override;

	virtual void OnShow() override;
	virtual void onHide() override;

	UINT ProcessIntLanguage();
	void HandleEvents(TDataArray<EventID_Cred>&)override;

	void ProcessFile(TrecPointer<TEnvironment> env);

protected:
	void PrepLanguage(TFile& sourceCode);

	void OnSave(TFile& file)override;
	void OnLoad(TFile& file)override;

	// Interpretors, incase code needs to be run or compiled
	// TInterpretor* interpret;
	TTextField* lines;
	TTextField* code;

	D2D1_COLOR_F col_keyword, col_type_id, col_var_id, col_fun_id;
	TDataArray<SCBuilderEvents> sc_events;
};

