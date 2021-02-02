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



/**
 * Method: TWebNode::TWebNode
 * Purpose: Constructor
 * Parameters: TrecPointer<DrawingBoard> board - the board to draw on
 * Returns: new Html Node
 */
TWebNode::TWebNode(TrecPointer<DrawingBoard> board)
{
	this->board = board;
	outsideDisplay = WebNodeDisplayOutside::wndo_block;
	insideDisplay = WebNodeDisplayInside::wndi_flex;
	internalDisplay = WebNodeDisplayInternal::wndi_not_set;
	doDisplay = true;
	isListItem = false;
	location = D2D1::RectF();
}



/**
 * Method: TWebNode::ProcessHtml
 * Purpose: Processes new HTML code from a file
 * Parameters: TStringSliceManager& html the string to process
 *              UINT start - the index to start at
 * Returns: UINT - error code (0 for success)
 */
UINT TWebNode::ProcessHtml(TStringSliceManager& html, UINT& start, HWND win)
{
	while (start < html->GetSize() && (!tagName.GetSize() || !IsWhitespace(html->GetAt(start))))
	{
		if (!IsWhitespace(html->GetAt(start)))
			tagName.AppendChar(html->GetAt(start));
		start++;
	}

	bool getAtts = true;

	if (tagName.StartsWith(L"<"))
		tagName.Set(tagName.SubString(1));

	if (tagName.EndsWith(L'>'))
	{
		getAtts = false;
		tagName.Set(tagName.SubString(0, tagName.Find(L'>')));
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

	return ProcessInnerHtml(html, start, win);
}

/**
 * Method: TWebNode::ProcessInnerHtml
 * Purpose: Compiles the html the node has been given
 * Parameters: void
 * Returns: UINT - error code (0 for success)
 */
UINT TWebNode::ProcessInnerHtml(TStringSliceManager& html, UINT& start, HWND win)
{
	this->win = win;
	UINT currentStart = start;
	TString printableText;
	while (start < html->GetSize())
	{
		WCHAR ch = html->GetAt(start++);

		if (ch == L'<')
		{
			printableText.Trim();
			if (printableText.GetSize())
			{
				TrecSubPointer<TControl, TTextField> textEle = TrecPointerKey::GetNewSelfTrecSubPointer<TControl, TTextField>(board, TrecPointer<TArray<styleTable>>(), win);
				textEle->SetText(printableText);
				printableText.Empty();

				childNodes.push_back(TrecPointerKey::GetNewTrecPointer<TWebNode::TWebNodeContainer>(textEle));
			}


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
			childNodes.push_back(TrecPointerKey::GetNewTrecPointer<TWebNode::TWebNodeContainer>(newNode));

			UINT result = newNode->ProcessHtml(html, start, win);

			if (result) return result;

			if (!start)break;
		}
		else
		{
			printableText.AppendChar(ch);
		}
		
	}

	innerHtml = html.GetSlice(currentStart, html->GetSize() -1);

	start = html->Find(L'>', start) + 1;

	attributes.retrieveEntry(L"class", this->nodeClass);
	attributes.retrieveEntry(L"id", this->id);

	return 2;
}

/**
 * Method: TWebNode::SetInnerHtml
 * Purpose: Allows the Node to have it's contents changed dynamically
 * Parameters: const TString& html - the html to set the insides of this node to
 * Returns: UINT - error code (0 for success)
 */
UINT TWebNode::SetInnerHtml(TStringSliceManager& html)
{
	return 0;
}


/**
 * Method: TWebNode::GetElementsByName
 * Purpose: Retrieves a list of elements with the given name
 * Parameters: const TString& name - the name of the nodes to collect
 *              TDataArray<TrecPointer<TWebNode>>& nodes - the structure to hold the nodes in
 * Returns: void
 */
void TWebNode::GetElementsByName(const TString& name, TDataArray<TrecPointer<TWebNode>>& nodes)
{
	if (!name.GetSize())
		return;

	if (!name.Compare(this->name))
		nodes.push_back(TrecPointerKey::GetTrecPointerFromSoft<TWebNode>(self));

	for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
	{
		auto node = childNodes[Rust];
		if(node.Get() && node->type == NodeContainerType::nct_web && node->webNode.Get())
		node->webNode->GetElementsByName(name, nodes);
	}
}

/**
 * Method: TWebNode::GetElementsByTag
 * Purpose: Retrieves a list of elements with the given tag type
 * Parameters: const TString& tag - the tag type of the nodes to collect
 *              TDataArray<TrecPointer<TWebNode>>& nodes - the structure to hold the nodes in
 * Returns: void
 */
void TWebNode::GetElementsByTag(const TString& tag, TDataArray<TrecPointer<TWebNode>>& nodes)
{
	if (!tag.GetSize())
		return;
	if (!tag.Compare(this->tagName))
		nodes.push_back(TrecPointerKey::GetTrecPointerFromSoft<TWebNode>(self));

	for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
	{
		auto node = childNodes[Rust];
		if (node.Get() && node->type == NodeContainerType::nct_web && node->webNode.Get())
			node->webNode->GetElementsByTag(name, nodes);
	}
}

/**
 * Method: TWebNode::GetElementsByClass
 * Purpose: Retrieves a list of elements with the given class type
 * Parameters: const TString& nodeClass - the class type of the nodes to collect
 *              TDataArray<TrecPointer<TWebNode>>& nodes - the structure to hold the nodes in
 * Returns: void
 */
void TWebNode::GetElementsByClass(const TString& nodeClass, TDataArray<TrecPointer<TWebNode>>& nodes)
{
	if (!nodeClass.GetSize())
		return;
	if (!nodeClass.Compare(this->nodeClass))
		nodes.push_back(TrecPointerKey::GetTrecPointerFromSoft<TWebNode>(self));

	for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
	{
		auto node = childNodes[Rust];
		if (node.Get() && node->type == NodeContainerType::nct_web && node->webNode.Get())
			node->webNode->GetElementsByClass(name, nodes);
	}
}

/**
 * Method: TWebNode::GetElementById
 * Purpose: Retrieves the
 * Parameters: const TString& id - the id to seek out
 * Returns: TrecPointer<TWebNode> - the first node with the given id, or noll if the node was not found
 */
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
		auto node = childNodes[Rust];
		if (node.Get() && node->type == NodeContainerType::nct_web && node->webNode.Get())
			ret = node->webNode->GetElementById(id);
		if (ret.Get())
			break;
	}
	return ret;
}

