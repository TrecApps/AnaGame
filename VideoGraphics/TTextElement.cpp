#include "TTextElement.h"

TrecComPointer<TTextRenderer> textRenderer;

TrecComPointer<TTextRenderer> GetTTextRenderer()
{
	if (!textRenderer.Get())
	{
		TrecComPointer<TTextRenderer>::TrecComHolder holder;
		*holder.GetPointerAddress() = new TTextRenderer();
		textRenderer = holder.Extract();
	}
	return textRenderer;
}



/**
 * Class: TTextElementInterceter
 * Purpose: Intercepts text from handlers to the TTextElements
 */
class _VIDEO_GRAPHICS TTextElementIntercepter : public TTextIntercepter
{
public:
	/**
	 * Method: TTextIntercepter::OnChar
	 * Purpose: Takes a character and feeds it to its target
	 * Parameters: WCHAR ch - the character to report
	 *          UINT count number of instances of that character to feed
	 *          UINT flags - flags (usually 0)
	 * Returns: void
	 *
	 * Attributes: abstract
	 */
	virtual void OnChar(UINT ch, UINT count, UINT flags)
	{
		element->OnInputChar(ch, count);
	}

	/**
	 * Method: TTextIntercepter::OnChar
	 * Purpose: Takes a character and feeds it to its target
	 * Parameters: WCHAR ch - the character to report
	 *          UINT count number of instances of that character to feed
	 *          UINT flags - flags (usually 0)
	 * Returns: void
	 *
	 * Attributes: override
	 */
	virtual void OnKey(UINT ch, UINT count, UINT flags) override
	{
		element->OnInputKey(ch, count);
	}


	/**
	 * Method: TTextIntercepter::OnLoseFocus
	 * Purpose: Alerts the target that it will no longer be intercepting characters
	 * Parameters: void
	 * Returns: void
	 *
	 * Attributes: abstract
	 */
	virtual void OnLoseFocus() 
	{
		element->OnLoseFocus();
	}

	/**
	 * Method: TTextIntercepter::OnCopy
	 * Purpose: Tells the target that CTRL-C was pressed
	 * Parameters: void
	 * Returns: void
	 *
	 * Attributes: abstract
	 */
	virtual void OnCopy()
	{
		element->OnCutCopyPaste(control_text_mode::ctm_copy);
	}
	/**
	 * Method: TTextIntercepter::OnCut
	 * Purpose: Tells the target that CTRL-X was pressed
	 * Parameters: void
	 * Returns: void
	 *
	 * Attributes: override
	 */
	virtual void OnCut()
	{
		element->OnCutCopyPaste(control_text_mode::ctm_cut);
	}

	/**
	 * Method: TTextElementIntercepter::GetTarget
	 * Purpose: Reports the TTextElement address it points to
	 * Parameters: void
	 * Returns: void* - address of the TTextElement
	 * 
	 * Attributes: override
	 */
	virtual void* GetTarget() override
	{
		return element.Get();
	}


	/**
	 * Method: TTextInterceptor::TakesInput
	 * Purpose: Reports whather input will be added to the target
	 * Parameters: void
	 * Returns: bool - whether the target supports input or not
	 */
	virtual bool TakesInput() override
	{
		return element->TakesInput();
	}

	TTextElementIntercepter(TrecPointer<TTextElement> element)
	{
		this->element = element;
		assert(element.Get());
	}
private:
	TrecPointer<TTextElement> element;
};





bool IsContained(const TPoint& point, const D2D1_RECT_F& loc)
{
	return loc.left <= point.x && point.x <= loc.right &&
		loc.top <= point.y && point.y <= loc.bottom;
}


TextFormattingDetails::TextFormattingDetails() :
	defaultBackgroundColor(L"lightyellow")
{
	weight = DWRITE_FONT_WEIGHT_NORMAL;
	style = DWRITE_FONT_STYLE_NORMAL;
	range = { 0,0 };
	fontSize = 12.0f;
	stretch = DWRITE_FONT_STRETCH_NORMAL;
}

