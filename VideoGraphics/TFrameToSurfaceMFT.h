#pragma once
#include <mfapi.h>
#include <d3d11.h>
#include <mftransform.h>
#include <TLinkedList.h>
#include <evr.h>
#include <TDataArray.h>
#include <D3D9Types.h>
#include <dxva2api.h>

/**
 * Class: IMFTransform
 * Purpose: Makes sure that the samples are capable of supplying Direct3D11 surfaces
 */
class TFrameToSurfaceMFT :
    public IMFTransform, public TObject
{
public:
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

    virtual ULONG STDMETHODCALLTYPE AddRef(void) override;

    virtual ULONG STDMETHODCALLTYPE Release(void) override;


    virtual HRESULT STDMETHODCALLTYPE GetStreamLimits(
        __RPC__out DWORD* pdwInputMinimum,
        __RPC__out DWORD* pdwInputMaximum,
        __RPC__out DWORD* pdwOutputMinimum,
        __RPC__out DWORD* pdwOutputMaximum) override;

    virtual HRESULT STDMETHODCALLTYPE GetStreamCount(
        __RPC__out DWORD* pcInputStreams,
        __RPC__out DWORD* pcOutputStreams) override;

    virtual HRESULT STDMETHODCALLTYPE GetStreamIDs(
        DWORD dwInputIDArraySize,
        __RPC__out_ecount_full(dwInputIDArraySize) DWORD* pdwInputIDs,
        DWORD dwOutputIDArraySize,
        __RPC__out_ecount_full(dwOutputIDArraySize) DWORD* pdwOutputIDs) override;

    virtual HRESULT STDMETHODCALLTYPE GetInputStreamInfo(
        DWORD dwInputStreamID,
        __RPC__out MFT_INPUT_STREAM_INFO* pStreamInfo) override;

    virtual HRESULT STDMETHODCALLTYPE GetOutputStreamInfo(
        DWORD dwOutputStreamID,
        __RPC__out MFT_OUTPUT_STREAM_INFO* pStreamInfo) override;

    virtual HRESULT STDMETHODCALLTYPE GetAttributes(
        __RPC__deref_out_opt IMFAttributes** pAttributes) override;

    virtual HRESULT STDMETHODCALLTYPE GetInputStreamAttributes(
        DWORD dwInputStreamID,
        __RPC__deref_out_opt IMFAttributes** pAttributes) override;

    virtual HRESULT STDMETHODCALLTYPE GetOutputStreamAttributes(
        DWORD dwOutputStreamID,
        __RPC__deref_out_opt IMFAttributes** pAttributes) override;

    virtual HRESULT STDMETHODCALLTYPE DeleteInputStream(
        DWORD dwStreamID) override;

    virtual HRESULT STDMETHODCALLTYPE AddInputStreams(
        DWORD cStreams,
        __RPC__in DWORD* adwStreamIDs) override;

    virtual HRESULT STDMETHODCALLTYPE GetInputAvailableType(
        DWORD dwInputStreamID,
        DWORD dwTypeIndex,
        __RPC__deref_out_opt IMFMediaType** ppType) override;

    virtual HRESULT STDMETHODCALLTYPE GetOutputAvailableType(
        DWORD dwOutputStreamID,
        DWORD dwTypeIndex,
        __RPC__deref_out_opt IMFMediaType** ppType) override;

    virtual HRESULT STDMETHODCALLTYPE SetInputType(
        DWORD dwInputStreamID,
        __RPC__in_opt IMFMediaType* pType,
        DWORD dwFlags) override;

    virtual HRESULT STDMETHODCALLTYPE SetOutputType(
        DWORD dwOutputStreamID,
        __RPC__in_opt IMFMediaType* pType,
        DWORD dwFlags) override;

    virtual HRESULT STDMETHODCALLTYPE GetInputCurrentType(
        DWORD dwInputStreamID,
        __RPC__deref_out_opt IMFMediaType** ppType) override;

    virtual HRESULT STDMETHODCALLTYPE GetOutputCurrentType(
        DWORD dwOutputStreamID,
        __RPC__deref_out_opt IMFMediaType** ppType) override;

    virtual HRESULT STDMETHODCALLTYPE GetInputStatus(
        DWORD dwInputStreamID,
        __RPC__out DWORD* pdwFlags) override;

    virtual HRESULT STDMETHODCALLTYPE GetOutputStatus(
        __RPC__out DWORD* pdwFlags) override;

    virtual HRESULT STDMETHODCALLTYPE SetOutputBounds(
        LONGLONG hnsLowerBound,
        LONGLONG hnsUpperBound) override;

    virtual HRESULT STDMETHODCALLTYPE ProcessEvent(
        DWORD dwInputStreamID,
        __RPC__in_opt IMFMediaEvent* pEvent) override;

    virtual HRESULT STDMETHODCALLTYPE ProcessMessage(
        MFT_MESSAGE_TYPE eMessage,
        ULONG_PTR ulParam) override;

    virtual HRESULT STDMETHODCALLTYPE ProcessInput(
        DWORD dwInputStreamID,
        IMFSample* pSample,
        DWORD dwFlags) override;

    virtual HRESULT STDMETHODCALLTYPE ProcessOutput(
        DWORD dwFlags,
        DWORD cOutputBufferCount,
        MFT_OUTPUT_DATA_BUFFER* pOutputSamples,
        DWORD* pdwStatus) override;

    HRESULT CreateInstance(TFrameToSurfaceMFT** in);

    protected:

        TFrameToSurfaceMFT();
        ~TFrameToSurfaceMFT();

        // Helper Methods
        HRESULT SetUpDevices();
        HRESULT OnFlush();
        HRESULT OnDiscontinue();
        HRESULT AllocateStreamers();
        HRESULT FreeStreamers();



        TLinkedList<IMFSample*> samples;

        // Media Types
        TrecComPointer<IMFMediaType> mediaInputType, outputType;
        
        // Data management
        IMFMediaBuffer* mediaBuffer;

        // Format Info
        DWORD imageSize;
        UINT pixelHeight, pixelWidth;
        MFRatio frameRate;

        IMFDXGIDeviceManager* manager;
        IDirectXVideoProcessor* vService;
        ID3D11VideoDevice* vDevice;
        ID3D11Device* device;
        DWORD inputId, outputId;
        UINT decoderCount;
        TDataArray<GUID> guids;
        TDataArray<D3D11_VIDEO_DECODER_CONFIG> decodeConfigs;
        GUID inputType;
        D3D11_VIDEO_DECODER_DESC decodeDesc;
        D3D11_VIDEO_DECODER_OUTPUT_VIEW_DESC outputDesc;

        IMFAttributes* atts;
        ULONG counter;
        HANDLE devHand;
        D3D11_TEXTURE2D_DESC textureDesc;
};

