#include "SourceCodeApp2.h"
#include <TabPage.h>

SourceCodeApp2::SourceCodeApp2(TrecSubPointer<TWindow, TIdeWindow> win): MiniApp(win)
{
}

void SourceCodeApp2::DispatchAnagameMessage(TrecPointer<HandlerMessage> message)
{
}

UINT SourceCodeApp2::Initialize(TrecPointer<TFileShell> file)
{
	if (!win.Get() || !instance.Get())
		return 1;

	codeHandler = TrecPointerKey::GetNewSelfTrecSubPointer<TPage::EventHandler, TCodeHandler>(TrecPointerKey::GetTrecPointerFromSoft<>(instance));

	if (!codeHandler.Get())
		return 2;

	codeHandler->SetSaveFile(file);

	commandHandler = TrecPointerKey::GetNewSelfTrecSubPointer<TPage::EventHandler, TerminalHandler>(TrecPointerKey::GetTrecPointerFromSoft<>(instance));
	if (!commandHandler.Get())
		return 3;

	mainPage = win->AddNewPage(anagame_page::anagame_page_code_file, ide_page_type::ide_page_type_body, TString(L"Code"), TString(), TrecPointerKey::GetTrecPointerFromSub<TPage::EventHandler, TCodeHandler>(codeHandler));
	win->AddPage(anagame_page::anagame_page_command_prompt, ide_page_type::ide_page_type_deep_console, TString(L"Build"));
	win->AddPage(anagame_page::anagame_page_command_prompt, ide_page_type::ide_page_type_deep_console, TString(L"Debug"));

	win->AddPage(anagame_page::anagame_page_file_node, ide_page_type::ide_page_type_upper_right, TString(L"Files"));

	auto env = win->GetEnvironment();
	if (env.Get())
	{
		if (!env->GetPrompt().Get())
		{
			auto promptPage = 
				win->AddNewPage(
					anagame_page::anagame_page_console,
					ide_page_type::ide_page_type_deep_console,
					TString(L"Program Output"),
					TString(),
					TrecPointerKey::GetTrecPointerFromSub<TPage::EventHandler, TerminalHandler>(commandHandler)
				);

			//auto promptHandler = TrecPointerKey::GetTrecSubPointerFromTrec<TPage::EventHandler, TerminalHandler>(promptPage->GetHandler());

			//env->SetPrompt(promptHandler->GetTerminal()->GetConsoleHolder());
		}
	}

	codeHandler->SetMiniApp(TrecPointerKey::GetTrecPointerFromSoft<MiniApp>(self));

	win->SetCurrentApp(TrecPointerKey::GetTrecPointerFromSoft<MiniApp>(self));

	return 0;
}

void SourceCodeApp2::OnSave()
{
	if (codeHandler.Get())
		codeHandler->OnSave();
}

TrecPointer<TPage::EventHandler> SourceCodeApp2::GetMainHandler()
{
	return TrecPointerKey::GetTrecPointerFromSub<TPage::EventHandler, TCodeHandler>(codeHandler);
}