/**
 * Method: TWebNode::SetSelf
 * Purpose: Supports the Self Trait of this class
 * Parameters: TrecPointer<TWebNode> s - the pointer to use to set self to
 * Returns: void
 */
void TWebNode::SetSelf(TrecPointer<TWebNode> s)
{
	if (s.Get() != this)
		throw L"Expected Self to be this";
	self = TrecPointerKey::GetSoftPointerFromTrec<TWebNode>(s);
}

/**
 * Method: TWebNode::GetType
 * Purpose: Reports the Type of this object
 * Parameters: void
 * Returns: TString - the string representation of this objects type
 */
TString TWebNode::GetType()
{
	return TString(L"TWebNode;TObject");
}

/**
 * Method: TWebNode::PreCreate
 * Purpose: Allows Node to predict ahead of time what space it will need and how it will be shown
 * Parameters: TrecPointerSoft<TWebNode> parent - the Web Node that called the method
 * Returns: void
 */
void TWebNode::PreCreate(TrecPointerSoft<TWebNode> parent)
{
	this->parent = parent;

	// Figure out how to set the location based off of the display CSS Atribute
	TString cssDisplay;
	if (attributes.retrieveEntry(L"display", cssDisplay))
	{
		SetDisplay(cssDisplay);
	}
}

/**
 * Method: TWebNode::SetDisplay
 * Purpose: Sets up the display attributes of the web node according to the string provided
 * Parameters: const TString& display - the string holding display info
 * Return: void
 */
void TWebNode::SetDisplay(const TString& display)
{
	doDisplay = true;
	if (display.CountFinds(L"none"))
	{
		doDisplay = false;
		return;
	}

	// Look For legacy attributes (from CSS 2)
	// While The Mozilla Developer Network lists for such values,
	// here: https://developer.mozilla.org/en-US/docs/Web/CSS/display
	// We only check for the first as the other three will be detected when checking css 3 values
	if (display.CountFinds(L"inline-block"))
	{
		insideDisplay = WebNodeDisplayInside::wndi_flow_root;
		outsideDisplay = WebNodeDisplayOutside::wndo_inline;
	}

	// Outside Display
	if (display.CountFinds(L"inline"))
		outsideDisplay = WebNodeDisplayOutside::wndo_inline;
	else outsideDisplay = WebNodeDisplayOutside::wndo_block;

	// Inside Display
	if (display.CountFinds(L"flow-root"))
		insideDisplay = WebNodeDisplayInside::wndi_flow_root;
	else if (display.CountFinds(L"flex"))
		insideDisplay = WebNodeDisplayInside::wndi_flex;
	if (display.CountFinds(L"table"))
		insideDisplay = WebNodeDisplayInside::wndi_table;
	else if (display.CountFinds(L"grid"))
		insideDisplay = WebNodeDisplayInside::wndi_grid;
	if (display.CountFinds(L"ruby"))
		insideDisplay = WebNodeDisplayInside::wndi_ruby;
	else
		insideDisplay = WebNodeDisplayInside::wndi_flow;

	// Handle List item
	isListItem = display.CountFinds(L"list-item") > 0;

	if (display.CountFinds(L"table-row-group"))
		internalDisplay = WebNodeDisplayInternal::wndi_row_group;
	else if (display.CountFinds(L"table-header-group"))
		internalDisplay = WebNodeDisplayInternal::wndi_header_group;
	if (display.CountFinds(L"table-footer-group"))
		internalDisplay = WebNodeDisplayInternal::wndi_footer_group;
	else if (display.CountFinds(L"table-row"))
		internalDisplay = WebNodeDisplayInternal::wndi_row;
	if (display.CountFinds(L"table-cell"))
		internalDisplay = WebNodeDisplayInternal::wndi_cell;
	else if (display.CountFinds(L"table-column-group"))
		internalDisplay = WebNodeDisplayInternal::wndi_column_group;
	if (display.CountFinds(L"table-column"))
		internalDisplay = WebNodeDisplayInternal::wndi_column;
	else if (display.CountFinds(L"table-caption"))
		internalDisplay = WebNodeDisplayInternal::wndi_caption;
	else // To Do: Add support for ruby values
		internalDisplay = WebNodeDisplayInternal::wndi_not_set;
}

