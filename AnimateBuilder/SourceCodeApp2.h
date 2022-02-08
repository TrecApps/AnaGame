#pragma once
#include <MiniApp.h>
#include <TerminalHandler.h>
#include <TCodeHandler.h>

class SourceCodeApp2 :
	public MiniApp
{
public:
	SourceCodeApp2(TrecSubPointer<TWindow, TIdeWindow> win);

	virtual void DispatchAnagameMessage(TrecPointer<HandlerMessage> message) override;

	virtual UINT Initialize(TrecPointer<TFileShell> file)override;

	virtual void OnSave() override;

	virtual TrecPointer<TPage::EventHandler> GetMainHandler()override;

protected:
	TrecSubPointer<TPage::EventHandler, TerminalHandler> commandHandler;
	TrecSubPointer<TPage::EventHandler, TCodeHandler> codeHandler;
};

