#include "pch.h"
#include "TWebNode.h"
#include <TPromptControl.h>

#define WEB_EVENT_HANDLER_COUNT 71
#define WEB_BORDER_STYLE_COUNT 8
#define WEB_LIST_STYLE_COUNT 22

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

typedef struct StyleString {
	TString str;
	stroke_style style;
}StyleString;

static StyleString styleStrings[WEB_BORDER_STYLE_COUNT] = {
	{TString(L"dashed"), stroke_style::bs_dashed},
	{TString(L"dotted"), stroke_style::bs_dotted},
	{TString(L"double"), stroke_style::bs_double},
	{TString(L"groove"), stroke_style::bs_groove},
	{TString(L"inset"), stroke_style::bs_inset},
	{TString(L"outset"), stroke_style::bs_outset},
	{TString(L"ridge"), stroke_style::bs_ridge},
	{TString(L"solid"), stroke_style::bs_solid},
};

typedef struct ListStyleString 
{
	TString str;
	list_style style;
}ListStyleString;

static ListStyleString listStyleStrings[WEB_LIST_STYLE_COUNT] = {
	{TString(L"disc"), list_style::ls_disc},
	{TString(L"circle"), list_style::ls_circle},
	{TString(L"none"), list_style::ls_none},
	{TString(L"square"), list_style::ls_square},

	{TString(L"armenian"), list_style::ls_armenian},
	{TString(L"cjk-ideographic"), list_style::ls_cjk},
	{TString(L"decimal"), list_style::ls_decimal},
	{TString(L"decimal-leading-zero"), list_style::ls_decimal_0},
	{TString(L"georgian"), list_style::ls_georgian},
	{TString(L"hebrew"), list_style::ls_hebrew},
	{TString(L"hiragana"), list_style::ls_hiragana},
	{TString(L"hiragana-iroha"), list_style::ls_hiragana_iroha},
	{TString(L"katakana"), list_style::ls_katakana},
	{TString(L"katakana-iroha"), list_style::ls_katakana_iroha},
	{TString(L"lower-alpha"), list_style::ls_lower_alph},
	{TString(L"lower-greek"), list_style::ls_lower_greek},
	{TString(L"lower-latin"), list_style::ls_lower_latin},
	{TString(L"lower-roman"), list_style::ls_lower_roman},
	{TString(L"upper-alpha"), list_style::ls_upper_alpha},
	{TString(L"upper-greek"), list_style::ls_upper_greek},
	{TString(L"upper-latin"), list_style::ls_upper_latin},
	{TString(L"upper-roman"), list_style::ls_upper_roman},
};

bool GetListStyle(const TString& styleStr, list_style& style)
{
	for (UINT Rust = 0; Rust < WEB_LIST_STYLE_COUNT; Rust++)
	{
		if (styleStr.CompareNoCase(listStyleStrings[Rust].str))
		{
			style = listStyleStrings[Rust].style;
			return true;
		}
	}
	return false;
}

void IncrementDecrementString(TString& str, WCHAR lower, WCHAR upper, UCHAR mode)
{
	if (!mode)
		return;

	if (mode > 0)
	{
		// Incrementing
		bool changeDetected = false;
		UINT Rust = str.GetSize() - 1;
		for (; Rust < str.GetSize(); Rust--)
		{
			if (str.GetAt(Rust) < upper)
			{
				str[Rust] = str[Rust] + 1;
				changeDetected = true;
				break;
			}
		}

		if (changeDetected)
			Rust++;
		else
		{
			Rust = 1;
			str.Set(TString(lower) + str);
		}
		for (; Rust < str.GetSize(); Rust++)
			str[Rust] = lower;
	}
	else
	{
		if (!str.Compare(lower))
			return; // Can't go any lower

		// Decrementing
		UINT Rust = str.GetSize() - 1;
		bool changeDetected = false;
		for (; Rust < str.GetSize(); Rust--)
		{
			if (str[Rust] > lower)
			{
				str[Rust] = str[Rust] - 1;
				changeDetected = true;
				break;
			}
		}

		if (changeDetected)
			Rust++;
		else
		{
			// They are all the lower letter, delete one character and set the rest to the upper char
			Rust = 0;
			str.Delete(0);
		}
		for (; Rust < str.GetSize(); Rust++)
			str[Rust] = upper;
	}
}


/**
 * Function: GetListPrepend
 * Purpose: Helper function for getting the list bullet
 * Parameters: TString& str - the current point
 *				list_style style - the list style
 *				UCHAR mode - positive if getting the next one, negtive if moving backwards
 * Returns: void
 * 
 * Note: Values for the unordered list were found courtesy of https://unicode-table.com/en/
 */
void GetListPrepend(TString& str, list_style style, signed char mode)
{
	switch (style)
	{
	case list_style::ls_disc:
		str.Set(L'\x25cf');
		break;
	case list_style::ls_circle:
		str.Set(L'\x25cb');
		break;
	case list_style::ls_square:
		str.Set(L'\x25ad');
		break;
	case list_style::ls_none:
		str.Set(L'\s');
		break;
	case list_style::ls_decimal:
	case list_style::ls_decimal_0:
		if (!str.GetSize())
			str.Set(style == list_style::ls_decimal ? L"1." : L"01.");
		else
		{
			TString num(str.SubString(0, str.GetSize() - 1));
			int val = 1;
			num.ConvertToInt(val);
			if (mode > 0)
				val++;
			else if (mode < 0)
				val--;
			str.Format(style == list_style::ls_decimal ? L"%i." : L"0%i.", val);
		}
		break;
	case list_style::ls_lower_alph:
		if (!str.GetSize())
			str.Set(L"a.");
		else
		{
			str.Set(str.SubString(0, str.GetSize() - 1));
			IncrementDecrementString(str, L'a', L'z', mode);
			str.AppendChar(L'.');
		}
		break;
	case list_style::ls_upper_alpha:
		if (!str.GetSize())
			str.Set(L"A.");
		else
		{
			str.Set(str.SubString(0, str.GetSize() - 1));
			IncrementDecrementString(str, L'A', L'Z', mode);
			str.AppendChar(L'.');
		}
		break;
	}
}


