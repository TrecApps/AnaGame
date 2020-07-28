#include "MiniApp.h"
#include "IDEPage.h"

MiniApp::MiniApp(TrecSubPointer<TWindow, TIdeWindow> win)
{
	this->win = win;
	if (this->win.Get())
	{
		auto tempInstance = this->win->GetInstance();
		instance = TrecPointerKey::GetSoftPointerFromTrec<TInstance>(tempInstance);
	}
}

bool MiniApp::ShouldDestroy()
{
	return mainPage.Get() == nullptr;
}

void MiniApp::SetSelf(TrecPointer<MiniApp> s)
{
	if (s.Get() != this)
		throw L"Error! Needed this to be pointed to self";
	self = TrecPointerKey::GetSoftPointerFromTrec<MiniApp>(s);
}
