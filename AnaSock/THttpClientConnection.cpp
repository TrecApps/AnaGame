#include "THttpClientConnection.h"

typedef struct HttpMethodMap {
    THttpConnectionMethod method;
    TString methodStr;
}HttpMethodMap;

HttpMethodMap httpMethods[] = {
    {THttpConnectionMethod::http_get, L"get"},
    {THttpConnectionMethod::http_post, L"post"},
    {THttpConnectionMethod::http_put, L"put"},
    {THttpConnectionMethod::http_delete, L"delete"},
    {THttpConnectionMethod::http_head, L"head"},
    {THttpConnectionMethod::http_connect, L"connect"},
    {THttpConnectionMethod::http_options, L"options"},
    {THttpConnectionMethod::http_trace, L"trace"},
    {THttpConnectionMethod::http_patch, L"patch"}
};

TString getMethod(THttpConnectionMethod method)
{
    return httpMethods[static_cast<UINT>(method)].methodStr;
}

bool GetMethod(const TString& method, THttpConnectionMethod& methodValue)
{
    TString m(method.GetTrim().GetLower());

    for (UINT Rust = 0; Rust < ARRAYSIZE(httpMethods); Rust++)
    {
        if (!httpMethods[Rust].methodStr.Compare(method))
        {
            methodValue = httpMethods[Rust].method;
            return true;
        }
    }
    return false;
}

THttpClientConnection::THttpClientConnection()
{
	sent = false;
	method = THttpConnectionMethod::http_get;
}

UINT THttpClientConnection::SetAddress(const TString& address)
{
    mainSocket = TrecPointerKey::GetNewTrecPointer<TClientSocket>(1);
    TString add(address);
    return mainSocket->InitializeSocket(add);
}

bool THttpClientConnection::SupportsHeaders()
{
    return true;
}

bool THttpClientConnection::SetHeaders(const TDataMap<TString>& headers)
{
    this->headers.clear();
    TDataEntry<TString> entry;
    for (UINT Rust = 0; Rust < headers.GetEntryAt(Rust, entry); Rust++)
    {
        int rep;
        this->headers.addEntry(entry.key.GetReplace(rep, L"\r", L"\\r").GetReplace(rep, L"\n", L"\\n"),
            entry.object.GetReplace(rep, L"\r", L"\\r").GetReplace(rep, L"\n", L"\\n"));
    }
    return true;
}

bool THttpClientConnection::SetHeader(const TString& key, const TString& value)
{
    int rep;
    headers.addEntry(key.GetReplace(rep, L"\r", L"\\r").GetReplace(rep, L"\n", L"\\n"),
        value.GetReplace(rep, L"\r", L"\\r").GetReplace(rep, L"\n", L"\\n"));
    return true;
}

bool THttpClientConnection::GetHeaders(TDataMap<TString>& headers)
{
    headers.clear();
    TDataEntry<TString> entry;
    for (UINT Rust = 0; Rust < this->headers.GetEntryAt(Rust, entry); Rust++)
    {
        int rep;
        this->headers.addEntry(entry.key.GetReplace(rep, L"\r", L"\\r").GetReplace(rep, L"\n", L"\\n"), entry.object);
    }
    return true;
}

bool THttpClientConnection::GetHeader(const TString& key, TString& value)
{
    int rep;
    return headers.retrieveEntry(key.GetReplace(rep, L"\r", L"\\r").GetReplace(rep, L"\n", L"\\n"),
        value);
}

bool THttpClientConnection::SupportsSettings()
{
    return true;
}

bool THttpClientConnection::SetSetting(const TString& key, const TString& value)
{
    if (!key.GetTrim().GetLower().Compare(L"method"))
    {
        return GetMethod(value, this->method);
    }
    if (!key.GetTrim().GetLower().Compare(L"endpoint"))
    {
        this->endpoint.Set(endpoint);
        return true;
    }
    return false;
}

