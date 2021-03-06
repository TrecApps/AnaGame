#pragma once
#include <TObject.h>
#include <d2d1.h>
#include <TrecReference.h>
#include "TWindowEngine.h"
#include "TBrush.h"
#include "TBitmapBrush.h"

class TWindow;

typedef enum class stroke_style
{
	bs_not_set,
	bs_dotted,
	bs_dashed,
	bs_solid,
	bs_double,
	bs_groove,
	bs_ridge,
	bs_inset,
	bs_outset,
	bs_none,
	bs_hidden
}stroke_style;

bool _VIDEO_GRAPHICS hasStyle(stroke_style style);

class _VIDEO_GRAPHICS StrokeStyle
{
public:
	StrokeStyle();
	StrokeStyle(const StrokeStyle& copy);
	StrokeStyle(stroke_style tag, TrecComPointer<ID2D1StrokeStyle> value);
	stroke_style tag;
	TrecComPointer<ID2D1StrokeStyle> value;
};


/**
 * Enum Class: TShape
 * Purpose: Determines the basic size of the TControl
 * NOTE: Feature is unstable, stick with T_Rect for now
 */
typedef enum class TShape {
	T_Rect,
	T_Rounded_Rect,
	T_Ellipse,
	T_Custom_shape
}TShape;


/**
 * Class: TVideoSlot
 * Purpose: Used By the DrawingBoard to track Video Frams and where they belong on the Screen
 */
class _VIDEO_GRAPHICS TVideoSlot
{
public:
	TVideoSlot();
	TVideoSlot(const TVideoSlot& copy);

	bool set;
	D2D1_RECT_F loc;
	TrecComPointer<ID2D1Bitmap> frame;
};

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
	 * Method: DrawingBoard::GetWindowHandle
	 * Purpose: Retirevs the Window Handle being used by the Board
	 * Parameters: void
	 * Returns: HWND - the window handle
	 */
	HWND GetWindowHandle();

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
	 * Method: DrawingBoard::DrawFrame
	 * Purpose: Adds a Frame to the Drawing Board, expected to be called by Media Foundation Classes
	 * Parameters:
	 */

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
	void Resize(HWND window, RECT size, TrecPointer<TWindowEngine> surface);


	void Prep3DResize();

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
	TrecComPointer<ID2D1BitmapRenderTarget> GetRenderer();


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

	/**
	 * Method: DrawingBoard::SetToSecondaryTarget
	 * Purpose: Have the Drawing board draw to the secondary Render Target
	 * Parameters: void
	 * Returns: void
	 * 
	 * Note: May be depricated soon if secondary target is deemed unecessary
	 */
	void SetToSecondaryTarget();
	/**
	 * Method: DrawingBoard::SetToPrimaryTarget
	 * Purpose: Have the Drawing board draw to the primary Render Target
	 * Parameters: void
	 * Returns: void
	 *
	 * Note: May be depricated soon if secondary target is deemed unecessary
	 */
	void SetToPrimaryTarget();

	/**
	 * Method: DrawingBoard::BeginDraw
	 * Purpose: Have the Drawing Board commence the drawing Process
	 * Parameters: void
	 * Returns: void
	 */
	void BeginDraw();

	/**
	 * Method: DrawingBoard::EndDraw
	 * Purpose: Have the drawing board finalize the drawing procedure, by copying the contents of the Bitmap targets over to the
	 *	main one
	 * Parameters: void
	 * Return: void
	 */
	void EndDraw();

	/**
	 * Method: DrawingBoard::GetDefaultColor
	 * Purpose: Reports the default color for drawing basic content if a control has no content to draw itself
	 * Parameters: void
	 * Returns: D2D1_COLOR_F - the default content color to draw
	 */
	D2D1_COLOR_F GetDefaultColor();

	/**
	 * Method: DrawingBoard::GetDefaultTextColor
	 * Purpose: Reports the default color for drawing basic content if a control has no content to draw itself
	 * Parameters: void
	 * Returns: D2D1_COLOR_F - the default content color to draw
	 */
	D2D1_COLOR_F GetDefaultTextColor();

	/**
	 * Method: DrawingBoard::SetDefaultColor
	 * Purpose: Sets the default color to use
	 * Parameters: const D2D1_COLOR_F& color - the color to set the default background color to
	 * Returns: void
	 */
	void SetDefaultColor(const D2D1_COLOR_F& color);

	/**
	 * Method: DrawingBoard::SetDefaultTextColor
	 * Purpose: Sets the default text color to use
	 * Parameters: const D2D1_COLOR_F& color - the color to set the default background color to
	 * Returns: void
	 */
	void SetDefaultTextColor(const D2D1_COLOR_F& color);

	/**
	 * Method: DrawingBoard::FillControlBackground
	 * Purpose: Enables controls to easily draw to the drawing board without holding a brush object themselves
	 * Parameters: const D2D1_RECT_F& location - where to draw
	 *				TShape shape - the default chape to draw
	 * Returns: void
	 * 
	 * Note: Initially, the only shapes supported are RECT and ELLIPSE as an elipse can easily be calculated from the rect param.
	 * However other shapes need more data to properly articulate
	 */
	void FillControlBackground(const D2D1_RECT_F& location, TShape shape = TShape::T_Rect);

	/// Methods to manage Video Slots in the board

	/**
	 * Method: DrawingBoard::AddFrameSlot
	 * Purpose: Alows a Media Sink and it's associated objects to register itself for a frame 
	 * Parameters: void
	 * Returns: UINT - index of the slot in the list
	 */
	UINT AddFrameSlot();

	/**
	 * Method: DrawingBoard::SetFrame
	 * Purpose: Allows Video Display Managers to set the next Frame to display
	 * Parameters: DXGI_MAPPED_RECT& data - the data for the map
	 *				D2D1_SIZE_U& size - current Size of the Map
	 *				UINT slot - the slot to find the index in
	 * Returns: bool - whether it was successful or not			
	 */
	bool SetFrame(DXGI_MAPPED_RECT& data, D2D1_SIZE_U& size, UINT slot);

	/**
	 * Method: DrawingBoard::PresentFrame
	 * Purpose: Presents the Frame for Presentation
	 * Parameters: UINT slot - the slot for video presentation
	 * Returns: void
	 */
	void PresentFrame(UINT slot);

	/**
	 * Method: DrawingBoard::ReleaseFrame
	 * Purpose: Allows sinks to release the hold they have on the drawing board, frees up a slot for a new sink
	 * Parameters: UINT slot - the slot to free up
	 * Returns: void
	 */
	void ReleaseFrame(UINT slot);

	/**
	 * Method: DrawingBoard::SetFrame
	 * Purpose: Allows Display controls to set the location of the frame
	 * Parameters: UINT slot - the slot to free up
	 *				const D2D1_RECT_F& loc - location on the Render Target to present the frame
	 * Returns: bool - whether the situation was valid
	 */
	bool SetFrame(UINT slot, const D2D1_RECT_F& loc);

	/**
	 * Method: DrawingBoard::GetWindowEngine
	 * Purpose: Retireves the TWindowEngine held by the drawing board
	 * Parameters: void
	 * Returns: TrecPointer<TWindowEngine> - the 3D engine held
	 */
	TrecPointer<TWindowEngine> GetWindowEngine();

	/**
	 * Method: DrawingBoard::GetVideoPosition
	 * Purpose: Retireves the video location of the frame
	 * Parameters: UINT slot - the slot given
	 *				D2D1_RECT_F& loc - the location
	 * Returns: bool - whether the slot was valid
	 */
	bool GetVideoPosition(UINT slot, D2D1_RECT_F& loc);

	/**
	 * Method: DrawingBoard::SetVideoPosition
	 * Purpose: Sets the video location of the frame
	 * Parameters: UINT slot - the slot given
	 *				D2D1_RECT_F& loc - the location given
	 * Returns: bool - whether the slot was valid
	 */
	bool SetVideoPosition(UINT slot, const D2D1_RECT_F& loc);

	/**
	 * Method: DrawingBoard::GetStrokeStyle
	 * Purpose: Returns the actual stroke style based off of the provided tag
	 * Parameters: stroke_style tag - the stroke style requested
	 * Returns: TrecComPointer<ID2D1StrokeStyle> - the stroke style
	 */
	TrecComPointer<ID2D1StrokeStyle> GetStrokeStyle(stroke_style tag);

