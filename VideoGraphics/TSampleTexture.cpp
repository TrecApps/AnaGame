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

/**
 * Method: TSampleTexture::QueryInterface
 * Purpose: Retrieves a pointer to an object based off of the specified interface
 * Parameters: REFIID riid - the id of the interface to get
 *              void** ppv - where to place the pointer, if riid is supported
 * Returns: HRESULT - E_POINTER if ppv is null, E_NOINTERFACE if riid is not supported, or S_OK
 *
 * Note: Supported Interfaces are IUnknown, and ISampleTexture (unique to Anagame)
 */
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
    else if (iid == __uuidof(ISampleTexture))
    {
        *ppv = static_cast<ISampleTexture*>(this);
        AddRef();
    }
    else {
        *ppv = NULL;
        return E_NOINTERFACE;
    }
    
    return S_OK;
}

/**
 * Method: TSampleTexture::AddRef
 * Purpose: Increments the COM Reference
 * Parameters: void
 * Returns: ULONG - the new counter
 */
ULONG __stdcall TSampleTexture::AddRef(void)
{
    return InterlockedIncrement(&counter);
}

/**
 * Method: TSampleTexture::Release
 * Purpose: Decrements the counter (possibly leading to deletion), to be called when code is done with this object
 * Parameters: void
 * Returns: ULONG - the counter set now (if zero, then this object should be deleted)
 */
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