/**
 * Method: TWebNode::CreateWebNode
 * Purpose: Sets up the Web Node for Rendering, same purpose as TControl::onCreate()
 * Parameters: D2D1_RECT_F location - the location within the Window the Node is expected to operate in
 *              TrecPointer<TWindowEngine> d3dEngine - Pointer to the 3D manager, for controls with a 3D component to them
 *              TrecPointer<TArray<styleTable>> styles - list of CSS styles that the node should adhere to
 *              HWND window - handle to the window the node is operating in
 *              TrecPointerSoft<TWebNode> parent - the node that called this method
 * Returns: UINT - Error Code
 */
UINT TWebNode::CreateWebNode(D2D1_RECT_F location, TrecPointer<TWindowEngine> d3dEngine, TrecPointer<TArray<styleTable>>& styles, HWND window)
{
	// Go through each bit of the child elements, determining where they lay
	this->location = location;
	// First Compile this nodes own CSS properties
	CompileProperties(styles);

	TrecPointer<TWebNode::TWebNodeContainer> currentTextNode, nonTextNode;
	
	for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
	{
		TrecPointer<TWebNode::TWebNodeContainer> ch = childNodes[Rust];
		if (!ch.Get() || ch->type == TWebNode::NodeContainerType::ntc_null)
		{
			// Take care of null elements
			childNodes.RemoveAt(Rust--);
			continue;
		}

		if (ch->type == TWebNode::NodeContainerType::nct_web)
		{
			if (!ch->webNode.Get())
			{
				// Take care of null elements
				childNodes.RemoveAt(Rust--);
				continue;
			}
			if (ch->webNode->IsText() && (insideDisplay == WebNodeDisplayInside::wndi_flow || insideDisplay == WebNodeDisplayInside::wndi_flow_root))
			{
				ch->webNode->CompileProperties(styles);
				if (nonTextNode.Get())
				{
					auto tLoc = nonTextNode->GetLocation();

					location.top = tLoc.bottom;
					nonTextNode.Nullify();
				}
				if (!currentTextNode.Get())
				{
					TrecSubPointer<TControl, TTextField> textEle = TrecPointerKey::GetNewSelfTrecSubPointer<TControl, TTextField>(board, TrecPointer<TArray<styleTable>>(), win);
					currentTextNode = TrecPointerKey::GetNewTrecPointer<TWebNode::TWebNodeContainer>(textEle);
				}

				ch->webNode->RetrieveText(currentTextNode->textDataList);

			}
			else
			{
				if (currentTextNode.Get())
				{
					CompileText(currentTextNode, location);
					auto tLoc = currentTextNode->GetLocation();

					location.top = tLoc.bottom;
					currentTextNode.Nullify();
				}
				if (nonTextNode.Get())
				{
					auto tLoc = nonTextNode->GetLocation();

					location.top = tLoc.bottom;
					nonTextNode.Nullify();
				}
				
				UINT ret = ch->webNode->CreateWebNode(location, d3dEngine, styles, window);
				if (ret)
					return ret;
				nonTextNode = ch;
			}
			

			continue;
		}

		if (ch->type == TWebNode::NodeContainerType::nct_text)
		{
			TrecSubPointer<TControl, TTextField> t = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TTextField>(ch->control);
			if(!t.Get())
			{
				// Take care of null elements
				childNodes.RemoveAt(Rust--);
				continue;
			}
			TextData tempTextData(thisTextData);
			tempTextData.text.Set(t->GetText());
			if (nonTextNode.Get())
			{
				auto tLoc = nonTextNode->GetLocation();

				location.top = tLoc.bottom;
				nonTextNode.Nullify();
			}
			if (!currentTextNode.Get())
			{
				currentTextNode = ch;
				currentTextNode->initTextSize = tempTextData.text.GetSize();
			}
			
			currentTextNode->textDataList.push_back(thisTextData);
		}

	}

	if (currentTextNode.Get())
	{
		CompileText(currentTextNode, location);
		auto tLoc = currentTextNode->GetLocation();

		location.top = tLoc.bottom;
		currentTextNode.Nullify();
	}
	if (nonTextNode.Get())
	{
		auto tLoc = nonTextNode->GetLocation();

		location.top = tLoc.bottom;
		nonTextNode.Nullify();
	}


	return 0;
}

D2D1_RECT_F TWebNode::GetLocation()
{
	return location;
}

/**
 * Method: TWebNode::OnLButtonDown
 * Purpose: Allows the Node to react to the Button down event
 * Parameters: TDataArray<TString>& script - the script to append to if the node has a relevent event attribute and deems it necessary to append it
 *              TDataArray<TrecObjectPointer>& thisCollection - the collection of objects to use as 'this' when handler is run
 *              TDataArray<TrecPointer<TWebNode>>& nodeCollection - the collection of nodes that could have this handler apply
 *              const TPoint& point - the point in the window where the mouse was clicked
 * Returns: void
 */
