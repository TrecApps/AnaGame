#pragma once
#include <MiniHandler.h>

class ArenaModelApp;
typedef void (ArenaModelApp::* ModelBuilderEvents)(TControl tc, EventArgs ea);


class ArenaModelApp :
	public MiniHandler
{
public:
	ArenaModelApp(TrecPointer<TControl> m, TrecPointer<TControl> o, TrecPointer<TControl> e, TrecPointer<TInstance> i);
	~ArenaModelApp();

	bool InitializeControls() override;

protected:
	TDataArray<ModelBuilderEvents> handlers;

};

