#pragma once
#include <MiniApp.h>

#include <CameraHandler.h>
#include <ArenaHandler.h>

class ArenaApp2 :
	public MiniApp
{
public:
	ArenaApp2(TrecSubPointer<TWindow, TIdeWindow> win, TString& name);

	virtual void DispatchAnagameMessage(TrecPointer<HandlerMessage> message) override;

	virtual UINT Initialize()override;

	virtual void OnSave()override;

private:
	TString name;

	TrecSubPointer<EventHandler, CameraHandler> cameraControls;
	TrecSubPointer<EventHandler, ArenaHandler> arena;

	TrecSubPointer<Page, IDEPage> cameraPage;
};