void TWebNode::OnLButtonDown(TDataArray<TString>& script, TDataArray<TrecObjectPointer>& thisCollection, TDataArray<TrecPointer<TWebNode>>& nodeCollection, const TPoint& point)
{
	if (!control.Get())
	{
		// To-Do: Handle scenario
	}
	else
	{
		auto loc = control->getLocation();

		if (isContained(point, loc))
		{
			messageOutput mo = messageOutput::negative;
			TDataArray<EventID_Cred> cred;
			TDataArray<TControl*> cl;
			control->OnLButtonDown(0, point, &mo, cred, cl);

			TrecPointer<TWebNode> activeSelf = TrecPointerKey::GetTrecPointerFromSoft<TWebNode>(self);

			nodeCollection.push_back(activeSelf);
			EventPropagater prop;
			if (handlers.retrieveEntry(L"onmousedown", prop))
			{
				if (prop.useCapture)
				{
					script.push_back(prop.event);
					thisCollection.push_back(TrecPointerKey::GetTrecObjectPointer<TWebNode>(activeSelf));
					for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
					{
						if (childNodes[Rust].Get())
						{
							switch (childNodes[Rust]->type)
							{
							case NodeContainerType::nct_control:
							case NodeContainerType::nct_text:
								childNodes[Rust]->control->OnLButtonDown(0, point, &mo,cred, cl);
								break;
							case NodeContainerType::nct_web:
								childNodes[Rust]->webNode->OnLButtonDown(script, thisCollection, nodeCollection, point);
							}
						}
							
					}
					
				}
				else
				{
					for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
					{
						if (childNodes[Rust].Get())
						{
							switch (childNodes[Rust]->type)
							{
							case NodeContainerType::nct_control:
							case NodeContainerType::nct_text:
								childNodes[Rust]->control->OnLButtonDown(0, point, &mo, cred, cl);
								break;
							case NodeContainerType::nct_web:
								childNodes[Rust]->webNode->OnLButtonDown(script, thisCollection, nodeCollection, point);
							}
						}
					}
					script.push_back(prop.event);
					thisCollection.push_back(TrecPointerKey::GetTrecObjectPointer<TWebNode>(activeSelf));
				}
			}
			else
			{
				for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
				{
					if (childNodes[Rust].Get())
					{
						switch (childNodes[Rust]->type)
						{
						case NodeContainerType::nct_control:
						case NodeContainerType::nct_text:
							childNodes[Rust]->control->OnLButtonDown(0, point, &mo, cred, cl);
							break;
						case NodeContainerType::nct_web:
							childNodes[Rust]->webNode->OnLButtonDown(script, thisCollection, nodeCollection, point);
						}
					}
				}
			}
		}
	}
}

/**
 * Method: TWebNode::OnLButtonUp
 * Purpose: Allows the Node to react to the Button Up event
 * Parameters: TDataArray<TString>& script - the script to append to if the node has a relevent event attribute and deems it necessary to append it
 *              TDataArray<TrecObjectPointer>& thisCollection - the collection of objects to use as 'this' when handler is run
 *              TDataArray<TrecPointer<TWebNode>>& nodeCollection - the collection of nodes that could have this handler apply
 *              TrecPointer<TWebNode>& focusNode - the node that has current user focus
 *              const TPoint& point - the point in the window where the mouse was clicked
 * Returns: void
 */
void TWebNode::OnLButtonUp(TDataArray<TString>& script, TDataArray<TrecObjectPointer>& thisCollection, TDataArray<TrecPointer<TWebNode>>& nodeCollection, TrecPointer<TWebNode>& focusNode, const TPoint& point)
{
	bool found = false;

	// Check to see if we are eligable for an on click event
	for (UINT Rust = 0; Rust < nodeCollection.Size(); Rust++)
	{
		if (nodeCollection[Rust].Get() == this)
		{
			found = true;
			nodeCollection.RemoveAt(Rust);
			break;
		}
	}

	if (!control.Get())
	{
		// To-Do: Handle scenario
	}
	else
	{
		auto loc = control->getLocation();
		if (isContained(point, loc))
		{
			messageOutput mo = messageOutput::negative;
			TDataArray<EventID_Cred> cred;
			TDataArray<TControl*> cl;
			control->OnLButtonUp(0, point, &mo, cred);

			TrecPointer<TWebNode> activeSelf = TrecPointerKey::GetTrecPointerFromSoft<TWebNode>(self);

			nodeCollection.push_back(activeSelf);
			EventPropagater prop, clickProp;

			bool hasClick =  found && handlers.retrieveEntry(L"onclick", prop);

			if (handlers.retrieveEntry(L"onmouseup", prop))
			{
				if (prop.useCapture)
				{
					script.push_back(prop.event);
					thisCollection.push_back(TrecPointerKey::GetTrecObjectPointer<TWebNode>(activeSelf));

					if (hasClick && clickProp.useCapture)
					{
						script.push_back(clickProp.event);
						thisCollection.push_back(TrecPointerKey::GetTrecObjectPointer<TWebNode>(activeSelf));
					}

					for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
					{
						if (childNodes[Rust].Get())
						{
							switch (childNodes[Rust]->type)
							{
							case NodeContainerType::nct_control:
							case NodeContainerType::nct_text:
								childNodes[Rust]->control->OnLButtonUp(0, point, &mo, cred);
								break;
							case NodeContainerType::nct_web:
								childNodes[Rust]->webNode->OnLButtonUp(script, thisCollection, nodeCollection, focusNode, point);
							}
						}
					}

					if (hasClick && !clickProp.useCapture)
					{
						script.push_back(clickProp.event);
						thisCollection.push_back(TrecPointerKey::GetTrecObjectPointer<TWebNode>(activeSelf));
					}
					
				}
				else
				{
					if (hasClick && clickProp.useCapture)
					{
						script.push_back(clickProp.event);
						thisCollection.push_back(TrecPointerKey::GetTrecObjectPointer<TWebNode>(activeSelf));
					}

					for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
					{
						if (childNodes[Rust].Get())
						{
							switch (childNodes[Rust]->type)
							{
							case NodeContainerType::nct_control:
							case NodeContainerType::nct_text:
								childNodes[Rust]->control->OnLButtonUp(0, point, &mo, cred);
								break;
							case NodeContainerType::nct_web:
								childNodes[Rust]->webNode->OnLButtonUp(script, thisCollection, nodeCollection, focusNode, point);
							}
						}
					}
					
					script.push_back(prop.event);
					thisCollection.push_back(TrecPointerKey::GetTrecObjectPointer<TWebNode>(activeSelf));
					if (hasClick && !clickProp.useCapture)
					{
						script.push_back(clickProp.event);
						thisCollection.push_back(TrecPointerKey::GetTrecObjectPointer<TWebNode>(activeSelf));
					}
				}
			}
			else
			{
				for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
				{
					if (childNodes[Rust].Get())
					{
						switch (childNodes[Rust]->type)
						{
						case NodeContainerType::nct_control:
						case NodeContainerType::nct_text:
							childNodes[Rust]->control->OnLButtonUp(0, point, &mo, cred);
							break;
						case NodeContainerType::nct_web:
							childNodes[Rust]->webNode->OnLButtonUp(script, thisCollection, nodeCollection, focusNode, point);
						}
					}
				}
			}
		}
	}
}

