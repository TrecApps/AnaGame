#include "TTextLayout.h"

/*
* Function: operator==
* Purpose: Compares two colors
* Parameters: D2D1_COLOR_F & c1 - the first color to compare
*				D2D1_COLOR_F & c2 - the second color to compare
* Returns: bool - whether the values are equal
*/
bool operator==(D2D1_COLOR_F& c1, D2D1_COLOR_F& c2)
{
	return c1.r == c2.r && c1.g == c2.b
		&& c1.b == c2.b && c1.a == c2.a;
}


void TTextLayout::SetUpTextElement()
{
	TextFormattingDetails details;

	TString locale(L"en-us"), font(L"ariel");

	TString actText;

	DWRITE_TEXT_ALIGNMENT hAlign = DWRITE_TEXT_ALIGNMENT_CENTER;
	DWRITE_PARAGRAPH_ALIGNMENT vAlign = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;

	TString valpoint;

	TColor fontColor;
	if (attributes.retrieveEntry(TString(L"Caption"), valpoint))
	{
		actText.Set(valpoint);
	}

	if (attributes.retrieveEntry(TString(L"CaptionLocale"), valpoint))
	{
		locale.Set(valpoint);
	}

	if (attributes.retrieveEntry(TString(L"Font"), valpoint))
	{
		font.Set(valpoint);
	}
	if (attributes.retrieveEntry(TString(L"FontSize"), valpoint))
	{
		valpoint.ConvertToFloat(details.fontSize);
	}
	if (attributes.retrieveEntry(TString(L"HorizontalAlignment"), valpoint))
	{
		hAlign = convertStringToTextAlignment(valpoint);
	}
	if (attributes.retrieveEntry(TString(L"VerticalAlignment"), valpoint))
	{
		vAlign = convertStringToParagraphAlignment(valpoint);
	}

	if (attributes.retrieveEntry(TString(L"FontColor"), valpoint))
	{
		bool b = false;
		fontColor.SetColor(TColor::GetColorFromString(valpoint, b));
	}
	else
		fontColor.SetColor(drawingBoard->GetDefaultTextColor());

	// Commented out code (in case gradient colors should be added

	//valpoint = att->retrieveEntry(TString(L"TextGrad"));
	//if (valpoint.Get())
	//{
	//	if (!text1.Get())
	//		text1 = TrecPointerKey::GetNewTrecPointer<TText>(drawingBoard, (this));
	//	float entry = 0.0f;
	//	valpoint->ConvertToFloat(entry);
	//	//text1->secondColor = true;
	//	UINT gradCount = 0;
	//	if (gradCount = text1->stopCollection.GetGradientCount())
	//		text1->stopCollection.SetPositionAt(entry, gradCount - 1);
	//}


	//valpoint = att->retrieveEntry(TString(L"TextGradMode"));
	//if (valpoint.Get())
	//{
	//	if (!text1.Get())
	//		text1 = TrecPointerKey::GetNewTrecPointer<TText>(drawingBoard, (this));
	//	if (valpoint->Compare(L"Radial"))
	//		text1->useRadial = true;
	//}

	// See if have text. If we do, then set up text element

	details.color = drawingBoard->GetBrush(fontColor);

	text = TrecPointerKey::GetNewSelfTrecPointerAlt<TTextElement, TComplexTextElement>(drawingBoard, windowHandle);

	text->SetLocation(location);
	text->SetBasicFormatting(details);
	text->SetHorizontallignment(hAlign);
	text->SetVerticalAlignment(vAlign);
	text->SetText(actText);
}

UINT TTextLayout::AppendText(const TString& str)
{
	// To Do: Add means of deleting characters at the beggining if a limit was set
	TString t;

	// End to to
	this->text->GetText(t);
	this->text->SetText(t + str);

	return 0;
}

void TTextLayout::AddColorEffect(D2D1_COLOR_F col, UINT start, UINT length)
{
	TObjectLocker threadLock(&thread);
	if (!text.Get() || !drawingBoard.Get())
	{
		return;
	}
	IDWriteTextLayout* layout = text->GetLayout().Get();
	if (!layout)
	{
		return;
	}
	for (UINT C = 0; C < colors.Size(); C++)
	{
		if (col == colors[C].color)
		{
			layout->SetDrawingEffect(colors[C].colBrush, DWRITE_TEXT_RANGE{ start, length });

			return;
		}
	}

	ColorEffect ce{ nullptr, col };

	HRESULT h = drawingBoard->GetRenderer()->CreateSolidColorBrush(col, &ce.colBrush);
	if (SUCCEEDED(h))
	{
		colors.push_back(ce);
		layout->SetDrawingEffect(ce.colBrush, DWRITE_TEXT_RANGE{ start, length });
	}
}

TrecPointer<LineMetrics2> TTextLayout::GetLineMetrics()
{
	TObjectLocker threadLock(&thread);
	if (text.Get() && text->GetLayout().Get())
	{
		TrecPointer<LineMetrics2> ret = TrecPointerKey::GetNewTrecPointer<LineMetrics2>(5);
		HRESULT res = text->GetLayout()->GetLineMetrics(ret->metrics.data(), ret->metrics.Size(), &ret->sizeNeeded);
		if (res == E_NOT_SUFFICIENT_BUFFER)
		{
			ret->SetSize(ret->sizeNeeded);
			text->GetLayout()->GetLineMetrics(ret->metrics.data(), ret->metrics.Size(), &ret->sizeNeeded);

			return ret;
		}
	}

	return TrecPointer<LineMetrics2>();
}

bool TTextLayout::ApplyFormatting(const TextFormattingDetails& ds)
{
	TObjectLocker threadLock(&thread);
	if ((ds.range.startPosition + ds.range.length) > text->GetTextLength())
	{

		return false;
	}
	this->details.push_back(ds);

	return true;
}

void TTextLayout::ShrinkHeight()
{
	if (text.Get())
	{
		bool worked;
		float height = text->GetMinHeight(worked);
		if (worked) {
			location.bottom = location.top + height;
			text->SetLocation(location);
		}
	}
}

TTextLayout::TTextLayout(TrecPointer<DrawingBoard> rt, TrecPointer<TArray<styleTable>> ta, HWND win): TTextInput(rt, ta, win)
{
}

TTextLayout::~TTextLayout()
{
}

bool TTextLayout::onCreate(const D2D1_RECT_F& loc, TrecPointer<TWindowEngine> d3d)
{
	TString valpoint;
	if (attributes.retrieveEntry(L"IsLocked", valpoint) && !valpoint.CompareNoCase(L"true"))
	{
		this->textLock = true;
	}
	return TGadget::onCreate(loc, d3d);
}

LineMetrics2::LineMetrics2()
{
	sizeNeeded = 0;
}

LineMetrics2::LineMetrics2(const LineMetrics2& orig)
{
	sizeNeeded = orig.sizeNeeded ? orig.sizeNeeded : orig.metrics.Size();
	for (UINT Rust = 0; Rust < sizeNeeded; Rust++)
	{
		auto met = orig.metrics.data()[Rust];
		metrics.push_back(met);
	}
}

LineMetrics2::LineMetrics2(USHORT i)
{
	SetSize(i, true);
}

void LineMetrics2::SetSize(UINT i, bool fromConstructor)
{
	DWRITE_LINE_METRICS met;
	sizeNeeded = fromConstructor ? 0 : i;
	ZeroMemory(&met, sizeof(met));
	metrics.RemoveAll();
	for (UINT Rust = 0; Rust < i; Rust++)
		metrics.push_back(met);
}
