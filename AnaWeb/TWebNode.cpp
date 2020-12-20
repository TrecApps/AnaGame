#include "pch.h"
#include "TWebNode.h"
#include <TPromptControl.h>

#define WEB_EVENT_HANDLER_COUNT 71

static TString eventHandlers[WEB_EVENT_HANDLER_COUNT] = {
	// Window Events
	TString(L"onafterprint"),		TString(L"onbeforeprint"),
	TString(L"onbeforeunload"),		TString(L"onerror"),
	TString(L"onhashchange"),		TString(L"onload"),
	TString(L"onmessage"),			TString(L"onoffline"),
	TString(L"ononline"),			TString(L"onpagehide"),
	TString(L"onpageshow"),			TString(L"onpopstate"),
	TString(L"onresize"),			TString(L"onstorage"),
	TString(L"onunload"),

	// Form Events
	TString(L"onblur"),				TString(L"onchange"),
	TString(L"oncontextmenu"),		TString(L"onfocus"),
	TString(L"oninput"),			TString(L"oninvalid"),
	TString(L"onreset"),			TString(L"onsearch"),
	TString(L"onselect"),			TString(L"onsubmit"),

	// Keyboard Events
	TString(L"onkeydown"),			TString(L"onkeypress"),
	TString(L"onkeyup"),
	
	// Mouse Events
	TString(L"onclick"),			TString(L"ondblclick"),
	TString(L"onmousedown"),		TString(L"onmousemove"),
	TString(L"onmouseout"),			TString(L"onmouseover"),
	TString(L"onmouseup"),			TString(L"onwheel"),

	// Drag Events
	TString(L"ondrag"),				TString(L"ondragend"),
	TString(L"ondragenter"),		TString(L"ondragleave"),
	TString(L"ondragover"),			TString(L"ondragstart"),
	TString(L"ondrop"),				TString(L"onscroll"),

	// Clipboard Events
	TString(L"oncopy"), TString(L"oncut"),
	TString(L"onpaste"),

	// Media Events
	TString(L"onabort"), TString(L"oncanplay"),
	TString(L"oncanplaythrough"), TString(L"oncuechange"),
	TString(L"ondurationchange"), TString(L"onemptied"),
	TString(L"onended"), TString(L"onerror"),
	TString(L"onloadeddata"), TString(L"onloadedmetadata"),
	TString(L"onloadstart"), TString(L"onpause"),
	TString(L"onplay"), TString(L"onplaying"),
	TString(L"onprogress"), TString(L"onratechange"),
	TString(L"onseeked"), TString(L"onseeking"),
	TString(L"onstalled"), TString(L"onsuspend"),
	TString(L"ontimeupdate"), TString(L"onvolumechange"),
	TString(L"onwaiting"),
	
	// Toggle
	TString(L"ontoggle")
};



TWebNode::TWebNode(TrecPointer<DrawingBoard> board)
{
	this->board = board;
}



UINT TWebNode::ProcessHtml(TStringSliceManager& html, UINT& start)
{
	while (start < html->GetSize() && (!tagName.GetSize() || !IsWhitespace(html->GetAt(start))))
	{
		if (!IsWhitespace(html->GetAt(start)))
			tagName.AppendChar(!IsWhitespace(html->GetAt(start)));
		start++;
	}

	bool getAtts = true;

	if (tagName.EndsWith(L'>'))
	{
		getAtts = false;
		tagName.Set(tagName.SubString(tagName.Find(L'>')));
	}

	if (getAtts)
	{
		WCHAR quote = L'\0';

		bool attMode = true;
		TString attribute, value;
		UINT backslashCount = 0;
		while (start < html->GetSize() && (quote || html->GetAt(start) != L'>'))
		{
			WCHAR ch = html->GetAt(start);
			if (ch == L'\\') backslashCount++;

			if (quote)
			{
				if (attMode)
					attribute.AppendChar(ch);
				if (quote == ch && !(backslashCount % 2))
					quote = L'\0';
				else if (!attMode)
					value.AppendChar(ch);
			}
			else
			{
				if (ch == L'\'' || ch == L'\"')
				{
					quote = ch;
					if (attMode)
						attribute.AppendChar(ch);
				}
				else if (ch == L'=')
					attMode = false;
				else if (IsWhitespace(ch))
				{
					attMode = true;
					attributes.addEntry(attribute, value);
					attribute.Empty();
					value.Empty();
				}
				else
					if (attMode)
						attribute.AppendChar(ch);
					else value.AppendChar(ch);
			}

			if (ch != L'\\') backslashCount=0;
			start++;
		}

		if (start >= html->GetSize())
			return 1;

		if (html->GetAt(start) == L'>' && html->GetAt(start - 1) == L'/')
			return 0;
		start++;
	}

	return ProcessInnerHtml(html, start);
}

UINT TWebNode::ProcessInnerHtml(TStringSliceManager& html, UINT& start)
{
	UINT currentStart = start;
	while (start < html->GetSize())
	{
		WCHAR ch = html->GetAt(start++);

		if (ch == L'<')
		{
			try
			{
				if (html->GetAt(start) == L'/')
				{
					innerHtml = html.GetSlice(currentStart, start - 1);

					start = html->Find(L'>', start) + 1;

					return 0;
				}
			}
			catch (WCHAR*)
			{
				return 1;
			}
			// We have a new HTML element to process
			TrecPointer<TWebNode> newNode = TrecPointerKey::GetNewSelfTrecPointer<TWebNode>(board);
			childNodes.push_back(newNode);

			UINT result = newNode->ProcessHtml(html, start);

			if (result) return result;

			if (!start)break;
		}
		
	}

	innerHtml = html.GetSlice(currentStart, html->GetSize() -1);

	start = html->Find(L'>', start) + 1;

	return 2;
}

