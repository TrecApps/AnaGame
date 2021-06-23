#include "TTextRenderer.h"

HRESULT __stdcall TTextRenderer::QueryInterface(REFIID riid, void** ppvObject)
{
    return E_NOTIMPL;
}

ULONG __stdcall TTextRenderer::AddRef(void)
{
    return 0;
}

ULONG __stdcall TTextRenderer::Release(void)
{
    return 0;
}

STDMETHODIMP_(HRESULT __stdcall) TTextRenderer::DrawGlyphRun(void* clientDrawingContext, FLOAT baselineOriginX, FLOAT baselineOriginY, DWRITE_MEASURING_MODE measuringMode, DWRITE_GLYPH_RUN const* glyphRun, DWRITE_GLYPH_RUN_DESCRIPTION const* glyphRunDescription, IUnknown* clientDrawingEffect)
{
    return E_NOTIMPL;
}

STDMETHODIMP_(HRESULT __stdcall) TTextRenderer::DrawUnderline(void* clientDrawingContext, FLOAT baselineOriginX, FLOAT baselineOriginY, DWRITE_UNDERLINE const* underline, IUnknown* clientDrawingEffect)
{
    return E_NOTIMPL;
}

STDMETHODIMP_(HRESULT __stdcall) TTextRenderer::DrawStrikethrough(void* clientDrawingContext, FLOAT baselineOriginX, FLOAT baselineOriginY, DWRITE_STRIKETHROUGH const* strikethrough, IUnknown* clientDrawingEffect)
{
    return E_NOTIMPL;
}

STDMETHODIMP_(HRESULT __stdcall) TTextRenderer::DrawInlineObject(void* clientDrawingContext, FLOAT originX, FLOAT originY, IDWriteInlineObject* inlineObject, BOOL isSideways, BOOL isRightToLeft, IUnknown* clientDrawingEffect)
{
    return E_NOTIMPL;
}

STDMETHODIMP_(HRESULT __stdcall) TTextRenderer::IsPixelSnappingDisabled(void* clientDrawingContext, BOOL* isDisabled)
{
    return E_NOTIMPL;
}

STDMETHODIMP_(HRESULT __stdcall) TTextRenderer::GetCurrentTransform(void* clientDrawingContext, DWRITE_MATRIX* transform)
{
    return E_NOTIMPL;
}

STDMETHODIMP_(HRESULT __stdcall) TTextRenderer::GetPixelsPerDip(void* clientDrawingContext, FLOAT* pixelsPerDip)
{
    return E_NOTIMPL;
}