bool THttpClientConnection::GetSetting(const TString& key, TString& value)
{
    if (!key.GetTrim().GetLower().Compare(L"method"))
    {
        value.Set(getMethod(this->method));
        return true;
    }
    if (!key.GetTrim().GetLower().Compare(L"endpoint"))
    {
        value.Set(endpoint);
        return true;
    }
    return false;
}

bool THttpClientConnection::SetMainData(const TString& body)
{
    requestBody.Set(body);
    return true;
}

bool THttpClientConnection::GetMainData(bool& readString, TString& strValue, TDataArray<UCHAR>& binValue)
{
    readString = true;
    strValue.Set(requestBody);
    return true;
}

TString THttpClientConnection::CompileAndSend()
{
	if (!mainSocket.Get())
		return L"Error! Connection Not Set!";
	std::string request;

	TString top;
	switch (method)
	{
	case THttpConnectionMethod::http_connect:
		top.Set(L"CONNECT");
		break;
	case THttpConnectionMethod::http_delete:
		top.Set(L"DELETE");
		break;
	case THttpConnectionMethod::http_get:
		top.Set(L"GET");
		break;
	case THttpConnectionMethod::http_head:
		top.Set(L"HEAD");
		break;
	case THttpConnectionMethod::http_options:
		top.Set(L"OPTIONS");
		break;
	case THttpConnectionMethod::http_patch:
		top.Set(L"PATCH");
		break;
	case THttpConnectionMethod::http_post:
		top.Set(L"POST");
		break;
	case THttpConnectionMethod::http_put:
		top.Set(L"PUT");
		break;
	case THttpConnectionMethod::http_trace:
		top.Set(L"TRACE");
		break;
	}

	top.AppendFormat(L" %ws %ws", endpoint.GetSize() ? endpoint.GetConstantBuffer().getBuffer() : L"/", L"HTTP/1.1");

	std::string uBody;

	if (requestBody.GetSize())
	{
		int requiredSize = WideCharToMultiByte(CP_UTF8, // Http Uses UTF-8
			0,											// No Special flags
			requestBody.GetConstantBuffer().getBuffer(),					// The Data to scan
			-1,											// Dat is null-terminated
			nullptr,									// no data for output
			0,											// We are seeking the required space
			nullptr,									// Using System codes
			nullptr);									// First parameter requires this one to be null

		char* buffer = new char[requiredSize + 1]; // not sure if required size includes null terminator

		// Zero memory for safety
		ZeroMemory(buffer, sizeof(char) * (requiredSize + 1));

		// Now covert body to UTF-8
		WideCharToMultiByte(CP_UTF8,
			0,
			requestBody.GetConstantBuffer().getBuffer(),
			-1,
			buffer,
			requiredSize,
			nullptr,
			nullptr);
		uBody = buffer;
		delete[] buffer;
		buffer = nullptr;
	}

	if (uBody.size())
	{
		// Here, we have a budy, so set the content length attribute
		TString remField(L"Content-Length");
		TString holder;
		while (headers.removeEntry(remField, holder));

		holder.Format(L"%d", uBody.size());
	}


	// Now Compile The Headers
	TString tHeaders;
	for (UINT Rust = 0; Rust < headers.count(); Rust++)
	{
		TDataEntry<TString> entry;
		if (headers.GetEntryAt(Rust, entry))
		{
			tHeaders.AppendFormat(L"%ws%ws:%ws",
				tHeaders.GetSize() ? L"\r\n" : L"", // Add CRLF but only if this isn't the first iteration (that one will be added later
				entry.key.GetConstantBuffer().getBuffer(),		// Add the Key
				entry.object.GetConstantBuffer().getBuffer());	// Add the Value
		}
	}

	// Append the headers to the top line
	top.AppendFormat(L"%ws%ws", tHeaders.GetSize() ? L"\r\n" : L"", tHeaders.GetConstantBuffer().getBuffer());


	// Add headers to main request data
	request.append(top.GetRegString());

	// Check for a body and add to request if present
	if (uBody.size())
	{
		request.append("\r\n\r\n"); // There needs to be two CRLF's between headers and body
		request.append(uBody);
	}

	TString error;

	TDataArray<char> data;
	for (UINT Rust = 0; Rust < request.size(); Rust++)
		data.push_back(request[Rust]);

	error = mainSocket->Send(data);
	sent = !error.GetSize();
	return error;
}