/**
 * Method: TWebNode::OnLButtonDblClck
 * Purpose: Allows the Node to react to the Button down event
 * Parameters: TDataArray<TString>& script - the script to append to if the node has a relevent event attribute and deems it necessary to append it
 *              TDataArray<TrecObjectPointer>& thisCollection - the collection of objects to use as 'this' when handler is run
 *              TDataArray<TrecPointer<TWebNode>>& nodeCollection - the collection of nodes that could have this handler apply
 *              const TPoint& point - the point in the window where the mouse was clicked
 * Returns: void
 */
void TWebNode::OnLButtonDblClck(TDataArray<TString>& script, TDataArray<TrecObjectPointer>& thisCollection, const TPoint& point)
{
	if (!control.Get())
	{
		// To-Do: Handle scenario
	}
	else
	{
		auto loc = control->getLocation();
		if (isContained(point, loc))
		{
			messageOutput mo = messageOutput::negative;
			TDataArray<EventID_Cred> cred;
			TDataArray<TControl*> cl;
			control->OnLButtonDblClk(0, point, &mo, cred);

			TrecPointer<TWebNode> activeSelf = TrecPointerKey::GetTrecPointerFromSoft<TWebNode>(self);

			EventPropagater prop;
			if (handlers.retrieveEntry(L"ondblclick", prop))
			{
				if (prop.useCapture)
				{
					script.push_back(prop.event);
					thisCollection.push_back(TrecPointerKey::GetTrecObjectPointer<TWebNode>(activeSelf));
					for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
					{
						if (childNodes[Rust].Get())
						{
							switch (childNodes[Rust]->type)
							{
							case NodeContainerType::nct_control:
							case NodeContainerType::nct_text:
								childNodes[Rust]->control->OnLButtonDblClk(0, point, &mo, cred);
								break;
							case NodeContainerType::nct_web:
								childNodes[Rust]->webNode->OnLButtonDblClck(script, thisCollection, point);
							}
						}
					}
					
				}
				else
				{
					for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
					{
						if (childNodes[Rust].Get())
						{
							switch (childNodes[Rust]->type)
							{
							case NodeContainerType::nct_control:
							case NodeContainerType::nct_text:
								childNodes[Rust]->control->OnLButtonDblClk(0, point, &mo, cred);
								break;
							case NodeContainerType::nct_web:
								childNodes[Rust]->webNode->OnLButtonDblClck(script, thisCollection, point);
							}
						}
					}
					script.push_back(prop.event);
					thisCollection.push_back(TrecPointerKey::GetTrecObjectPointer<TWebNode>(activeSelf));
				}
			}
			else
			{
				for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
				{
					if (childNodes[Rust].Get())
					{
						switch (childNodes[Rust]->type)
						{
						case NodeContainerType::nct_control:
						case NodeContainerType::nct_text:
							childNodes[Rust]->control->OnLButtonDblClk(0, point, &mo, cred);
							break;
						case NodeContainerType::nct_web:
							childNodes[Rust]->webNode->OnLButtonDblClck(script, thisCollection, point);
						}
					}
				}
			}
		}
	}
}

/**
 * Method: TWebNode::OnLButtonDown
 * Purpose: Allows the Node to react to the Button down event
 * Parameters: TDataArray<TString>& script - the script to append to if the node has a relevent event attribute and deems it necessary to append it
 *              TDataArray<TrecObjectPointer>& thisCollection - the collection of objects to use as 'this' when handler is run
 *              TDataArray<TrecPointer<TWebNode>>& nodeCollection - the collection of nodes that could have this handler apply
 *              const TPoint& point - the point in the window where the mouse was clicked
 * Returns: void
 */
