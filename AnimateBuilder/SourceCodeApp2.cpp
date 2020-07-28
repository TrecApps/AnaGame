#include "SourceCodeApp2.h"

SourceCodeApp2::SourceCodeApp2(TrecSubPointer<TWindow, TIdeWindow> win): MiniApp(win)
{
}

void SourceCodeApp2::DispatchAnagameMessage(TrecPointer<HandlerMessage> message)
{
}

UINT SourceCodeApp2::Initialize()
{
	if (!win.Get() || !instance.Get())
		return 1;

	codeHandler = TrecPointerKey::GetNewSelfTrecSubPointer<EventHandler, TCodeHandler>(TrecPointerKey::GetTrecPointerFromSoft<TInstance>(instance));

	if (!codeHandler.Get())
		return 2;

	commandHandler = TrecPointerKey::GetNewSelfTrecSubPointer<EventHandler, TerminalHandler>(TrecPointerKey::GetTrecPointerFromSoft<TInstance>(instance));
	if (!commandHandler.Get())
		return 3;

	mainPage = win->AddNewPage(anagame_page::anagame_page_code_file, ide_page_type::ide_page_type_body, TString(L"Code"), TString(), TrecPointerKey::GetTrecPointerFromSub<EventHandler, TCodeHandler>(codeHandler));
	win->AddPage(anagame_page::anagame_page_command_prompt, ide_page_type::ide_page_type_deep_console, TString(L"Build"));
	win->AddPage(anagame_page::anagame_page_command_prompt, ide_page_type::ide_page_type_deep_console, TString(L"Debug"));

	win->AddPage(anagame_page::anagame_page_file_node, ide_page_type::ide_page_type_upper_right, TString(L"Files"));

	codeHandler->SetMiniApp(TrecPointerKey::GetTrecPointerFromSoft<MiniApp>(self));

	return 0;
}

void SourceCodeApp2::OnSave()
{
	if (codeHandler.Get())
		codeHandler->OnSave();
}
