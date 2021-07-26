#pragma once
#include <TObject.h>
class TPresentEngine :
    public TObject
{
public:
    HRESULT STDMETHODCALLTYPE GetService(
        __RPC__in REFGUID guidService,
        __RPC__in REFIID riid,
        __RPC__deref_out_opt LPVOID* ppvObject);

};

