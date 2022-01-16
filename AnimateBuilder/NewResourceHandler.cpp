#include "NewResourceHandler.h"
#include <TGrid.h>
#include <TStringVariable.h>
#include <AnafacePage.h>

NewResourceHandler::NewResourceHandler(TrecPointer<TProcess> instance): TapEventHandler(instance)
{
}

NewResourceHandler::~NewResourceHandler()
{
}

TString NewResourceHandler::GetType()
{
	return TString();
}

void NewResourceHandler::Initialize(TrecPointer<TPage> page)
{
	TrecSubPointer<TPage, AnafacePage> aPage = TrecPointerKey::GetTrecSubPointerFromTrec<TPage, AnafacePage>(page);
	TrecSubPointer<TPage, TGrid> rootGrid = TrecPointerKey::GetTrecSubPointerFromTrec<TPage, TGrid>(aPage->GetRootControl());
	this->envComboBox = TrecPointerKey::GetTrecSubPointerFromTrec<TPage, TCombobox>( rootGrid->GetPage(1, 1));
	SetUpEnvironmentInCombobox();
}

void NewResourceHandler::HandleEvents(TDataArray<TPage::EventID_Cred>& eventAr)
{
}

void NewResourceHandler::ProcessMessage(TrecPointer<HandlerMessage> message)
{
}

void NewResourceHandler::SetWindow(TrecPointer<TWindow> window)
{
	TapEventHandler::SetWindow(window);
	SetUpEnvironmentInCombobox();
}

void NewResourceHandler::OnOkay(TControl* control, EventArgs ea)
{
}

void NewResourceHandler::SetUpEnvironmentInCombobox()
{

	if (window.Get() && envComboBox.Get())
	{
		TrecSubPointer<TEnvironment, TPageEnvironment> pEnv = 
			TrecPointerKey::GetTrecSubPointerFromTrec<TEnvironment, TPageEnvironment>(window->GetEnvironment());

		if (!pEnv.Get()) return;

		pEnv->GetPageList(L"", entries);

		auto varList = envComboBox->GetVariableList();
		for (UINT Rust = 0; Rust < entries.Size(); Rust++)
		{
			varList->AppendValue(TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(entries[Rust]));
		}
	}
}

bool NewResourceHandler::ShouldProcessMessageByType(TrecPointer<HandlerMessage> message)
{
	return false;
}
