#include "SocketHandler.h"
#include <TWindow.h>
#include <TLayout.h>
#include <AnafacePage.h>

SocketHandler::SocketHandler(TrecPointer<TProcess> ins): TapEventHandler(ins)
{
	method = THttpMethod::http_get;
	window = 0;

	events.addEntry(L"OnSubmitEndpoint", 0);
	sockHandlers.push_back(&SocketHandler::OnSubmitEndpoint);

	events.addEntry(L"SetMethod", 1);
	sockHandlers.push_back(&SocketHandler::SetMethod);
}

SocketHandler::~SocketHandler()
{
}

void SocketHandler::Initialize(TrecPointer<TPage> page)
{
	TrecSubPointer<TPage, AnafacePage> aPage = TrecPointerKey::GetTrecSubPointerFromTrec<TPage, AnafacePage>(page);
	assert(aPage.Get());
	TrecSubPointer<TPage, TLayout> rootLayout = TrecPointerKey::GetTrecSubPointerFromTrec<TPage, TLayout>(aPage->GetRootControl());
	assert(rootLayout.Get());
	TrecSubPointer<TPage, TLayout> topLayout = TrecPointerKey::GetTrecSubPointerFromTrec<TPage, TLayout>(rootLayout->GetPage(0,0));
	assert(topLayout.Get());

	submitBar = TrecPointerKey::GetTrecSubPointerFromTrec<TPage, TTextInput>(topLayout->GetPage(0, 0));
	urlBar = TrecPointerKey::GetTrecSubPointerFromTrec<TPage, TTextInput>(topLayout->GetPage(1, 0));

	assert(submitBar.Get() && urlBar.Get());

	// Retrieve Resources for HTTP Request
	TrecSubPointer<TPage, TSwitchControl> aUI = TrecPointerKey::GetTrecSubPointerFromTrec<TPage, TSwitchControl>(rootLayout->GetPage(0, 1));
	//assert(aUI.Get());

	reqHeaders = TrecPointerKey::GetTrecSubPointerFromTrec<TPage, TTextInput>(aUI->GetPage(1));
	reqBody = TrecPointerKey::GetTrecSubPointerFromTrec<TPage, TTextInput>(aUI->GetPage(2));

	aUI = TrecPointerKey::GetTrecSubPointerFromTrec<TPage, TSwitchControl>(rootLayout->GetPage(0, 2));
	assert(aUI.Get());

	resHeaders = TrecPointerKey::GetTrecSubPointerFromTrec<TPage, TTextInput>(aUI->GetPage(0));
	resBody = TrecPointerKey::GetTrecSubPointerFromTrec<TPage, TTextInput>(aUI->GetPage(1));


}

void SocketHandler::HandleEvents(TDataArray<TPage::EventID_Cred>& eventAr)
{
	int e_id = -1;
	EventArgs ea;
	TapEventHandler::HandleEvents(eventAr);
	for (UINT c = 0; c < eventAr.Size(); c++)
	{
		auto tc = eventAr.at(c).args;

		if (!tc.Get()) continue;

		UINT u_id = 0;
		if (!events.retrieveEntry(tc->methodID, u_id))
			continue;
		e_id = u_id;
		// At this point, call the appropriate method
		if (e_id > -1 && e_id < this->sockHandlers.Size())
		{
			// call method
			if (sockHandlers[e_id])
				(this->*sockHandlers[e_id])(eventAr[c].control, ea);
		}
		eventAr[c].args.Nullify();
	}
}

void SocketHandler::ProcessMessage(TrecPointer<HandlerMessage> message)
{

}

bool SocketHandler::ShouldProcessMessageByType(TrecPointer<HandlerMessage> message)
{
	return false;
}

void SocketHandler::OnSubmitEndpoint(TrecPointer<TPage> tc, EventArgs ea)
{
	TString url(L"localhost:8080/");

	url.Append(urlBar->GetText());

	url.Replace(L"//", L"/");
	url.Replace(L"\n", L"");
	url.Replace(L"\t", L"");
	url.Replace(L"\r", L"");
	url.Replace(L"\s", L"");

	THttpRequest req(method);
	req.SetBody(reqBody->GetText());

	TString headers(reqHeaders->GetText());
	headers.Trim();
	if (headers.GetSize())
	{
		auto lines = headers.split(L"\n");

		for (UINT Rust = 0; lines->Size(); Rust++)
		{
			auto keyValue = lines->at(Rust).splitn(L":", 2);

			if (keyValue->Size() != 2)
			{
				MessageBoxExW(window, L"Improperly formatted headers detected", nullptr, 0, 0);
				return;
			}

			req.AddHeader(keyValue->at(0).GetTrim(), keyValue->at(1).GetTrim());
		}
	}

	THttpClientSocket socket;
	socket.ConfigurePort_(8080);
	TString urlLocal(L"localhost");
	UINT result = socket.InitializeSocket(urlLocal);

	if (result)
	{
		MessageBoxExW(window, L"Failed to initialize a socket", nullptr, 0, 0);
		return;
	}

	result = socket.Connect();
	if (result)
	{
		MessageBoxExW(window, L"Failed to Connect our socket", nullptr, 0, 0);
		return;
	}
	TString httpResult;
	THttpResponse httpResponse = socket.Transmit(req, httpResult);

	resBody->SetText(httpResponse.GetBody());
	httpResult.Append(L"\n\n");
	httpResult.Append(httpResponse.GetHttpMode() + L" " + httpResponse.GetFullStatus() + L"\n\n");
	
	bool headerAvailable = true;
	UINT Rust = 0;

	TString key, value;
	while (httpResponse.GetHeader(Rust++, key, value))
	{
		httpResult.AppendFormat(L"%ws: %ws\n", key.GetConstantBuffer().getBuffer(), value.GetConstantBuffer().getBuffer());
	}

	resHeaders->SetText(httpResult);
}

void SocketHandler::SetMethod(TrecPointer<TPage> tc, EventArgs ea)
{
	if (!ea.text.Compare(L"GET"))
	{
		method = THttpMethod::http_get;
		if (submitBar.Get())
			submitBar->SetText(L"Submit (GET)");
	}
	else if (!ea.text.Compare(L"POST"))
	{
		method = THttpMethod::http_post;
		if (submitBar.Get())
			submitBar->SetText(L"Submit (POST)");
	}
	else if (!ea.text.Compare(L"PUT"))
	{
		method = THttpMethod::http_put;
		if (submitBar.Get())
			submitBar->SetText(L"Submit (PUT)");
	}
	else if (!ea.text.Compare(L"DELETE"))
	{
		method = THttpMethod::http_delete;
		if (submitBar.Get())
			submitBar->SetText(L"Submit (DELETE)");
	}
}
