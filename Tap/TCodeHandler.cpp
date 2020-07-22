#include "TCodeHandler.h"
#include "Page.h"
#include <AnafaceUI.h>
#include <TScrollerControl.h>
TCodeHandler::TCodeHandler(TrecPointer<TInstance> in) : DocumentHandler(in)
{
}

TCodeHandler::TCodeHandler(TrecPointer<TInstance> in, const TString& name) : DocumentHandler(in, name)
{
}

void TCodeHandler::Initialize(TrecPointer<Page> page)
{
	if (!page.Get())
		return;
	auto control = page->GetRootControl();

	if (!control.Get())
		return;

	if(dynamic_cast<TScrollerControl*>(control.Get()))
		control = dynamic_cast<TScrollerControl*>(control.Get())->GetChildControl();

	TLayout* lay = dynamic_cast<TLayout*>(control.Get());
	if (!lay)
		return;
	lines = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TTextField>(lay->GetLayoutChild(0, 0));
	code = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TTextField>(lay->GetLayoutChild(1, 0));

	this->page = page;
}

void TCodeHandler::HandleEvents(TDataArray<EventID_Cred>& eventAr)
{
}

void TCodeHandler::ProcessMessage(TrecPointer<HandlerMessage> message)
{
}

void TCodeHandler::OnSave()
{
	SetSaveFile();

	if (filePointer.Get() && !filePointer->IsDirectory())
	{
		
		TFile saver(filePointer->GetPath(), TFile::t_file_write | TFile::t_file_create_always);

		OnSave(saver);
	}
}

bool TCodeHandler::ShouldProcessMessageByType(TrecPointer<HandlerMessage> message)
{
	return false;
}

void TCodeHandler::OnSave(TFile& file)
{
	if (!file.IsOpen() || !code.Get()) return;
	file.WriteString(code->GetText());
}

void TCodeHandler::OnLoad(TFile&)
{
}
