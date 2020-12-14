#pragma once
#include <TObject.h>
#include <d2d1.h>
#include <TrecReference.h>
#include "TWindowEngine.h"
#include "TBrush.h"
#include "TBitmapBrush.h"

class TWindow;

/**
 * Class: DrawingBoard
 * Purpose: Holds the Direct2D RenderTarget and provides methods on manipulatingit and retrieving Brushes to draw upon it
 * 
 * SuperClass: TObject
 */
class _VIDEO_GRAPHICS DrawingBoard : public TObject
{
public:


	/**
	 * Method: DrawingBoard::GetType
	 * Purpose: Returns a String Representation of the object type
	 * Parameters: void
	 * Returns: TString - representation of the object type
	 */
	virtual TString GetType();

	/**
	 * Method: DrawingBoard::DrawingBoard
	 * Purpose: Constructor that sets the mode to 2D
	 * Parameters: TrecComPointer<ID2D1Factory1> fact - the Direct2D Factory associated with the intance
	 *				 HWND window -  the Window handle associated with the window the Drawing board is attaching to
	 * Returns: New DrawingBoard Object set for regular 2D presentation
	 *
	 * Note: The DrawingBoard produces will not work for 3D drawings. To configure it to work with 3D
	 *	simply call the "Set3D" method 
	 */
	DrawingBoard(TrecComPointer<ID2D1Factory1> fact, HWND window);

	/**
	 * Method: DrawingBoard::~DrawingBoard
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	~DrawingBoard();

	/**
	 * Method: DrawingBoard::Resize
	 * Purpose: Resets the Underlying Rendertarget should the window size be changed
	 * Parameters: HWND window -  the window to get the size from
	 * Returns: void
	 */
	void Resize(HWND window);

	/**
	 * Method: DrawingBoard::GetBrush
	 * Purpose: Produces a Solid Color-enabled TBrush
	 * Parameters: const TColor& col - the color to set the brush to
	 * Returns: TrecPointer<TBrush> - the TBrush with solid-color enabled
	 */
	TrecPointer<TBrush> GetBrush(const TColor& col);

	/**
	 * Method: DrawingBoard::GetBrush
	 * Purpose: Produces a radial-gradient enabled TBrush
	 * Parameters: const TGradientStopCollection& coll - the collection of colors to work with
	 *				const POINT_2D& p1 -Center of the gradient ellipse
	 *				const POINT_2D& p2 - offset of the gradient origin
	 *				float x - x-radius of the ellipse center
	 *				float y - y-radius of the ellipse center
	 * Returns: TrecPointer<TBrush> - the TBrush holding a Radial gradient brush
	 */
	TrecPointer<TBrush> GetBrush(const TGradientStopCollection& coll, const POINT_2D& p1, const POINT_2D& p2, float x, float y);

	/**
	 * Method: DrawingBoard::GetBrush
	 * Purpose: Produces a linear-gradient enabled TBrush
	 * Parameters: const TGradientStopCollection& coll - the collection of colors to work with
	 *				const POINT_2D& p1 - one point the line is expected to pass through
	 *				const POINT_2D& p2 - second point line is expected to pass through
	 * Returns: TrecPointer<TBrush> - the TBrush holding a Linear gradient brush
	 */
	TrecPointer<TBrush> GetBrush(const TGradientStopCollection& coll, const POINT_2D& p1, const POINT_2D& p2);

	/**
	 * Method: DrawingBoard::GetBrush
	 * Purpose: Produces a Brush containing an image and capable of drawing an image
	 * Parameters: TrecPointer<TFileShell> picture - Pointer to the file to get the picture from
	 *				RECT_2D& loc - the location where the picture is expected to be drawn
	 * Returns: TrecSubPointer<TBrush, TBitmapBrush> - the Bursh containing the image (or null if failure occurs)
	 */
	TrecSubPointer<TBrush, TBitmapBrush> GetBrush(TrecPointer<TFileShell> picture, RECT_2D& loc);


	/**
	 * Method: DrawingBoard::GetRenderer
	 * Purpose: Returns the Raw Render Object in case direct operations need to be performed on it
	 * Parameters: void
	 * Returns: TrecComPointer<ID2D1RenderTarget> - the underlying Render Target
	 */
	TrecComPointer<ID2D1DCRenderTarget> GetRenderer();


	/**
	 * Method: DrawingBoard::SetSelf
	 * Purpose: Enables a reference to itself so that when Brushes are created, they have a reference to their creator
	 * Parameters: TrecPointer<DrawingBoard> self -  the Parameter holding a reference to this object
	 * Returns: void
	 */
	void SetSelf(TrecPointer<DrawingBoard> self);


	/**
	 * Method: DrawingBoard::SetTransform
	 * Purpose: Sets the transform of the underlying RenderTarget. Enables the rotation, translation, skewing, and sizing of various images
	 * Parameters: const TRANSFORM_2D& matrix
	 * Returns: bool - whether the operation was successful
	 */
	bool SetTransform(const TRANSFORM_2D& matrix);


