#include "TSpriteControl.h"


/**
 * Method: TSpriteControl::GetType
 * Purpose: Returns a String Representation of the object type
 * Parameters: void
 * Returns: TString - representation of the object type
 */
TString TSpriteControl::GetType()
{
	return TString(L"TSpriteControl;") + TControl::GetType();
}

/**
 * Method: TSpriteControl::TSpriteControl
 * Purpose: Constructor
 * Parameters: TrecPointer<DrawingBoard> db - Smart Pointer to the Render Target to draw on
 *				TrecPointer<TArray<styleTable>> styTab - Smart Pointer to the list of styles to draw from
 * Return: New TControl Object
 */
TSpriteControl::TSpriteControl(TrecPointer<DrawingBoard> drawingBoard, TrecPointer<TArray<styleTable>> styles) : TControl(drawingBoard, styles)
{

}

/**
 * Method: TSpriteControl::~TSpriteControl
 * Purpose: Destructor
 * Parameters: void
 * Returns: void
 */
TSpriteControl::~TSpriteControl()
{
}

/**
 * Method: TSpriteControl::onDraw
 * Purpose: Draws the control
 * Parameters: TObject* obj - Raw reference to a TObject that might have specific text to say
 * Returns: void
 */
void TSpriteControl::onDraw(TObject* obj)
{
	if (sprite.Get())
	{
		sprite->Draw(location);
	}
	TControl::onDraw(obj);
}

/**
 * Method: TSpriteControl::SetSpriteEngine
 * Purpose: Sets the Sprite Engine to use
 * Parameters: TrecPointer<T2DSpriteEngine> - the engine to use
 * Returns: void
 */
void TSpriteControl::SetSpriteEngine(TrecPointer<T2DSpriteEngine> sprite)
{
	if(sprite.Get())
		this->sprite = sprite;
}
