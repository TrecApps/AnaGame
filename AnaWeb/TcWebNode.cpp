#include "TcWebNode.h"


#define WEB_EVENT_HANDLER_COUNT 71
#define WEB_BORDER_STYLE_COUNT 8
#define WEB_LIST_STYLE_COUNT 22
#define WEB_VOID_ELEMENT_COUNT 14\

static TString tcEventHandlers[WEB_EVENT_HANDLER_COUNT] = {
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

// Elements considered VOID (i.e. no closing tag expected)
static TString tcVoidElements[WEB_VOID_ELEMENT_COUNT] = {
    TString(L"area"),
    TString(L"base"),
    TString(L"br"),
    TString(L"col"),
    TString(L"embed"),
    TString(L"hr"),
    TString(L"img"),
    TString(L"input"),
    TString(L"link"),
    TString(L"meta"),
    TString(L"param"),
    TString(L"source"),
    TString(L"track"),
    TString(L"wbr")//,
    // TString(L"colgroup")
};

bool TcIsVoidElement(const TString& tag)
{
    for (UINT Rust = 0; Rust < WEB_VOID_ELEMENT_COUNT; Rust++)
    {
        if (!tag.CompareNoCase(tcVoidElements[Rust]))
            return true;
    }
    return false;
}





TcWebNode::TcTextNodeElement::TcTextNodeElement(bool isFirst)
{
    this->isFirst = isFirst;
}

TcWebNode::NodeContainerType TcWebNode::TcTextNodeElement::GetElementType()
{
    return NodeContainerType::nct_raw_text;
}

D2D1_RECT_F TcWebNode::TcTextNodeElement::GetLocation()
{
    return D2D1_RECT_F();
}

bool TcWebNode::TcTextNodeElement::IsFirst()
{
    return isFirst;
}

TcWebNode::TcWebNodeElement::TcWebNodeElement(TrecSubPointer<TPage, TcWebNode> node)
{
    nodeType = NodeContainerType::ntc_null; // don't know what it is yet
    assert(node.Get());
    this->node = node;
}

TrecSubPointer<TPage, TcWebNode> TcWebNode::TcWebNodeElement::GetWebNode()
{
    return node;
}

TcWebNode::NodeContainerType TcWebNode::TcWebNodeElement::GetElementType()
{
    if (nodeType == NodeContainerType::ntc_null)
    {
        nodeType = node->IsText() ? NodeContainerType::nct_tex_node : NodeContainerType::nct_reg_node;
    }
    return nodeType;
}

D2D1_RECT_F TcWebNode::TcWebNodeElement::GetLocation()
{
    return node->GetArea();
}

TcWebNode::TcWebNode(TrecPointer<DrawingBoard> board): TPage(board)
{

    outsideDisplay = TcWebNodeDisplayOutside::wndo_block;
    insideDisplay = TcWebNodeDisplayInside::wndi_flex;
    internalDisplay = TcWebNodeDisplayInternal::wndi_not_set;
    doDisplay = shouldDraw = true;
    isListItem = false;
}

bool TcWebNode::HandlesEvents()
{
    return false;
}

void TcWebNode::Draw(TrecPointer<TVariable> object)
{
}

ag_msg void TcWebNode::OnRButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&)
{

}

ag_msg void TcWebNode::OnRButtonDown(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&)
{

}

ag_msg void TcWebNode::OnLButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&)
{

}

ag_msg void TcWebNode::OnLButtonDown(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&)
{

}

ag_msg void TcWebNode::OnMouseMove(UINT nFlags, TPoint point, message_output& mOut, TDataArray<EventID_Cred>&)
{

}

ag_msg void TcWebNode::OnLButtonDblClk(UINT nFlags, TPoint point, message_output& mOut, TDataArray<EventID_Cred>& eventAr)
{

}

ag_msg void TcWebNode::OnResize(D2D1_RECT_F& newLoc, UINT nFlags, TDataArray<EventID_Cred>& eventAr)
{

}

ag_msg bool TcWebNode::OnDestroy()
{
    return true;
}

ag_msg bool TcWebNode::OnScroll(bool fromBars, const TPoint& point, const TPoint& direction, TDataArray<EventID_Cred>&)
{
    return false;
}

