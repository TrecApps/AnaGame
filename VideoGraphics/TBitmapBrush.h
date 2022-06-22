#pragma once
#include "TBrush.h"
#include <TFileShell.h>
#include <wincodec.h>

/**
 * Class: TBitmapBrush
 * Purpose: Hold a Bitmap Brush so that images can be drawn
 * 
 * SuperClass: TBrush - allows it to be drawn to the drawing board like a regular brush
 */
class _VIDEO_GRAPHICS TBitmapBrush : public TBrush
{
	friend class TrecPointerKey;
public:

	/**
	 * Method: TBitmapBrush::GetDefaultSize
	 * Purpose: Retrieves the Default (Max) Size of the Image in question
	 * Parameters: void
	 * Returns: D2D1_SIZE_F - the size of the image (if 0-0, then assume somehting went wrong)
	 */
	D2D1_SIZE_F GetDefaultSize();

	/**
	 * Method: TBitmapBrush::GetType
	 * Purpose: Returns a String Representation of the object type
	 * Parameters: void
	 * Returns: TString - representation of the object type
	 */
	virtual TString GetType()override;

	/**
	 * Method: TBitmapBrush::NextFrame
	 * Purpose: In a multiframe image, sets the brush to draw the next frame in a sequence
	 * Parameters:void
	 * Returns: void
	 */
	void NextFrame();

	/**
	 * Method: TBitmapBrush::PrevFrame
	 * Purpose: In a multifram image, sets the brush to draw the previous frame in an image
	 * Parameters: void
	 * Returns: void
	 */
	void PrevFrame();


	/**
	 * Method: TBitmapBrush::IsValid
	 * Purpose: Reports whether the Brush Construction worked and is ready to draw
	 * Parameters: void
	 * Returns: bool
	 */
	bool IsValid();

	/**
	 * Method: TBitmapBrush::SetLocation
	 * Purpose: Sets the location where the image will be drawn
	 * Parameters: RECT_2D& loc - the location on the board to draw the image
	 * Returns: void
	 */
	void SetLocation(RECT_2D& loc);

	/**
	 * Method: TBitmapBrush::GetCurrentFrame
	 * Purpose: Returns the index of the current frame to be drawn
	 * Parameters: void
	 * Returns: UINT - the index of the current frame
	 */
	UINT GetCurrentFrame();

	/**
	 * Method: TBitmapBrush::GetFrameCount
	 * Purpose: Retrieves the total number of frames in the image
	 * Parameters: void
	 * Returns: UINT - the total frame count
	 */
	UINT GetFrameCount();

	/**
	 * Method: TBitmapBrush::~TBitmapBrush
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	virtual ~TBitmapBrush();


	// Override fill methods so that a bitmap is drawin instead of a collor pallette

	/**
	 * Method: TBitmapBrush::FillRectangle
	 * Purpose: Fills the Rectangle with the currently selected image
	 * Parameters: const RECT_2D& r - the rectangle to apply the brush to
	 * Returns: void
	 * 
	 * Attributes: override
	 */
	virtual void FillRectangle(const RECT_2D& r)override;

	/**
	 * Method: TBitmapBrush::FillRoundedRectangle
	 * Purpose:  Fills the Rounded Rectangle with the currently selected image
	 * Parameters: const ROUNDED_RECT_2D& r - the rounded rectangle to apply the brush to
	 * Returns: void
	 * 
	 * Attributes: override
	 */
	virtual void FillRoundedRectangle(const ROUNDED_RECT_2D& r)override;

	/**
	 * Method: TBitmapBrush::FillEllipse
	 * Purpose: Fills the Ellipse with the currently selected image
	 * Parameters: const ELLIPSE_2D& r - the Ellipse to apply the brush to
	 * Returns: void
	 * 
	 * Attributes: override
	 */
	virtual void FillEllipse(const ELLIPSE_2D& r)override;

	/**
	 * Method: TBitmapBrush::FillGeometry
	 * Purpose: Fills the Geometry with the currently selected image
	 * Parameters: TrecPointer<TGeometry> geo - the Geometry to apply the brush to
	 * Returns:void
	 * 
	 * Attributes: override
	 */
	virtual void FillGeometry(TrecPointer<TGeometry> geo)override;
	void UpdateFrame(TrecComPointer<ID2D1Bitmap> map);
protected:



	/**
	 * Method: TBitmapBrush::TBitmapBrush
	 * Purpose: Cretes a TBitmap Brush
	 * Parameters: TrecPointer<TFileShell> picture - the location of the image file
	 *				TrecPointer<DrawingBoard> rt - the Drawing Board crating the Brush
	 *				RECT_2D& loc - the initial location to draw the image
	 * Returns: New Bitmap Brush Object
	 */
	TBitmapBrush(TrecPointer<TFileShell> picture, TrecPointer<DrawingBoard> rt, RECT_2D& loc);

	/**
	 * Method: TBitmapBrush::TBitmapBrush
	 * Purpose: Creates a bitmap brush using an existing bitmap
	 * Parameters: TrecPointer<DrawingBoard> rt - the Drawing Board crating the Brush
	 *				TrecComPointer<ID2D1Bitmap> map - the bitmap to draw
	 * Returns: New Bitmap BrushObject
	 */
	TBitmapBrush(TrecPointer<DrawingBoard> rt, TrecComPointer<ID2D1Bitmap> map);

	/**
	 * reports whether constriction worked properly or not. 
	 */
	bool valid;

	/**
	 * flags related to how the Brush was generated
	 */
	bool usesFile;

	/**
	 * Set of bitmaps from the image
	 */
	TDataArray < TrecComPointer<ID2D1Bitmap>> bitmaps;
	/**
	 * Set of brushes that go with the bitmaps
	 */
	TDataArray < TrecComPointer<ID2D1Brush>> brushes;

	/**
	 * the Current frame to draw
	 */
	UINT currentFrame;

	
	IWICImagingFactory* imageFactory;
	IWICBitmapDecoder* decoder;
	IWICBitmapFrameDecode* frameDec;
	IWICFormatConverter* converter;


	/**
	 * Method: TBitmapBrush::RefreshBrush
	 * Purpose: Makes sure that the Brush is compatible with the Render Target, as Direct2D demands that a new Brush be created when
	 *		a new Render Target is used
	 * Parameters: void
	 * Returns: void
	 * 
	 * Attributes: override
	 */
	virtual void RefreshBrush()override;
	RECT_2D location;
};

