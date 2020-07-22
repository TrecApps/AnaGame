#include "ArenaModelApp.h"

ArenaModelApp::ArenaModelApp(TrecPointer<TControl> m, TrecPointer<TControl> o, TrecPointer<TControl> e, TrecPointer<TInstance> i):MiniHandler(m,o,e,i)
{
}

ArenaModelApp::~ArenaModelApp()
{
}

bool ArenaModelApp::InitializeControls()
{
	return false;
}
