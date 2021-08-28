#include "TInputTextElement.h"

void TInputTextElement::UpdateCarotPoisition(UINT loc)
{
	DWRITE_HIT_TEST_METRICS mets;
	ZeroMemory(&mets, sizeof(mets));
	float x = 0.0f, y = 0.0f;
	if (SUCCEEDED(mainLayout->HitTestTextPosition(loc, TRUE, &x, &y, &mets)))
	{
		if (carotActive)
			DestroyCaret();
		CreateCaret(window, nullptr, 1.0f, mets.height);
		SetCaretPos(x + this->bounds.left, y + bounds.top);
		ShowCaret(window);
		carotActive = true;
		carotLoc = loc;
	}

}

TInputTextElement::TInputTextElement(TrecPointer<DrawingBoard> drawingBoard, HWND window) : TTextElement(drawingBoard)
{
	carotLoc = 0;
	carotActive = false;
	grabbedRange = false;
	this->window = window;
}

bool TInputTextElement::OnCLickDown(const TPoint& point)
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

			
			isClickDown = true;

			// Check to see if we just grabbed onto highlighted text
			UINT curStart = 0, curEnd = 0;
			if (this->highlightRange.GetHighlightRange(curStart, curEnd) && curStart <= pos && pos <= curEnd && curStart != curEnd)
				this->grabbedRange = true;
			else
				this->highlightRange.SetBegin(pos);
		}


		return true;
	}
	return false;
}

bool TInputTextElement::OnCLickUp(const TPoint& point)
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
			if (grabbedRange)
			{
				OnTransferText(pos);
				grabbedRange = false;
			}

			if (this->highlightRange.GetHighlightRange(start, end))
				this->mainLayout->SetDrawingEffect(nullptr, DWRITE_TEXT_RANGE{ start, end - start });

				


				// Range Highlighter
				if (this->highlightRange.GetHighlightRange(start, end) && start < end)
				{
					TrecPointer<TBrush> newB = drawingBoard->GetBrush(TColor(this->basicDetails.defaultBackgroundColor));
					TDoubleBrushHolder* dHolder = new TDoubleBrushHolder(this->basicDetails.color->GetUnderlyingBrush().Get(), newB->GetUnderlyingBrush().Get());
					this->mainLayout->SetDrawingEffect(dHolder, DWRITE_TEXT_RANGE{ start, end - start });
					dHolder->Release();
					dHolder = nullptr;
				}
				this->highlightRange.SetEnd(pos);

				if (this->highlightRange.GetCarotLocation(this->carotLoc))
				{
					CreateCaret(window, nullptr, 1, mets.height);
					SetCaretPos(mets.left + bounds.left, mets.top + bounds.top);
					ShowCaret(window);
					carotActive = true;
				}
			
		}

		return true;
	}
	return false;
}

bool TInputTextElement::OnMouseMove(const TPoint& point)
{
	return TTextElement::OnMouseMove(point);
}

void TInputTextElement::OnLoseFocus()
{
	if (carotActive)
		DestroyCaret();
	carotActive = false;
}

void TInputTextElement::OnCutCopyPaste(control_text_mode mode)
{
}

bool TInputTextElement::OnInputChar(WCHAR ch, UINT count)
{
	UINT start = 0, end = 0;
	if (carotActive)
	{
		for (int c = 0; c < count; c++)
		{
			switch (ch)
			{
			case VK_BACK:
				if (carotLoc)
					text.Delete(--carotLoc);
				break;
			case VK_OEM_PERIOD:
				text.Insert(carotLoc++, L'.');
				break;
			case VK_SUBTRACT:
				if (ch == L'm')
					goto def;
				text.Insert(carotLoc++, L'-');
				break;
			case VK_DIVIDE:
				if (ch == L'o')
					goto def;
				text.Insert(carotLoc++, L'/');
				break;
			default:
			def:
					text.Insert(carotLoc++, ch);
			}
		}
		ReCreateLayout();
		UpdateCarotPoisition(carotLoc);
	}
	else if (this->highlightRange.GetHighlightRange(start, end))
	{
		text.Delete(start, end - start);

		for (int c = 0; c < count; c++)
		{
			switch (ch)
			{
			case VK_BACK:
				continue;
			case VK_OEM_PERIOD:
				text.Insert(start, L'.');
				break;
			case VK_SUBTRACT:
				if (ch == L'm')
					goto def2;
				text.Insert(start, L'-');
				break;
			case VK_DIVIDE:
				if (ch == L'o')
					goto def2;
				text.Insert(start, L'/');
				break;
			default:
			def2:
				if (ch != VK_RETURN)
					text.Insert(start, ch);
			}
			
		}
		ReCreateLayout();
		UpdateCarotPoisition(start + count);
	}
	return true;
}

void TInputTextElement::OnTransferText(UINT newPos)
{
	UINT start = 0, end = 0;

	if (!highlightRange.GetHighlightRange(start, end) || start == end)
		return;

	if (start <= newPos || newPos <= end)
		return;

	TString subRange(text.SubString(start, end + 1));

	mainLayout->SetDrawingEffect(nullptr, DWRITE_TEXT_RANGE{ start, end });

	if (newPos < start)
	{
		text.Delete(start, end - start);
		text.Insert(newPos, subRange);
	}
	else
	{
		text.Insert(newPos, subRange);
		text.Delete(start, end - start);
		start -= subRange.GetSize();
		end -= subRange.GetSize();
	}

	ReCreateLayout();

	TrecPointer<TBrush> newB = drawingBoard->GetBrush(TColor(this->basicDetails.defaultBackgroundColor));
	TDoubleBrushHolder* dHolder = new TDoubleBrushHolder(this->basicDetails.color->GetUnderlyingBrush().Get(), newB->GetUnderlyingBrush().Get());
	this->mainLayout->SetDrawingEffect(dHolder, DWRITE_TEXT_RANGE{ start, end - start });
	dHolder->Release();
	dHolder = nullptr;
}
