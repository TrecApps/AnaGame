#include "SocketHandler.h"
#include <Page.h>
#include <TWindow.h>
SocketHandler::SocketHandler(TrecPointer<TInstance> ins): EventHandler(ins)
{
	method = THttpMethod::http_get;
	window = 0;
}

SocketHandler::~SocketHandler()
{
}

void SocketHandler::Initialize(TrecPointer<Page> page)
{
	TrecSubPointer<TControl, TLayout> rootLayout = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TLayout>(page->GetRootControl());
	assert(rootLayout.Get());
	TrecSubPointer<TControl, TLayout> topLayout = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TLayout>(rootLayout->GetLayoutChild(0,0));
	assert(topLayout.Get());

	submitBar = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TTextField>(topLayout->GetLayoutChild(0, 0));
	urlBar = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TTextField>(topLayout->GetLayoutChild(1, 0));

	assert(submitBar.Get() && urlBar.Get());

	// Retrieve Resources for HTTP Request
	TrecSubPointer<TControl, AnafaceUI> aUI = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, AnafaceUI>(rootLayout->GetLayoutChild(0, 1));
	assert(aUI.Get());

	reqHeaders = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TTextField>(aUI->GetChildAt(1));
	reqBody = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TTextField>(aUI->GetChildAt(2));

	aUI = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, AnafaceUI>(rootLayout->GetLayoutChild(0, 2));
	assert(aUI.Get());

	resHeaders = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TTextField>(aUI->GetChildAt(0));
	resBody = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TTextField>(aUI->GetChildAt(1));

	window = page->GetWindowHandle()->GetWindowHandle();
}

void SocketHandler::HandleEvents(TDataArray<EventID_Cred>& eventAr)
{
	for (UINT Rust = 0; Rust < eventAr.Size(); Rust++)
	{
		auto cont = eventAr[Rust].control;
		if (!cont.Get()) continue;

		EventArgs ea = cont->getEventArgs();


		int ea_id = ea.methodID;

		if (ea_id > -1 && ea_id < sockHandlers.Size())
		{
			if (sockHandlers[ea_id])
				(this->*sockHandlers[ea_id])(cont, ea);
		}
		cont->resetArgs();
	}
}

void SocketHandler::ProcessMessage(TrecPointer<HandlerMessage> message)
{

}

void SocketHandler::OnSubmitEndpoint(TrecPointer<TControl> tc, EventArgs ea)
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
		httpResult.AppendFormat(L"%ws: %ws\n", key.GetConstantBuffer(), value.GetConstantBuffer());
	}

	resHeaders->SetText(httpResult);
}

void SocketHandler::SetMethod(TrecPointer<TControl> tc, EventArgs ea)
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