TString THttpClientConnection::Retrieve(bool& readString, TString& strValue, TDataArray<UCHAR>& binValue)
{
	readString = true;
	if (!sent)
		return L"Error! No data has been Sent!";
	if (!mainSocket.Get())
		return L"Error! Connection Not Set!";
	TDataArray<char> resp;
	TString error(mainSocket->Recieve(resp));
	sent = false;

	if (error.GetSize())
	{
		strValue.Set(L"HTTP / 1.1 0 ERROR");
		return error;
	}
	std::string cReq;
	cReq.clear();
	for (UINT Rust = 0; Rust < resp.Size(); Rust++)
		cReq += resp[Rust];
	strValue.Set(cReq);
	return TString();
}

bool THttpClientConnection::HasVariableSupport()
{
	return true;
}

bool THttpClientConnection::GetVariable(const TString& name, TrecPointer<TVariable>& var)
{
	return false;
}

bool THttpClientConnection::SetVariable(const TString& name, TrecPointer<TVariable> var)
{
	return false;
}




///
///
/// Async Connection Resources
/// 
/// 
/// 






THttpClientConnectionAsync::THttpClientConnectionAsync()
{
}

UINT THttpClientConnectionAsync::SetAddress(const TString& address)
{
	return baseConnection.SetAddress(address);
}

bool THttpClientConnectionAsync::SupportsHeaders()
{
	return true;
}

bool THttpClientConnectionAsync::SetHeaders(const TDataMap<TString>& headers)
{
	return baseConnection.SetHeaders(headers);
}

bool THttpClientConnectionAsync::SetHeader(const TString& key, const TString& value)
{
	return baseConnection.SetHeader(key, value);
}

bool THttpClientConnectionAsync::GetHeaders(TDataMap<TString>& headers)
{
	return baseConnection.GetHeaders(headers);
}

bool THttpClientConnectionAsync::GetHeader(const TString& key, TString& value)
{
	return baseConnection.GetHeader(key, value);
}

bool THttpClientConnectionAsync::SupportsSettings()
{
	return baseConnection.SupportsSettings();
}

bool THttpClientConnectionAsync::SetSetting(const TString& key, const TString& value)
{
	return baseConnection.SetSetting(key, value);
}

bool THttpClientConnectionAsync::GetSetting(const TString& key, TString& value)
{
	return baseConnection.GetSetting(key, value);
}

bool THttpClientConnectionAsync::SetMainData(const TString& body)
{
	return baseConnection.SetMainData(body);
}

bool THttpClientConnectionAsync::GetMainData(bool& readString, TString& strValue, TDataArray<UCHAR>& binValue)
{
	return baseConnection.GetMainData(readString, strValue, binValue);
}

TString THttpClientConnectionAsync::CompileAndSend()
{
	TString ret(baseConnection.CompileAndSend());
	if (ret.GetSize())
	{
		// To Do: Respond to Error


		// End To-Do
		return ret;
	}

	//if (asyncResponse.Get())
	//{
	//	asyncResponse->RunAsyncObject();
	//}

	return TString();
}

bool THttpClientConnectionAsync::HasVariableSupport()
{
	return false;
}

bool THttpClientConnectionAsync::GetVariable(const TString& name, TrecPointer<TVariable>& var)
{
	return false;
}

bool THttpClientConnectionAsync::SetVariable(const TString& name, TrecPointer<TVariable> var)
{
	return false;
}
