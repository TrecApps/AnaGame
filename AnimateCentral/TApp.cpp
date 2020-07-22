#include "stdafx.h"
#include "TApp.h"
#include "TDllApp.h"

TApp::TApp()
{
}

TApp::~TApp()
{
}

TApp* TApp::Initialize(TFile& file)
{
	if(!file.IsOpen())
		return nullptr;

	TString line;

	TMap<TString> properties;

	while (file.ReadString(line))
	{
		auto pair = line.split(TString(L"="));
		if (pair->Size() == 2)
		{
			TString prop = pair->at(0);
			TrecPointer<TString> value = TrecPointerKey::GetNewTrecPointer<TString>(pair->at(1));
			prop.Trim();
			value->Trim();
			properties.addEntry(prop, TrecPointer<TString>(value));
		}
	}

	TString* appType = properties.retrieveEntry(TString(L"AppType")).Get();

	TDllApp* app = nullptr;
	if (!appType)
		return nullptr;
	if (!appType->Compare(L"DLL_App"))
	{
		app = new TDllApp();
	}

	

	if (app && !app->Initialize(properties,file.GetFileDirectory()))
	{
		delete app;
		app = nullptr;
	}
	return app;
}
