#pragma once
#include <dwrite.h>
#include <d2d1.h>
#include "VideoGraphics.h"


MIDL_INTERFACE("616F37A8-C34B-450D-9707-B2AA73223636") IDoubleBrushHolder : public IUnknown
{
    virtual HRESULT STDMETHODCALLTYPE GetBackgroundBrush(ID2D1Brush** brush) =0;
};

class _VIDEO_GRAPHICS TDoubleBrushHolder : public IDoubleBrushHolder
{
public:
    virtual HRESULT STDMETHODCALLTYPE GetBackgroundBrush(ID2D1Brush** brush) override;

    virtual HRESULT STDMETHODCALLTYPE QueryInterface(
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

    virtual ULONG STDMETHODCALLTYPE AddRef(void) override;

    virtual ULONG STDMETHODCALLTYPE Release(void) override;

    TDoubleBrushHolder(ID2D1Brush* f, ID2D1Brush* b);

    ~TDoubleBrushHolder();

private:
    ID2D1Brush* frontBrush, *backBrush;
    UINT counter;
};


class _VIDEO_GRAPHICS TTextRenderer :
    public IDWriteTextRenderer
{
public:
    TTextRenderer();

    // IUnknown
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

    virtual ULONG STDMETHODCALLTYPE AddRef(void) override;

    virtual ULONG STDMETHODCALLTYPE Release(void) override;

    // IDWrite Text Renderer

    STDMETHOD(DrawGlyphRun)(
        _In_opt_ void* clientDrawingContext,
        FLOAT baselineOriginX,
        FLOAT baselineOriginY,
        DWRITE_MEASURING_MODE measuringMode,
        _In_ DWRITE_GLYPH_RUN const* glyphRun,
        _In_ DWRITE_GLYPH_RUN_DESCRIPTION const* glyphRunDescription,
        _In_opt_ IUnknown* clientDrawingEffect
        )override;

    STDMETHOD(DrawUnderline)(
        _In_opt_ void* clientDrawingContext,
        FLOAT baselineOriginX,
        FLOAT baselineOriginY,
        _In_ DWRITE_UNDERLINE const* underline,
        _In_opt_ IUnknown* clientDrawingEffect
        )override;

    STDMETHOD(DrawStrikethrough)(
        _In_opt_ void* clientDrawingContext,
        FLOAT baselineOriginX,
        FLOAT baselineOriginY,
        _In_ DWRITE_STRIKETHROUGH const* strikethrough,
        _In_opt_ IUnknown* clientDrawingEffect
        )override;

    STDMETHOD(DrawInlineObject)(
        _In_opt_ void* clientDrawingContext,
        FLOAT originX,
        FLOAT originY,
        _In_ IDWriteInlineObject* inlineObject,
        BOOL isSideways,
        BOOL isRightToLeft,
        _In_opt_ IUnknown* clientDrawingEffect
        ) override;

    // IDWritePixelSnapping

    STDMETHOD(IsPixelSnappingDisabled)(
        _In_opt_ void* clientDrawingContext,
        _Out_ BOOL* isDisabled
        )override;

    STDMETHOD(GetCurrentTransform)(
        _In_opt_ void* clientDrawingContext,
        _Out_ DWRITE_MATRIX* transform
        ) override;

    STDMETHOD(GetPixelsPerDip)(
        _In_opt_ void* clientDrawingContext,
        _Out_ FLOAT* pixelsPerDip
        ) override;

private:
    UINT counter;
};

