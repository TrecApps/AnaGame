#include "TPresentEngine.h"

HRESULT __stdcall TPresentEngine::GetService(REFGUID guidService, REFIID riid, LPVOID* ppvObject)
{
    return E_NOTIMPL;
}

RECT TPresentEngine::GetDestinationRect()
{
    return destRect;
}

HRESULT TPresentEngine::SetDestinationRect(RECT& rect)
{
    return E_NOTIMPL;
}
