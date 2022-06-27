#include "TClientConnection.h"

bool TClientConnection::SupportsHeaders()
{
    return false;
}

bool TClientConnection::SetHeaders(const TDataMap<TString>& headers)
{
    return false;
}

bool TClientConnection::SetHeader(const TString& key, const TString& value)
{
    return false;
}

bool TClientConnection::GetParameters(TDataMap<TString>& Parameters)
{
    return false;
}

bool TClientConnection::GetSettings(TDataMap<TString>& Settings)
{
    return false;
}

bool TClientConnection::GetSetting(const TString& key, TString& value)
{
    return false;
}

bool TClientConnection::GetHeaders(TDataMap<TString>& headers)
{
    return false;
}

bool TClientConnection::GetHeader(const TString& key, TString& value)
{
    return false;
}

bool TClientConnection::SetMainData(const TString& body)
{
    return false;
}

bool TClientConnection::SetMainData(const TDataArray<UCHAR>& body)
{
    return false;
}

bool TClientConnection::GetMainData(bool& readString, TString& strValue, TDataArray<UCHAR>& binValue)
{
    return false;
}

bool TClientConnection::SupportsParameters()
{
    return false;
}

bool TClientConnection::SetParameters(const TDataMap<TString>& Parameters)
{
    return false;
}

bool TClientConnection::SupportsSettings()
{
    return false;
}

bool TClientConnection::SetSettings(const TDataMap<TString>& Settings)
{
    return false;
}

bool TClientConnection::SetSetting(const TString& key, const TString& value)
{
    return false;
}

TString TClientConnectionAsync::Retrieve(bool& readString, TString& strValue, TDataArray<UCHAR>& binValue)
{
    return L"Error! This Connections Works asynchronously! Set up a Runner or an Async Variable to respond to the Result!";
}

void TClientConnectionAsync::SetBroadResponse(TrecSubPointer<TVariable, TAsyncVariable> asyncResponse)
{
    if (asyncResponse.Get())
    {
        this->onSuccess.Nullify();
        this->onFailure.Nullify();
        this->onAbort.Nullify();
        this->asyncResponse = asyncResponse;
    }
}
void TClientConnectionAsync::SetBasicSuccessResponse(TrecSubPointer<TVariable, TcRunner> success)
{
    if (success.Get())
    {
        this->asyncResponse.Nullify();
        this->onSuccess = success;
    }
}

void TClientConnectionAsync::SetBasicAbortResponse(TrecSubPointer<TVariable, TcRunner> abort) {
    if (abort.Get())
    {
        this->asyncResponse.Nullify();
        this->onAbort = abort;
    }
}

void TClientConnectionAsync::SetBasicFailureResponse(TrecSubPointer<TVariable, TcRunner> failure) {
    if (failure.Get())
    {
        this->asyncResponse.Nullify();
        this->onFailure = failure;
    }
}

void TClientConnectionAsync::Abort() {

}