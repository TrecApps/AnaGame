#pragma once
#include <Unknwnbase.h>
#include <mfidl.h>

/**
 * Interface: IVideoMarker
 * Purpose: Provides a GUID for the TVideoMarkerClass
 */
MIDL_INTERFACE("F06BDDE7-AA0A-4D9A-A833-A945481820CC")
IVideoMarker: public IUnknown
{

};

/**
 * Class: TVideoMarker
 * Purpoe: Manages Markers
 */
class TVideoMarker: public IVideoMarker
{
public:
    /**
     * Method: TVideoMarker::QueryInterface
     * Purpose: Retrieves a pointer to an object based off of the specified interface
     * Parameters: REFIID riid - the id of the interface to get
     *              void** ppv - where to place the pointer, if riid is supported
     * Returns: HRESULT - E_POINTER if ppv is null, E_NOINTERFACE if riid is not supported, or S_OK
     *
     * Note: Supported Interfaces are IUnknown, and IVideoMarker (Anagame Specific)
     */
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

    /**
     * Method: TVideoMarker::AddRef
     * Purpose: Increments the COM Reference
     * Parameters: void
     * Returns: ULONG - the new counter
     */
    virtual ULONG STDMETHODCALLTYPE AddRef(void) override;

    /**
     * Method: TVideoMarker::Release
     * Purpose: Decrements the counter (possibly leading to deletion), to be called when code is done with this object
     * Parameters: void
     * Returns: ULONG - the counter set now (if zero, then this object should be deleted)
     */
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

