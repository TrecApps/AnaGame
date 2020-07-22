#pragma once
#include <TObject.h>
#include <TrecReference.h>
#include "Structure2D.h"
#include "TGradientStopCollection.h"

class DrawingBoard;
class TColor;
class TGeometry;

/**
 * Helps the TBrush keep track of the brush type it has
 */
typedef enum class brush_type
{
	brush_type_solid,
	brush_type_linear,
	brush_type_radial,
	brush_type_bitmap
}brush_type;

/**
 * class TBrush
 * Purpose: Applies rendering operations to the Drawing Board depending on the color settings applied to it
 *
 * Note: The base class only supports solid colors, linear gradients, and radial gradients. For bitmap support, use the TBitmapClass
 */
class _VIDEO_GRAPHICS TBrush : public TObject
{
	friend class TrecPointerKey;
	friend class DrawingBoard;
public:

	/**
	 * Method: TBrush::DrawRectangle
	 * Purpose: draws the given Rectangle on the DrawingBoard
	 * Parameters: const RECT_2D& r - The Rectangle to apply
	 *				float thickness -  the thickness of the line (default is 1.0f)
	 * Returns: void 
	 */
	void DrawRectangle(const RECT_2D& r, float thickness = 1.0f);

	/**
	 * Method: TBrush::FillRectangle
	 * Purpose: fills the given Rectangle on the DrawingBoard
	 * Parameters: const RECT_2D& r - The Rectangle to apply
	 * Returns: void
	 */
	virtual void FillRectangle(const RECT_2D& r);

	/**
	 * Method: TBrush::DrawRoundedRectangle
	 * Purpose: draws the given Rounded Rectangle on the DrawingBoard
	 * Parameters: const ROUNDED_RECT_2D& r - The Rounded Rectangle to apply
	 *				float thickness -  the thickness of the line (default is 1.0f)
	 * Returns: void
	 */
	void DrawRoundedRectangle(const ROUNDED_RECT_2D& r, float thickness = 1.0f);

	/**
	 * Method: TBrush::FillRoundedRectangle
	 * Purpose: fills the given Rounded Rectangle on the DrawingBoard
	 * Parameters: const ROUNDED_RECT_2D& r - The Rounded Rectangle to apply
	 * Returns: void
	 */
	virtual void FillRoundedRectangle(const ROUNDED_RECT_2D& r);

	/**
	 * Method: TBrush::DrawEllipse
	 * Purpose: draws the given Ellipse on the DrawingBoard
	 * Parameters: const ELLIPSE_2D& r - The Ellipse to apply
	 *				float thickness -  the thickness of the line (default is 1.0f)
	 * Returns: void
	 */
	void DrawEllipse(const ELLIPSE_2D& r, float thickness = 1.0f);

	/**
	 * Method: TBrush::FillEllipse
	 * Purpose: fills the given Ellipse on the DrawingBoard
	 * Parameters:const ELLIPSE_2D& r - The Ellipse to apply
	 * Returns: void
	 */
	virtual void FillEllipse(const ELLIPSE_2D& r);

	/**
	 * Method: TBrush::DrawGeometry
	 * Purpose: draws the given Geometry on the DrawingBoard
	 * Parameters: TrecPointer<TGeometry> geo - The Geometry to apply
	 *				float thickness -  the thickness of the line (default is 1.0f) 
	 * Returns: void
	 */
	void DrawGeometry(TrecPointer<TGeometry> geo, float thickness = 1.0f);

	/**
	 * Method: TBrush::FillGeometry
	 * Purpose: fills the given Geometry on the DrawingBoard
	 * Parameters: TrecPointer<TGeometry> geo - The Geometry to apply
	 * Returns: void
	 */
	virtual void FillGeometry(TrecPointer<TGeometry> geo);


	/**
	 * Method: TBrush::DrawLine
	 * Purpose: Draws a specific line to the Drawing Board
	 * Parameters: const POINT_2D& p1 - the beginning of the line
	 *				const POINT_2D& p2 - the end of the line
	 *				float thickness -  the thickness of the line (default is 1.0f)
	 * Returns: void
	 */
	void DrawLine(const POINT_2D& p1, const POINT_2D& p2, float thickness = 1.0f);
	

	/**
	 * Method: TBrush::GetMaxColors
	 * Purpose: Reports the number of gradients held by the brush. Use to ensure successful calls to SetColor() and GetColor()
	 * Parameters: void
	 * Returns: UINT - the Number of colors held by the Brush
	 */
	UINT GetMaxColors();


