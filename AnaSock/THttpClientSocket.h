#pragma once
#include <TClientSocket.h>
#include <TDataMap.h>

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

class THttpRequest : public TObject
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

class THttpResponse : public TObject
{
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
    TString httpType, status;
    TDataMap<TString> headers;
    TString body;
};


class THttpClientSocket :
    public TClientSocket
{
public:
    THttpClientSocket();
    ~THttpClientSocket();


};

