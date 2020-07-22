#include "TBrush.h"
#include "DrawingBoard.h"
#include "TGeometry.h"
#include <atltrace.h>

/**
 * Method: TBrush::DrawRectangle
 * Purpose: draws the given Rectangle on the DrawingBoard
 * Parameters: const RECT_2D& r - The Rectangle to apply
 *				float thickness -  the thickness of the line (default is 1.0f)
 * Returns: void
 */
void TBrush::DrawRectangle(const RECT_2D& r, float thickness)
{
	if (Refresh())
	{
		currentRenderer->DrawRectangle(r, brush.Get(), thickness);
	}
}

/**
 * Method: TBrush::FillRectangle
 * Purpose: fills the given Rectangle on the DrawingBoard
 * Parameters: const RECT_2D& r - The Rectangle to apply
 * Returns: void
 */
void TBrush::FillRectangle(const RECT_2D& r)
{
	if (Refresh())
	{
		currentRenderer->FillRectangle(r, brush.Get());
	}
}

/**
 * Method: TBrush::DrawRoundedRectangle
 * Purpose: draws the given Rounded Rectangle on the DrawingBoard
 * Parameters: const ROUNDED_RECT_2D& r - The Rounded Rectangle to apply
 *				float thickness -  the thickness of the line (default is 1.0f)
 * Returns: void
 */
void TBrush::DrawRoundedRectangle(const ROUNDED_RECT_2D& r, float thickness)
{
	if (Refresh())
	{
		currentRenderer->DrawRoundedRectangle(r, brush.Get(), thickness);
	}
}

/**
 * Method: TBrush::FillRoundedRectangle
 * Purpose: fills the given Rounded Rectangle on the DrawingBoard
 * Parameters: const ROUNDED_RECT_2D& r - The Rounded Rectangle to apply
 * Returns: void
 */
void TBrush::FillRoundedRectangle(const ROUNDED_RECT_2D& r)
{
	if (Refresh())
	{
		currentRenderer->FillRoundedRectangle(r, brush.Get());
	}
}

/**
 * Method: TBrush::DrawEllipse
 * Purpose: draws the given Ellipse on the DrawingBoard
 * Parameters: const ELLIPSE_2D& r - The Ellipse to apply
 *				float thickness -  the thickness of the line (default is 1.0f)
 * Returns: void
 */
void TBrush::DrawEllipse(const ELLIPSE_2D& r, float thickness)
{
	if (Refresh())
	{
		currentRenderer->DrawEllipse(r, brush.Get(), thickness);
	}
}

/**
 * Method: TBrush::FillEllipse
 * Purpose: fills the given Ellipse on the DrawingBoard
 * Parameters:const ELLIPSE_2D& r - The Ellipse to apply
 * Returns: void
 */
void TBrush::FillEllipse(const ELLIPSE_2D& r)
{
	if (Refresh())
	{
		currentRenderer->FillEllipse(r, brush.Get());
	}
}

/**
 * Method: TBrush::DrawGeometry
 * Purpose: draws the given Geometry on the DrawingBoard
 * Parameters: TrecPointer<TGeometry> geo - The Geometry to apply
 *				float thickness -  the thickness of the line (default is 1.0f)
 * Returns: void
 */
void TBrush::DrawGeometry(TrecPointer<TGeometry> geo, float thickness)
{
	if (Refresh() && geo.Get() && geo->IsValid())
	{
		currentRenderer->DrawGeometry(geo->GetUnderlyingGeometry().Get(), brush.Get(), thickness);
	}
}

/**
 * Method: TBrush::FillGeometry
 * Purpose: fills the given Geometry on the DrawingBoard
 * Parameters: TrecPointer<TGeometry> geo - The Geometry to apply
 * Returns: void
 */
void TBrush::FillGeometry(TrecPointer<TGeometry> geo)
{
	if (Refresh() && geo.Get() && geo->IsValid())
	{
		currentRenderer->FillGeometry(geo->GetUnderlyingGeometry().Get(), brush.Get());
	}
}