UINT TWebNode::SetInnerHtml(TStringSliceManager& html)
{
	return 0;
}


void TWebNode::GetElementsByName(const TString& name, TDataArray<TrecPointer<TWebNode>>& nodes)
{
	if (!name.GetSize())
		return;

	if (!name.Compare(this->name))
		nodes.push_back(TrecPointerKey::GetTrecPointerFromSoft<TWebNode>(self));

	for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
	{
		childNodes[Rust]->GetElementsByName(name, nodes);
	}
}

void TWebNode::GetElementsByTag(const TString& tag, TDataArray<TrecPointer<TWebNode>>& nodes)
{
	if (!tag.GetSize())
		return;
	if (!tag.Compare(this->tagName))
		nodes.push_back(TrecPointerKey::GetTrecPointerFromSoft<TWebNode>(self));

	for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
	{
		childNodes[Rust]->GetElementsByTag(name, nodes);
	}
}

void TWebNode::GetElementsByClass(const TString& nodeClass, TDataArray<TrecPointer<TWebNode>>& nodes)
{
	if (!nodeClass.GetSize())
		return;
	if (!nodeClass.Compare(this->nodeClass))
		nodes.push_back(TrecPointerKey::GetTrecPointerFromSoft<TWebNode>(self));

	for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
	{
		childNodes[Rust]->GetElementsByClass(name, nodes);
	}
}

TrecPointer<TWebNode> TWebNode::GetElementById(const TString& id)
{
	// If an empty string, just return
	if(!id.GetSize())
		return TrecPointer<TWebNode>();

	// If this is the node
	if (!id.Compare(this->id))
		return TrecPointerKey::GetTrecPointerFromSoft<TWebNode>(self);

	TrecPointer<TWebNode> ret;

	for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
	{
		ret = childNodes.at(Rust)->GetElementById(id);
		if (ret.Get())
			break;
	}
	return ret;
}

void TWebNode::SetSelf(TrecPointer<TWebNode> s)
{
	if (s.Get() != this)
		throw L"Expected Self to be this";
	self = TrecPointerKey::GetSoftPointerFromTrec<TWebNode>(s);
}

TString TWebNode::GetType()
{
	return TString(L"TWebNode;TObject");
}

UINT TWebNode::CreateWebNode(D2D1_RECT_F location, TrecPointer<TWindowEngine> d3dEngine, TrecPointer<TArray<styleTable>> styles, HWND window)
{
	// This is done to ensure that when events are activated, the node has a smaller map to retireve
	// data from, thus boosting response times
	for (UINT Rust = 0; Rust < WEB_EVENT_HANDLER_COUNT; Rust++)
	{
		TString handlerValue;
		if (attributes.retrieveEntry(eventHandlers[Rust], handlerValue))
		{
			// Add the handler attribute to the handler map, and add a semi-colon in case
			// multiple statements are added
			handlers.addEntry(eventHandlers[Rust], handlerValue + L';');

			// Remove the handler data from attributes to same some memory
			attributes.removeEntry(eventHandlers[Rust], handlerValue);
		}
	}



	if (!tagName.Compare(L"p"))
	{
		// We are dealing with a paragraph block
		control = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TTextField>(board, TrecPointer<TArray<styleTable>>(), window);

		TString text(innerHtml->GetStringCopy());

		for (UINT Rust = 0; Rust < childNodes.Size(); Rust)
		{
			if (!childNodes[Rust].Get())
				continue;
			int found = -1, innerFound = -1,endFound = -1, outerEnd = -1;

			FormattingDetails fd;
			fd.style = DWRITE_FONT_STYLE_NORMAL;
			fd.weight = DWRITE_FONT_WEIGHT_NORMAL;

			if (!childNodes[Rust]->tagName.Compare(L"b"))
			{
				found = text.Find(L"<b");
				innerFound = text.Find(L">", found);
				endFound = text.Find(L"</b>",innerFound);
				outerEnd = 4;
				fd.weight = DWRITE_FONT_WEIGHT_BOLD;
			}
			else if (!childNodes[Rust]->tagName.Compare(L"strong"))
			{
				found = text.Find(L"<strong");
				innerFound = text.Find(L">", found);
				endFound = text.Find(L"</string>", innerFound);
				outerEnd = 9;
				fd.weight = DWRITE_FONT_WEIGHT_BOLD;
			}
			else if (!childNodes[Rust]->tagName.Compare(L"i"))
			{
				found = text.Find(L"<i");
				innerFound = text.Find(L">", found);
				endFound = text.Find(L"</i>", innerFound);
				outerEnd = 4;
				fd.style = DWRITE_FONT_STYLE_ITALIC;
			}
			else if (!childNodes[Rust]->tagName.Compare(L"em"))
			{
				found = text.Find(L"<em");
				innerFound = text.Find(L">", found);
				endFound = text.Find(L"</em>", innerFound);
				outerEnd = 5;
				fd.style = DWRITE_FONT_STYLE_ITALIC;
			}

			if (found == -1 || innerFound == -1 || endFound == -1)
				continue;
			if (innerFound > endFound)
				continue;

			fd.range.startPosition = found;
			fd.range.length = endFound - innerFound;
			

			text.Delete(endFound, outerEnd);
			text.Delete(found, innerFound - found + 1);

			formattingDetails.push_back(fd);
		}


		control->onCreate(location, d3dEngine);
		TrecSubPointer<TControl, TTextField> tControl = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TTextField>(control);

		tControl->SetText(text);
		for (UINT Rust = 0; Rust < formattingDetails.Size(); Rust++)
		{
			tControl->ApplyFormatting(formattingDetails[Rust]);
		}
	}


	return 0;
}
