#pragma once
#include <THttpClientSocket.h>
#include <EventHandler.h>


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

	/**
	 * Method: EventHandler::ShouldProcessMessageByType
	 * Purpose: Reports whether this Object is of the correct type to recieve the message
	 * Parameters: TrecPointer<HandlerMessage> message - the message to scan
	 * Returns: bool - true if the type matches, false oherwise
	 *
	 * Attributes: abstract
	 */
	virtual bool ShouldProcessMessageByType(TrecPointer<HandlerMessage> message) override;

	// Relevent textBoxes
	TrecSubPointer<TControl, TTextField> urlBar, submitBar, reqHeaders, reqBody, resHeaders, resBody;

	void OnSubmitEndpoint(TrecPointer<TControl> tc, EventArgs ea);
	void SetMethod(TrecPointer<TControl> tc, EventArgs ea);

	THttpMethod method;

	TDataArray<socketMethod> sockHandlers;

	HWND window;
};

