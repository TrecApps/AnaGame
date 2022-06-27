#include "NewResourceHandler.h"
#include <TGrid.h>
#include <TStringVariable.h>
#include <AnafacePage.h>



NewResourceHandler::NewResourceHandler(TrecPointer<TProcess> instance): TapEventHandler(instance)
{
	ready = onDestroy = false;
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
	this->page = page;
	TrecSubPointer<TPage, AnafacePage> aPage = TrecPointerKey::GetTrecSubPointerFromTrec<TPage, AnafacePage>(page);
	TrecSubPointer<TPage, TGrid> rootGrid = TrecPointerKey::GetTrecSubPointerFromTrec<TPage, TGrid>(aPage->GetRootControl());
	this->envComboBox = TrecPointerKey::GetTrecSubPointerFromTrec<TPage, TCombobox>( rootGrid->GetPage(1, 1));
	this->nameInput = TrecPointerKey::GetTrecSubPointerFromTrec<TPage, TTextInput>(rootGrid->GetPage(0, 1));
	SetUpEnvironmentInCombobox();
}

void NewResourceHandler::HandleEvents(TDataArray<TPage::EventID_Cred>& eventAr)
{
	TapEventHandler::HandleEvents(eventAr);

	for (UINT Rust = 0; Rust < eventAr.Size(); Rust++)
	{
		if (eventAr[Rust].args.Get())
		{
			TString call(eventAr[Rust].args->methodID);
			if (!call.Compare(L"OnOkay"))
				OnOkay(eventAr[Rust].control, *eventAr[Rust].args.Get());
			else if (!call.Compare(L"OnCancel"))
				OnCancel(eventAr[Rust].control, *eventAr[Rust].args.Get());
		}
	}
}

void NewResourceHandler::ProcessMessage(TrecPointer<HandlerMessage> message)
{
}

void NewResourceHandler::SetWindow(TrecPointer<TWindow> window)
{
	TapEventHandler::SetWindow(window);
	SetUpEnvironmentInCombobox();
}

bool NewResourceHandler::IsReady()
{
	return ready;
}

bool NewResourceHandler::GetSpecs(TString& name, TString& type)
{
	if(!ready || !envComboBox.Get() || !nameInput.Get())
		return false;

	auto boxVar = envComboBox->GetCurrentVariable();

	type.Set(boxVar.Get() ? boxVar->GetString() : L"");
	name.Set(nameInput->GetText());
	return true;
}

void NewResourceHandler::OnOkay(TrecPointer<TPage> tc, EventArgs ea)
{
	TObjectLocker lock(&thread);
	ready = true;
	OnCancel(tc, ea);
}

void NewResourceHandler::OnCancel(TrecPointer<TPage> tc, EventArgs ea)
{
	TObjectLocker lock(&thread);
	if (onDestroy)
		return;
	onDestroy = true;
	if (!page.Get())
	{
		ThreadRelease();
		throw L"Error! Handler expected pointer to a page!";
	}
	if (!window.Get())
	{
		ThreadRelease();
		throw L"Error! Handler's Page Object returned a NULL window handle";
	}
	DestroyWindow(window->GetWindowHandle());
}

void NewResourceHandler::SetUpEnvironmentInCombobox()
{

	if (window.Get() && envComboBox.Get())
	{
		TrecSubPointer<TEnvironment, TPageEnvironment> pEnv = 
			TrecPointerKey::GetTrecSubPointerFromTrec<TEnvironment, TPageEnvironment>(window->GetEnvironment());

		if (!pEnv.Get()) return;

		pEnv->GetPageList(TPageEnvironment::handler_type::ht_file, L"", entries);

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