TextFormattingDetails::TextFormattingDetails(const TextFormattingDetails& copy):
defaultBackgroundColor(copy.defaultBackgroundColor)
{
	this->bColor = copy.bColor;
	this->color = copy.color;
	this->fontSize = copy.fontSize;
	this->range = copy.range;
	this->style = copy.style;
	this->weight = copy.weight;
	this->stretch = copy.stretch;
}

HighlightRange::HighlightRange()
{
	Reset();
}

HighlightRange::HighlightRange(const HighlightRange& range)
{
	begin = range.begin;
	end = range.end;
	beginSet = range.beginSet;
	endSet = range.endSet;
}

void HighlightRange::Reset()
{
	this->begin = this->end = 0;
	this->beginSet = this->endSet = false;
}

bool HighlightRange::GetCarotLocation(UINT& loc)
{
	bool ret = begin == end && beginSet && endSet;
	if (ret)
		loc = begin;
	return ret;
}

bool HighlightRange::GetHighlightRange(UINT& begin, UINT& end)
{
	if(!beginSet || !endSet || (this->begin == this->end))
		return false;
	begin = this->begin;
	end = this->end;
	if (end < begin)
	{
		UINT temp = end;
		end = begin;
		begin = temp;
	}
	return true;
}

void HighlightRange::SetBegin(UINT begin)
{
	Reset();
	this->begin = begin;
	this->beginSet = true;
}

bool HighlightRange::SetEnd(UINT end)
{
	if(!beginSet)
		return false;
	this->end = end;
	this->endSet = true;
}

void TTextElement::SetSelf(TrecPointer<TTextElement> self)
{
	if (this != self.Get())
		throw L"False Self Reference";
	this->self = TrecPointerKey::GetSoftPointerFromTrec<>(self);
}

void TTextElement::ReCreateLayout()
{
	if (recreateFormat)
	{
		TrecComPointer<IDWriteTextFormat>::TrecComHolder wtf;
		HRESULT results = writeFact->CreateTextFormat(
			fontType.GetConstantBuffer().getBuffer(),
			NULL,
			basicDetails.weight,
			basicDetails.style,
			basicDetails.stretch,
			basicDetails.fontSize,
			locale.GetConstantBuffer().getBuffer(),
			wtf.GetPointerAddress());
		assert(SUCCEEDED(results));
		format = wtf.Extract();

		format->SetParagraphAlignment(verticalAlignment);
		format->SetTextAlignment(horizontalAlignment);

		recreateFormat = false;
	}

	TrecComPointer<IDWriteTextLayout>::TrecComHolder wfl;
	writeFact->CreateTextLayout(text.GetConstantBuffer().getBuffer(), text.GetSize(), format.Get(), bounds.right - bounds.left, bounds.bottom - bounds.top, wfl.GetPointerAddress());
	mainLayout = wfl.Extract();
}

/**
 * Method: TTextElement::GetLayout
 * Purpose: Retrieves the underlying layout
 * Parameters: void
 * Returns: TrecCOmPointer<IDWriteTextLayout> - the layout held
 */
TrecComPointer<IDWriteTextLayout> TTextElement::GetLayout()
{
	return mainLayout;
}

UINT TTextElement::GetTextLength()
{
	return text.GetSize();
}

TrecPointer<TBrush> TTextElement::GetBrush()
{
	return this->basicDetails.color;
}

bool TTextElement::TakesInput()
{
	return false;
}

/*
 * Method: TTextElement::GetMinWidth
 * Purpose: Retirvees the minimum width needed before DirectWrtie has to add emergency breaks in line
 * Parameters: bool& worked - whether the value returned is truely the reported value
 * Return: float - the min width needed. If inspection fails, this represents the width currently used
 */
float TTextElement::GetMinWidth(bool& worked)
{
	TObjectLocker threadLock(&thread);
	float ret = 0.0f;
	if (!(worked = (mainLayout.Get() && SUCCEEDED(mainLayout->DetermineMinWidth(&ret)))))
		ret = bounds.right - bounds.left;


	return ret;
}