UINT TcWebNode::ProcessHtml(TStringSliceManager& html, UINT& start, HWND win, TrecPointer<TArray<styleTable>>& styles)
{
    // First Figure out the Tag Name
    while (start < html->GetSize() && (!tagName.GetSize() || !IsWhitespace(html->GetAt(start))))
    {
        if (!IsWhitespace(html->GetAt(start)))
            tagName.AppendChar(html->GetAt(start));
        bool doBreak = html->GetAt(start) == L'>';
        start++;
        if (doBreak)
            break;
    }

    // Assume we are collecting attributes until '>' is detected
    bool getAtts = true;

    // In case '<' got caught in our tag name
    if (tagName.StartsWith(L"<"))
        tagName.Set(tagName.SubString(1));

    // if '>' is detected, no more attributes
    if (tagName.EndsWith(L'>'))
    {
        getAtts = false;
        tagName.Set(tagName.SubString(0, tagName.Find(L'>')));
    }
    // Prepare to collect attributes
    TString attribute, value;

    // While still collecting attributes ...
    if (getAtts)
    {
        WCHAR quote = L'\0';

        bool attMode = true;

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
                    if (attribute.GetSize() && value.GetSize())
                        attributes.addEntry(attribute, value);
                    attribute.Empty();
                    value.Empty();
                }
                else
                    if (attMode)
                        attribute.AppendChar(ch);
                    else value.AppendChar(ch);
            }

            if (ch != L'\\') backslashCount = 0;
            start++;
        }

        if (start >= html->GetSize())
            return 1;

        if (html->GetAt(start) == L'>' && html->GetAt(start - 1) == L'/')
            return 0;
        start++;
    }
    if (attribute.GetSize() && value.GetSize())
        attributes.addEntry(attribute, value);

    attributes.retrieveEntry(L"class", this->nodeClass);
    attributes.retrieveEntry(L"id", this->id);
    if (TcIsVoidElement(tagName))
        return 0;
    return ProcessInnerHtml(html, start, win,styles);

    return 0;
}

void TcWebNode::CompileProperties(TrecPointer<TArray<styleTable>>& styles)
{
    TString empty;
    attributes.removeEntry(empty, empty);


    // This is done to ensure that when events are activated, the node has a smaller map to retireve
    // data from, thus boosting response times
    //for (UINT Rust = 0; Rust < WEB_EVENT_HANDLER_COUNT; Rust++)
    //{
    //    TString handlerValue;
    //    if (attributes.retrieveEntry(eventHandlers[Rust], handlerValue))
    //    {
    //        // Add the handler attribute to the handler map
    //        EventPropagater prop;
    //        prop.event = handlerValue;
    //        handlers.addEntry(eventHandlers[Rust], prop);

    //        // Remove the handler data from attributes to same some memory
    //        attributes.removeEntry(eventHandlers[Rust], handlerValue);
    //    }
    //}

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
        auto classes = nodeClass.split(L" \n\r\t");
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
    for (UINT Rust = 0; Rust < exAttributes.count(); Rust++)
    {
        TDataEntry<TString> entry;
        if (exAttributes.GetEntryAt(Rust, entry))
            atts.addEntry(entry.key, entry.object);
    }

    CompileProperties(atts);
    for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
    {
        auto ch = childNodes[Rust];

        if (ch->GetElementType() != NodeContainerType::nct_raw_text)
        {

            dynamic_cast<TcWebNodeElement*>(ch.Get())->GetWebNode()->CompileProperties(styles);
        }
    }
}

void TcWebNode::CompileProperties(TDataMap<TString>& atts)
{
    TString val;

    if (atts.retrieveEntry(L"display", val))
    {
        SetDisplay(val);
    }
}

