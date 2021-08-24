#pragma once
#include <Unknwnbase.h>
#include <d3d11.h>
#include <mfobjects.h>


MIDL_INTERFACE("FC749371-E1F6-42D2-9C55-C87098767D49") ISampleTexture :
    public IUnknown
{

};

class TSampleTexture : public ISampleTexture
{
public:
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(
        REFIID riid,
        _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

    virtual ULONG STDMETHODCALLTYPE AddRef(void) override;

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