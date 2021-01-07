#pragma once
#include <EventHandler.h>
#include <THttpClientSocket.h>

class SocketHandler;

typedef void (SocketHandler::* socketMethod)(TrecPointer<TControl> tc, EventArgs ea);



class SocketHandler : public EventHandler
{
public:
	SocketHandler(TrecPointer<TInstance> ins);
	~SocketHandler();

	void Initialize(TrecPointer<Page> page)override;
	void HandleEvents(TDataArray<EventID_Cred>& eventAr)override;

	virtual void ProcessMessage(TrecPointer<HandlerMessage> message)override;
protected:
	// Relevent textBoxes
	TrecSubPointer<TControl, TTextField> urlBar, submitBar, reqHeaders, reqBody, resHeaders, resBody;

	void OnSubmitEndpoint(TrecPointer<TControl> tc, EventArgs ea);
	void SetMethod(TrecPointer<TControl> tc, EventArgs ea);

	THttpMethod method;

	TDataArray<socketMethod> sockHandlers;

	HWND window;
};

