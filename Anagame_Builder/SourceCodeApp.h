#pragma once
#include "BuilderApp.h"

class SourceCodeApp;

typedef void (SourceCodeApp::* SCBuilderEvents)(TControl* tc, EventArgs ea);

class SourceCodeApp :
	public BuilderApp
{
public:
	SourceCodeApp(TrecPointer<TControl> m, TrecPointer<TControl>o, TrecPointer<TControl> e, TrecPointer<TInstance> r);
	~SourceCodeApp();

	bool InitializeControls() override;

	void SetContents(TFile& content);
	virtual void OnSave() override;

	virtual void OnShow() override;
	virtual void onHide() override;

	UINT ProcessIntLanguage();

protected:
	void PrepLanguage(TFile& sourceCode);


	// Interpretors, incase code needs to be run or compiled
	// TInterpretor* interpret;
	TTextField* lines;
	TTextField* code;

	D2D1_COLOR_F col_keyword, col_type_id, col_var_id, col_fun_id;
	TDataArray<SCBuilderEvents> sc_events;
};