private:

	TDataArray<TVideoSlot> slots;

	TrecSubPointer<TBrush, TBitmapBrush> frameBrush;

	TrecPointer<TBrush> defaultBrush;

	/**
	 * Holds the Stroke Styles
	 */
	TDataArray<StrokeStyle> strokeStyles;

	void PrepStyles();

	/**
	 * Default colors for controls to use if no color is specified
	 */
	D2D1_COLOR_F defaultColor, defaultTextColor;

	/**
	 * whether to use the pimary Device Context or Secondary Context
	 */
	bool usePrimaryDc;

	/**
	 * Used to determine if Drawing Board is Drawing or not
	 */
	bool isDrawing;

	/**
	 * Whether we are in 3D mode or not
	 */
	TrecComPointer<IDXGISurface1> surface3D;

	/**
	 * The Engine to facilitate Video Processing Drawing
	 */
	TrecPointer<TWindowEngine> d3dEngine;

	/**
	 * the factory object the Board is working with, used to retieve Geometries
	 */
	TrecComPointer<ID2D1Factory1> fact;

	/**
	 * The Offscreen Render Target to work with
	 */
	TrecComPointer<ID2D1BitmapRenderTarget> renderer, renderer2;

	/**
	 * The OnScreen RenderTarget to copy the contents to in 2D mode
	 */
	TrecComPointer<ID2D1HwndRenderTarget> windowTarget;

	/**
	 * The OnScreen RenderTarget to copy the contents to in 3D mode
	 */
	TrecComPointer<ID2D1RenderTarget> dxgiTarget;

	/**
	 * Reference to the self, used for initializing TBrush's
	 */
	TrecPointerSoft<DrawingBoard> self;

	/**
	 * Handle to the window operating on
	 */
	HWND window;

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

