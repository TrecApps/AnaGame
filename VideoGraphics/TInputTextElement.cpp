#include "TInputTextElement.h"

bool TInputTextElement::FindString(const TString& target, UINT& index, bool fromFront)
{
	int iindex = fromFront ? text.Find(target, 0) : text.FindLast(target);
	if(iindex == -1)
		return false;
	index = static_cast<UINT>(iindex);

	return true;
}

void TInputTextElement::UpdateCarotPoisition(UINT loc)
{
	DWRITE_HIT_TEST_METRICS mets;
	ZeroMemory(&mets, sizeof(mets));
	float x = 0.0f, y = 0.0f;
	if (SUCCEEDED(mainLayout->HitTestTextPosition(loc, FALSE, &x, &y, &mets)))
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

UINT TInputTextElement::GetCarotLoc()
{
	return carotLoc;
}

bool TInputTextElement::TakesInput()
{
	return editAllowed;
}

void TInputTextElement::LockText(bool doLock)
{
	editAllowed = !doLock;
}

TInputTextElement::TInputTextElement(TrecPointer<DrawingBoard> drawingBoard, HWND window) : TTextElement(drawingBoard)
{
	carotLoc = 0;
	carotActive = false;
	grabbedRange = false;
	this->window = window;
	editAllowed = true;
}

bool TInputTextElement::OnCLickDown(const TPoint& point)
{
	if (IsContained(point, bounds))
	{
		

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
			{
				this->highlightRange.Reset();
				this->highlightRange.SetBegin(pos);
			}
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
	if (IsContained(point, bounds))
	{
		DWRITE_HIT_TEST_METRICS mets;
		ZeroMemory(&mets, sizeof(mets));
		BOOL trailing = FALSE, inside = FALSE;
		if (isClickDown && SUCCEEDED(mainLayout->HitTestPoint(point.x - bounds.left, point.y - bounds.top, &trailing, &inside, &mets)))
		{
			UINT pos = trailing ? 1 : 0;
			pos += mets.textPosition;
			if (grabbedRange)
			{
				UpdateCarotPoisition(pos);
			}
			else
			{
				
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
		}
		return true;

	}
	return false;
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
	if (carotActive && editAllowed)
	{
		bool didDelete = false;
		if (this->highlightRange.GetHighlightRange(start, end))
		{
			text.Delete(start, end - start);
			this->mainLayout->SetDrawingEffect(nullptr, DWRITE_TEXT_RANGE{ start, end - start });
			highlightRange.Reset();
			didDelete = true;
			carotLoc = start;
		}
		for (int c = 0; c < count; c++)
		{
			switch (ch)
			{
			case VK_BACK:
				if (carotLoc && !didDelete)
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
	return true;
}

bool TInputTextElement::OnInputKey(UINT key, UINT count)
{
	if (!carotActive)
		return false;
	UINT lineCount = text.CountFinds(L'\r');
	UINT curLine = text.CountFinds(L'\r', carotLoc);
	DWRITE_LINE_METRICS* metrics = new DWRITE_LINE_METRICS[lineCount + 1];

	UINT actLineCount = 0;

	mainLayout->GetLineMetrics(metrics, lineCount + 1, &actLineCount);

	bool processed = false;
	float data = 0;

	DWRITE_HIT_TEST_METRICS mets;
	ZeroMemory(&mets, sizeof(mets));
	DWRITE_TEXT_METRICS mets2;
	assert(SUCCEEDED(mainLayout->GetMetrics(&mets2)));
	
	float x = 0.f, y = 0.f;
	BOOL trail = FALSE, inside = FALSE;

	POINT p = { 0,0 };

	GetCaretPos(&p);

	switch (key)
	{
	case VK_UP:
		processed = true;
		if (!curLine)
			break;
		actLineCount = ((curLine < count) ? 0 : curLine - count);

		data = mets2.top + bounds.top;

		for (UINT Rust = 0; Rust <= actLineCount; Rust++)
		{
			data += metrics[Rust].height;
		}

		data -= (metrics[actLineCount].height / 2);

		assert(SUCCEEDED(mainLayout->HitTestPoint(p.x - bounds.left, data, &trail, &inside, &mets)));

		UpdateCarotPoisition(mets.textPosition + (trail ? 0 : 1));

		break;
	case VK_DOWN:
		processed = true;
		if (curLine == lineCount)
			break;
		actLineCount = (curLine + count > lineCount) ? lineCount + 1 : curLine + count;
		data = mets2.top + bounds.top;

		for (UINT Rust = 0; Rust <= actLineCount; Rust++)
		{
			data += metrics[Rust].height;
		}

		data -= (metrics[actLineCount].height / 2);

		assert(SUCCEEDED(mainLayout->HitTestPoint(p.x - bounds.left, data, &trail, &inside, &mets)));

		UpdateCarotPoisition(mets.textPosition + (trail ? 0 : 1));

		break;
	case VK_LEFT:
		if (carotLoc)
			UpdateCarotPoisition(carotLoc - (count > carotLoc ? carotLoc : count));
		processed = true;
		break;
	case VK_RIGHT:
		UpdateCarotPoisition((carotLoc + count >= text.GetSize()) ? text.GetSize() : carotLoc + count);
		processed = true;
	}

	delete[] metrics;
	metrics = nullptr;

	return processed;
}

void TInputTextElement::OnTransferText(UINT newPos)
{
	UINT start = 0, end = 0;

	if (!highlightRange.GetHighlightRange(start, end) || start == end)
		return;

	if (start <= newPos && newPos <= end)
		return;

	TString subRange(text.SubString(start, end));

	mainLayout->SetDrawingEffect(nullptr, DWRITE_TEXT_RANGE{ start, end - start });

	if (newPos < start)
	{
		text.Delete(start, end - start);
		text.Insert(newPos, subRange);
		start = newPos;
		end = start + subRange.GetSize();
	}
	else
	{
		text.Insert(newPos, subRange);
		text.Delete(start, end - start);
		start = newPos - subRange.GetSize();
		end = start + subRange.GetSize();
	}

	highlightRange.Reset();
	highlightRange.SetBegin(start);
	highlightRange.SetEnd(end);

	ReCreateLayout();

	TrecPointer<TBrush> newB = drawingBoard->GetBrush(TColor(this->basicDetails.defaultBackgroundColor));
	TDoubleBrushHolder* dHolder = new TDoubleBrushHolder(this->basicDetails.color->GetUnderlyingBrush().Get(), newB->GetUnderlyingBrush().Get());
	this->mainLayout->SetDrawingEffect(dHolder, DWRITE_TEXT_RANGE{ start, end - start });
	dHolder->Release();
	dHolder = nullptr;
}