void TWebNode::OnMouseMove(TDataArray<TString>& script, TDataArray<TrecObjectPointer>& thisCollection, TDataArray<TrecPointer<TWebNode>>& nodeCollection, const TPoint& point)
{
	int found = -1;
	for (int Rust = 0; Rust < nodeCollection.Size(); Rust++)
	{
		if (nodeCollection[Rust].Get() == this)
		{
			found = Rust;
			break;
		}
	}


	if (!control.Get())
	{
		// To-Do: Handle scenario
	}
	else
	{
		auto loc = control->getLocation();
		if (isContained(point, loc))
		{
			

			messageOutput mo = messageOutput::negative;
			TDataArray<EventID_Cred> cred;
			TDataArray<TControl*> cl;
			control->OnLButtonDblClk(0, point, &mo, cred);

			TrecPointer<TWebNode> activeSelf = TrecPointerKey::GetTrecPointerFromSoft<TWebNode>(self);
			if (found == -1)
				nodeCollection.push_back(activeSelf);
			EventPropagater prop, statProp;

			bool hasStat = (found == -1) && handlers.retrieveEntry(L"onmouseover", statProp);

			if (handlers.retrieveEntry(L"ondblclick", prop))
			{
				if (prop.useCapture)
				{
					if (hasStat && statProp.useCapture)
					{
						script.push_back(statProp.event);
						thisCollection.push_back(TrecPointerKey::GetTrecObjectPointer<TWebNode>(activeSelf));
					}

					script.push_back(prop.event);
					thisCollection.push_back(TrecPointerKey::GetTrecObjectPointer<TWebNode>(activeSelf));

					for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
					{
						if (childNodes[Rust].Get())
						{
							switch (childNodes[Rust]->type)
							{
							case NodeContainerType::nct_control:
							case NodeContainerType::nct_text:
								childNodes[Rust]->control->OnMouseMove(0, point, &mo, cred, cl);
								break;
							case NodeContainerType::nct_web:
								childNodes[Rust]->webNode->OnMouseMove(script, thisCollection, nodeCollection, point);
							}
						}
					}

					if (hasStat && !statProp.useCapture)
					{
						script.push_back(statProp.event);
						thisCollection.push_back(TrecPointerKey::GetTrecObjectPointer<TWebNode>(activeSelf));
					}
				}
				else
				{
					if (hasStat && statProp.useCapture)
					{
						script.push_back(statProp.event);
						thisCollection.push_back(TrecPointerKey::GetTrecObjectPointer<TWebNode>(activeSelf));
					}
					for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
					{
						if (childNodes[Rust].Get())
						{
							switch (childNodes[Rust]->type)
							{
							case NodeContainerType::nct_control:
							case NodeContainerType::nct_text:
								childNodes[Rust]->control->OnMouseMove(0, point, &mo, cred, cl);
								break;
							case NodeContainerType::nct_web:
								childNodes[Rust]->webNode->OnMouseMove(script, thisCollection, nodeCollection, point);
							}
						}
					}

					script.push_back(prop.event);
					thisCollection.push_back(TrecPointerKey::GetTrecObjectPointer<TWebNode>(activeSelf));
					if (hasStat && !statProp.useCapture)
					{
						script.push_back(statProp.event);
						thisCollection.push_back(TrecPointerKey::GetTrecObjectPointer<TWebNode>(activeSelf));
					}
				}
			}
			else
			{
				for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
				{
					if (childNodes[Rust].Get())
					{
						switch (childNodes[Rust]->type)
						{
						case NodeContainerType::nct_control:
						case NodeContainerType::nct_text:
							childNodes[Rust]->control->OnMouseMove(0, point, &mo, cred, cl);
							break;
						case NodeContainerType::nct_web:
							childNodes[Rust]->webNode->OnMouseMove(script, thisCollection, nodeCollection, point);
						}
					}
				}
			}
		}
		else
		{
			if (found != -1)
			{
				EventPropagater statProp;
				if (handlers.retrieveEntry(L"onmouseover", statProp))
				{
					script.push_back(statProp.event);
					thisCollection.push_back(TrecPointerKey::GetTrecObjectPointer<TWebNode>(TrecPointerKey::GetTrecPointerFromSoft<TWebNode>(self)));
				}
				nodeCollection.RemoveAt(found);
			}
		}
	}
}

/**
 * Method: TWebNode::OnLoseFocus
 * Purpose: Alert the WebNode that it has lost focus
 * Parameters: void
 * Returns: the script to run (empty if no script is found)
 */
TString TWebNode::OnLoseFocus()
{
	EventPropagater prop;
	if (handlers.retrieveEntry(L"onblur", prop))
		return prop.event;
	return TString();
}

