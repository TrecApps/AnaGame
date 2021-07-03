#include "TVideoMarker.h"
#include <Shlwapi.h>
#include <Mferror.h>
TVideoMarker::~TVideoMarker()
{
    PropVariantClear(&marker);
    PropVariantClear(&context);
}

TVideoMarker::TVideoMarker(MFSTREAMSINK_MARKER_TYPE t): type(t)
{
    PropVariantInit(&marker);
    PropVariantInit(&context);
    counter = 0;
}

/**
 * Method: TVideoMarker::QueryInterface
 * Purpose: Retrieves a pointer to an object based off of the specified interface
 * Parameters: REFIID riid - the id of the interface to get
 *              void** ppv - where to place the pointer, if riid is supported
 * Returns: HRESULT - E_POINTER if ppv is null, E_NOINTERFACE if riid is not supported, or S_OK
 *
 * Note: Supported Interfaces are IUnknown, and IVideoMarker (Anagame Specific)
 */
HRESULT TVideoMarker::QueryInterface(REFIID riid, __RPC__deref_out _Result_nullonfailure_ void** ppv)
{
    static const QITAB qit[] = {
    QITABENT(IVideoMarker, IUnknown), {0}
    };
    return QISearch(this, qit, riid, ppv);
}

/**
 * Method: TVideoMarker::AddRef
 * Purpose: Increments the COM Reference
 * Parameters: void
 * Returns: ULONG - the new counter
 */
ULONG TVideoMarker::AddRef(void)
{
    return InterlockedIncrement(&counter);
}

/**
 * Method: TVideoMarker::Release
 * Purpose: Decrements the counter (possibly leading to deletion), to be called when code is done with this object
 * Parameters: void
 * Returns: ULONG - the counter set now (if zero, then this object should be deleted)
 */
ULONG TVideoMarker::Release(void)
{
    ULONG count = InterlockedDecrement(&counter);
    if (!count)
        delete this;
    return count;
}

HRESULT TVideoMarker::Create(MFSTREAMSINK_MARKER_TYPE eMarkerType, const PROPVARIANT* pvarMarkerValue, const PROPVARIANT* pvarContextValue, TVideoMarker** marker)
{
    if (!marker)
        return E_POINTER;
    TVideoMarker* cmarker = new TVideoMarker(eMarkerType);

    UCHAR failed = 0;

    if (pvarMarkerValue && FAILED(PropVariantCopy(&cmarker->marker, pvarMarkerValue)))
        failed++;
    if (pvarContextValue && FAILED(PropVariantCopy(&cmarker->context, pvarContextValue)))
        failed++;

    if (failed)
    {
        delete cmarker;
        cmarker = nullptr;
        return E_ABORT;
    }
    *marker = cmarker;
    return S_OK;
}

HRESULT TVideoMarker::GetMarkerType(MFSTREAMSINK_MARKER_TYPE* type)
{
    if(!type)
    return E_POINTER;
    *type = this->type;
    return S_OK;
}

HRESULT TVideoMarker::GetMarker(PROPVARIANT* var)
{
    if (!type)
        return E_POINTER;
    return PropVariantCopy(var, &marker);
}

HRESULT TVideoMarker::GetContext(PROPVARIANT* var)
{
    if (!type)
        return E_POINTER;
    return PropVariantCopy(var, &context);
}
