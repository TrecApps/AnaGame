#include "ArenaApp2.h"

ArenaApp2::ArenaApp2(TrecSubPointer<TWindow, TIdeWindow> win, TString& name): MiniApp(win)
{
	this->name.Set(name);
}

void ArenaApp2::DispatchAnagameMessage(TrecPointer<HandlerMessage> message)
{
}

UINT ArenaApp2::Initialize()
{
	if (!win.Get() || !instance.Get())
		return 1;

	cameraControls = TrecPointerKey::GetNewSelfTrecSubPointer<EventHandler, CameraHandler>(instance, name);

	if (!cameraControls.Get())
		return 2;

	arena = TrecPointerKey::GetNewSelfTrecSubPointer<EventHandler, ArenaHandler>(instance, name);

	if (!arena.Get())
		return 3;

	mainPage = win->AddNewPage(anagame_page::anagame_page_arena, ide_page_type::ide_page_type_body, name, TString(), TrecPointerKey::GetTrecPointerFromSub<EventHandler, ArenaHandler>(arena));
	cameraPage = win->AddNewPage(anagame_page::anagame_page_camera, ide_page_type::ide_page_type_deep_console, name, TString(), TrecPointerKey::GetTrecPointerFromSub<EventHandler, CameraHandler>(cameraControls));


	win->SetCurrentApp(TrecPointerKey::GetTrecPointerFromSoft<MiniApp>(self));
	arena->SetMiniApp(TrecPointerKey::GetTrecPointerFromSoft<MiniApp>(self));


	return 0;
}

void ArenaApp2::OnSave()
{
}
