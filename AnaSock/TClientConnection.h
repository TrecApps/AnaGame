#pragma once
#include <TVariable.h>
#include <TClientSocket.h>
#include <TDataMap.h>
#include <TcRunner.h>
#include <TAsyncVariable.h>
class TClientConnection :
    public TVObject
{
protected:
    TrecPointer<TClientSocket> mainSocket;

public:
    virtual bool SupportsHeaders() ;
    virtual bool SetHeaders(const TDataMap<TString>& headers) ;
    virtual bool SetHeader(const TString& key, const TString& value) ;
    virtual bool GetHeaders(TDataMap<TString>& headers) ;
    virtual bool GetHeader(const TString& key, TString& value) ;

    virtual bool SupportsSettings() ;
    virtual bool SetSettings(const TDataMap<TString>& Settings) ;
    virtual bool SetHeader(const TString& key, const TString& value) ;
    virtual bool GetSettings(TDataMap<TString>& Settings) ;
    virtual bool GetHeader(const TString& key, TString& value) ;

    virtual bool SupportsParameters() ;
    virtual bool SetParameters(const TDataMap<TString>& Parameters) ;
    virtual bool SetHeader(const TString& key, const TString& value) ;
    virtual bool GetParameters(TDataMap<TString>& Parameters) ;
    virtual bool GetHeader(const TString& key, TString& value) ;

    virtual TString CompileAndSend() = 0;

    virtual TString Retrieve(bool& readString, TString& strValue,TDataArray<UCHAR>& binValue) = 0;
};

class TClientConnectionAsync : public TClientConnection
{
protected:
    TrecSubPointer<TVariable, TcRunner> onSuccess, onFailure, onAbort;
    TrecSubPointer<TVariable, TAsyncVariable> asyncResponse;
public:
    virtual TString Retrieve(bool& readString, TString& strValue, TDataArray<UCHAR>& binValue) override;

    virtual void SetBroadResponse(TrecSubPointer<TVariable, TAsyncVariable> asyncResponse);
    virtual void SetBasicSuccessResponse(TrecSubPointer<TVariable, TcRunner> success);

    virtual void SetBasicAbortResponse(TrecSubPointer<TVariable, TcRunner> abort);

    virtual void SetBasicFailureResponse(TrecSubPointer<TVariable, TcRunner> failure);

    virtual void Abort();
};