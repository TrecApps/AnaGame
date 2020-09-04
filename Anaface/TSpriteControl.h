#pragma once
#include "TControl.h"
#include <T2DSpriteEngine.h>
class TSpriteControl :
    public TControl
{
	/**
	 * Method: TSpriteControl::TSpriteControl
	 * Purpose: Constructor
	 * Parameters: TrecPointer<DrawingBoard> db - Smart Pointer to the Render Target to draw on
	 *				TrecPointer<TArray<styleTable>> styTab - Smart Pointer to the list of styles to draw from
	 * Return: New TControl Object
	 */
	TSpriteControl(TrecPointer<DrawingBoard> drawingBoard, TrecPointer<TArray<styleTable>> styles);


	/**
	 * Method: TSpriteControl::~TSpriteControl
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	virtual ~TSpriteControl();


	/**
	 * Method: TSpriteControl::onDraw
	 * Purpose: Draws the control
	 * Parameters: TObject* obj - Raw reference to a TObject that might have specific text to say
	 * Returns: void
	 */
	virtual void onDraw(TObject* obj = nullptr) override;

	/**
	 * Method: TSpriteControl::SetSpriteEngine
	 * Purpose: Sets the Sprite Engine to use
	 * Parameters: TrecPointer<T2DSpriteEngine> - the engine to use
	 * Returns: void
	 */
	void SetSpriteEngine(TrecPointer<T2DSpriteEngine> sprite);

protected:

	/**
	 * The sprite to work with
	 */
	TrecPointer<T2DSpriteEngine> sprite;
};