/**
 * Method: TBrush::DrawLine
 * Purpose: Draws a specific line to the Drawing Board
 * Parameters: const POINT_2D& p1 - the beginning of the line
 *				const POINT_2D& p2 - the end of the line
 *				float thickness -  the thickness of the line (default is 1.0f)
 * Returns: void
 */
void TBrush::DrawLine(const POINT_2D& p1, const POINT_2D& p2, float thickness)
{
	if (Refresh())
	{
		currentRenderer->DrawLine(p1, p2, brush.Get(), thickness);
	}
}

/**
 * Method: TBrush::GetMaxColors
 * Purpose: Reports the number of gradients held by the brush. Use to ensure successful calls to SetColor() and GetColor()
 * Parameters: void
 * Returns: UINT - the Number of colors held by the Brush
 */
UINT TBrush::GetMaxColors()
{
	if (brushType == brush_type::brush_type_solid)
		return 1;
	return gradients.GetGradientCount();
}

/**
 * Method: TBrush::SetColor
 * Purpose: Sets one of the gradients to a specific color
 * Parameters: const TColor& color - the color to set the brush to
 *				UINT index - the index of the gradient to apply the color to (default is 0 for solid brushes will only have 1 gradient)
 * Returns: void
 *
 * Note: if the index is out of bounds, the operation will silently fail. to ensure success, call the GetMaxColors() to ensure your index is within bounds
 */
void TBrush::SetColor(const TColor& color, UINT index)
{
	switch (brushType)
	{
	case brush_type::brush_type_solid:
		ATLTRACE(L"SOLID BRUSH TYPE\n");
		break;
	case brush_type::brush_type_linear:
		ATLTRACE(L"LINEAR BRUSH TYPE\n");
		break;
	case brush_type::brush_type_radial:
		ATLTRACE(L"RADIAL BRUSH TYPE\n");
		break;
	case brush_type::brush_type_bitmap:
		ATLTRACE(L"BITMAP BRUSH TYPE\n");
		break;
	default:
		ATLTRACE(L"OTHER BRUSH TYPE\n");
	}


	if (brushType == brush_type::brush_type_solid)
	{
		
		if (brush.Get())
		{
			reinterpret_cast<ID2D1SolidColorBrush*>(brush.Get())->SetColor(color.GetColor());
			ATLTRACE(L"Set color!");
		}
		else
			ATLTRACE(L"COLD NOT GET BRUSH!");
	}
	else
	{
		gradients.SetColorAt(color, index);
	}
}

/**
 * Method: TBrush::GetColor
 * Purpose: Retrieves the Specified color being drawn by the brush
 * Parameters: UINT index - the index of the gradient to get the color from (defaults to 0 as a solid brush will only have 1 gradient)
 * Returns: TColor - the color requested
 *
 * Note: If the index is out of bounds, the color black will be returned. To ensure that the returned color is legitimate, call GetMaxColors()
 */
TColor TBrush::GetColor(UINT index)
{
	if (brushType == brush_type::brush_type_solid)
	{
		TColor col;
		if (brush.Get())
			col.SetColor(reinterpret_cast<ID2D1SolidColorBrush*>(brush.Get())->GetColor());
		return col;
	}
	return gradients.GetColorAt(index);
}

/**
 * Method: TBrush::GetBrushType
 * Purpose:  Reports the type of brush this TBrush is using
 * Parameters: void
 * Returns: brush_type the type of brush the TBrush is set to
 */
brush_type TBrush::GetBrushType()
{
	return brushType;
}

/**
 * Method: TBrush::GetUnderlyingBrush
 * Purpose: Retrieves the underlying brush
 * Parameters: void
 * Returns: TrecComPointer<ID2D1Brush> - the underlying brush
 */
TrecComPointer<ID2D1Brush> TBrush::GetUnderlyingBrush()
{
	if(Refresh())
		return brush;
	return TrecComPointer<ID2D1Brush>();
}

/**
 * Method: TBrush::~TBrush
 * Purpose: Destructor
 * Parameters: void
 * Returns: void
 */
TBrush::~TBrush()
{
}

/**
 * Method: TBrush::TBrush
 * Purpose:  Constructor that sets the brush to draw solid colors
 * Parameters: const TColor& col - the initial color to set the brush to
 *				TrecPointer<DrawingBoard> rt - the Drawing Board that created this brush
 * Returns: New TBrush holder optimized for a solid color
 */
