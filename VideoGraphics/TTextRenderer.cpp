#include "TTextRenderer.h"

HRESULT __stdcall TTextRenderer::QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppv)
{
    if (!ppv)
        return E_POINTER;
    if (riid == IID_IUnknown)
        *ppv = static_cast<IUnknown*>(this);
    else if (riid == __uuidof(IDWriteTextRenderer))
        *ppv = static_cast<IDWriteTextRenderer*>(this);
    else if (riid == __uuidof(IDWritePixelSnapping))
        *ppv = static_cast<IDWritePixelSnapping*>(this);
    else
    {
        *ppv = NULL;
        return E_NOINTERFACE;
    }
    AddRef();
    return S_OK;
}

ULONG __stdcall TTextRenderer::AddRef(void)
{
    return InterlockedIncrement(&counter);
}

ULONG __stdcall TTextRenderer::Release(void)
{
    UINT c = InterlockedDecrement(&counter);
    if (!c)
        delete this;
    return c;
}

STDMETHODIMP_(HRESULT __stdcall) TTextRenderer::DrawGlyphRun(_In_opt_ void* clientDrawingContext,
    FLOAT baselineOriginX, 
    FLOAT baselineOriginY, 
    DWRITE_MEASURING_MODE measuringMode, 
    _In_ DWRITE_GLYPH_RUN const* glyphRun,
    _In_ DWRITE_GLYPH_RUN_DESCRIPTION const* glyphRunDescription,
    _In_opt_ IUnknown* clientDrawingEffect)
{
    ID2D1RenderTarget* renderer = static_cast<ID2D1RenderTarget*>(clientDrawingContext);
    if (!renderer || !clientDrawingEffect)
        return E_POINTER;

    IDoubleBrushHolder* bHolder = nullptr;
    ID2D1Brush* fBrush = nullptr;
    HRESULT ret =clientDrawingEffect->QueryInterface<ID2D1Brush>(&fBrush);
    if (FAILED(ret))
        return ret;
    ret = clientDrawingEffect->QueryInterface<IDoubleBrushHolder>(&bHolder);
    ID2D1Brush* bBrush = nullptr;
    if (SUCCEEDED(ret) && SUCCEEDED(bHolder->GetBackgroundBrush(&bBrush)))
    {
        // Get width of text
        float totalWidth = 0;

        for (UINT32 index = 0; index < glyphRun->glyphCount; index++)
        {
            totalWidth += glyphRun->glyphAdvances[index];
        }

        // Get height of text
        DWRITE_FONT_METRICS fontMetrics;
        glyphRun->fontFace->GetMetrics(&fontMetrics);
        float adjust = glyphRun->fontEmSize / fontMetrics.designUnitsPerEm;
        float ascent = adjust * fontMetrics.ascent;
        float descent = adjust * fontMetrics.descent;
        D2D1_RECT_F rect = D2D1::RectF(baselineOriginX,
            baselineOriginY - ascent,
            baselineOriginX + totalWidth,
            baselineOriginY + descent);

        // Fill Rectangle
        renderer->FillRectangle(rect, bBrush);
    }
    renderer->DrawGlyphRun(D2D1::Point2F(baselineOriginX, baselineOriginY),
        glyphRun, fBrush, measuringMode);

    return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) TTextRenderer::DrawUnderline(_In_opt_ void* clientDrawingContext,
    FLOAT baselineOriginX,
    FLOAT baselineOriginY,
    _In_ DWRITE_UNDERLINE const* underline,
    _In_opt_ IUnknown* clientDrawingEffect)
{
    ID2D1RenderTarget* renderer = static_cast<ID2D1RenderTarget*>(clientDrawingContext);
    if (!renderer || !clientDrawingEffect)
        return E_POINTER;
    ID2D1Brush* fBrush = nullptr;
    HRESULT ret = clientDrawingEffect->QueryInterface<ID2D1Brush>(&fBrush);
    if (FAILED(ret))
        return ret;

    D2D1_RECT_F loc = D2D1::RectF(baselineOriginX, baselineOriginY + underline->offset,
        baselineOriginX + underline->width, baselineOriginY + underline->offset + underline->thickness);

    renderer->FillRectangle(loc, fBrush);

    return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) TTextRenderer::DrawStrikethrough(_In_opt_ void* clientDrawingContext,
    FLOAT baselineOriginX,
    FLOAT baselineOriginY, 
    _In_ DWRITE_STRIKETHROUGH const* strikethrough,
    _In_opt_ IUnknown* clientDrawingEffect)
{
    ID2D1RenderTarget* renderer = static_cast<ID2D1RenderTarget*>(clientDrawingContext);
    if (!renderer || !clientDrawingEffect)
        return E_POINTER;
    ID2D1Brush* fBrush = nullptr;
    HRESULT ret = clientDrawingEffect->QueryInterface<ID2D1Brush>(&fBrush);
    if (FAILED(ret))
        return ret;

    D2D1_RECT_F loc = D2D1::RectF(baselineOriginX, baselineOriginY + strikethrough->offset,
        baselineOriginX + strikethrough->width, baselineOriginY + strikethrough->offset + strikethrough->thickness);

    renderer->FillRectangle(loc, fBrush);

    return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) TTextRenderer::DrawInlineObject(_In_opt_ void* clientDrawingContext, FLOAT originX, FLOAT originY, _In_ IDWriteInlineObject* inlineObject, BOOL isSideways, BOOL isRightToLeft, _In_opt_ IUnknown* clientDrawingEffect)
{
    return inlineObject->Draw(clientDrawingContext,
        this,
        originX,
        originY,
        isSideways,
        isRightToLeft,
        clientDrawingEffect);
}