float TTextElement::GetMinHeight(bool& worked)
{
	float ret = 0.0f;
	DWRITE_TEXT_METRICS mets;
	worked = false;
	if (mainLayout.Get() && SUCCEEDED(mainLayout->GetMetrics(&mets)))
	{
		worked = true;
		ret = mets.height;
	}

	return ret;
}

TTextElement::TTextElement(TrecPointer<DrawingBoard> drawingBoard)
{
	this->drawingBoard = drawingBoard;
	this->bounds = { 0,0,0,0 };
	this->horizontalAlignment = DWRITE_TEXT_ALIGNMENT_JUSTIFIED;
	this->verticalAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;

	TrecComPointer<IDWriteFactory>::TrecComHolder rawFact;

	HRESULT results = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(rawFact.GetPointerAddress()));

	assert(SUCCEEDED(results));

	this->writeFact = rawFact.Extract();



	recreateFormat = true;
	isClickDown = false;

	ReCreateLayout();

	this->basicDetails.color = drawingBoard->GetBrush(TColor());
}

bool TTextElement::SetText(const TString& text)
{
	this->text.Set(text);
	ReCreateLayout();
	return true;
}


/**
 * Method: TTextElement::GetTextInterceptor
 * Purpose: Retrieves an interceptor for this element
 * Parameters: void
 * Returns: TrecPointer<TTextInterceptor> - the interceptor that delivers characters to this element
 *
 */
TrecPointer<TTextIntercepter> TTextElement::GetTextInterceptor()
{
	auto fullSelf = TrecPointerKey::GetTrecPointerFromSoft<>(self);
	return TrecPointerKey::GetNewTrecPointerAlt<TTextIntercepter, TTextElementIntercepter>(fullSelf);
}

void TTextElement::GetText(TString& text)
{
	text.Set(this->text);
}

D2D1_RECT_F TTextElement::GetLocation()
{
	return bounds;
}

void TTextElement::SetLocation(const D2D1_RECT_F& loc)
{
	bounds = loc;
	if (bounds.left > bounds.right)
	{
		auto t = bounds.left;
		bounds.left = bounds.right;
		bounds.right = t;
	}
	if (bounds.top > bounds.bottom)
	{
		auto t = bounds.bottom;
		bounds.bottom = bounds.top;
		bounds.top = t;
	}
	ReCreateLayout();
}

bool TTextElement::SetBasicFormatting(const TextFormattingDetails& details)
{
	this->basicDetails = details;
	recreateFormat = true;
	ReCreateLayout();
	return true;
}

bool TTextElement::SetFormatting(const TextFormattingDetails& details, UINT start, UINT end, bool correctInvalidParams)
{
	return SetBasicFormatting(details);
}

bool TTextElement::GetBasicFormatting(TextFormattingDetails& details)
{
	details = this->basicDetails;
	return true;
}

bool TTextElement::OnCLickDown(const TPoint& point)
{
	if (IsContained(point, bounds))
	{
		this->highlightRange.Reset();

		DWRITE_HIT_TEST_METRICS mets;
		ZeroMemory(&mets, sizeof(mets));
		BOOL trailing = FALSE, inside = FALSE;
		if (SUCCEEDED(mainLayout->HitTestPoint(point.x - bounds.left, point.y - bounds.top, &trailing, &inside, &mets)))
		{
			UINT pos = trailing ? 1 : 0;
			pos += mets.textPosition;

			this->highlightRange.SetBegin(pos);
			isClickDown = true;
		}


		return true;
	}
	return false;
}

