#pragma once
#include "TClientConnection.h"

typedef enum class THttpConnectionMethod
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
}THttpConnectionMethod;

class THttpClientConnection : public TClientConnection
{
protected:
	TDataMap<TString> headers;
    TString requestBody;
    THttpConnectionMethod method;
    TString endpoint;

    bool sent;
public:
    THttpClientConnection();

    virtual UINT SetAddress(const TString& address);

    virtual bool SupportsHeaders();
    virtual bool SetHeaders(const TDataMap<TString>& headers);
    virtual bool SetHeader(const TString& key, const TString& value);
    virtual bool GetHeaders(TDataMap<TString>& headers);
    virtual bool GetHeader(const TString& key, TString& value);

    virtual bool SupportsSettings();
    virtual bool SetSetting(const TString& key, const TString& value);
    virtual bool GetSetting(const TString& key, TString& value);

    virtual bool SetMainData(const TString& body);
    virtual bool GetMainData(bool& readString, TString& strValue, TDataArray<UCHAR>& binValue);

    virtual TString CompileAndSend();

    virtual TString Retrieve(bool& readString, TString& strValue, TDataArray<UCHAR>& binValue);

    /**
 * Method: TVObject::HasVariableSupport
 * Purpose: Reports whether this TObject has TVariable support (since TVariables expect TObjects to exist and thus Raw TObjects cannot support it)
 * Parameters: void
 * Returns: bool - false for base object, true for any object that extends TVObject
 */
    virtual bool HasVariableSupport();

    /**
     * Method: TVObject::GetVariable
     * Purpose: Revtrieves a value in Variable form
     * Parameters: const TString& name - the name of the variable
     *              TrecPointer<TVariable>& var - holds the variable retrieved if valid
     * Returns: bool - whether the variable was valid or not
     *
     * Attributes: abstract
     */
    virtual bool GetVariable(const TString& name, TrecPointer<TVariable>& var) override;

    /**
     * Method: TVObject::SetVariable
     * Purpose: Sets a value in Variable form
     * Parameters: const TString& name - the name of the variable
     *              TrecPointer<TVariable> var - the value to set
     * Returns: bool - whether the variable was valid or not
     *
     * Attributes: abstract
     */
    virtual bool SetVariable(const TString& name, TrecPointer<TVariable> var)override;
};

class THttpClientConnectionAsync : public TClientConnectionAsync
{
protected:
    THttpClientConnection baseConnection;

public:
    THttpClientConnectionAsync();

    virtual UINT SetAddress(const TString& address);

    virtual bool SupportsHeaders();
    virtual bool SetHeaders(const TDataMap<TString>& headers);
    virtual bool SetHeader(const TString& key, const TString& value);
    virtual bool GetHeaders(TDataMap<TString>& headers);
    virtual bool GetHeader(const TString& key, TString& value);

    virtual bool SupportsSettings();
    virtual bool SetSetting(const TString& key, const TString& value);
    virtual bool GetSetting(const TString& key, TString& value);

    virtual bool SetMainData(const TString& body);
    virtual bool GetMainData(bool& readString, TString& strValue, TDataArray<UCHAR>& binValue);

    virtual TString CompileAndSend();

    virtual bool HasVariableSupport();
    virtual bool GetVariable(const TString& name, TrecPointer<TVariable>& var) override;
    virtual bool SetVariable(const TString& name, TrecPointer<TVariable> var)override;
};

