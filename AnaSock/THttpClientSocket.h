#pragma once
#include <TClientSocket.h>
#include <TDataMap.h>
#include "AnaSock.h"

typedef enum class THttpMethod
{
    http_get,
    http_post,
    http_put,
    http_delete,
    http_head,
    http_connect,
    http_options,
    http_trace,
    http_patch
}THttpMethod;

class _ANA_SOCK_DLL THttpRequest : public TObject
{
public:
    THttpRequest(THttpMethod method);

    void AddHeader(const TString& key, const TString& value);
    void RemoveHeader(const TString& key);
    TString GetHeader(const TString& key, bool& present);

    void UpdateMethod(THttpMethod method);
    THttpMethod GetMethod();

    TString GetBody();
    void SetBody(const TString& body);

    TString GetEndpoint();
    void SetEndpoint(const TString& endpoint);

    void CompileRequest(std::string& request);

private:
    THttpMethod method;
    TDataMap<TString> headers;
    TString body;
    TString endpoint;
};

class _ANA_SOCK_DLL THttpResponse : public TObject
{
    friend class TAsyncHttpResponse;
public:
    THttpResponse(const std::string& data);
    THttpResponse(const THttpResponse& copy);

    short GetStatusCode();
    TString GetFullStatus();
    TString GetHttpMode();

    bool GetHeader(const TString& key, TString& value);
    bool GetHeader(UINT index, TString& key, TString& value);

    TString GetBody();


private:

    THttpResponse();

    TString httpType, status;
    TDataMap<TString> headers;
    TString body;
};

class _ANA_SOCK_DLL TAsyncHttpResponse : public TObject
{
public:
    TAsyncHttpResponse(SOCKET sock, UINT threadId);
    TAsyncHttpResponse(const TAsyncHttpResponse& copy);
    bool Abort();

    void SetResponse(const THttpResponse& resp);
    void SetError(const TString& err);

    bool IsComplete();

    THttpResponse GetResponse(TString& error);

protected:
    UINT threadId;
    SOCKET sock;
    THttpResponse response;
    TString error;
    bool ready;
};


class _ANA_SOCK_DLL THttpClientSocket :
    public TClientSocket
{
public:
    THttpClientSocket();
    ~THttpClientSocket();

    THttpResponse Transmit(THttpRequest& req, TString& error);
    TrecPointer<TAsyncHttpResponse> TransmitAsync(THttpRequest& req);
};

