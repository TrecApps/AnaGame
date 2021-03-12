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

/**
 * Method: MiniApp::GetType
 * Purpose: Returns a String Representation of the object type
 * Parameters: void
 * Returns: TString - representation of the object type
 */
TString MiniApp::GetType()
{
	return TString(L"MiniApp;") + TObject::GetType();
}

bool MiniApp::ShouldDestroy()
{
	ThreadLock();
	bool ret = mainPage.Get() == nullptr;
	ThreadRelease();
	return ret;
}

void MiniApp::SetSelf(TrecPointer<MiniApp> s)
{
	if (s.Get() != this)
		throw L"Error! Needed this to be pointed to self";
	self = TrecPointerKey::GetSoftPointerFromTrec<MiniApp>(s);
}


/**
 * Method: MiniApp::GetMainHandler
 * Purpose: Reports the main Handler of the App, null if there is none
 * Parameters: void
 * Returns: TrecPointer<EventHandler> - the main handler associated with this app
 */
TrecPointer<EventHandler> MiniApp::GetMainHandler()
{
	return TrecPointer<EventHandler>();
}