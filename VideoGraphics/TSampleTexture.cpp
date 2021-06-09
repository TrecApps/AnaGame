#include "TSampleTexture.h"

TSampleTexture::TSampleTexture(IMFSample* sample, ID3D11Texture2D* texture)
{
    this->sample = sample;
    this->texture = texture;
    sample->AddRef();
    texture->AddRef();
    counter = 1;
}

TSampleTexture::~TSampleTexture()
{
    sample->Release();
    texture->Release();
}

HRESULT __stdcall TSampleTexture::QueryInterface(REFIID iid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppv)
{
    if (!ppv)
        return E_POINTER;
    if (iid == IID_IUnknown)
    {
        AddRef();
        *ppv = static_cast<IUnknown*>(this);
    }
    else if (iid == __uuidof(ID3D11Texture2D))
    {
        *ppv = texture;
        texture->AddRef();
    }
    else if (iid == IID_IMFSample)
    {
        *ppv = sample;
        sample->AddRef();
    }
    else {
        *ppv = NULL;
        return E_NOINTERFACE;
    }
    
    return S_OK;
}

ULONG __stdcall TSampleTexture::AddRef(void)
{
    return InterlockedIncrement(&counter);
}

ULONG __stdcall TSampleTexture::Release(void)
{
    counter = InterlockedDecrement(&counter);
    ULONG ret = counter;
    if (!counter)
        delete this;
    return ret;
}

HRESULT TSampleTexture::GetTSampleTexture(IMFSample* sample, ID3D11Texture2D* texture, TSampleTexture** text)
{
    if (!sample || !texture || !text)
        return E_POINTER;

    *text = new TSampleTexture(sample, texture);
    return S_OK;
}