	/**
	 * Method: DrawingBoard::GetTransform
	 * Purpose: Retrieves the current transform matrix of the underlying RenderTarget
	 * Parameters: TRANSFORM_2D& matrix
	 * Returns: bool - whether the operation was successful
	 */
	bool GetTransform(TRANSFORM_2D& matrix);




	// Methods handling layers


	/**
	 * Method: DrawingBoard::PopLayer
	 * Purpose: Removes a layer from the Render Target
	 * Parameters: void
	 * Returns: void
	 */
	void PopLayer();

	/**
	 * Method: DrawingBoard::AddLayer
	 * Purpose: Adds a layer so that contents drawn withing the Rectangle are presented
	 * Parameters: const RECT_2D& ret
	 * Returns: bool - whether the operation was successful
	 */
	bool AddLayer(const RECT_2D& ret);

	/**
	 * Method: DrawingBoard::AddLayer
	 * Purpose:Adds a layer so that contents drawn withing the Ellipse are presented
	 * Parameters: const ELLIPSE_2D& ellipse
	 * Returns: bool - whether the operation was successful
	 */
	bool AddLayer(const ELLIPSE_2D& ellipse);

	/**
	 * Method: DrawingBoard::AddLayer
	 * Purpose: Adds a layer so that contents drawn withing the Rounded Rectangle are presented
	 * Parameters: const ROUNDED_RECT_2D& rRect
	 * Returns: bool - whether the operation was successful
	 */
	bool AddLayer(const ROUNDED_RECT_2D& rRect);

	/**
	 * Method: DrawingBoard::AddLayer
	 * Purpose:Adds a layer so that contents drawn withing the geometry are presented
	 * Parameters: const TDataArray<POINT_2D>& points
	 * Returns: bool - whether the operation was successful
	 */
	bool AddLayer(const TDataArray<POINT_2D>& points);

	/**
	 * Method: DrawingBoard::AddLayer
	 * Purpose: Adds a layer so that contents drawn withing the geometry are presented
	 * Parameters: TrecPointer<TGeometry> geo
	 * Returns: bool - whether the operation was successful
	 */
	bool AddLayer(TrecPointer<TGeometry> geo);





	// Methods to Generate Geometries

	/**
	 * Method: DrawingBoard::GetGeometry
	 * Purpose: Creates a TGeometry based off of the provided Rectabgle
	 * Parameters: const RECT_2D& ret - the rectangle to get a geometry object from
	 * Returns: TrecPointer<TGeometry> - the TGeometry object
	 */
	TrecPointer<TGeometry> GetGeometry(const RECT_2D& ret);

	/**
	 * Method: DrawingBoard::GetGeometry
	 * Purpose: Creates a TGeometry based off of the provided Ellipse
	 * Parameters: const ELLIPSE_2D& ellipse  - the ellipse to get a geometry object from
	 * Returns: TrecPointer<TGeometry> - the TGeometry object
	 */
	TrecPointer<TGeometry> GetGeometry(const ELLIPSE_2D& ellipse);

	/**
	 * Method: DrawingBoard::GetGeometry
	 * Purpose:Creates a TGeometry based off of the provided Rounded-Rectabgle
	 * Parameters: const ROUNDED_RECT_2D& rRect - the rounded-rectangle to get a geometry object from
	 * Returns: TrecPointer<TGeometry> - the TGeometry object
	 */
	TrecPointer<TGeometry> GetGeometry(const ROUNDED_RECT_2D& rRect);

	/**
	 * Method: DrawingBoard::GetGeometry
	 * Purpose:Creates a TGeometry based off of the provided set pf points
	 * Parameters:const TDataArray<POINT_2D>& points - the points to get a geometry object from
	 * Returns: TrecPointer<TGeometry> - the TGeometry object
	 */
	TrecPointer<TGeometry> GetGeometry(const TDataArray<POINT_2D>& points);


	/**
	 * Method: DrawingBoard::GetLayerCount
	 * Purpose: Retrieves the number of layers currently stacked on the render target
	 * Parameters: void
	 * Returns: UINT
	 */
	UINT GetLayerCount();

	HDC GetDc();

	HDC GetDc2();

	void SetToSecondaryTarget();
	void SetToPromaryTarget();

private:

	void SelectObjectDc();

	/**
	 * whether to use the pimary Device Context or Secondary Context
	 */
	bool usePrimaryDc;

	/**
	 * the factory object the Board is working with, used to retieve Geometries
	 */
	TrecComPointer<ID2D1Factory1> fact;

	/**
	 * The Render Target to work with
	 */
	TrecComPointer<ID2D1DCRenderTarget> renderer, renderer2;

	/**
	 * Reference to the self, used for initializing TBrush's
	 */
	TrecPointerSoft<DrawingBoard> self;

	/**
	 * Handle to the window operating on
	 */
	HWND window;

	HDC dc, dc2;

	HBITMAP hMap;

	RECT r;

	/**
	 * heeps track of the number of layers pushed
	 */
	UINT layersPushed;

	/**
	 * holds the layers pushed
	 */
	TDataArray<TrecComPointer<ID2D1Layer>> layers, layers2;

	/**
	 * Keeps track of the Geometries that exist
	 */
	TDataArray<TrecPointer<TGeometry>> geometries;

};

