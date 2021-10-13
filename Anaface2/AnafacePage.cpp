#include "AnafacePage.h"
#include <TFormatReader.h>
#include <TScrollerPage.h>

TString objectTypes[] = {
	L"TControl",		// Basic Control
	L"TRandomLayout",	// Control that does not organize child layouts
	L"TLayout",			// Control that organizes child layouts in one dimension
	L"TGrid",			// Control that organizes child layouts in a grid
	L"TDataLayout",		// TLayout that adds data-binding
	L"TDataGrid",		// TGrid that supports data-binding
	L"TRadioButton",	// Control that acts as a radio button
	L"TCheckBox",		// Control that acts as a Checkbox
	L"TInput",			// Control that acts as a basic input element
	L"TLongInput",		// Control that provides more complex input
	L"TBasicPrompt",	// Extends TLongInput to support Command Prompt-like interface
	L"TArena",			// Supports a view for 3D content
	L"TVideo",			// Provides support for video playback
	L"TCanvas",			// Allows for drawing custom items
	L"TImage",			// Provides advanged image manipulation options
	
};



AnafacePage::AnafacePage(TrecPointer<DrawingBoard> board): TPage(board)
{
}

bool AnafacePage::HandlesEvents()
{
	return handler.Get() != nullptr;
}

TString AnafacePage::PrepPage(TrecPointer<TFileShell> file, TrecPointer<EventHandler> handler)
{
	TString ret;
	if (!file.Get())
		ret.Set(L"Null File");
	if (!handler.Get())
	{
		if (ret.GetSize())
			ret.Append(L" And ");
		ret.Append(L"Null Handler");
	}

	if (ret.GetSize())
	{
		ret.Append(L"provided!");
		return ret;
	}

	TrecPointer<TFormatReader> reader = TFormatReader::GetReader(file);

	if (!reader.Get())
	{
		ret.Format(L"Could Not get a format reader for '%ws'. Likely because File format is not supported!", file->GetName().GetConstantBuffer().getBuffer());
		return ret;
	}

	ret.Set(reader->Read());

	if (ret.GetSize())
		return ret;

	TrecPointer<TVariable> uiData = reader->GetData();

	HandleNode(L"", ret, uiData, rootPage);


	return ret;
}

void AnafacePage::Draw(TrecPointer<TVariable> object)
{
	if (rootPage.Get())
		rootPage->Draw(object);
}

void AnafacePage::OnRButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>& cred, TDataArray<EventArgs>& events)
{
	if (rootPage.Get())
		rootPage->OnRButtonUp(nFlags, point, mOut, cred, events);
	if (mOut != message_output::mo_negative)
	{
		if (handler.Get())
			handler->HandleEvents(events);

		mOut = message_output::mo_positive_handled;

		// We are handling events by Child element, so no need to keep them
		events.RemoveAll();
	}
}

void AnafacePage::OnRButtonDown(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<EventArgs>& events)
{
	if (rootPage.Get())
		rootPage->OnRButtonDown(nFlags, point, mOut, eventAr, events);
	if (mOut != message_output::mo_negative)
	{
		if (handler.Get())
			handler->HandleEvents(events);

		mOut = message_output::mo_positive_handled;

		// We are handling events by Child element, so no need to keep them
		events.RemoveAll();
	}
}

void AnafacePage::OnLButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<EventArgs>& events)
{
	if (rootPage.Get())
		rootPage->OnLButtonUp(nFlags, point, mOut, eventAr, events);
	if (mOut != message_output::mo_negative)
	{
		if (handler.Get())
			handler->HandleEvents(events);

		mOut = message_output::mo_positive_handled;

		// We are handling events by Child element, so no need to keep them
		events.RemoveAll();
	}
}

void AnafacePage::OnLButtonDown(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<EventArgs>& events)
{
	if (rootPage.Get())
		rootPage->OnLButtonDown(nFlags, point, mOut, eventAr, events);
	if (mOut != message_output::mo_negative)
	{
		if (handler.Get())
			handler->HandleEvents(events);

		mOut = message_output::mo_positive_handled;

		// We are handling events by Child element, so no need to keep them
		events.RemoveAll();
	}
}

void AnafacePage::OnMouseMove(UINT nFlags, TPoint point, message_output& mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<EventArgs>& events)
{
	if (rootPage.Get())
		rootPage->OnMouseMove(nFlags, point, mOut, eventAr, events);
	if (mOut != message_output::mo_negative)
	{
		if (handler.Get())
			handler->HandleEvents(events);

		mOut = message_output::mo_positive_handled;

		// We are handling events by Child element, so no need to keep them
		events.RemoveAll();
	}
}

void AnafacePage::OnLButtonDblClk(UINT nFlags, TPoint point, message_output& mOut, TDataArray<EventArgs>& events)
{
	if (rootPage.Get())
		rootPage->OnLButtonDblClk(nFlags, point, mOut, events);
	if (mOut != message_output::mo_negative)
	{
		if (handler.Get())
			handler->HandleEvents(events);

		mOut = message_output::mo_positive_handled;

		// We are handling events by Child element, so no need to keep them
		events.RemoveAll();
	}
}

void AnafacePage::OnResize(D2D1_RECT_F& newLoc, UINT nFlags, TDataArray<EventID_Cred>& eventAr, TDataArray<EventArgs>& events)
{
	if (rootPage.Get())
		rootPage->OnResize(newLoc, nFlags, eventAr, events);
	if (handler.Get())
		handler->HandleEvents(events);

	// We are handling events by Child element, so no need to keep them
	events.RemoveAll();
	this->area = newLoc;
}

bool AnafacePage::OnDestroy()
{
	return true;
}

bool AnafacePage::OnScroll(bool, const TPoint& point, const TPoint& direction, TDataArray<EventArgs>&)
{
	return false;
}

void AnafacePage::InjectScrollerPage(const D2D1_RECT_F& bounds, const D2D1_RECT_F& needs, TrecPointer<TPage> page)
{
	if (page.Get() == rootPage.Get())
	{
		TrecPointer<TPage> scroller = TrecPointerKey::GetNewSelfTrecPointerAlt<TPage, TScrollerPage>(drawingBoard, page);
		TDataArray<EventID_Cred> cred;
		TDataArray<EventArgs> args;
		auto b = bounds;
		scroller->OnResize(b, 0, cred, args);
		rootPage = scroller;
	}
}

void AnafacePage::HandleNode(const TString& name, TString& result, TrecPointer<TVariable> var, TrecPointer<TPage> curPage)
{
}

TrecPointer<TPage> AnafacePage::HandleControl(const TString& name, TString& result, TrecPointer<TVariable> var)
{
	return TrecPointer<TPage>();
}