	/**
	 * Method: TBrush::SetColor
	 * Purpose: Sets one of the gradients to a specific color
	 * Parameters: const TColor& color - the color to set the brush to
	 *				UINT index - the index of the gradient to apply the color to (default is 0 for solid brushes will only have 1 gradient)
	 * Returns: void
	 *
	 * Note: if the index is out of bounds, the operation will silently fail. to ensure success, call the GetMaxColors() to ensure your index is within bounds
	 */
	void SetColor(const TColor& color, UINT index = 0);

	/**
	 * Method: TBrush::GetColor
	 * Purpose: Retrieves the Specified color being drawn by the brush
	 * Parameters: UINT index - the index of the gradient to get the color from (defaults to 0 as a solid brush will only have 1 gradient)
	 * Returns: TColor - the color requested
	 *
	 * Note: If the index is out of bounds, the color black will be returned. To ensure that the returned color is legitimate, call GetMaxColors()
	 */
	TColor GetColor(UINT index = 0);


	/**
	 * Method: TBrush::GetBrushType
	 * Purpose:  Reports the type of brush this TBrush is using
	 * Parameters: void
	 * Returns: brush_type the type of brush the TBrush is set to
	 */
	brush_type GetBrushType();

	/**
	 * Method: TBrush::GetUnderlyingBrush
	 * Purpose: Retrieves the underlying brush
	 * Parameters: void
	 * Returns: TrecComPointer<ID2D1Brush> - the underlying brush
	 */
	TrecComPointer<ID2D1Brush> GetUnderlyingBrush();


	/**
	 * Method: TBrush::~TBrush
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	virtual ~TBrush();

protected:
	brush_type brushType;

	/**
	 * Method: TBrush::TBrush
	 * Purpose:  Constructor that sets the brush to draw solid colors
	 * Parameters: const TColor& col - the initial color to set the brush to
	 *				TrecPointer<DrawingBoard> rt - the Drawing Board that created this brush
	 * Returns: New TBrush holder optimized for a solid color
	 */
	TBrush(const TColor& col, TrecPointer<DrawingBoard> rt);

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
	TBrush(const TGradientStopCollection& coll, TrecPointer<DrawingBoard> rt, const POINT_2D& p1, const POINT_2D& p2, float x, float y);

	/**
	 * Method: TBrush::TBrush
	 * Purpose: Constructor that sets the brush to draw linear gradients
	 * Parameters: const TGradientStopCollection& coll - the collection of colors to work with
	 *				TrecPointer<DrawingBoard> rt - the Drawing Board that created this brush
	 *				const POINT_2D& p1 - one point the line is expected to pass through
	 *				const POINT_2D& p2 - second point line is expected to pass through
	 * Returns: New TBrush holder optimized for linar gradient drawing
	 */
	TBrush(const TGradientStopCollection& coll, TrecPointer<DrawingBoard> rt, const POINT_2D& p1, const POINT_2D& p2);


	/**
	 * Method: TBrush::TBrush
	 * Purpose:
	 * Parameters: TrecPointer<DrawingBoard> rt - 
	 * Returns: New TBrush Object
	 *
	 * Note: Not Trusted at this time
	 */
	TBrush(TrecPointer<DrawingBoard> rt);


	/**
	 * Method: TBrush::Refresh
	 * Purpose: Allows Brushes to know that it is safe to Draw and attempt to correct any issue that pops up
	 * Parameters: void
	 * Returns: bool
	 */
	bool Refresh();

	/**
	 * Method: TBrush::RefreshBrush
	 * Purpose: Makes sure that if the Render Target is changed, that the Brushes are refreshed as Direct2D demands that new brushes are created when a new Render Target is used
	 * Parameters: void
	 * Returns: void
	 */
	virtual void RefreshBrush();


	/**
	 * The Brush to draw with
	 */
	TrecComPointer<ID2D1Brush> brush;

	/**
	 * The RenderTarget to draw against
	 */
	TrecComPointer<ID2D1RenderTarget> currentRenderer;

	/**
	 * The Drawing Board that holds the Render Target, allows Brushes to insect their ender Target and make necessary changes before drawing
	 */
	TrecPointer<DrawingBoard> board;

	/**
	 * holds the gradients that influence the flow of the various colors (or the one color in the solid brush case)
	 */
	TGradientStopCollection gradients;
};

