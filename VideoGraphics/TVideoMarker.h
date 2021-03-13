#pragma once
#include <Unknwnbase.h>
#include <mfidl.h>

class TVideoMarker: public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

    virtual ULONG STDMETHODCALLTYPE AddRef(void) override;

    virtual ULONG STDMETHODCALLTYPE Release(void) override;

    static HRESULT Create(MFSTREAMSINK_MARKER_TYPE eMarkerType,
        const PROPVARIANT* pvarMarkerValue,
        const PROPVARIANT* pvarContextValue,
        TVideoMarker** marker);

    HRESULT GetMarkerType(MFSTREAMSINK_MARKER_TYPE* type);
    HRESULT GetMarker(PROPVARIANT* var);
    HRESULT GetContext(PROPVARIANT* var);
    

private:
    virtual ~TVideoMarker();
    TVideoMarker(MFSTREAMSINK_MARKER_TYPE t);
    long counter;
    MFSTREAMSINK_MARKER_TYPE type;
    PROPVARIANT marker, context;

};

