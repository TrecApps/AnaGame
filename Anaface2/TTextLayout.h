#pragma once
#include "TTextInput.h"
#include <TComplexTextElement.h>

typedef struct ColorEffect
{
	ID2D1SolidColorBrush* colBrush;
	D2D1_COLOR_F color;
};

/**
 * Class: LineMetrics
 * Purpose: Structure holding line Metrics of a text
 */
class _ANAFACE_DLL2 LineMetrics2
{
public:
	LineMetrics2();
	LineMetrics2(const LineMetrics2& orig);
	LineMetrics2(USHORT i);

	void SetSize(UINT i, bool fromConstructor = false);

	TDataArray<DWRITE_LINE_METRICS> metrics;
	UINT sizeNeeded;
};

class _ANAFACE_DLL2 TTextLayout :
    public TTextInput
{
protected:

	/**
	 * Method: TControl::SetUpTextElement
	 * Purpose: Sets up Text Elements in case Some attributes support it
	 * Parameters: void
	 * Returns: void
	 *
	 * Attributes: virtual - allows sub classes to create more complex Text Elements than what the standard TControl uses
	 */
	virtual void SetUpTextElement() override;


	/**
	 * List of colors affecting the text
	 */
	TDataArray<ColorEffect> colors;

	/**
	 * List for details such as weight
	 */
	TDataArray<TextFormattingDetails> details;
public:

	/**
	 * Method: TTextayout::AppendText
	 * Purpose: Adds text to the End
	 * Parameters: const TString& str - the text to append
	 * Returns: UINT - the number of characters deleted at the beginning
	 */
	UINT AppendText(const TString& str);


	/*
	 * Method: TTextField::AddColorEffect
	 * Purpose: Adds a color effect to the text presented
	 * Parameters: D2D1_COLOR_F col - the color to set the text
	 *				UINT start - the starting index to apply the color
	 *				UINT length - the number of characters to apply the color to
	 * Returns: void
	 */
	void AddColorEffect(D2D1_COLOR_F col, UINT start, UINT length);

	TrecPointer<LineMetrics2> GetLineMetrics();


	/**
	 * Method: TTextField::ApplyFormatting
	 * Purpose: Applies formatting to a portion of the String
	 * Parameters: formattingDetails details
	 * Returns: bool - true if no issue was found, false otherwise
	 */
	bool ApplyFormatting(const TextFormattingDetails& details);

	/**
	 * Method: TTextField::ShrinkHeight
	 * Purpose: Reduces the height of the control down to what is needed --> just shrinks its children
	 *		some of whom might find ways to shrink themselves
	 * Parameters: void
	 * Returns: void
	 *
	 * Attributes: override
	 */
	virtual void ShrinkHeight();

	/**
	 * Method: TTextLayout::TTextLayout
	 * Purpose: Constructor
	 * Parameters: TrecPointer<DrawingBoard> rt - the Render Target to use
	 *				TrecPointer<TArray<styleTable>> ta - List of Styles for the TControl
	 *				bool isGadgetBase - the gadget base
	 * Return: new Gadget Control object
	 */
	TTextLayout(TrecPointer<DrawingBoard> rt, TrecPointer<TArray<styleTable>> ta, HWND win);
	/**
	 * Method: TGadget::~TGadget
	 * Purpose: Destructor
	 * Parameters: void
	 * Return: void
	 */
	virtual ~TTextLayout();

	/**
	 * Method: TGadget::onCreate
	 * Purpose: To set up the Gadget Control
	 * Parameters: D2D1_RECT_F r - the location control will be
	 * Return: bool - false, ignore for now
	 *
	 * Attributes: override
	 */
	bool onCreate(const D2D1_RECT_F& loc, TrecPointer<TWindowEngine> d3d)override;
};