TBrush::TBrush(const TColor& col, TrecPointer<DrawingBoard> rt)
{
	if (!rt.Get())
		throw L"ERROR! Needed Render Target to Not Be Null";

	currentRenderer = rt->GetRenderer();
	if (!currentRenderer.Get())
		throw L"Error! Drawing Board had a NULL Render Target";

	board = rt;

	TrecComPointer<ID2D1SolidColorBrush>::TrecComHolder brushHolder;

	currentRenderer->CreateSolidColorBrush(col.GetColor(), brushHolder.GetPointerAddress());

	brush = TrecPointerKey::GetComPointer<ID2D1Brush, ID2D1SolidColorBrush>(brushHolder);
	brushType = brush_type::brush_type_solid;

	gradients.AddGradient(TGradientStop(col, 0.0f));
}

/**
 * Method: TBrush::TBrush
 * Purpose: Constructor that sets the brush to draw radial gradients
 * Parameters: const TGradientStopCollection& coll - the collection of colors to work with
 *				TrecPointer<DrawingBoard> rt - the Drawing Board that created this brush
 *				const POINT_2D& p1 -Center of the gradient ellipse
 *				const POINT_2D& p2 - offset of the gradient origin
 *				float x - x-radius of the ellipse center
 *				float y - y-radius of the ellipse center
 * Returns: New TBrush holder optimized for linar gradient drawing
 */
TBrush::TBrush(const TGradientStopCollection& coll, TrecPointer<DrawingBoard> rt, const POINT_2D& p1, const POINT_2D& p2, float x, float y)
{
	if (!rt.Get())
		throw L"ERROR! Needed Render Target to Not Be Null";

	currentRenderer = rt->GetRenderer();
	if (!currentRenderer.Get())
		throw L"Error! Drawing Board had a NULL Render Target";

	board = rt;
	gradients = coll;

	ID2D1GradientStopCollection* stopColl = nullptr;
	auto rawGradients = gradients.GetRawCollection();
	currentRenderer->CreateGradientStopCollection(rawGradients.data(), rawGradients.Size(), &stopColl);
	TrecComPointer<ID2D1RadialGradientBrush>::TrecComHolder radBrush;
	currentRenderer->CreateRadialGradientBrush(D2D1::RadialGradientBrushProperties(
		p1,
		p2,
		x, y),
		stopColl, radBrush.GetPointerAddress());

	brush = TrecPointerKey::GetComPointer<ID2D1Brush, ID2D1RadialGradientBrush>(radBrush);

	stopColl->Release();
	stopColl = nullptr;
	brushType = brush_type::brush_type_radial;
}

/**
 * Method: TBrush::TBrush
 * Purpose: Constructor that sets the brush to draw linear gradients
 * Parameters: const TGradientStopCollection& coll - the collection of colors to work with
 *				TrecPointer<DrawingBoard> rt - the Drawing Board that created this brush
 *				const POINT_2D& p1 - one point the line is expected to pass through
 *				const POINT_2D& p2 - second point line is expected to pass through
 * Returns: New TBrush holder optimized for linar gradient drawing
 */
TBrush::TBrush(const TGradientStopCollection& coll, TrecPointer<DrawingBoard> rt, const POINT_2D& p1, const POINT_2D& p2)
{
	if (!rt.Get())
		throw L"ERROR! Needed Render Target to Not Be Null";

	currentRenderer = rt->GetRenderer();
	if (!currentRenderer.Get())
		throw L"Error! Drawing Board had a NULL Render Target";

	board = rt;
	gradients = coll;

	ID2D1GradientStopCollection* stopColl = nullptr;
	auto rawGradients = gradients.GetRawCollection();
	currentRenderer->CreateGradientStopCollection(rawGradients.data(), rawGradients.Size(), &stopColl);
	TrecComPointer<ID2D1LinearGradientBrush>::TrecComHolder linBrush;
	currentRenderer->CreateLinearGradientBrush(D2D1::LinearGradientBrushProperties(
		p1,
		p2),
		stopColl, linBrush.GetPointerAddress());

	brush = TrecPointerKey::GetComPointer<ID2D1Brush, ID2D1LinearGradientBrush>(linBrush);

	stopColl->Release();
	stopColl = nullptr;
	brushType = brush_type::brush_type_linear;
}

