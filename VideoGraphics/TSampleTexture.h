#pragma once
#include <Unknwnbase.h>
#include <d3d11.h>
#include <mfobjects.h>

/**
 * Interface: ISampleTextures
 * Purpose: Provides a GUID for the TSampleTexture class
 */
MIDL_INTERFACE("FC749371-E1F6-42D2-9C55-C87098767D49") ISampleTexture :
    public IUnknown
{

};

/**
 * Class: TSampleTexture
 * Purpose: Holds Both an IMFSample and a Direct-3D Texture
 */
class TSampleTexture : public ISampleTexture
{
public:
    /**
     * Method: TSampleTexture::QueryInterface
     * Purpose: Retrieves a pointer to an object based off of the specified interface
     * Parameters: REFIID riid - the id of the interface to get
     *              void** ppv - where to place the pointer, if riid is supported
     * Returns: HRESULT - E_POINTER if ppv is null, E_NOINTERFACE if riid is not supported, or S_OK
     *
     * Note: Supported Interfaces are IUnknown, and ISampleTexture (unique to Anagame)
     */
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(
        REFIID riid,
        _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

    /**
     * Method: TSampleTexture::AddRef
     * Purpose: Increments the COM Reference
     * Parameters: void
     * Returns: ULONG - the new counter
     */
    virtual ULONG STDMETHODCALLTYPE AddRef(void) override;

    /**
     * Method: TSampleTexture::Release
     * Purpose: Decrements the counter (possibly leading to deletion), to be called when code is done with this object
     * Parameters: void
     * Returns: ULONG - the counter set now (if zero, then this object should be deleted)
     */
    virtual ULONG STDMETHODCALLTYPE Release(void) override;

    static HRESULT GetTSampleTexture(IMFSample* sample,
        ID3D11Texture2D* texture, TSampleTexture** text);
private:
    TSampleTexture(IMFSample* sample, ID3D11Texture2D* texture);
    ~TSampleTexture();
    IMFSample* sample;
    ID3D11Texture2D* texture;
    UINT counter;
};