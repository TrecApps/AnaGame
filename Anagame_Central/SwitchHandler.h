#pragma once
#include <EventHandler.h>


#define SWITCH_HANDLER_METHOD_COUNT 12

class SwitchHandler;

typedef void (SwitchHandler::* methodArray)(TrecPointer<TControl> tc, EventArgs ea);

class SwitchHandler :
	public EventHandler
{
public:
	SwitchHandler(TrecPointer<TInstance> ins);
	~SwitchHandler();

	void Initialize(TrecPointer<Page> page);
	void HandleEvents(TDataArray<EventID_Cred>& eventAr);

	virtual void ProcessMessage(TrecPointer<HandlerMessage> message)override;
protected:
	TrecPointer<Page> page;
	methodArray handlers[SWITCH_HANDLER_METHOD_COUNT];
	TLayout* rootLayout;

	TrecPointer<TControl> changeControl;

	void OnSelect1(TrecPointer<TControl> tc, EventArgs ea);
	void OnSelect2(TrecPointer<TControl> tc, EventArgs ea);
	void OnSelect3(TrecPointer<TControl> tc, EventArgs ea);
	void OnSelectRows(TrecPointer<TControl> tc, EventArgs ea);
	void OnSelectColumns(TrecPointer<TControl> tc, EventArgs ea);
	void OnSelectGrid(TrecPointer<TControl> tc, EventArgs ea);
	void OnSelectLayers(TrecPointer<TControl> tc, EventArgs ea);
	void OnSelectGadget(TrecPointer<TControl> tc, EventArgs ea);
	void OnSelectText(TrecPointer<TControl> tc, EventArgs ea);
	void OnSelectCombo(TrecPointer<TControl> tc, EventArgs ea);
	void OnSelectScroll(TrecPointer<TControl> tc, EventArgs ea);
	void OnSelectGif(TrecPointer<TControl> tc, EventArgs ea);

	virtual bool ShouldProcessMessageByType(TrecPointer<HandlerMessage> message) override;

};