bool TTextElement::OnCLickUp(const TPoint& point)
{
	isClickDown = false;
	if (IsContained(point, bounds))
	{
		DWRITE_HIT_TEST_METRICS mets;
		ZeroMemory(&mets, sizeof(mets));
		BOOL trailing = FALSE, inside = FALSE;
		if (SUCCEEDED(mainLayout->HitTestPoint(point.x - bounds.left, point.y - bounds.top, &trailing, &inside, &mets)))
		{
			UINT pos = trailing ? 1 : 0;
			pos += mets.textPosition;
			UINT start = 0, end = 0;
			if(this->highlightRange.GetHighlightRange(start, end))
				this->mainLayout->SetDrawingEffect(nullptr, DWRITE_TEXT_RANGE{ start, end - start });

			this->highlightRange.SetEnd(pos);

			// Range Highlighter
			if (this->highlightRange.GetHighlightRange(start, end))
			{
				TrecPointer<TBrush> newB = drawingBoard->GetBrush(TColor(this->basicDetails.defaultBackgroundColor));
				TDoubleBrushHolder* dHolder = new TDoubleBrushHolder(this->basicDetails.color->GetUnderlyingBrush().Get(), newB->GetUnderlyingBrush().Get());
				this->mainLayout->SetDrawingEffect(dHolder, DWRITE_TEXT_RANGE{ start, end - start });
				dHolder->Release();
				dHolder = nullptr;
			}
		}

		return true;
	}
	return false;
}

bool TTextElement::OnMouseMove(const TPoint& point)
{
	if (IsContained(point, bounds))
	{
		DWRITE_HIT_TEST_METRICS mets;
		ZeroMemory(&mets, sizeof(mets));
		BOOL trailing = FALSE, inside = FALSE;
		if (isClickDown && SUCCEEDED(mainLayout->HitTestPoint(point.x - bounds.left, point.y - bounds.top, &trailing, &inside, &mets)))
		{
			UINT pos = trailing ? 1 : 0;
			pos += mets.textPosition;
			UINT start = 0, end = 0;
			if (this->highlightRange.GetHighlightRange(start, end))
				this->mainLayout->SetDrawingEffect(nullptr, DWRITE_TEXT_RANGE{ start, end - start });

			this->highlightRange.SetEnd(pos);

			// Range Highlighter
			if (this->highlightRange.GetHighlightRange(start, end))
			{
				TrecPointer<TBrush> newB = drawingBoard->GetBrush(TColor(this->basicDetails.defaultBackgroundColor));
				TDoubleBrushHolder* dHolder = new TDoubleBrushHolder(this->basicDetails.color->GetUnderlyingBrush().Get(), newB->GetUnderlyingBrush().Get());
				this->mainLayout->SetDrawingEffect(dHolder, DWRITE_TEXT_RANGE{ start, end - start });
				dHolder->Release();
				dHolder = nullptr;
			}
		}
		return true;

	}
	return false;
}

void TTextElement::OnLoseFocus()
{

}

void TTextElement::OnCutCopyPaste(control_text_mode mode)
{
}

bool TTextElement::OnInputChar(WCHAR ch, UINT count)
{
	return false;
}

bool TTextElement::OnInputKey(UINT key, UINT count)
{
	return false;
}

void TTextElement::OnDraw(TObject* obj)
{
	TString curText;
	bool textReplaced = false;
	if (obj && text.GetSize() > 0 && text.GetAt(0) == L'{' && text.GetAt(text.GetSize() - 1) == L'}')
	{
		
		text.Set(obj->getVariableValueStr(text.SubString(1, text.GetSize() - 1)));
		ReCreateLayout();
	}


		mainLayout->SetMaxHeight(bounds.bottom - bounds.top);
		mainLayout->SetMaxWidth(bounds.right - bounds.left);
		ID2D1Brush* b = basicDetails.color->GetUnderlyingBrush().Get();
		if (b)
		{
			TDrawingContext tContext{ drawingBoard->GetRenderer().Get(), b };
			mainLayout->Draw(&tContext, GetTTextRenderer().Get(), bounds.left, bounds.top);
			//drawingBoard->GetRenderer()->DrawTextLayout(D2D1::Point2F(), fontLayout.Get(), b);
		}
	
}

