#include "TTextCombobox.h"
#include <TInputTextElement.h>

class TTextComboElement : public TInputTextElement
{
public:

	TTextComboElement(TrecSubPointerSoft<TPage, TTextCombobox> sub, TrecPointer<DrawingBoard> drawingBoard, HWND window) : TInputTextElement(drawingBoard, window)
	{
		assert(sub.Get());
		combo = sub;
	}

	/**
	 * Method: TTextElement::OnInputChar
	 * Purpose: Inserts Text at the moment
	 * Parameters: WCHAR ch - text to insert
	 *				UINT count - number of times to insert it
	 * Returns: bool - whether text was altered (this base class does not support edit-mode so it will return false, but subclasses might and thus return true)
	 *
	 * Attributes: override
	 */
	virtual bool OnInputChar(WCHAR ch, UINT count) override
	{
		bool ret = TInputTextElement::OnInputChar(ch, count);
		if (ret && combo.Get())
		{
			combo.Get()->UpdateText(this->text);
		}
		return ret;
	}

private:
	TrecSubPointerSoft<TPage, TTextCombobox> combo;
};




TTextCombobox::TTextCombobox(TrecPointer<DrawingBoard> rt, TrecPointer<TArray<styleTable>> ta, HWND win): TCombobox(rt,ta)
{
	window = win;
}

TTextCombobox::~TTextCombobox()
{
}

bool TTextCombobox::onCreate(const D2D1_RECT_F& loc, TrecPointer<TWindowEngine> d3d)
{
	return TCombobox::onCreate(loc, d3d);
}

void TTextCombobox::Draw(TrecPointer<TVariable> object)
{
	TCombobox::Draw(object);
}

void TTextCombobox::OnLButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<EventArgs>& args)
{
	if (IsContained(point, TCombobox::area) && this->TCombobox::isLeftClicked)
	{
		dataLayout->SetData(TrecPointerKey::GetTrecPointerFromSub<>(vars));
		text->OnCLickUp(point);
	}
	TCombobox::OnLButtonUp(nFlags, point, mOut, eventAr, args);

}

void TTextCombobox::OnLButtonDown(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<EventArgs>& args)
{
	if (!text->OnCLickDown(point))
		return;

	TGadget::OnLButtonDown(nFlags, point, mOut, eventAr, args);
}

void TTextCombobox::OnMouseMove(UINT nFlags, TPoint point, message_output& mOut, TDataArray<EventID_Cred>&eventAr, TDataArray<EventArgs>&args)
{
	if (text->OnMouseMove(point))
		TControl::OnMouseMove(nFlags, point, mOut, eventAr, args);
}

void TTextCombobox::SetUpTextElement()
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

	text = TrecPointerKey::GetNewSelfTrecPointerAlt<TTextElement, TTextComboElement>(TrecPointerKey::GetSoftSubPointerFromSoft<TPage, TTextCombobox>(self) ,drawingBoard, window);

	text->SetLocation(location);
	text->SetBasicFormatting(details);
	text->SetHorizontallignment(hAlign);
	text->SetVerticalAlignment(vAlign);
	text->SetText(actText);
}

void TTextCombobox::UpdateText(const TString& str)
{
	TrecSubPointer<TVariable, TContainerVariable> filteredOutText = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TContainerVariable>(ContainerType::ct_array);
	UINT Rust = 0;
	for (TrecPointer<TVariable> cVar = vars->GetValueAt(Rust); cVar.Get(); cVar = vars->GetValueAt(++Rust))
	{
		if (cVar->GetString().Find(str) != -1)
			filteredOutText->AppendValue(cVar);
		else
		{

		}
	}

	dataLayout->SetData(TrecPointerKey::GetTrecPointerFromSub<>(filteredOutText));
}