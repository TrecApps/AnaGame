#pragma once
#include <mfobjects.h>

template<class T> class TComCallback : public IMFAsyncCallback
{
public:
	typedef HRESULT(T::*CallMethod)(IMFAsyncResult* result);

	TComCallback(T* obj, CallMethod method) {
		parent = obj;
		this->method = method;
	}

    // IUnknown
    STDMETHODIMP_(ULONG) AddRef(void)
    {
        // Delegate to parent class.
        return 1;
    }

    STDMETHODIMP_(ULONG) Release(void)
    {
        // Delegate to parent class.
        return 1;
    }

    STDMETHODIMP QueryInterface(REFIID iid, __RPC__deref_out _Result_nullonfailure_ void** ppv)
    {
        if (!ppv)
            return E_POINTER;
        if (iid == __uuidof(IUnknown))
            *ppv = static_cast<IUnknown*>(static_cast<IMFAsyncCallback*>(this));
        else if (iid == __uuidof(IMFAsyncCallback))
            *ppv = static_cast<IMFAsyncCallback*>(this);
        else
        {
            *ppv = NULL;
            return E_NOINTERFACE;
        }
        AddRef();
        return S_OK;
    }

    // IMFAsyncCallback methods
    STDMETHODIMP GetParameters(__RPC__out DWORD* pdwFlags, __RPC__out DWORD* pdwQueue)
    {
        // Implementation of this method is optional.
        return E_NOTIMPL;
    }

    STDMETHODIMP Invoke(__RPC__in_opt IMFAsyncResult* pAsyncResult)
    {
        return (parent->*method)(pAsyncResult);
    }


private:
	T* parent;
	CallMethod method;
};