void TTextElement::OnDraw(TrecPointer<TVariable> dataText)
{
	TString curText;
	bool textReplaced = false;
	if (dataText.Get() && text.GetSize() > 0 && text.GetAt(0) == L'{' && text.GetAt(text.GetSize() - 1) == L'}')
	{

		text.Set(dataText->GetString(/*text.SubString(1, text.GetSize() - 1)*/));
		ReCreateLayout();
	}


	mainLayout->SetMaxHeight(bounds.bottom - bounds.top);
	mainLayout->SetMaxWidth(bounds.right - bounds.left);
	ID2D1Brush* b = basicDetails.color->GetUnderlyingBrush().Get();
	if (b)
	{
		TDrawingContext tContext{ drawingBoard->GetRenderer().Get(), b };
		mainLayout->Draw(&tContext, GetTTextRenderer().Get(), bounds.left, bounds.top);
		//drawingBoard->GetRenderer()->DrawTextLayout(D2D1::Point2F(), fontLayout.Get(), b);
	}
}

bool TTextElement::GetColor(TColor& color, bool foreground)
{
	if (foreground)
	{
		color.SetColor(basicDetails.color->GetColor());
	}
	else
	{
		color.SetColor(basicDetails.defaultBackgroundColor);
	}
	return true;
}

bool TTextElement::SetColor(const TColor& color, bool foreground)
{
	if (foreground)
		basicDetails.color->SetColor(color);
	else
		basicDetails.defaultBackgroundColor.SetColor(color);
	return true;
}

DWRITE_PARAGRAPH_ALIGNMENT TTextElement::GetVerticalAligment()
{
	return verticalAlignment;
}

void TTextElement::SetVerticalAlignment(DWRITE_PARAGRAPH_ALIGNMENT vAlignment)
{
	verticalAlignment = vAlignment;
	format->SetParagraphAlignment(vAlignment);
}

DWRITE_TEXT_ALIGNMENT TTextElement::GetHorizontalAlignment()
{
	return horizontalAlignment;
}

void TTextElement::SetHorizontallignment(DWRITE_TEXT_ALIGNMENT hAlignment)
{
	horizontalAlignment = hAlignment;
	format->SetTextAlignment(hAlignment);
}

bool TTextElement::GetSetLocale(TString& local, bool doGet)
{
	if (doGet)
	{
		local.Set(this->locale);
		return true;
	}
	TrecComPointer<IDWriteTextFormat>::TrecComHolder formatHolder;
	if (FAILED(writeFact->CreateTextFormat(
		fontType.GetConstantBuffer().getBuffer(),
		nullptr,
		basicDetails.weight,
		basicDetails.style,
		basicDetails.stretch,
		basicDetails.fontSize,
		local.GetConstantBuffer().getBuffer(),
		formatHolder.GetPointerAddress()
	)))
	return false;

	this->format = formatHolder.Extract();
	this->locale.Set(local);
	ReCreateLayout();
	return true;
}

bool TTextElement::GetSetFont(TString& font, bool doGet)
{
	if (doGet)
	{
		font.Set(this->fontType);
		return true;
	}
	TrecComPointer<IDWriteTextFormat>::TrecComHolder formatHolder;
	if (FAILED(writeFact->CreateTextFormat(
		font.GetConstantBuffer().getBuffer(),
		nullptr,
		basicDetails.weight,
		basicDetails.style,
		basicDetails.stretch,
		basicDetails.fontSize,
		locale.GetConstantBuffer().getBuffer(),
		formatHolder.GetPointerAddress()
	)))
		return false;

	this->format = formatHolder.Extract();
	this->fontType.Set(font);
	ReCreateLayout();
	return true;
}

bool TTextElement::GetSetFontSize(float& fontSize, bool doGet)
{
	if (doGet)
	{
		fontSize = basicDetails.fontSize;
		return true;
	}
	TrecComPointer<IDWriteTextFormat>::TrecComHolder formatHolder;
	if (FAILED(writeFact->CreateTextFormat(
		fontType.GetConstantBuffer().getBuffer(),
		nullptr,
		basicDetails.weight,
		basicDetails.style,
		basicDetails.stretch,
		fontSize,
		locale.GetConstantBuffer().getBuffer(),
		formatHolder.GetPointerAddress()
	)))
		return false;

	this->format = formatHolder.Extract();
	basicDetails.fontSize = fontSize;
	ReCreateLayout();
	return true;
}

void TTextElement::LockText(bool doLock)
{
}