void TWebNode::CompileProperties(TrecPointer<TArray<styleTable>>& styles)
{
	// A Good Metric to assess whether CSS Compilation is necessary
	if (handlers.count())
		return;

	// This is done to ensure that when events are activated, the node has a smaller map to retireve
	// data from, thus boosting response times
	for (UINT Rust = 0; Rust < WEB_EVENT_HANDLER_COUNT; Rust++)
	{
		TString handlerValue;
		if (attributes.retrieveEntry(eventHandlers[Rust], handlerValue))
		{
			// Add the handler attribute to the handler map
			EventPropagater prop;
			prop.event = handlerValue;
			handlers.addEntry(eventHandlers[Rust], prop);

			// Remove the handler data from attributes to same some memory
			attributes.removeEntry(eventHandlers[Rust], handlerValue);
		}
	}

	TDataMap<TString> atts;
	for (UINT Rust = 0; Rust < styles->Count(); Rust++)
	{
		TrecPointer<styleTable> style = styles->ElementAt(Rust);
		if (!style.Get())
		{
			styles->DeleteAt(Rust--);
			continue;
		}

		// Start with the Node type
		if (style->style.Find(tagName) != -1)
		{
			for (UINT C = 0; C < style->names.count(); C++)
			{
				auto styleEntry = style->names.GetEntryAt(C);
				atts.addEntry(styleEntry->key, styleEntry->object.Get());
			}
			continue;
		}

		// Next check the ID type
		if (id.GetSize() && style->style.Find(TString(L"#") + id) != -1)
		{
			for (UINT C = 0; C < style->names.count(); C++)
			{
				auto styleEntry = style->names.GetEntryAt(C);
				atts.addEntry(styleEntry->key, styleEntry->object.Get());
			}
			continue;
		}

		// Next get attributes for classes
		auto classes = nodeClass.split(L"\s\n\r\t");
		for (UINT c = 0; c < classes->Size(); c++)
		{
			TString cl = classes->at(c).GetTrim();
			if (cl.GetSize() && style->style.Find(tagName + L"." + cl) != -1)
			{
				for (UINT C = 0; C < style->names.count(); C++)
				{
					auto styleEntry = style->names.GetEntryAt(C);
					atts.addEntry(styleEntry->key, styleEntry->object.Get());
				}
				continue;
			}
			else if (cl.GetSize() && style->style.Find(TString(L".") + cl) != -1)
			{
				for (UINT C = 0; C < style->names.count(); C++)
				{
					auto styleEntry = style->names.GetEntryAt(C);
					atts.addEntry(styleEntry->key, styleEntry->object.Get());
				}
				continue;
			}
		}
	}

	for (UINT Rust = 0; Rust < attributes.count(); Rust++)
	{
		TDataEntry<TString> entry;
		if (attributes.GetEntryAt(Rust, entry))
			atts.addEntry(entry.key, entry.object);
	}

	TString val;

	// take care of text attributes
	if (atts.retrieveEntry(L"color", val))
	{

	}

	if (atts.retrieveEntry(L"font-style", val))
	{
		if (!val.Compare(L"italic"))
			thisTextData.fontStyle = DWRITE_FONT_STYLE_ITALIC;
		else if (!val.Compare(L"oblique"))
			thisTextData.fontStyle = DWRITE_FONT_STYLE_OBLIQUE;
		// Default is normal
	}


	if (atts.retrieveEntry(L"font-weight", val))
	{
		if (!val.Compare(L"100"))
		{
			thisTextData.fontWeight = DWRITE_FONT_WEIGHT_THIN;
		}
		else if (!val.Compare(L"200"))
		{
			thisTextData.fontWeight = DWRITE_FONT_WEIGHT_EXTRA_LIGHT;
		}
		else if (!val.Compare(L"300") || !val.Compare(L"lighter"))
		{
			thisTextData.fontWeight = DWRITE_FONT_WEIGHT_LIGHT;
		}
		else if (!val.Compare(L"400"))
		{
			thisTextData.fontWeight = DWRITE_FONT_WEIGHT_NORMAL;
		}
		else if (!val.Compare(L"500"))
		{
			thisTextData.fontWeight = DWRITE_FONT_WEIGHT_MEDIUM;
		}
		else if (!val.Compare(L"600"))
		{
			thisTextData.fontWeight = DWRITE_FONT_WEIGHT_DEMI_BOLD;
		}
		else if (!val.Compare(L"700") || !val.Compare(L"bold"))
		{
			thisTextData.fontWeight = DWRITE_FONT_WEIGHT_BOLD;
		}
		else if (!val.Compare(L"800"))
		{
			thisTextData.fontWeight = DWRITE_FONT_WEIGHT_ULTRA_BOLD;
		}
		else if (!val.Compare(L"900"))
		{
			thisTextData.fontWeight = DWRITE_FONT_WEIGHT_BLACK;
		}
		else if (!val.Compare(L"950"))
		{
			thisTextData.fontWeight = DWRITE_FONT_WEIGHT_EXTRA_BLACK;
		}
		else if (!val.Compare(L"350"))
		{
			thisTextData.fontWeight = DWRITE_FONT_WEIGHT_SEMI_LIGHT;
		}
		else if (!val.Compare(L"inherit"))
		{
			// To-Do:
		}

	}


	if (atts.retrieveEntry(L"", val))
	{

	}


	if (atts.retrieveEntry(L"", val))
	{

	}


	if (atts.retrieveEntry(L"", val))
	{

	}

	for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
	{
		auto ch = childNodes[Rust];
		if (!ch.Get() || ch->type == NodeContainerType::ntc_null)
		{
			childNodes.RemoveAt(Rust--);
			continue;
		}

		if (ch->type == NodeContainerType::nct_web)
		{
			if(!ch->webNode.Get())
			{
				childNodes.RemoveAt(Rust--);
				continue;
			}

			ch->webNode->CompileProperties(styles);
		}
	}
	
}

bool TWebNode::IsText()
{
	if(this->outsideDisplay != WebNodeDisplayOutside::wndo_inline)
		return false; // In this case, text generated here cannot be injected into a parent Node

	// Assume true until discovered Otherwise

	for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
	{
		auto childNode = childNodes[Rust];
		if (!childNode.Get())
		{
			childNodes.RemoveAt(Rust--);
			continue;
		}
		switch (childNode->type)
		{
		case NodeContainerType::nct_text:
			if (!dynamic_cast<TTextField*>(childNode->control.Get()))
				throw L"Unexpected Null TextField detected in IsText Method Call";
			continue;
		case NodeContainerType::nct_control:
			return false;
		case NodeContainerType::ntc_null:
			childNodes.RemoveAt(Rust--);
			continue;
		case NodeContainerType::nct_web:
			if (!childNode->webNode.Get())
				throw L"Unexpected Null Web Node detected in IsText Method Call";
			if (!childNode->webNode->IsText())
				return false;
		}

	}
	return true;
}