// False string
TrecPointer<TString> falseString = TrecPointerKey::GetNewTrecPointer<TString>(L"false");


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
	doDisplay = shouldDraw = true;
	isListItem = false;
	location = innerMargin = outerMargin = D2D1::RectF();
	columnSpan = rowSpan = 1;
	listForward = true;
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
		bool doBreak = html->GetAt(start) == L'>';
		start++;
		if (doBreak)
			break;
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
			// printableText.Trim();
			if (printableText.GetTrim().GetSize())
			{
				TrecSubPointer<TControl, TTextField> textEle = TrecPointerKey::GetNewSelfTrecSubPointer<TControl, TTextField>(board, TrecPointer<TArray<styleTable>>(), win);
				
				textEle->SetText(printableText);
				TrecPointer<TWebNode::TWebNodeContainer> newNode = TrecPointerKey::GetNewTrecPointer<TWebNode::TWebNodeContainer>(textEle);
				
				childNodes.push_back(newNode);
				printableText.Empty();
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
 *              TrecPointer<TArray<styleTable>> styles - list of CSS styles that the node should adhere to
 * Returns: void
 */
void TWebNode::PreCreate(TrecPointerSoft<TWebNode> parent, TrecPointer<TArray<styleTable>>& styles)
{
	this->parent = parent;
	// First Compile this nodes own CSS properties
	CompileProperties(styles);


	// Figure out how to set the location based off of the display CSS Atribute
	TString cssDisplay;
	if (attributes.retrieveEntry(L"display", cssDisplay))
	{
		SetDisplay(cssDisplay);
	}

	for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
	{
		TrecPointer<TWebNode::TWebNodeContainer> ch = childNodes[Rust];
		if (!ch.Get() || ch->type == TWebNode::NodeContainerType::ntc_null)
		{
			// Take care of null elements
			childNodes.RemoveAt(Rust--);
			continue;
		}

		if (ch->type == TWebNode::NodeContainerType::nct_web && ch->webNode.Get())
		{
			ch->webNode->PreCreate(self, styles);
		}
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

	if (display.CountFinds(L"table-row-group"))
		internalDisplay = WebNodeDisplayInternal::wndi_row_group;
	else if (display.CountFinds(L"table-header-group"))
		internalDisplay = WebNodeDisplayInternal::wndi_header_group;
	else if (display.CountFinds(L"table-footer-group"))
		internalDisplay = WebNodeDisplayInternal::wndi_footer_group;
	else if (display.CountFinds(L"table-row"))
		internalDisplay = WebNodeDisplayInternal::wndi_row;
	else if (display.CountFinds(L"table-cell"))
		internalDisplay = WebNodeDisplayInternal::wndi_cell;
	else if (display.CountFinds(L"table-column-group"))
		internalDisplay = WebNodeDisplayInternal::wndi_column_group;
	else if (display.CountFinds(L"table-column"))
		internalDisplay = WebNodeDisplayInternal::wndi_column;
	else if (display.CountFinds(L"table-caption"))
		internalDisplay = WebNodeDisplayInternal::wndi_caption;
	else // To Do: Add support for ruby values
		internalDisplay = WebNodeDisplayInternal::wndi_not_set;


	// Inside Display
	if (display.CountFinds(L"flow-root"))
		insideDisplay = WebNodeDisplayInside::wndi_flow_root;
	else if (display.CountFinds(L"flex"))
		insideDisplay = WebNodeDisplayInside::wndi_flex;
	else if ((internalDisplay == WebNodeDisplayInternal::wndi_not_set) ? (display.CountFinds(L"table") == 1) : (display.CountFinds(L"table") > 1))
		insideDisplay = WebNodeDisplayInside::wndi_table;
	else if (display.CountFinds(L"grid"))
		insideDisplay = WebNodeDisplayInside::wndi_grid;
	else if (display.CountFinds(L"ruby"))
		insideDisplay = WebNodeDisplayInside::wndi_ruby;
	else
		insideDisplay = WebNodeDisplayInside::wndi_flow;

	// Handle List item
	isListItem = display.CountFinds(L"list-item") > 0;


}

/**
 * Method: TWebNode::CreateWebNode
 * Purpose: Sets up the Web Node for Rendering, same purpose as TControl::onCreate()
 * Parameters: D2D1_RECT_F location - the location within the Window the Node is expected to operate in
 *              TrecPointer<TWindowEngine> d3dEngine - Pointer to the 3D manager, for controls with a 3D component to them
 *              HWND window - handle to the window the node is operating in
 *              TrecPointerSoft<TWebNode> parent - the node that called this method
 * Returns: UINT - Error Code
 */
UINT TWebNode::CreateWebNode(D2D1_RECT_F location, TrecPointer<TWindowEngine> d3dEngine, HWND window)
{
	// Go through each bit of the child elements, determining where they lay
	this->location = location;
	if(window)
		this->win = window;
	this->d3dEngine = d3dEngine;

	TrecPointer<TWebNode::TWebNodeContainer> currentTextNode, nonTextNode;

	// Handle Scenario where this Element is a table
	if (insideDisplay == WebNodeDisplayInside::wndi_table)
	{
		for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
		{
			TrecPointer<TWebNode::TWebNodeContainer> ch = childNodes[Rust];
			if (!ch.Get() || ch->type == TWebNode::NodeContainerType::ntc_null)
			{
				// Take care of null elements
				childNodes.RemoveAt(Rust--);
				continue;
			}

			if (ch->type == TWebNode::NodeContainerType::nct_web && ch->webNode.Get())
			{
				ch->webNode->PreEstablishTable();
			}
		}

		// To-Do: Figure out the actual sizes of the columns. 
		
		// For the time being, assume that all columns are the same
		float totalWidth = location.right - location.left;
		for (UINT Rust = 0; Rust < columnSizes.Size(); Rust++)
		{
			columnSizes[Rust] = totalWidth / columnSizes.Size();
		}

		for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
		{
			TrecPointer<TWebNode::TWebNodeContainer> ch = childNodes[Rust];

			if (ch->type == TWebNode::NodeContainerType::nct_web && ch->webNode.Get())
			{
				ch->webNode->SetColumnsAndRows(columnSizes, rowSizes);
			}
			
		}
	}
	
	UINT cellLeft = 0;

	// Handle case where this Node is a list item
	if (isListItem && parent.Get())
	{
		listInfo.Set(parent.Get()->GetListPrepend());
	}


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
					if (internalDisplay != WebNodeDisplayInternal::wndi_row)
					{
						auto tLoc = nonTextNode->GetLocation();
						location.top = tLoc.bottom;
					}
					nonTextNode.Nullify();
				}
				if (internalDisplay == WebNodeDisplayInternal::wndi_row && Rust < columnSizes.Size())
				{
					// Specific code for Table Rows
					auto tempLoc = location;
					tempLoc.left = cellLeft;
					tempLoc.right = cellLeft + columnSizes[Rust];
					cellLeft = tempLoc.right;
					UINT ret = ch->webNode->CreateWebNode(tempLoc, d3dEngine, window);
					if (ret)
						return ret;
				}
				else
				{
					// COde for every other element type
					UINT ret = ch->webNode->CreateWebNode(location, d3dEngine, window);
					if (ret)
						return ret;
				}
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

			if(tempTextData.text.GetTrim().GetSize() && window)
				currentTextNode->textDataList.push_back(tempTextData);
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

		if (internalDisplay != WebNodeDisplayInternal::wndi_row)
			location.top = tLoc.bottom;
		else
			location.right = tLoc.left;
		nonTextNode.Nullify();
	}
	if (window)
	{
		ShrinkHeight();
		if (insideDisplay == WebNodeDisplayInside::wndi_table)
			ShrinkWidth(0);
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
	for (UINT Rust = 0; Rust < nodeCollection.Size(); Rust++)
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

/**
 * Method: TWebNode::OnDraw
 * Purpose: Draws the Node
 * Parameters: void
 * Returns: void
 */
void TWebNode::OnDraw()
{
	if (!shouldDraw)
		return;
	// To-Do: Draw Self Content once such resources are added to the node
	DrawBorder();


	// Now draw the ChildNodes
	for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
	{
		if (!childNodes[Rust].Get() || childNodes[Rust]->type == NodeContainerType::ntc_null)
		{
			childNodes.RemoveAt(Rust--);
			continue;
		}

		if (childNodes[Rust]->webNode.Get())
			childNodes[Rust]->webNode->OnDraw();
		else if (childNodes[Rust]->control.Get())
			childNodes[Rust]->control->onDraw();
	}
}


void TWebNode::CompileProperties(TrecPointer<TArray<styleTable>>& styles)
{
	TString empty;
	attributes.removeEntry(empty, empty);

	// A Good Metric to assess whether CSS Compilation is necessary
	if (attributes.count())
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
		if (style->style.Find(tagName, 0, true, true) != -1)
		{
			for (UINT C = 0; C < style->names.count(); C++)
			{
				auto styleEntry = style->names.GetEntryAt(C);
				atts.addEntry(styleEntry->key, styleEntry->object.Get());
			}
			continue;
		}

		// Next check the ID type
		if (id.GetSize() && style->style.Find(TString(L"#") + id, 0, true, true) != -1)
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
			if (cl.GetSize() && style->style.Find(tagName + L"." + cl, 0, true, true) != -1)
			{
				for (UINT C = 0; C < style->names.count(); C++)
				{
					auto styleEntry = style->names.GetEntryAt(C);
					atts.addEntry(styleEntry->key, styleEntry->object.Get());
				}
				continue;
			}
			else if (cl.GetSize() && style->style.Find(TString(L".") + cl, 0, true, true) != -1)
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

	if (atts.retrieveEntry(L"display", val))
	{
		SetDisplay(val);
	}

	// List Attributes
	if (atts.retrieveEntry(L"list-style-type", val))
	{
		GetListStyle(val, listStyle);
	}

	::GetListPrepend(this->listInfo, this->listStyle, 0);


	// take care of text attributes
	if (atts.retrieveEntry(L"color", val))
	{
		thisTextData.textColor.SetColor(val);
		thisTextData.textColorUpdated = true;
	}

	if (atts.retrieveEntry(L"font-style", val))
	{
		if (!val.Compare(L"italic"))
			thisTextData.fontStyle = DWRITE_FONT_STYLE_ITALIC;
		else if (!val.Compare(L"oblique"))
			thisTextData.fontStyle = DWRITE_FONT_STYLE_OBLIQUE;
		// Default is normal
		thisTextData.fontStyleUpdated = true;
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

		thisTextData.fontWeightUpdated = true;
	}


	if (atts.retrieveEntry(L"border", val))
	{
		borderData.CompileAttributes(val, border_side::bs_all);
	}

	if (atts.retrieveEntry(L"border-bottom", val))
	{
		borderData.CompileAttributes(val, border_side::bs_bottom);
	}

	if (atts.retrieveEntry(L"border-left", val))
	{
		borderData.CompileAttributes(val, border_side::bs_left);
	}

	if (atts.retrieveEntry(L"border-right", val))
	{
		borderData.CompileAttributes(val, border_side::bs_right);
	}

	if (atts.retrieveEntry(L"border-top", val))
	{
		borderData.CompileAttributes(val, border_side::bs_top);
	}

	if (atts.retrieveEntry(L"border-color", val))
		borderData.CompileColor(val, border_side::bs_all);
	if (atts.retrieveEntry(L"border-left-color", val))
		borderData.CompileColor(val, border_side::bs_left);
	if (atts.retrieveEntry(L"border-bottom-color", val))
		borderData.CompileColor(val, border_side::bs_bottom);
	if (atts.retrieveEntry(L"border-top-color", val))
		borderData.CompileColor(val, border_side::bs_top);
	if (atts.retrieveEntry(L"border-right-color", val))
		borderData.CompileColor(val, border_side::bs_right);

	if (atts.retrieveEntry(L"border-width", val))
		borderData.CompileBorder(val, border_side::bs_all);
	if (atts.retrieveEntry(L"border-left-width", val))
		borderData.CompileBorder(val, border_side::bs_left);
	if (atts.retrieveEntry(L"border-bottom-width", val))
		borderData.CompileBorder(val, border_side::bs_bottom);
	if (atts.retrieveEntry(L"border-top-width", val))
		borderData.CompileBorder(val, border_side::bs_top);
	if (atts.retrieveEntry(L"border-right-width", val))
		borderData.CompileBorder(val, border_side::bs_right);

	if (atts.retrieveEntry(L"border-style", val))
		borderData.CompileStyle(val, border_side::bs_all);
	if (atts.retrieveEntry(L"border-left-style", val))
		borderData.CompileStyle(val, border_side::bs_left);
	if (atts.retrieveEntry(L"border-bottom-style", val))
		borderData.CompileStyle(val, border_side::bs_bottom);
	if (atts.retrieveEntry(L"border-top-style", val))
		borderData.CompileStyle(val, border_side::bs_top);
	if (atts.retrieveEntry(L"border-right-style", val))
		borderData.CompileStyle(val, border_side::bs_right);

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
	shouldDraw = true;
	if(this->outsideDisplay != WebNodeDisplayOutside::wndo_inline)
		return false; // In this case, text generated here cannot be injected into a parent Node
	if (this->isListItem)
		return false;

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
	shouldDraw = false;
	return true;
}

void TWebNode::RetrieveText(TDataArray<TextData>& textDataList)
{
	if (!IsText())
		return;

	UINT curListSize = textDataList.Size();

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
			submitData.text.Set(dynamic_cast<TTextField*>(childNode->control.Get())->GetText());
			if(submitData.text.GetTrim().GetSize())
				textDataList.push_back(submitData);
		}
		else
		{
			// We Are dealing with a WebNode, as the other options would have been detected by Call to IsText and we would not reach here
			childNode->webNode->RetrieveText(textDataList);
		}

	}
	// Now to Update all sections added with this Nodes Text Data
	for (UINT Rust = curListSize + 1; Rust < textDataList.Size(); Rust++)
	{
		textDataList[Rust].Update(thisTextData);
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

	// Limit the location based off of the margins and borders
	loc.left += innerMargin.left + borderData.GetBorderThickness(border_side::bs_left);
	loc.right -= (innerMargin.right + borderData.GetBorderThickness(border_side::bs_right));
	loc.top += innerMargin.top + borderData.GetBorderThickness(border_side::bs_top);



	TTextField* textField = dynamic_cast<TTextField*>(textNode->control.Get());
	textField->addAttribute(L"|CanEdit", falseString);
	textField->onCreate(loc, TrecPointer<TWindowEngine>());
	FormattingDetails det;
	TString theText;
	for (UINT Rust = 0; Rust < textNode->textDataList.Size(); Rust++)
	{
		theText.Append(textNode->textDataList[Rust].text);
	}
	UINT beginningIndex = 0;
	if (isListItem)
	{
		textField->SetText(listInfo + L"  " + theText);
		beginningIndex = listInfo.GetSize() + 2;
	}
	else
	{
		textField->SetText(theText);
	}

	for (UINT Rust = 0; Rust < textNode->textDataList.Size(); Rust++)
	{
		det.range.startPosition = beginningIndex;
		det.range.length = textNode->textDataList[Rust].text.GetSize();
		beginningIndex += det.range.length;

		det.style = textNode->textDataList[Rust].fontStyle;
		det.weight = textNode->textDataList[Rust].fontWeight;
		det.color = board->GetBrush(textNode->textDataList[Rust].textColor);
		det.fontSize = textNode->textDataList[Rust].fontSize;

		// Do-To: Add more fields to the FormattingDetails object and transfer dditional fields over to it


		// End to-do
		textField->ApplyFormatting(det);
	}
}

void TWebNode::AddColumn(UCHAR count)
{
	if (insideDisplay == WebNodeDisplayInside::wndi_table)
	{
		while (columnSizes.Size() < count)
		{
			columnSizes.push_back(0);
		}
	}
	else if (parent.Get())
	{
		auto fullParent = TrecPointerKey::GetTrecPointerFromSoft<TWebNode>(parent);
		if (fullParent.Get())
			fullParent->AddColumn(count);
	}
}

void TWebNode::AddRow()
{
	if (insideDisplay == WebNodeDisplayInside::wndi_table)
		rowSizes.push_back(0);
	else
	{
		auto fullParent = TrecPointerKey::GetTrecPointerFromSoft<TWebNode>(parent);
		if (fullParent.Get())
			fullParent->AddRow();
	}
}

void TWebNode::SetColumnsAndRows(TDataArray<UINT>& cols, TDataArray<UINT>& rows)
{
	columnSizes.RemoveAll();
	for (UINT Rust = 0; Rust < cols.Size(); Rust++)
	{
		columnSizes.push_back(cols[Rust]);
	}
	rowSizes.RemoveAll();
	for (UINT Rust = 0; Rust < rows.Size(); Rust++)
	{
		rowSizes.push_back(rows[Rust]);
	}

	for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
	{
		if (!childNodes[Rust].Get() || childNodes[Rust]->type == NodeContainerType::ntc_null)
		{
			childNodes.RemoveAt(Rust--);
			continue;
		}

		if (childNodes[Rust]->type == NodeContainerType::nct_web && childNodes[Rust]->webNode.Get())
			childNodes[Rust]->webNode->SetColumnsAndRows(cols, rows);
	}
}

void TWebNode::PreEstablishTable()
{
	auto fullParent = TrecPointerKey::GetTrecPointerFromSoft<TWebNode>(parent);
	switch (internalDisplay)
	{
	case WebNodeDisplayInternal::wndi_row:
		// To-Do: Call Add Row once Available
		AddRow();
	case WebNodeDisplayInternal::wndi_footer_group:
	case WebNodeDisplayInternal::wndi_header_group:
	case WebNodeDisplayInternal::wndi_row_group:
		for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
		{
			if (!childNodes[Rust].Get() || childNodes[Rust]->type == NodeContainerType::ntc_null)
			{
				childNodes.RemoveAt(Rust--);
				continue;
			}

			if (childNodes[Rust]->type == NodeContainerType::nct_web && childNodes[Rust]->webNode.Get())
			{
				childNodes[Rust]->webNode->PreEstablishTable();
				if (childNodes[Rust]->webNode->columnSizes.Size())
					childNodes[Rust]->webNode->AddColumn(Rust + 1);
			}
		}

		break;
	default: 
		columnSizes.push_back(0);
		return;
	}

	if (fullParent.Get())
		fullParent->AddColumn(columnSizes.Size());
	

}

void TWebNode::ShrinkHeight()
{
	// To-Do: Update Method once minimum height is tracked

	float curBottom = location.top;
	for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
	{
		if (!childNodes[Rust].Get() || childNodes[Rust]->type == NodeContainerType::ntc_null)
		{
			childNodes.RemoveAt(Rust--);
			continue;
		}

		if (childNodes[Rust]->control.Get())
		{
			childNodes[Rust]->control->ShrinkHeight();
			float chBottom = childNodes[Rust]->control->getLocation().bottom;
			if (chBottom > curBottom)
				curBottom = chBottom;
		}
		else if (childNodes[Rust]->webNode.Get())
		{
			childNodes[Rust]->webNode->ShrinkHeight();
			float chBottom = childNodes[Rust]->webNode->location.bottom;
			if (chBottom > curBottom)
				curBottom = chBottom;
		}
		else
		{
			childNodes.RemoveAt(Rust--);
			continue;
		}
	}
	if (curBottom > location.top)
		location.bottom = curBottom;
}

void TWebNode::ShrinkWidth(UINT minWidth)
{
	if (insideDisplay == WebNodeDisplayInside::wndi_table)
	{
		float width = NeedsWidth(0);

		for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
		{
			auto wnode = childNodes[Rust];
			if (!wnode.Get())
			{
				childNodes.RemoveAt(Rust--);
				continue;
			}
			TTextField* field = dynamic_cast<TTextField*>(wnode->control.Get());
			TWebNode* node = wnode->webNode.Get();

			if (field)
			{
				auto fieldLoc = field->getLocation();
				fieldLoc.right = fieldLoc.left + width;
				field->Resize(fieldLoc);
			}
			else if (node)
			{
				if (node->internalDisplay == WebNodeDisplayInternal::wndi_row)
				{
					node->SetColumnsAndRows(columnSizes, rowSizes);
					node->ShrinkWidth(0);
				}
				else
				{
					auto nodeLoc = node->GetLocation();
					nodeLoc.right = nodeLoc.left + width;
					node->CreateWebNode(nodeLoc, d3dEngine, nullptr);
				}
			}
		}
	}

	if (internalDisplay == WebNodeDisplayInternal::wndi_row)
	{
		auto tempLoc = location;
		if (columnSizes.Size())
			tempLoc.right = tempLoc.left + columnSizes[0];
		for (UINT Rust = 0, C = 0; Rust < columnSizes.Size() && C < childNodes.Size(); Rust++)
		{
			UINT curWidth = tempLoc.right - tempLoc.left;
			auto wnode = childNodes[Rust];
			if (!wnode.Get())
			{
				childNodes.RemoveAt(Rust--);
				continue;
			}
			TTextField* field = dynamic_cast<TTextField*>(wnode->control.Get());
			TWebNode* node = wnode->webNode.Get();

			if (field)
			{
				field->Resize(tempLoc);
				C++;
			}
			else if (node)
			{
				UINT span = 1;
				

				for (; span < node->columnSpan; span++)
				{
					if (C + span < columnSizes.Size())
						curWidth += columnSizes[C + span];
				}
				C += span;
				tempLoc.right = tempLoc.left + curWidth;
				node->CreateWebNode(tempLoc, d3dEngine, nullptr);
			}

			if ((Rust + 1) < columnSizes.Size())
			{
				tempLoc.left = tempLoc.right;
				tempLoc.right += columnSizes[Rust + 1];
			}
		}
	}

}

float TWebNode::NeedsWidth(UINT column)
{
	if (internalDisplay == WebNodeDisplayInternal::wndi_row)
	{
		for (UINT Rust = 0, C = 0; Rust < childNodes.Size() && C <= column; Rust++)
		{
			float ret = 0.0f;
			UINT colSpan = 1;

			if (childNodes[Rust].Get())
			{
				TTextField* field = dynamic_cast<TTextField*>(childNodes[Rust]->control.Get());
				TWebNode* node = childNodes[Rust]->webNode.Get();
				if (field)
				{
					ret = field->GetMinWidth();
				}
				else if(node)
				{
					ret = node->NeedsWidth(0);
					colSpan = node->columnSpan;
				}
			}
			if (C == column)
				return ret + 1;
			if (!colSpan)
				colSpan = 1;
			C += colSpan;
		}
	}
	else if (insideDisplay == WebNodeDisplayInside::wndi_table)
	{
		float minNeeded = 0.0f;
		for (UINT Rust = 0; Rust < columnSizes.Size(); Rust++)
		{
			float minOfCol = 0;
			for (UINT C = 0; C < childNodes.Size(); C++)
			{
				auto wnode = childNodes[C];
				if (!wnode.Get())
				{
					childNodes.RemoveAt(Rust--);
					continue;
				}
				TTextField* field = dynamic_cast<TTextField*>(wnode->control.Get());
				TWebNode* node = wnode->webNode.Get();
				if (field)
				{
					float needs = field->GetMinWidth();
					if (minNeeded < needs)
						minNeeded = needs;
				}
				else if (node && node->internalDisplay == WebNodeDisplayInternal::wndi_row)
				{
					float needs = node->NeedsWidth(Rust);
					if (minOfCol < needs)
						minOfCol = needs;
				}
				else if (node)
				{
					float needs = node->NeedsWidth(0);
					if (minNeeded < needs)
						minNeeded = needs;
				}
			}

			columnSizes[Rust] = minOfCol;
		}

		float totalColumnSpan = 0;
		for (UINT Rust = 0; Rust < columnSizes.Size(); Rust++)
		{
			totalColumnSpan += columnSizes[Rust];
		}
		return ((minNeeded < totalColumnSpan) ? totalColumnSpan : minNeeded) + 1;
	}
	else
	{
		float ret = 0.0;
		for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
		{
			auto wnode = childNodes[Rust];
			if (!wnode.Get())
			{
				childNodes.RemoveAt(Rust--);
				continue;
			}
			TTextField* field = dynamic_cast<TTextField*>(wnode->control.Get());
			TWebNode* node = wnode->webNode.Get();
			if (field)
			{
				float needs = field->GetMinWidth();
				if (ret < needs)
					ret = needs;
			}
			else if (node)
			{
				float needs = node->NeedsWidth(0);
				if (ret < needs)
					ret = needs;
			}
		}
		return ret ? ret + 1 : ret;
	}

	return 0.0f;
}

/**
 * Method: TWebNode::GetListPrePend
 * Purpose: Called by list-items, provides the list item with the Prepend String
 * Parameters: void
 * Returns: TString - the current prepender
 */
TString TWebNode::GetListPrepend()
{
	TString ret(listInfo);
	
	if (this->listForward)
		::GetListPrepend(listInfo, listStyle, 1);
	else
		::GetListPrepend(listInfo, listStyle, -1);
	return ret;
}

/**
 * Method: TWebNode::DrawBorder
 * Purpose: Holds the logic for actually drawing a border around the content
 * Parameters: void
 * Returns: void
 */
void TWebNode::DrawBorder()
{
	TColor color;

	// Start with the top border
	float curThick = static_cast<float>(borderData.GetBorderThickness(border_side::bs_top));
	stroke_style curStyle = borderData.GetStrokeStyle(border_side::bs_top);
	if (curThick && hasStyle(curStyle))
	{
		borderData.GetBorderColor(color, border_side::bs_top);
		if (!borderData.brush.Get())
			borderData.brush = board->GetBrush(color);
		else borderData.brush->SetColor(color);
		if (borderData.brush.Get())
		{
			float top = location.top + (static_cast<float>(curThick) / 2.0f);
			borderData.brush->DrawLine(D2D1::Point2F(location.left, top), D2D1::Point2F(location.right, top), board->GetStrokeStyle(curStyle), curThick);
		}
	}

	// go for the right border
	curThick = static_cast<float>(borderData.GetBorderThickness(border_side::bs_right));
	stroke_style curStyle = borderData.GetStrokeStyle(border_side::bs_right);
	if (curThick && hasStyle(curStyle))
	{
		borderData.GetBorderColor(color, border_side::bs_right);
		if (!borderData.brush.Get())
			borderData.brush = board->GetBrush(color);
		else borderData.brush->SetColor(color);
		if (borderData.brush.Get())
		{
			float right = location.right - (static_cast<float>(curThick) / 2.0f);
			borderData.brush->DrawLine(D2D1::Point2F(right, location.top), D2D1::Point2F(right, location.bottom), board->GetStrokeStyle(curStyle), curThick);
		}
	}

	// now the left border
	curThick = static_cast<float>(borderData.GetBorderThickness(border_side::bs_left));
	stroke_style curStyle = borderData.GetStrokeStyle(border_side::bs_left);
	if (curThick && hasStyle(curStyle))
	{
		borderData.GetBorderColor(color, border_side::bs_left);
		if (!borderData.brush.Get())
			borderData.brush = board->GetBrush(color);
		else borderData.brush->SetColor(color);
		if (borderData.brush.Get())
		{
			float left = location.left + (static_cast<float>(curThick) / 2.0f);
			borderData.brush->DrawLine(D2D1::Point2F(left, location.top), D2D1::Point2F(left, location.bottom), board->GetStrokeStyle(curStyle), curThick);
		}
	}

	// finally, the bottom border
	curThick = static_cast<float>(borderData.GetBorderThickness(border_side::bs_bottom));
	stroke_style curStyle = borderData.GetStrokeStyle(border_side::bs_bottom);
	if (curThick && hasStyle(curStyle))
	{
		borderData.GetBorderColor(color, border_side::bs_bottom);
		if (!borderData.brush.Get())
			borderData.brush = board->GetBrush(color);
		else borderData.brush->SetColor(color);
		if (borderData.brush.Get())
		{
			float bottom = location.bottom + (static_cast<float>(curThick) / 2.0f);
			borderData.brush->DrawLine(D2D1::Point2F(location.left, bottom), D2D1::Point2F(location.right, bottom), board->GetStrokeStyle(curStyle), curThick);
		}
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
	fontSize = 16;
	flowDirection = DWRITE_FLOW_DIRECTION_TOP_TO_BOTTOM;
	fontStretch = DWRITE_FONT_STRETCH_NORMAL;
	fontStyle = DWRITE_FONT_STYLE_NORMAL;
	fontWeight = DWRITE_FONT_WEIGHT_NORMAL;
	lineSpacing = DWRITE_LINE_SPACING_METHOD_DEFAULT;
	textColor = backgroundColor = D2D1::ColorF(D2D1::ColorF::Black);
	hasBackgroundColor = false;

	fontSizeUpdated = flowDirectionUpdated = fontStretchUpdated =
		fontStyleUpdated = fontWeightUpdated = lineSpacingUpdated = textColorUpdated = false;
}

TextData::TextData(const TextData& data)
{
	fontSize = data.fontSize;
	fontSizeUpdated = data.fontSizeUpdated;

	flowDirection = data.flowDirection;
	flowDirectionUpdated = data.flowDirectionUpdated;

	fontStretch = data.fontStretch;
	fontStretchUpdated = data.fontStretchUpdated;

	fontStyle = data.fontStyle;
	fontStyleUpdated = data.fontStyleUpdated;

	fontWeight = data.fontWeight;
	fontWeightUpdated = data.fontWeightUpdated;
	
	lineSpacing = data.lineSpacing;
	lineSpacingUpdated = data.lineSpacingUpdated;

	textColor = data.textColor;
	textColorUpdated = data.textColorUpdated;

	backgroundColor = data.backgroundColor;

	hasBackgroundColor = data.hasBackgroundColor;
	text.Set(data.text);

}

void TextData::Update(const TextData& copy)
{
	if (copy.flowDirectionUpdated && !flowDirectionUpdated)
	{
		flowDirection = copy.flowDirection;
		flowDirectionUpdated = true;
	}
	if (copy.fontSizeUpdated && !fontSizeUpdated)
	{
		fontSize = copy.fontSize;
		fontSizeUpdated = true;
	}
	if (copy.fontStretchUpdated && !fontStretchUpdated)
	{
		fontStretch = copy.fontStretch;
		fontStretchUpdated = true;
	}
	if (copy.fontStyleUpdated && !fontStyleUpdated)
	{
		fontStyle = copy.fontStyle;
		fontStyleUpdated = true;
	}
	if (copy.fontWeightUpdated && !fontWeightUpdated)
	{
		fontWeight = copy.fontWeight;
		fontWeightUpdated = true;
	}
	if (copy.lineSpacingUpdated && !lineSpacingUpdated)
	{
		lineSpacing = copy.lineSpacing;
		lineSpacingUpdated = true;
	}
	if (copy.textColorUpdated && !textColorUpdated)
	{
		textColor = copy.textColor;
		textColorUpdated = true;
	}

}

BorderData::BorderData()
{
	borderStyle = topStyle = bottomStyle = rightStyle = leftStyle = stroke_style::bs_not_set;
	thick = topThick = bottomThick = rightThick = leftThick = 0;
}

BorderData::BorderData(const BorderData& copy)
{
	borderStyle = copy.borderStyle;
	topStyle = copy.topStyle;
	bottomStyle = copy.bottomStyle;
	leftStyle = copy.leftStyle;
	rightStyle = copy.rightStyle;

	thick = copy.thick;
	topThick = copy.topThick;
	bottomThick = copy.bottomThick;
	leftThick = copy.leftThick;
	rightThick = copy.rightThick;

	color = copy.color;
	topColor = copy.topColor;
	bottomColor = copy.bottomColor;
	leftColor = copy.leftColor;
	rightColor = copy.rightColor;
}

void BorderData::CompileAttributes(TString& atts, border_side side)
{
}

void BorderData::CompileBorder(TString& atts, border_side size)
{
	atts.Trim();
	
	UINT dpi = GetDpiForSystem();
	float dpif = static_cast<float>(dpi);
	assert(dpi && dpif);
	WebSizeUnit unit = WebSizeUnit::wsu_bl;

	if (atts.EndsWith(L"px"))
	{
		unit = WebSizeUnit::wsu_px;
	}
	if (atts.EndsWith(L"cm"))
	{
		unit = WebSizeUnit::wsu_cm;
	}
	if (atts.EndsWith(L"mm"))
	{
		unit = WebSizeUnit::wsu_mm;
	}
	if (atts.EndsWith(L"in"))
	{
		unit = WebSizeUnit::wsu_in;
	}
	if (atts.EndsWith(L"em"))
	{
		unit = WebSizeUnit::wsu_em;
	}
	if (atts.EndsWith(L"pt"))
	{
		unit = WebSizeUnit::wsu_pt;
	}

	if (unit != WebSizeUnit::wsu_bl)
		atts.Set(atts.SubString(0, atts.GetSize() - 2));

	atts.Trim();

	float sizeVal = 0.0f;
	if (!atts.ConvertToFloat(sizeVal)) {
		// We were able to get a value out of what was provided
		switch (unit)
		{
		case WebSizeUnit::wsu_cm:
			sizeVal = sizeVal * 2.54 / dpif;
			break;
		case WebSizeUnit::wsu_em:
			
			break;
		case WebSizeUnit::wsu_in:
			sizeVal = sizeVal / dpif;
			break;
		case WebSizeUnit::wsu_mm:
			sizeVal = sizeVal * 0.254 / dpif;
			break;
		case WebSizeUnit::wsu_pt:
			sizeVal = sizeVal * 72.0f / dpif;
			break;
		case WebSizeUnit::wsu_bl:
		case WebSizeUnit::wsu_px:
			sizeVal = sizeVal * (dpif / 96.0f);
		}

		switch (size)
		{
		case border_side::bs_bottom:
			this->bottomThick = sizeVal;
			break;
		case border_side::bs_left:
			this->leftThick = sizeVal;
			break;
		case border_side::bs_right:
			this->rightThick = sizeVal;
			break;
		case border_side::bs_top:
			this->topThick = sizeVal;
			break;
		default:
			this->thick = sizeVal;
		}


	}

}

void BorderData::CompileColor(TString& atts, border_side side)
{
	TColor color;
	bool worked = false;

	if (side == border_side::bs_all)
	{
		TDataArray<TString> str1, str2;
		TString tok;
		bool parenthStack = false;
		for (UINT Rust = 0; Rust < atts.GetSize(); Rust++)
		{
			WCHAR ch = atts[Rust];

			if (ch == L' ')
			{
				if (tok.GetSize() && !parenthStack)
				{
					str1.push_back(tok);
					tok.Empty();
				}
			}
			else
				tok.AppendChar(ch);
			if (ch == L'(')
				parenthStack = true;
			else if (ch == L')')
				parenthStack = false;
		}
		if (tok.GetSize())
			str1.push_back(tok);

		tok.Empty();

		for (UINT Rust = 0; Rust < str1.Size(); Rust++)
		{
			TString curTok(str1[Rust]);
			if (!curTok.StartsWith(L'(') && tok.GetSize())
			{
				str2.push_back(tok);
				tok.Empty();
			}
			tok.Append(curTok);
		}
		if (tok.GetSize())
			str2.push_back(tok);

		switch (str2.Size())
		{
		case 1:
			// Single color applies to all borders
			color = TColor::GetColorFromString(str2[0], worked);
			if (worked)
				this->color = color;
			break;
		case 2:
			// First Color applies to top and bottom
			color = TColor::GetColorFromString(str2[0], worked);
			if (worked)
				this->topColor = this->bottomColor = color;
			// Second color applies to left and right
			color = TColor::GetColorFromString(str2[1], worked);
			if (worked)
				this->leftColor = this->rightColor = color;
			break;
		case 3:
			// First Color applies to top
			color = TColor::GetColorFromString(str2[0], worked);
			if (worked)
				this->topColor = color;
			// Second color applies to left and right
			color = TColor::GetColorFromString(str2[1], worked);
			if (worked)
				this->leftColor = this->rightColor = color;
			// Third Color applies to bottom
			color = TColor::GetColorFromString(str2[2], worked);
			if (worked)
				this->bottomColor = color;
			break;
		case 4:
			// First color applies to top
			color = TColor::GetColorFromString(str2[0], worked);
			if (worked)
				this->topColor = color;
			// Second color applies to right
			color = TColor::GetColorFromString(str2[1], worked);
			if (worked)
				this->rightColor = color;
			// First color applies to bottom
			color = TColor::GetColorFromString(str2[2], worked);
			if (worked)
				this->bottomColor = color;
			// Second color applies to left
			color = TColor::GetColorFromString(str2[3], worked);
			if (worked)
				this->leftColor = color;
		}
	}
	else
	{
		switch (side)
		{
		case border_side::bs_bottom:
			color = TColor::GetColorFromString(atts, worked);
			if (worked)
				this->bottomColor = color;
			break;
		case border_side::bs_left:
			color = TColor::GetColorFromString(atts, worked);
			if (worked)
				this->leftColor = color;
			break;
		case border_side::bs_right:
			color = TColor::GetColorFromString(atts, worked);
			if (worked)
				this->rightColor = color;
			break;
		case border_side::bs_top:
			color = TColor::GetColorFromString(atts, worked);
			if (worked)
				this->topColor = color;
			break;
		}
	}
}

bool BorderData::CompileStyle(TString& atts, border_side side)
{
	for (UINT Rust = 0; Rust < WEB_BORDER_STYLE_COUNT; Rust++)
	{
		if (!styleStrings[Rust].str.Compare(atts))
		{
			switch (side)
			{
			case border_side::bs_bottom:
				this->bottomStyle = styleStrings[Rust].style;
				break;
			case border_side::bs_left:
				this->leftStyle = styleStrings[Rust].style;
				break;
			case border_side::bs_right:
				this->rightStyle = styleStrings[Rust].style;
				break;
			case border_side::bs_top:
				this->topStyle = styleStrings[Rust].style;
				break;
			default:
				this->borderStyle = styleStrings[Rust].style;
			}
			return true;
		}
	}

	return false;
}

USHORT BorderData::GetBorderThickness(border_side side)
{
	if (side == border_side::bs_all)
		return 0; // We need a specific side
	stroke_style sideStyle = stroke_style::bs_not_set;
	USHORT sideThick = 0;

	switch (side)
	{
	case border_side::bs_bottom:
		sideStyle = bottomStyle;
		sideThick = bottomThick;
		break;
	case border_side::bs_left:
		sideStyle = leftStyle;
		sideThick = leftThick;
		break;
	case border_side::bs_right:
		sideStyle = rightStyle;
		sideThick = rightThick;
		break;
	case border_side::bs_top:
		sideStyle = topStyle;
		sideThick = leftThick;
	}

	bool mainSet = false, sideSet = false;
	for (UINT Rust = 0; Rust < WEB_BORDER_STYLE_COUNT; Rust++)
	{
		auto style = styleStrings[Rust].style;
		if (style == this->borderStyle)
			mainSet = true;
		if (style == sideStyle)
			sideSet = true;
	}
	if (mainSet || sideSet)
	{
		return this->thick > sideThick ? this->thick : sideThick;
	}
	return 0;
}

bool BorderData::GetBorderColor(TColor& color, border_side side)
{
	if (side == border_side::bs_all)
		return false;
	bool ret = false;
	switch (side)
	{
	case border_side::bs_bottom:
		ret = bottomColor.set;
		color = bottomColor.color;
		break;
	case border_side::bs_left:
		ret = leftColor.set;
		color = leftColor.color;
		break;
	case border_side::bs_right:
		ret = rightColor.set;
		color = rightColor.color;
		break;
	case border_side::bs_top:
		ret = topColor.set;
		color = topColor.color;
		break;
	}
	if (!ret)
	{
		ret = true;
		color = this->color;
	}
	return ret;
}

stroke_style BorderData::GetStrokeStyle(border_side side)
{
	switch (side)
	{
	case border_side::bs_bottom:
		if (bottomStyle == stroke_style::bs_not_set)
			return borderStyle;
		return bottomStyle;
	case border_side::bs_left:
		if (leftStyle == stroke_style::bs_not_set)
			return borderStyle;
		return leftStyle;
	case border_side::bs_right:
		if (rightStyle == stroke_style::bs_not_set)
			return borderStyle;
		return rightStyle;
	case border_side::bs_top:
		if (rightStyle == stroke_style::bs_not_set)
			return borderStyle;
		return rightStyle;
	}
	return borderStyle;
}

TColorSet::TColorSet()
{
	set = false;
}
TColorSet::TColorSet(const TColorSet& copy)
{
	set = copy.set;
	color = copy.color;
}
TColorSet TColorSet::operator=(const TColorSet& copy)
{
	set = copy.set;
	color = copy.color;
	return *this;
}
TColorSet TColorSet::operator=(const TColor& color)
{
	set = true;
	this->color = color;
	return *this;
}