TBrush::TBrush(TrecPointer<DrawingBoard> rt)
{
	if (!rt.Get())
		throw L"ERROR! Needed Render Target to Not Be Null";

	currentRenderer = rt->GetRenderer();

	if (!currentRenderer.Get())
		throw L"Error! Drawing Board had a NULL Render Target";

	board = rt;
}

/**
 * Method: TBrush::Refresh
 * Purpose: Allows Brushes to know that it is safe to Draw and attempt to correct any issue that pops up
 * Parameters: void
 * Returns: bool
 */
bool TBrush::Refresh()
{
	if (!board.Get() || !brush.Get())
		return false;

	if (currentRenderer.Get() && currentRenderer.Get() == board->GetRenderer().Get())
		return true;
	currentRenderer = board->GetRenderer();
	RefreshBrush();
	return currentRenderer.Get() != nullptr;
}

/**
 * Method: TBrush::RefreshBrush
 * Purpose: Makes sure that if the Render Target is changed, that the Brushes are refreshed as Direct2D demands that new brushes are created when a new Render Target is used
 * Parameters: void
 * Returns: void
 */
void TBrush::RefreshBrush()
{
	if (!currentRenderer.Get() || !brush.Get())
		return;
	TrecComPointer<ID2D1SolidColorBrush>::TrecComHolder brushHolder;
	TrecComPointer<ID2D1LinearGradientBrush>::TrecComHolder linBrush;
	TrecComPointer<ID2D1RadialGradientBrush>::TrecComHolder radBrush;

	POINT_2D p1, p2;
	float x, y;
	TColor col;

	ID2D1GradientStopCollection* stopColl = nullptr;
	auto rawGradients = gradients.GetRawCollection();
	currentRenderer->CreateGradientStopCollection(rawGradients.data(), rawGradients.Size(), &stopColl);

	switch (brushType)
	{
	case brush_type::brush_type_solid:
		col.SetColor(reinterpret_cast<ID2D1SolidColorBrush*>(brush.Get())->GetColor());
		brush.Delete();
		currentRenderer->CreateSolidColorBrush(col.GetColor(), brushHolder.GetPointerAddress());

		brush = TrecPointerKey::GetComPointer<ID2D1Brush, ID2D1SolidColorBrush>(brushHolder);
		break;
	case brush_type::brush_type_linear:
		p1 = reinterpret_cast<ID2D1LinearGradientBrush*>(brush.Get())->GetStartPoint();
		p2 = reinterpret_cast<ID2D1LinearGradientBrush*>(brush.Get())->GetEndPoint();
		brush.Delete();
		currentRenderer->CreateLinearGradientBrush(D2D1::LinearGradientBrushProperties(
			p1,
			p2),
			stopColl, linBrush.GetPointerAddress());
		brush = TrecPointerKey::GetComPointer<ID2D1Brush, ID2D1LinearGradientBrush>(linBrush);
		break;
	case brush_type::brush_type_radial:
		p1 = reinterpret_cast<ID2D1RadialGradientBrush*>(brush.Get())->GetCenter();
		p2 = reinterpret_cast<ID2D1RadialGradientBrush*>(brush.Get())->GetGradientOriginOffset();
		x = reinterpret_cast<ID2D1RadialGradientBrush*>(brush.Get())->GetRadiusX();
		y = reinterpret_cast<ID2D1RadialGradientBrush*>(brush.Get())->GetRadiusY();

		brush.Delete();
		TrecComPointer<ID2D1RadialGradientBrush>::TrecComHolder radBrush;
		currentRenderer->CreateRadialGradientBrush(D2D1::RadialGradientBrushProperties(
			p1,
			p2,
			x, y),
			stopColl, radBrush.GetPointerAddress());

		brush = TrecPointerKey::GetComPointer<ID2D1Brush, ID2D1RadialGradientBrush>(radBrush);
	}
	stopColl->Release();
	stopColl = nullptr;


}

