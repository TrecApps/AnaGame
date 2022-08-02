#pragma once
#include <THttpClientSocket.h>
#include <EventHandler.h>
#include <TSwitchControl.h>
#include <TTextInput.h>



class SocketHandler : public TapEventHandler
{
protected:
	typedef void (SocketHandler::* socketMethod)(TrecPointer<TPage> tc, EventArgs ea);
public:
	SocketHandler(TrecPointer<TProcess> ins);
	~SocketHandler();

	void Initialize(TrecPointer<TPage> page)override;
	void HandleEvents(TDataArray<TPage::EventID_Cred>& eventAr)override;

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
	TrecSubPointer<TPage, TTextInput> urlBar, submitBar, reqHeaders, reqBody, resHeaders, resBody;
	TDataMap<UINT> events;

	void OnSubmitEndpoint(TrecPointer<TPage> tc, EventArgs ea);
	void SetMethod(TrecPointer<TPage> tc, EventArgs ea);

	THttpMethod method;

	TDataArray<socketMethod> sockHandlers;

	HWND window;
};