void TWebNode::RetrieveText(TDataArray<TextData>& textDataList)
{
	if (!IsText())
		return;

	for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
	{
		auto childNode = childNodes[Rust];
		if (!childNode.Get())
		{
			childNodes.RemoveAt(Rust--);
			continue;
		}
		if (childNode->type == NodeContainerType::nct_text)
		{
			// ThisTextData should be initialized in a create call prior to this method being called
			TextData submitData(thisTextData);
			submitData.text.Set(dynamic_cast<TTextField*>(childNode.Get())->GetText());
		}
		else
		{
			// We Are dealing with a WebNode, as the other options would have been detected by Call to IsText and we would not reach here
			childNode->webNode->RetrieveText(textDataList);
		}

	}
}

/**
 * Method: TWebNode::CompileText
 * Purpose: Compiles Text Nodes according to the text Data it contains
 * Parameters: TrecPointer<TWebNode::TWebNodeContainer> textNode - the text node to compile
 *              D2D1_RECT_F loc - the space the text has been allocated
 * Returns: void
 */
void TWebNode::CompileText(TrecPointer<TWebNode::TWebNodeContainer> textNode, D2D1_RECT_F loc)
{
	if (!textNode.Get() || textNode->type != NodeContainerType::nct_text || !dynamic_cast<TTextField*>(textNode->control.Get()))
		return;

	TTextField* textField = dynamic_cast<TTextField*>(textNode->control.Get());
	textField->Resize(loc);
	FormattingDetails det;
	TString theText;
	for (UINT Rust = 0; Rust < textNode->textDataList.Size(); Rust++)
	{
		theText.Append(textNode->textDataList[Rust].text);
	}
	textField->SetText(theText);
	UINT beginningIndex = 0;
	for (UINT Rust = 0; Rust < textNode->textDataList.Size(); Rust++)
	{
		det.range.startPosition = beginningIndex;
		det.range.length = textNode->textDataList[Rust].text.GetSize();
		beginningIndex = det.range.length + 1;

		det.style = textNode->textDataList[Rust].fontStyle;
		det.weight = textNode->textDataList[Rust].fontWeight;

		// Do-To: Add more fields to the FormattingDetails object and transfer dditional fields over to it


		// End to-do
		textField->ApplyFormatting(det);
	}
}

EventPropagater::EventPropagater()
{
	useCapture = false;
}

EventPropagater::EventPropagater(const EventPropagater& orig)
{
	this->event.Set(orig.event);
	this->useCapture = orig.useCapture;
}

EventPropagater::EventPropagater(const TString& e, bool useCapture)
{
	event.Set(e);
	this->useCapture = useCapture;
}

TWebNode::TWebNodeContainer::TWebNodeContainer(TrecSubPointer<TControl, TTextField> text)
{
	if (text.Get())
	{
		type = NodeContainerType::nct_text;
		this->control = TrecPointerKey::GetTrecPointerFromSub<TControl, TTextField>(text);
	}
	else
		type = NodeContainerType::ntc_null;
	initTextSize = 0;
}

TWebNode::TWebNodeContainer::TWebNodeContainer(TrecPointer<TControl> control)
{
	if (control.Get())
	{
		type = NodeContainerType::nct_control;
		this->control = control;
	}
	else
		type = NodeContainerType::ntc_null;
	initTextSize = 0;
}

TWebNode::TWebNodeContainer::TWebNodeContainer(TrecPointer<TWebNode> webNode)
{
	if (webNode.Get())
	{
		type = NodeContainerType::nct_web;
		this->webNode = webNode;
	}
	else
		type = NodeContainerType::ntc_null;
	initTextSize = 0;
}

D2D1_RECT_F TWebNode::TWebNodeContainer::GetLocation()
{
	if (webNode.Get())
		return webNode->GetLocation();
	else if (control.Get())
		return control->getLocation();
	return { 0,0,0,0 };
}

TextData::TextData()
{
	fontSize = 12;
	flowDirection = DWRITE_FLOW_DIRECTION_TOP_TO_BOTTOM;
	fontStretch = DWRITE_FONT_STRETCH_NORMAL;
	fontStyle = DWRITE_FONT_STYLE_NORMAL;
	fontWeight = DWRITE_FONT_WEIGHT_NORMAL;
	lineSpacing = DWRITE_LINE_SPACING_METHOD_DEFAULT;
	textColor = backgroundColor = D2D1::ColorF(D2D1::ColorF::Black);
	hasBackgroundColor = false;
}

TextData::TextData(const TextData& data)
{
	fontSize = data.fontSize;
	flowDirection = data.flowDirection;
	fontStretch = data.fontStretch;
	fontStyle = data.fontStyle;
	fontWeight = data.fontWeight;
	lineSpacing = data.lineSpacing;
	textColor = data.textColor;
	backgroundColor = data.backgroundColor;
	hasBackgroundColor = data.hasBackgroundColor;
}
