#include "pch.h"
#include "THttpClientSocket.h"

THttpClientSocket::THttpClientSocket() : TClientSocket(1)
{
}

THttpClientSocket::~THttpClientSocket()
{
}

THttpResponse THttpClientSocket::Transmit(THttpRequest& req, TString& error)
{
	error.Empty();
	std::string cReq;
	req.CompileRequest(cReq);

	TDataArray<char> data;
	for (UINT Rust = 0; Rust < cReq.size(); Rust++)
		data.push_back(cReq[Rust]);

	error = Send(data);

	if (error.GetSize())
	{
		return THttpResponse("HTTP/1.1 0 ERROR");
	}

	error = Recieve(data);

	if (error.GetSize())
	{
		return THttpResponse("HTTP/1.1 0 ERROR");
	}
	cReq.clear();
	for (UINT Rust = 0; Rust < data.Size(); Rust++)
		cReq += data[Rust];

	return THttpResponse(cReq);
}

THttpRequest::THttpRequest(THttpMethod method)
{
	UpdateMethod(method);
}

void THttpRequest::AddHeader(const TString& key, const TString& value)
{
	int rep;
	TString tKey(key.GetReplace(rep, L"\r", L"\\r").GetReplace(rep, L"\n", L"\\n"));
	TString tValue(value.GetReplace(rep, L"\r", L"\\r").GetReplace(rep, L"\n", L"\\n"));

	headers.addEntry(tKey, tValue);
}

void THttpRequest::RemoveHeader(const TString& key)
{
	int rep;
	TString tKey(key.GetReplace(rep, L"\r", L"\\r").GetReplace(rep, L"\n", L"\\n"));
	TString val;
	headers.removeEntry(tKey, val);
}

TString THttpRequest::GetHeader(const TString& key, bool& present)
{
	TString ret;
	int rep;
	TString tKey(key.GetReplace(rep, L"\r", L"\\r").GetReplace(rep, L"\n", L"\\n"));
	
	present = headers.retrieveEntry(tKey, ret);
	return ret;
}

void THttpRequest::UpdateMethod(THttpMethod method)
{
	this->method = method;
}

THttpMethod THttpRequest::GetMethod()
{
	return method;
}

void THttpRequest::SetBody(const TString& body)
{
	this->body.Set(body);
}

TString THttpRequest::GetEndpoint()
{
	return endpoint;
}

void THttpRequest::SetEndpoint(const TString& endpoint)
{
	this->endpoint.Set(endpoint);
}

void THttpRequest::CompileRequest(std::string& request)
{
	request.clear();

	TString top;
	switch (method)
	{
	case THttpMethod::http_connect:
		top.Set(L"CONNECT");
		break;
	case THttpMethod::http_delete:
		top.Set(L"DELETE");
		break;
	case THttpMethod::http_get:
		top.Set(L"GET");
		break;
	case THttpMethod::http_head:
		top.Set(L"HEAD");
		break;
	case THttpMethod::http_options:
		top.Set(L"OPTIONS");
		break;
	case THttpMethod::http_patch:
		top.Set(L"PATCH");
		break;
	case THttpMethod::http_post:
		top.Set(L"POST");
		break;
	case THttpMethod::http_put:
		top.Set(L"PUT");
		break;
	case THttpMethod::http_trace:
		top.Set(L"TRACE");
		break;
	}

	top.AppendFormat(L" %ws %ws", endpoint.GetSize() ? endpoint.GetConstantBuffer() : L"/", L"HTTP/1.1");

	std::string uBody;

	if (body.GetSize())
	{
		int requiredSize = WideCharToMultiByte(CP_UTF8, // Http Uses UTF-8
			0,											// No Special flags
			body.GetConstantBuffer(),					// The Data to scan
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
			body.GetConstantBuffer(),
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
				entry.key.GetConstantBuffer(),		// Add the Key
				entry.object.GetConstantBuffer());	// Add the Value
		}
	}

	// Append the headers to the top line
	top.AppendFormat(L"%ws%ws", tHeaders.GetSize() ? L"\r\n" : L"", tHeaders.GetConstantBuffer());


	// Add headers to main request data
	request.append(top.GetRegString());

	// Check for a body and add to request if present
	if (uBody.size())
	{
		request.append("\r\n\r\n"); // There needs to be two CRLF's between headers and body
		request.append(uBody);
	}
}

THttpResponse::THttpResponse(const std::string& data)
{
	std::string piece;

	// Parse the Http Section
	UINT Rust;
	for (Rust = 0; Rust < data.size(); Rust++)
	{
		char ch = data[Rust];
		if (ch == ' ')
			break;
		piece += ch;
	}

	this->httpType.Set(piece);
	piece.clear();

	// Parse the Status

	for (; Rust < data.size(); Rust++)
	{
		char ch = data[Rust];
		if (ch == '\r')
			break;
		piece += ch;
	}

	this->status.Set(piece);
	piece.clear();

	// Prepare to parse the  headers
	Rust += 2;

	TString headerPiece;

	int headerEnd = data.find("\r\n\r\n");

	for (; Rust < (headerEnd == -1 ? data.size() : headerEnd); Rust++)
	{
		char ch = data[Rust];
		if (ch == '\r')
		{
			headerPiece.Set(piece);
			headerPiece.Trim();
			piece.clear();
			auto keyValue = headerPiece.splitn(L':', 2);

			if (keyValue->Size() != 2)
			{
				// To-Do: Handle scenario
			}

			headers.addEntry(keyValue->at(0), keyValue->at(1));
		}
		else if (ch != '\n')
			piece += ch;
	}

	if (piece.size())
	{
		headerPiece.Set(piece);
		headerPiece.Trim();
		piece.clear();
		auto keyValue = headerPiece.splitn(L':', 2);

		if (keyValue->Size() != 2)
		{
			// To-Do: Handle scenario
		}

		headers.addEntry(keyValue->at(0), keyValue->at(1));
	}

	if (headerEnd != -1)
	{
		// We have a body to parse
		Rust = headerEnd + 4;

		body.Set(data.substr(Rust).c_str());
	}

	// To-Do: Validate Data recieved and Parsed
}

THttpResponse::THttpResponse(const THttpResponse& copy) : headers(copy.headers)
{
	this->body.Set(copy.body);
	this->httpType.Set(copy.httpType);
	this->status.Set(copy.status);
}

short THttpResponse::GetStatusCode()
{
	auto pieces = status.splitn(L' ', 2);

	if (pieces->Size())
	{
		int ret = 0;
		if (pieces->at(0).ConvertToInt(ret))
			return 0;
		return ret;
	}
	return 0;
}

TString THttpResponse::GetFullStatus()
{
	return status;
}

TString THttpResponse::GetHttpMode()
{
	return httpType;
}

bool THttpResponse::GetHeader(const TString& key, TString& value)
{
	return headers.retrieveEntry(key, value);
}

bool THttpResponse::GetHeader(UINT index, TString& key, TString& value)
{
	TDataEntry<TString> entry;
	bool ret = headers.GetEntryAt(index, entry);

	if (ret)
	{
		key.Set(entry.key);
		value.Set(entry.object);
	}
	return ret;
}

TString THttpResponse::GetBody()
{
	return body;
}