STDMETHODIMP_(HRESULT __stdcall) TTextRenderer::IsPixelSnappingDisabled(_In_opt_ void* clientDrawingContext, _Out_ BOOL* isDisabled)
{
    if (!isDisabled)
        return E_POINTER;
    *isDisabled = false;
    return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) TTextRenderer::GetCurrentTransform(_In_opt_ void* clientDrawingContext, _Out_ DWRITE_MATRIX* transform)
{
    ID2D1RenderTarget* renderer = static_cast<ID2D1RenderTarget*>(clientDrawingContext);
    if (!renderer || !transform)
        return E_POINTER;

    renderer->GetTransform((D2D1_MATRIX_3X2_F*)transform);
    return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) TTextRenderer::GetPixelsPerDip(_In_opt_ void* clientDrawingContext, _Out_ FLOAT* pixelsPerDip)
{
    ID2D1RenderTarget* renderer = static_cast<ID2D1RenderTarget*>(clientDrawingContext);
    if (!renderer || !pixelsPerDip)
        return E_POINTER;
    float dpiX = 0.0f, dpiY = 0.0f;
    renderer->GetDpi(&dpiX, &dpiY);
    *pixelsPerDip = dpiX / 96;
    return S_OK;
}

HRESULT __stdcall TDoubleBrushHolder::GetBackgroundBrush(ID2D1Brush** brush)
{
    HRESULT ret = S_OK;
    if (backBrush)
        *brush = backBrush;
    else
        ret = E_POINTER;
    return ret;
}

HRESULT __stdcall TDoubleBrushHolder::QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppv)
{
    if (!ppv)
        return E_POINTER;
    HRESULT ret = S_OK;
    if (riid == IID_IUnknown)
        *ppv = static_cast<IUnknown*>(this);
    else if (riid == __uuidof(ID2D1Brush))
    {
        *ppv = frontBrush;
        if (!frontBrush)
            ret = E_POINTER;
    }
    else if (riid == __uuidof(IDoubleBrushHolder))
        *ppv = static_cast<IDoubleBrushHolder*>(this);
    else
    {
        *ppv = NULL;
        return E_NOINTERFACE;
    }
    AddRef();
    return ret;
}

ULONG __stdcall TDoubleBrushHolder::AddRef(void)
{
    return InterlockedIncrement(&counter);
}

ULONG __stdcall TDoubleBrushHolder::Release(void)
{
    UINT c = InterlockedDecrement(&counter);
    if (!c)
        delete this;
    return c;
}

TDoubleBrushHolder::TDoubleBrushHolder(ID2D1Brush* f, ID2D1Brush* b)
{
    frontBrush = f;
    backBrush = b;
    if (f)f->AddRef();
    if (b)b->AddRef();
    counter = 1;
}

TDoubleBrushHolder::~TDoubleBrushHolder()
{
    if (frontBrush)
        frontBrush->Release();
    if (backBrush)
        backBrush->Release();
    frontBrush = backBrush = nullptr;
}