void TcWebNode::SetDisplay(const TString& display)
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
        insideDisplay = TcWebNodeDisplayInside::wndi_flow_root;
        outsideDisplay = TcWebNodeDisplayOutside::wndo_inline;
    }

    // Outside Display
    if (display.CountFinds(L"inline"))
        outsideDisplay = TcWebNodeDisplayOutside::wndo_inline;
    else outsideDisplay = TcWebNodeDisplayOutside::wndo_block;

    if (display.CountFinds(L"table-row-group"))
        internalDisplay = TcWebNodeDisplayInternal::wndi_row_group;
    else if (display.CountFinds(L"table-header-group"))
        internalDisplay = TcWebNodeDisplayInternal::wndi_header_group;
    else if (display.CountFinds(L"table-footer-group"))
        internalDisplay = TcWebNodeDisplayInternal::wndi_footer_group;
    else if (display.CountFinds(L"table-row"))
        internalDisplay = TcWebNodeDisplayInternal::wndi_row;
    else if (display.CountFinds(L"table-cell"))
        internalDisplay = TcWebNodeDisplayInternal::wndi_cell;
    else if (display.CountFinds(L"table-column-group"))
    {
        doDisplay = false;
        internalDisplay = TcWebNodeDisplayInternal::wndi_column_group;
    }
    else if (display.CountFinds(L"table-column"))
    {
        doDisplay = false;
        internalDisplay = TcWebNodeDisplayInternal::wndi_column;
    }
    else if (display.CountFinds(L"table-caption"))
        internalDisplay = TcWebNodeDisplayInternal::wndi_caption;
    else // To Do: Add support for ruby values
        internalDisplay = TcWebNodeDisplayInternal::wndi_not_set;


    // Inside Display
    if (display.CountFinds(L"flow-root"))
        insideDisplay = TcWebNodeDisplayInside::wndi_flow_root;
    else if (display.CountFinds(L"flex"))
        insideDisplay = TcWebNodeDisplayInside::wndi_flex;
    else if ((internalDisplay == TcWebNodeDisplayInternal::wndi_not_set) ? (display.CountFinds(L"table") == 1) : (display.CountFinds(L"table") > 1))
        insideDisplay = TcWebNodeDisplayInside::wndi_table;
    else if (display.CountFinds(L"grid"))
        insideDisplay = TcWebNodeDisplayInside::wndi_grid;
    else if (display.CountFinds(L"ruby"))
        insideDisplay = TcWebNodeDisplayInside::wndi_ruby;
    else
        insideDisplay = TcWebNodeDisplayInside::wndi_flow;

    // Handle List item
    isListItem = display.CountFinds(L"list-item") > 0;

}

bool TcWebNode::IsText()
{
    shouldDraw = true;
    if (this->outsideDisplay != TcWebNodeDisplayOutside::wndo_inline)
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
        switch (childNode->GetElementType())
        {
        case NodeContainerType::nct_raw_text:
            if (!dynamic_cast<TcTextNodeElement*>(childNode.Get()))
                throw L"Unexpected Null TextField detected in IsText Method Call";
            continue;
        case NodeContainerType::nct_reg_node:
            return false;
        case NodeContainerType::ntc_null:
            childNodes.RemoveAt(Rust--);
            continue;
        case NodeContainerType::nct_tex_node:
            if (!dynamic_cast<TcWebNodeElement*>(childNode.Get()))
                throw L"Unexpected Null Web Node detected in IsText Method Call";
            if (!dynamic_cast<TcWebNodeElement*>(childNode.Get())->GetWebNode()->IsText())
                return false;
        }

    }
    shouldDraw = false;
    return true;
}

UINT TcWebNode::ProcessInnerHtml(TStringSliceManager& html, UINT& start, HWND win, TrecPointer<TArray<styleTable>>& styles)
{
    this->win = win;
    UINT currentStart = start;
    TString printableText;
    while (start < html->GetSize())
    {
        WCHAR ch = html->GetAt(start++);

        if (ch == L'<')
        {
            // If Text has been Collected
            if (printableText.GetTrim().GetSize())
            {
                TrecSubPointer<TcNodeElement, TcTextNodeElement> textElement;
                if (!childNodes.Size() || childNodes[childNodes.Size() - 1]->GetElementType() == NodeContainerType::nct_reg_node)
                    // This is a 'first' text node, meaning it should hold the TTextElement
                    textElement = TrecPointerKey::GetNewTrecSubPointer<TcNodeElement, TcTextNodeElement>();
                else textElement = TrecPointerKey::GetNewTrecSubPointer<TcNodeElement, TcTextNodeElement>(false);
            
                textElement->data.text.Set(printableText);
                childNodes.push_back(TrecSubToTrec(textElement));
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
            TrecSubPointer<TPage, TcWebNode> newNode = TrecPointerKey::GetNewSelfTrecSubPointer<TPage, TcWebNode>(this->drawingBoard);
            UINT result = newNode->ProcessHtml(html, start, win, styles);
            if (result)return result;

            newNode->CompileProperties(styles);
            childNodes.push_back(TrecPointerKey::GetNewTrecPointerAlt<TcNodeElement, TcWebNodeElement>(newNode));


            if (!start)break;
        }
    }
    return 0;
}

TcTextData::TcTextData()
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

TcTextData::TcTextData(const TcTextData& data)
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

void TcTextData::Update(const TcTextData& copy)
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
