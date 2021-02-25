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

HRESULT TVideoMarker::QueryInterface(REFIID riid, __RPC__deref_out _Result_nullonfailure_ void** ppv)
{
    static const QITAB qit[] = {
    QITABENT(TVideoMarker, IUnknown), {0}
    };
    return QISearch(this, qit, riid, ppv);
}

ULONG TVideoMarker::AddRef(void)
{
    return InterlockedIncrement(&counter);
}

ULONG TVideoMarker::Release(void)
{
    ULONG count = InterlockedDecrement(&counter);
    if (!count)
        delete this;
    return count;
}

HRESULT TVideoMarker::Create(MFSTREAMSINK_MARKER_TYPE eMarkerType, const PROPVARIANT* pvarMarkerValue, const PROPVARIANT* pvarContextValue, const TVideoMarker** marker)
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
