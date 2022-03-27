#include "TcWebNode.h"
#include <TComplexTextElement.h>
#include <TStringVariable.h>


#define WEB_EVENT_HANDLER_COUNT 71
#define WEB_BORDER_STYLE_COUNT 8
#define WEB_LIST_STYLE_COUNT 22
#define WEB_VOID_ELEMENT_COUNT 14

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

typedef struct TcStyleString {
    TString str;
    stroke_style style;
}TcStyleString;

static TcStyleString tcStyleStrings[WEB_BORDER_STYLE_COUNT] = {
    {TString(L"dashed"), stroke_style::bs_dashed},
    {TString(L"dotted"), stroke_style::bs_dotted},
    {TString(L"double"), stroke_style::bs_double},
    {TString(L"groove"), stroke_style::bs_groove},
    {TString(L"inset"), stroke_style::bs_inset},
    {TString(L"outset"), stroke_style::bs_outset},
    {TString(L"ridge"), stroke_style::bs_ridge},
    {TString(L"solid"), stroke_style::bs_solid},
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

typedef struct TcListStyleString
{
    TString str;
    list_style style;
} TcListStyleString;

static TcListStyleString tcListStyleStrings[WEB_LIST_STYLE_COUNT] = {
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


void TcIncrementDecrementString(TString& str, WCHAR lower, WCHAR upper, UCHAR mode)
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
void TcGetListPrepend(TString& str, list_style style, signed char mode)
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
        str.Set(L' ');
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
            TcIncrementDecrementString(str, L'a', L'z', mode);
            str.AppendChar(L'.');
        }
        break;
    case list_style::ls_upper_alpha:
        if (!str.GetSize())
            str.Set(L"A.");
        else
        {
            str.Set(str.SubString(0, str.GetSize() - 1));
            TcIncrementDecrementString(str, L'A', L'Z', mode);
            str.AppendChar(L'.');
        }
        break;
    }
}

bool TcIsVoidElement(const TString& tag)
{
    for (UINT Rust = 0; Rust < WEB_VOID_ELEMENT_COUNT; Rust++)
    {
        if (!tag.CompareNoCase(tcVoidElements[Rust]))
            return true;
    }
    return false;
}

bool TcGetListStyle(const TString& styleStr, list_style& style)
{
    for (UINT Rust = 0; Rust < WEB_LIST_STYLE_COUNT; Rust++)
    {
        if (!styleStr.CompareNoCase(tcListStyleStrings[Rust].str))
        {
            style = tcListStyleStrings[Rust].style;
            return true;
        }
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

bool TcWebNode::TcTextNodeElement::GetVariable(const TString& name, TrecPointer<TVariable>& var)
{
    return false;
}

bool TcWebNode::TcTextNodeElement::SetVariable(const TString& name, TrecPointer<TVariable> var)
{
    return false;
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

bool TcWebNode::TcWebNodeElement::GetVariable(const TString& name, TrecPointer<TVariable>& var)
{
    return false;
}

bool TcWebNode::TcWebNodeElement::SetVariable(const TString& name, TrecPointer<TVariable> var)
{
    if (!name.Compare(L"NodeType"))
    {
        if (var.Get())
        {
            if (!var->GetString().Compare(L"Text"))
            {
                this->nodeType = NodeContainerType::nct_tex_node;
                return true;
            }
            if (!var->GetString().Compare(L"Web"))
            {
                this->nodeType = NodeContainerType::nct_reg_node;
                return true;
            }
        }
    }
    return false;
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
    win = board->GetWindowHandle();
    doDisplay = shouldDraw = true;
    isListItem = false;
    listForward = true;

    listStyle = list_style::ls_none;

    innerMargin = outerMargin = { 0,0,0,0 };
}

bool TcWebNode::HandlesEvents()
{
    return false;
}

void TcWebNode::Draw(TrecPointer<TVariable> object)
{
    if (!shouldDraw)
        return;

    DrawBorder();

    // Now draw the ChildNodes
    for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
    {
        if (childNodes[Rust]->GetElementType() != NodeContainerType::nct_raw_text)
            dynamic_cast<TcWebNodeElement*>(childNodes[Rust].Get())->GetWebNode()->Draw(object);
    }

    // Draw Text
    for (UINT Rust = 0; Rust < textElements.Size(); Rust++)
    {
        textElements[Rust]->text->OnDraw(object);
    }
}

void TcWebNode::DrawBorder()
{
    TColor color;

    // Start with the top border
    float curThick = static_cast<float>(borderData.GetBorderThickness(tc_border_side::bs_top));
    stroke_style curStyle = borderData.GetStrokeStyle(tc_border_side::bs_top);

    // These values will be used when preparing a rounded corner
    bool doTop = false, doLeft = false, doBottom = false, doRight = false;

    if (curThick && hasStyle(curStyle))
    {
        doTop = true;
        borderData.GetBorderColor(color, tc_border_side::bs_top);
        if (!borderData.brush.Get())
            borderData.brush = drawingBoard->GetBrush(color);
        else borderData.brush->SetColor(color);
        if (borderData.brush.Get())
        {
            float top = area.top + (static_cast<float>(curThick) / 2.0f);
            borderData.brush->DrawLine(D2D1::Point2F(borderData.topLeft ? area.left + borderData.topLeft->h : area.left, top),
                D2D1::Point2F(borderData.topRight ? area.right - borderData.topRight->h : area.right, top), drawingBoard->GetStrokeStyle(curStyle), curThick);
        }
    }

    // go for the right border
    curThick = static_cast<float>(borderData.GetBorderThickness(tc_border_side::bs_right));
    curStyle = borderData.GetStrokeStyle(tc_border_side::bs_right);
    if (curThick && hasStyle(curStyle))
    {
        doRight = true;
        borderData.GetBorderColor(color, tc_border_side::bs_right);
        if (!borderData.brush.Get())
            borderData.brush = drawingBoard->GetBrush(color);
        else borderData.brush->SetColor(color);
        if (borderData.brush.Get())
        {
            float right = area.right - (static_cast<float>(curThick) / 2.0f);
            borderData.brush->DrawLine(D2D1::Point2F(right, borderData.topRight ? area.top + borderData.topRight->v : area.top),
                D2D1::Point2F(right, borderData.bottomRight ? area.bottom - borderData.bottomRight->v : area.bottom), drawingBoard->GetStrokeStyle(curStyle), curThick);
        }
    }

    // now the left border
    curThick = static_cast<float>(borderData.GetBorderThickness(tc_border_side::bs_left));
    curStyle = borderData.GetStrokeStyle(tc_border_side::bs_left);
    if (curThick && hasStyle(curStyle))
    {
        doLeft = true;
        borderData.GetBorderColor(color, tc_border_side::bs_left);
        if (!borderData.brush.Get())
            borderData.brush = drawingBoard->GetBrush(color);
        else borderData.brush->SetColor(color);
        if (borderData.brush.Get())
        {
            float left = area.left + (static_cast<float>(curThick) / 2.0f);
            borderData.brush->DrawLine(D2D1::Point2F(left, borderData.topLeft ? area.top + borderData.topLeft->v : area.top),
                D2D1::Point2F(left, borderData.bottomLeft ? area.bottom - borderData.bottomLeft->v : area.bottom), drawingBoard->GetStrokeStyle(curStyle), curThick);
        }
    }

    // finally, the bottom border
    curThick = static_cast<float>(borderData.GetBorderThickness(tc_border_side::bs_bottom));
    curStyle = borderData.GetStrokeStyle(tc_border_side::bs_bottom);
    if (curThick && hasStyle(curStyle))
    {
        doBottom = true;
        borderData.GetBorderColor(color, tc_border_side::bs_bottom);
        if (!borderData.brush.Get())
            borderData.brush = drawingBoard->GetBrush(color);
        else borderData.brush->SetColor(color);
        if (borderData.brush.Get())
        {
            float bottom = area.bottom + (static_cast<float>(curThick) / 2.0f);
            borderData.brush->DrawLine(D2D1::Point2F(borderData.bottomLeft ? borderData.bottomLeft->h + area.left : area.left, bottom),
                D2D1::Point2F(borderData.bottomRight ? area.right - borderData.bottomRight->h : area.right, bottom), drawingBoard->GetStrokeStyle(curStyle), curThick);
        }
    }

    D2D1_ROUNDED_RECT rRect;
    rRect.rect = area;

    if (borderData.topLeft && doTop && doLeft)
    {
        rRect.radiusX = borderData.topLeft->h;
        rRect.radiusY = borderData.topLeft->v;

        borderData.GetBorderColor(color, tc_border_side::bs_left);
        if (!borderData.brush.Get())
            borderData.brush = drawingBoard->GetBrush(color);
        else borderData.brush->SetColor(color);

        drawingBoard->AddLayer(D2D1::RectF(area.left, area.top, area.left + borderData.topLeft->h, area.top + borderData.topLeft->v));
        borderData.brush->DrawRoundedRectangle(rRect, borderData.GetBorderThickness(tc_border_side::bs_left));
        drawingBoard->PopLayer();
    }

    if (borderData.topRight && doTop && doRight)
    {
        rRect.radiusX = borderData.topRight->h;
        rRect.radiusY = borderData.topRight->v;

        borderData.GetBorderColor(color, tc_border_side::bs_right);
        if (!borderData.brush.Get())
            borderData.brush = drawingBoard->GetBrush(color);
        else borderData.brush->SetColor(color);

        drawingBoard->AddLayer(D2D1::RectF(area.right - borderData.topRight->h, area.top, area.right, area.top + borderData.topRight->v));
        borderData.brush->DrawRoundedRectangle(rRect, borderData.GetBorderThickness(tc_border_side::bs_right));
        drawingBoard->PopLayer();
    }

    if (borderData.bottomLeft && doTop && doLeft)
    {
        rRect.radiusX = borderData.bottomLeft->h;
        rRect.radiusY = borderData.bottomLeft->v;

        borderData.GetBorderColor(color, tc_border_side::bs_left);
        if (!borderData.brush.Get())
            borderData.brush = drawingBoard->GetBrush(color);
        else borderData.brush->SetColor(color);

        drawingBoard->AddLayer(D2D1::RectF(area.left, area.bottom - borderData.bottomLeft->v, area.left + borderData.bottomLeft->h, area.bottom));
        borderData.brush->DrawRoundedRectangle(rRect, borderData.GetBorderThickness(tc_border_side::bs_left));
        drawingBoard->PopLayer();
    }

    if (borderData.bottomRight && doTop && doLeft)
    {
        rRect.radiusX = borderData.bottomRight->h;
        rRect.radiusY = borderData.bottomRight->v;

        borderData.GetBorderColor(color, tc_border_side::bs_right);
        if (!borderData.brush.Get())
            borderData.brush = drawingBoard->GetBrush(color);
        else borderData.brush->SetColor(color);

        drawingBoard->AddLayer(D2D1::RectF(area.right - borderData.bottomRight->h, area.bottom - borderData.bottomRight->v, area.right, area.bottom));
        borderData.brush->DrawRoundedRectangle(rRect, borderData.GetBorderThickness(tc_border_side::bs_right));
        drawingBoard->PopLayer();
    }
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

void TcWebNode::PreCreate(TrecSubPointerSoft<TPage, TcWebNode> parent, TrecPointer<TArray<styleTable>>& styles)
{
    this->parent = TrecPointerKey::GetSoftPointerFromSubSoft<>( parent);





    for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
    {
        TrecPointer<TcWebNode::TcNodeElement> ch = childNodes[Rust];


        if (ch->GetElementType() != TcWebNode::NodeContainerType::nct_raw_text && dynamic_cast<TcWebNodeElement*>(ch.Get())->GetWebNode().Get())
        {
            dynamic_cast<TcWebNodeElement*>(ch.Get())->GetWebNode()->PreCreate(TrecPointerKey::GetSoftSubPointerFromSoft<TPage, TcWebNode>( self ), styles);
        }
    }

    CompileProperties(styles);

    for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
    {
        TrecPointer<TcWebNode::TcNodeElement> ch = childNodes[Rust];

        if (ch->GetElementType() != NodeContainerType::nct_raw_text)
        {
            TrecSubPointer<TcNodeElement, TcWebNodeElement> webCh = TrecPointerKey::GetTrecSubPointerFromTrec<TcNodeElement, TcWebNodeElement>(ch);
            assert(webCh.Get());

            webCh->SetVariable(L"NodeType", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(
                webCh->GetWebNode()->IsText() ? L"Text" : L"Web"
                ));
        }
    }
}

UINT TcWebNode::CreateWebNode(D2D1_RECT_F location, TrecPointer<TWindowEngine> d3dEngine, HWND window)
{
    // Go through each bit of the child elements, determining where they lay
    if (this->doDisplay && !TcIsVoidElement(tagName))
    {
        location.top += outerMargin.top;
        location.left += outerMargin.left;
        location.right -= outerMargin.right;

        this->area = location;
        location.top += innerMargin.top + static_cast<float>(borderData.GetBorderThickness(tc_border_side::bs_top));
        location.left += innerMargin.left + static_cast<float>(borderData.GetBorderThickness(tc_border_side::bs_left));
        location.right -= innerMargin.right - static_cast<float>(borderData.GetBorderThickness(tc_border_side::bs_bottom));
    }
    else
        location = { 0.0f,0.0f,0.0f,0.0f };
    if (window)
        this->win = window;
    this->d3dEngine = d3dEngine;

    // Handle Scenario where this Element is a table
    if (insideDisplay == TcWebNodeDisplayInside::wndi_table)
    {
        // To-Do: add Table Code
    }

    // Handle case where this Node is a list item
    if (isListItem && parent.Get())
    {
        listInfo.Set(dynamic_cast<TcWebNode*>(parent.Get())->GetListPrepend());
    }

    UINT firstText = 0xf0000000;

    TDataArray<TrecPointer<TcNodeElement>> textNodes;
    TDataArray<TcTextData> textDataList;
    textElements.RemoveAll();

    for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
    {
        TrecPointer<TcWebNode::TcNodeElement> element = childNodes[Rust];

        if (element->GetElementType() == NodeContainerType::nct_reg_node)
        {
            if (textNodes.Size())
            {
                ProcessTextNodes(textNodes, textDataList, location);
                textNodes.RemoveAll();
                textDataList.RemoveAll();
            }
            TrecSubPointer<TPage, TcWebNode> childNode = dynamic_cast<TcWebNodeElement*>(element.Get())->GetWebNode();
            childNode->CreateWebNode(location, d3dEngine, window);
            childNode->ShrinkHeight();
            location.top = childNode->GetArea().top;

        }
        else
        {
            if (element->GetElementType() == NodeContainerType::nct_raw_text)
            {
                textNodes.push_back(element);
                textDataList.push_back(textData);
            }
            else dynamic_cast<TcWebNodeElement*>(element.Get())->GetWebNode()->RetrieveTextNodes(textNodes, textDataList);
        }
    }

    if (textNodes.Size())
    {
        ProcessTextNodes(textNodes, textDataList, location);
        textNodes.RemoveAll();
    }

    return 0;
}

void TcWebNode::CompileProperties(TrecPointer<TArray<styleTable>>& styles)
{
    TString empty;
    attributes.removeEntry(empty, empty);


    // This is done to ensure that when events are activated, the node has a smaller map to retireve
    // data from, thus boosting response times
    for (UINT Rust = 0; Rust < WEB_EVENT_HANDLER_COUNT; Rust++)
    {
        TString handlerValue;
        if (attributes.retrieveEntry(tcEventHandlers[Rust], handlerValue))
        {
            // Add the handler attribute to the handler map
            EventPropagater prop;
            prop.event = handlerValue;
            handlers.addEntry(tcEventHandlers[Rust], prop);

            // Remove the handler data from attributes to same some memory
            attributes.removeEntry(tcEventHandlers[Rust], handlerValue);
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
            auto webNode = dynamic_cast<TcWebNodeElement*>(ch.Get())->GetWebNode();
            webNode->textData.Update(textData);
            webNode->CompileProperties(styles);
            
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

    // List Attributes
    if (atts.retrieveEntry(L"list-style-type", val))
    {
        TcGetListStyle(val, listStyle);
    }

    TcGetListPrepend(this->listInfo, this->listStyle, 0);


    // take care of text attributes
    if (atts.retrieveEntry(L"color", val))
    {
        textData.textColor.SetColor(val);
        textData.textColorUpdated = true;
    }

    // Handle background color
    if (atts.retrieveEntry(L"", val))
    {
        textData.backgroundColor.SetColor(val);
        textData.hasBackgroundColor = true;
    }

    if (atts.retrieveEntry(L"font-style", val))
    {
        if (!val.Compare(L"italic"))
            textData.fontStyle = DWRITE_FONT_STYLE_ITALIC;
        else if (!val.Compare(L"oblique"))
            textData.fontStyle = DWRITE_FONT_STYLE_OBLIQUE;
        // Default is normal
        textData.fontStyleUpdated = true;
    }


    if (atts.retrieveEntry(L"font-weight", val))
    {
        if (!val.Compare(L"100"))
        {
            textData.fontWeight = DWRITE_FONT_WEIGHT_THIN;
        }
        else if (!val.Compare(L"200"))
        {
            textData.fontWeight = DWRITE_FONT_WEIGHT_EXTRA_LIGHT;
        }
        else if (!val.Compare(L"300") || !val.Compare(L"lighter"))
        {
            textData.fontWeight = DWRITE_FONT_WEIGHT_LIGHT;
        }
        else if (!val.Compare(L"400"))
        {
            textData.fontWeight = DWRITE_FONT_WEIGHT_NORMAL;
        }
        else if (!val.Compare(L"500"))
        {
            textData.fontWeight = DWRITE_FONT_WEIGHT_MEDIUM;
        }
        else if (!val.Compare(L"600"))
        {
            textData.fontWeight = DWRITE_FONT_WEIGHT_DEMI_BOLD;
        }
        else if (!val.Compare(L"700") || !val.Compare(L"bold"))
        {
            textData.fontWeight = DWRITE_FONT_WEIGHT_BOLD;
        }
        else if (!val.Compare(L"800"))
        {
            textData.fontWeight = DWRITE_FONT_WEIGHT_ULTRA_BOLD;
        }
        else if (!val.Compare(L"900"))
        {
            textData.fontWeight = DWRITE_FONT_WEIGHT_BLACK;
        }
        else if (!val.Compare(L"950"))
        {
            textData.fontWeight = DWRITE_FONT_WEIGHT_EXTRA_BLACK;
        }
        else if (!val.Compare(L"350"))
        {
            textData.fontWeight = DWRITE_FONT_WEIGHT_SEMI_LIGHT;
        }
        else if (!val.Compare(L"inherit"))
        {
            // To-Do:
        }

        textData.fontWeightUpdated = true;
    }


    if (atts.retrieveEntry(L"border", val))
    {
        borderData.CompileAttributes(val, tc_border_side::bs_all);
    }

    if (atts.retrieveEntry(L"border-bottom", val))
    {
        borderData.CompileAttributes(val, tc_border_side::bs_bottom);
    }

    if (atts.retrieveEntry(L"border-left", val))
    {
        borderData.CompileAttributes(val, tc_border_side::bs_left);
    }

    if (atts.retrieveEntry(L"border-right", val))
    {
        borderData.CompileAttributes(val, tc_border_side::bs_right);
    }

    if (atts.retrieveEntry(L"border-top", val))
    {
        borderData.CompileAttributes(val, tc_border_side::bs_top);
    }

    if (atts.retrieveEntry(L"border-color", val))
        borderData.CompileColor(val, tc_border_side::bs_all);
    if (atts.retrieveEntry(L"border-left-color", val))
        borderData.CompileColor(val, tc_border_side::bs_left);
    if (atts.retrieveEntry(L"border-bottom-color", val))
        borderData.CompileColor(val, tc_border_side::bs_bottom);
    if (atts.retrieveEntry(L"border-top-color", val))
        borderData.CompileColor(val, tc_border_side::bs_top);
    if (atts.retrieveEntry(L"border-right-color", val))
        borderData.CompileColor(val, tc_border_side::bs_right);

    if (atts.retrieveEntry(L"border-width", val))
        borderData.CompileBorder(val, tc_border_side::bs_all);
    if (atts.retrieveEntry(L"border-left-width", val))
        borderData.CompileBorder(val, tc_border_side::bs_left);
    if (atts.retrieveEntry(L"border-bottom-width", val))
        borderData.CompileBorder(val, tc_border_side::bs_bottom);
    if (atts.retrieveEntry(L"border-top-width", val))
        borderData.CompileBorder(val, tc_border_side::bs_top);
    if (atts.retrieveEntry(L"border-right-width", val))
        borderData.CompileBorder(val, tc_border_side::bs_right);

    if (atts.retrieveEntry(L"border-style", val))
        borderData.CompileStyle(val, tc_border_side::bs_all);
    if (atts.retrieveEntry(L"border-left-style", val))
        borderData.CompileStyle(val, tc_border_side::bs_left);
    if (atts.retrieveEntry(L"border-bottom-style", val))
        borderData.CompileStyle(val, tc_border_side::bs_bottom);
    if (atts.retrieveEntry(L"border-top-style", val))
        borderData.CompileStyle(val, tc_border_side::bs_top);
    if (atts.retrieveEntry(L"border-right-style", val))
        borderData.CompileStyle(val, tc_border_side::bs_right);

    if (atts.retrieveEntry(L"margin", val))
        CompileMargin(val, tc_border_side::bs_all, false);
    if (atts.retrieveEntry(L"margin-left", val))
        CompileMargin(val, tc_border_side::bs_left, false);
    if (atts.retrieveEntry(L"margin-right", val))
        CompileMargin(val, tc_border_side::bs_right, false);
    if (atts.retrieveEntry(L"margin-top", val))
        CompileMargin(val, tc_border_side::bs_top, false);
    if (atts.retrieveEntry(L"margin-bottom", val))
        CompileMargin(val, tc_border_side::bs_bottom, false);

    if (atts.retrieveEntry(L"padding", val))
        CompileMargin(val, tc_border_side::bs_all, true);
    if (atts.retrieveEntry(L"padding-left", val))
        CompileMargin(val, tc_border_side::bs_left, true);
    if (atts.retrieveEntry(L"padding-right", val))
        CompileMargin(val, tc_border_side::bs_right, true);
    if (atts.retrieveEntry(L"padding-top", val))
        CompileMargin(val, tc_border_side::bs_top, true);
    if (atts.retrieveEntry(L"padding-bottom", val))
        CompileMargin(val, tc_border_side::bs_bottom, true);

    if (atts.retrieveEntry(L"border-radius", val))
        borderData.CompileCorner(val, tc_border_side::bs_all);
    if (atts.retrieveEntry(L"border-bottom-right-radius", val))
        borderData.CompileCorner(val, tc_border_side::bs_bottom);	// Bottom-Right
    if (atts.retrieveEntry(L"border-bottom-left-radius", val))
        borderData.CompileCorner(val, tc_border_side::bs_left);	// Bottom_Left
    if (atts.retrieveEntry(L"border-top-right-radius", val))
        borderData.CompileCorner(val, tc_border_side::bs_right);	// Top-Right
    if (atts.retrieveEntry(L"border-top-left-radius", val))
        borderData.CompileCorner(val, tc_border_side::bs_top);		// Top-Left




    if (atts.retrieveEntry(L"rowspan", val) && internalDisplay != TcWebNodeDisplayInternal::wndi_row)
    {
        UINT rSpan = 0;
        if (TString::ConvertStringToUint(val, rSpan))
            rowSpan = rSpan;
    }
    if (atts.retrieveEntry(L"colspan", val))
    {
        UINT cSpan = 0;
        if (TString::ConvertStringToUint(val, cSpan))
            columnSpan = cSpan;
    }


    if (atts.retrieveEntry(L"name", val))
        name.Set(val);
    if (atts.retrieveEntry(L"value", val))
        value.Set(val);
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

void ProcessHtmlText(TString& htmlContent)
{
    while (htmlContent.Replace(L"  ", L" "));
}

TextFormattingDetails getTextFormattingDetails(TcTextData& textData, TrecPointer<DrawingBoard> board)
{
    TextFormattingDetails ret;

    if (textData.fontSizeUpdated)
        ret.fontSize = textData.fontSize;
    if (textData.fontStretchUpdated)
        ret.stretch = textData.fontStretch;
    if (textData.fontStyleUpdated)
        ret.style = textData.fontStyle;
    if (textData.fontWeightUpdated)
        ret.weight = textData.fontWeight;
    if (textData.textColorUpdated)
        ret.color = board->GetBrush(textData.textColor);
    if (textData.hasBackgroundColor)
        ret.bColor = board->GetBrush(textData.backgroundColor);

    return ret;
}

void TcWebNode::ProcessTextNodes(TDataArray<TrecPointer<TcNodeElement>>& textNodes, TDataArray<TcTextData>& textDataList, D2D1_RECT_F& location)
{
    TrecSubPointer<TTextElement, TComplexTextElement> element = TrecPointerKey::GetNewSelfTrecSubPointer<TTextElement, TComplexTextElement>(drawingBoard, win, false);
    TrecPointer<TcTextStructure> elementStructure = TrecPointerKey::GetNewTrecPointer<TcTextStructure>();
    elementStructure->text = element;
    TString theString;


    for (UINT Rust = 0; Rust < textNodes.Size(); Rust++)
    {
        TrecSubPointer<TcNodeElement, TcTextNodeElement> textElement = TrecPointerKey::GetTrecSubPointerFromTrec<TcNodeElement, TcTextNodeElement>(textNodes[Rust]);
        
        assert(textElement.Get());
        TString curText(textElement->data.text);
        ProcessHtmlText(curText);

        elementStructure->ranges.push_back({ theString.GetSize(), curText.GetSize() });
        theString.Append(curText);
    }

    element->SetText(theString);
    element->SetHorizontallignment(DWRITE_TEXT_ALIGNMENT_LEADING);
    element->SetVerticalAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
    element->SetLocation(location);
    if (textData.textColorUpdated)
        element->SetColor(textData.textColor);
    element->SetBasicFormatting(getTextFormattingDetails(textData, drawingBoard));

    for (UINT Rust = 0; Rust < textNodes.Size(); Rust++)
    {
        TrecSubPointer<TcNodeElement, TcTextNodeElement> textElement = TrecPointerKey::GetTrecSubPointerFromTrec<TcNodeElement, TcTextNodeElement>(textNodes[Rust]);

        TextFormattingDetails details = getTextFormattingDetails(textDataList[Rust], drawingBoard);
        details.range = elementStructure->ranges[Rust];
        element->SetFormatting(details, details.range.startPosition, details.range.startPosition + details.range.length, false);
    }

    D2D1_RECT_F loc = element->GetLocation();
    bool w;
    loc.bottom = loc.top + element->GetMinHeight(w);
    element->SetLocation(loc);
    location.top = loc.bottom;

    textElements.push_back(elementStructure);
}

TString TcWebNode::GetListPrepend()
{
    TString ret(listInfo);

    if (this->listForward)
        TcGetListPrepend(listInfo, listStyle, 1);
    else
        TcGetListPrepend(listInfo, listStyle, -1);
    return ret;
}

void TcWebNode::RetrieveTextNodes(TDataArray<TrecPointer<TcNodeElement>>& textNodes, TDataArray<TcTextData>& textDataList)
{
    for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
    {
        switch (childNodes[Rust]->GetElementType())
        {
        case NodeContainerType::nct_tex_node:
            dynamic_cast<TcWebNodeElement*>(childNodes[Rust].Get())->GetWebNode()->RetrieveTextNodes(textNodes, textDataList);
            break;
        case NodeContainerType::nct_raw_text:
            textNodes.push_back(childNodes[Rust]);
            textDataList.push_back(textData);
            break;
        case NodeContainerType::nct_reg_node:
            return;
        }
    }
}



void TcWebNode::ShrinkHeight()
{
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

            // newNode->CompileProperties(styles);
            childNodes.push_back(TrecPointerKey::GetNewTrecPointerAlt<TcNodeElement, TcWebNodeElement>(newNode));


            if (!start)break;
        }
        else
        {
            printableText.AppendChar(ch);
        }
    }
    return 0;
}

void TcWebNode::CompileMargin(const TString& marginString, tc_border_side side, bool inner)
{
    TString atts(marginString);
    TrecPointer<TDataArray<TString>> pieces = atts.split(L' ');

    if (!pieces->Size())
        return;
    D2D1_RECT_F* rect = inner ? &innerMargin : &outerMargin;

    switch (side)
    {
    case tc_border_side::bs_bottom:
        rect->bottom = ConvertMeasurement(pieces->at(0));
        break;
    case tc_border_side::bs_left:
        rect->left = ConvertMeasurement(pieces->at(0));
        break;
    case tc_border_side::bs_right:
        rect->right = ConvertMeasurement(pieces->at(0));
        break;
    case tc_border_side::bs_top:
        rect->top = ConvertMeasurement(pieces->at(0));
        break;
    default:
        switch (pieces->Size())
        {
        case 1:
            rect->bottom = rect->left = rect->right = rect->top = ConvertMeasurement(pieces->at(0));
            break;
        case 2:
            rect->top = rect->bottom = ConvertMeasurement(pieces->at(0));
            rect->left = rect->right = ConvertMeasurement(pieces->at(1));
            break;
        case 3:
            rect->top = ConvertMeasurement(pieces->at(0));
            rect->left = rect->right = ConvertMeasurement(pieces->at(1));
            rect->bottom = ConvertMeasurement(pieces->at(2));
            break;
        default:
            rect->top = ConvertMeasurement(pieces->at(0));
            rect->right = ConvertMeasurement(pieces->at(1));
            rect->bottom = ConvertMeasurement(pieces->at(2));
            rect->left = ConvertMeasurement(pieces->at(3));
        }
    }
}

float TcWebNode::ConvertMeasurement(TString& atts)
{
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
            sizeVal = sizeVal * textData.fontSize;
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
    }
    return sizeVal;
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

TcBorderData::TcBorderData()
{
    borderStyle = topStyle = bottomStyle = rightStyle = leftStyle = stroke_style::bs_not_set;
    thick = topThick = bottomThick = rightThick = leftThick = 0;

    topLeft = topRight = bottomLeft = bottomRight = nullptr;
}

TcBorderData::TcBorderData(const TcBorderData& copy)
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

    // First set all corner data to null. If the copy has corner data, then set each corner
    topLeft = topRight = bottomLeft = bottomRight = nullptr;

    if (copy.topLeft)
    {
        topLeft = new BorderCorners();
        topLeft->h = copy.topLeft->h;
        topLeft->v = copy.topLeft->v;
    }
    if (copy.topRight)
    {
        topRight = new BorderCorners();
        topRight->h = copy.topRight->h;
        topRight->v = copy.topRight->v;
    }
    if (copy.bottomLeft)
    {
        bottomLeft = new BorderCorners();
        bottomLeft->h = copy.bottomLeft->h;
        bottomLeft->v = copy.bottomLeft->v;
    }
    if (copy.bottomRight)
    {
        bottomRight = new BorderCorners();
        bottomRight->h = copy.bottomRight->h;
        bottomRight->v = copy.bottomRight->v;
    }
}

TcBorderData::~TcBorderData()
{
    ClearCorner(tc_border_side::bs_all);
}

void TcBorderData::CompileAttributes(TString& atts, tc_border_side side)
{
}

void TcBorderData::CompileBorder(TString& atts, tc_border_side size)
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
        case tc_border_side::bs_bottom:
            this->bottomThick = sizeVal;
            break;
        case tc_border_side::bs_left:
            this->leftThick = sizeVal;
            break;
        case tc_border_side::bs_right:
            this->rightThick = sizeVal;
            break;
        case tc_border_side::bs_top:
            this->topThick = sizeVal;
            break;
        default:
            this->thick = sizeVal;
        }


    }

}

void TcBorderData::CompileColor(TString& atts, tc_border_side side)
{
    TColor color;
    bool worked = false;

    if (side == tc_border_side::bs_all)
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
        case tc_border_side::bs_bottom:
            color = TColor::GetColorFromString(atts, worked);
            if (worked)
                this->bottomColor = color;
            break;
        case tc_border_side::bs_left:
            color = TColor::GetColorFromString(atts, worked);
            if (worked)
                this->leftColor = color;
            break;
        case tc_border_side::bs_right:
            color = TColor::GetColorFromString(atts, worked);
            if (worked)
                this->rightColor = color;
            break;
        case tc_border_side::bs_top:
            color = TColor::GetColorFromString(atts, worked);
            if (worked)
                this->topColor = color;
            break;
        }
    }
}

bool TcBorderData::CompileStyle(TString& atts, tc_border_side side)
{
    for (UINT Rust = 0; Rust < WEB_BORDER_STYLE_COUNT; Rust++)
    {
        if (!tcStyleStrings[Rust].str.Compare(atts))
        {
            switch (side)
            {
            case tc_border_side::bs_bottom:
                this->bottomStyle = tcStyleStrings[Rust].style;
                break;
            case tc_border_side::bs_left:
                this->leftStyle = tcStyleStrings[Rust].style;
                break;
            case tc_border_side::bs_right:
                this->rightStyle = tcStyleStrings[Rust].style;
                break;
            case tc_border_side::bs_top:
                this->topStyle = tcStyleStrings[Rust].style;
                break;
            default:
                this->borderStyle = tcStyleStrings[Rust].style;
            }
            return true;
        }
    }

    return false;
}

void TcBorderData::CompileCorner(TString& atts, tc_border_side side)
{
    auto d_atts = atts.splitn(L" ", 4);
    UINT dpi = GetDpiForSystem();
    float dpif = static_cast<float>(dpi);
    assert(dpi && dpif);
    WebSizeUnit unit = WebSizeUnit::wsu_bl;

    float vAtts[] = { 0.0f,0.0f,0.0f,0.0f };

    for (UINT Rust = 0; Rust < d_atts->Size(); Rust++)
    {
        if (d_atts->at(Rust).EndsWith(L"px"))
        {
            unit = WebSizeUnit::wsu_px;
        }
        if (d_atts->at(Rust).EndsWith(L"cm"))
        {
            unit = WebSizeUnit::wsu_cm;
        }
        if (d_atts->at(Rust).EndsWith(L"mm"))
        {
            unit = WebSizeUnit::wsu_mm;
        }
        if (d_atts->at(Rust).EndsWith(L"in"))
        {
            unit = WebSizeUnit::wsu_in;
        }
        if (d_atts->at(Rust).EndsWith(L"em"))
        {
            unit = WebSizeUnit::wsu_em;
        }
        if (d_atts->at(Rust).EndsWith(L"pt"))
        {
            unit = WebSizeUnit::wsu_pt;
        }

        if (unit != WebSizeUnit::wsu_bl)
            d_atts->at(Rust).Set(d_atts->at(Rust).SubString(0, d_atts->at(Rust).GetSize() - 2));

        d_atts->at(Rust).Trim();

        if (!atts.ConvertToFloat(vAtts[Rust])) {
            // We were able to get a value out of what was provided
            switch (unit)
            {
            case WebSizeUnit::wsu_cm:
                vAtts[Rust] = vAtts[Rust] * 2.54 / dpif;
                break;
            case WebSizeUnit::wsu_em:

                break;
            case WebSizeUnit::wsu_in:
                vAtts[Rust] = vAtts[Rust] / dpif;
                break;
            case WebSizeUnit::wsu_mm:
                vAtts[Rust] = vAtts[Rust] * 0.254 / dpif;
                break;
            case WebSizeUnit::wsu_pt:
                vAtts[Rust] = vAtts[Rust] * 72.0f / dpif;
                break;
            case WebSizeUnit::wsu_bl:
            case WebSizeUnit::wsu_px:
                vAtts[Rust] = vAtts[Rust] * (dpif / 96.0f);
            }
        }
    }

    if (!vAtts[1] && side != tc_border_side::bs_all)
        vAtts[1] = vAtts[0];

    if (vAtts[0])
    {
        ClearCorner(side);
        switch (side)
        {
        case tc_border_side::bs_top: // Top-left
            this->topLeft = new BorderCorners();
            topLeft->h = vAtts[0];
            topLeft->v = vAtts[1];
            break;
        case tc_border_side::bs_right: // Top-right
            this->topRight = new BorderCorners();
            topRight->h = vAtts[0];
            topRight->v = vAtts[1];
            break;
        case tc_border_side::bs_left: // Top-left
            this->bottomLeft = new BorderCorners();
            bottomLeft->h = vAtts[0];
            bottomLeft->v = vAtts[1];
            break;
        case tc_border_side::bs_bottom: // Top-left
            this->bottomRight = new BorderCorners();
            bottomRight->h = vAtts[0];
            bottomRight->v = vAtts[1];
            break;
        default: // All
            this->topLeft = new BorderCorners();
            this->topRight = new BorderCorners();
            this->bottomLeft = new BorderCorners();
            this->bottomRight = new BorderCorners();
            switch (d_atts->Size())
            {
            case 1:
                topLeft->v = topLeft->h = vAtts[0];
                topRight->v = topRight->h = vAtts[0];
                bottomLeft->v = bottomLeft->h = vAtts[0];
                bottomRight->v = bottomRight->h = vAtts[0];
                break;
            case 2:
                topLeft->v = topLeft->h = vAtts[0];
                topRight->v = topRight->h = vAtts[1];
                bottomLeft->v = bottomLeft->h = vAtts[1];
                bottomRight->v = bottomRight->h = vAtts[0];
                break;
            case 3:
                topLeft->v = topLeft->h = vAtts[0];
                topRight->v = topRight->h = vAtts[1];
                bottomLeft->v = bottomLeft->h = vAtts[1];
                bottomRight->v = bottomRight->h = vAtts[2];
                break;
            case 4:
                topLeft->v = topLeft->h = vAtts[0];
                topRight->v = topRight->h = vAtts[1];
                bottomLeft->v = bottomLeft->h = vAtts[3];
                bottomRight->v = bottomRight->h = vAtts[2];
            }
        }
    }
}

USHORT TcBorderData::GetBorderThickness(tc_border_side side)
{
    if (side == tc_border_side::bs_all)
        return 0; // We need a specific side
    stroke_style sideStyle = stroke_style::bs_not_set;
    USHORT sideThick = 0;

    switch (side)
    {
    case tc_border_side::bs_bottom:
        sideStyle = bottomStyle;
        sideThick = bottomThick;
        break;
    case tc_border_side::bs_left:
        sideStyle = leftStyle;
        sideThick = leftThick;
        break;
    case tc_border_side::bs_right:
        sideStyle = rightStyle;
        sideThick = rightThick;
        break;
    case tc_border_side::bs_top:
        sideStyle = topStyle;
        sideThick = leftThick;
    }

    bool mainSet = false, sideSet = false;
    for (UINT Rust = 0; Rust < WEB_BORDER_STYLE_COUNT; Rust++)
    {
        auto style = tcStyleStrings[Rust].style;
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

bool TcBorderData::GetBorderColor(TColor& color, tc_border_side side)
{
    if (side == tc_border_side::bs_all)
        return false;
    bool ret = false;
    switch (side)
    {
    case tc_border_side::bs_bottom:
        ret = bottomColor.set;
        color = bottomColor.color;
        break;
    case tc_border_side::bs_left:
        ret = leftColor.set;
        color = leftColor.color;
        break;
    case tc_border_side::bs_right:
        ret = rightColor.set;
        color = rightColor.color;
        break;
    case tc_border_side::bs_top:
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

stroke_style TcBorderData::GetStrokeStyle(tc_border_side side)
{
    switch (side)
    {
    case tc_border_side::bs_bottom:
        if (bottomStyle == stroke_style::bs_not_set)
            return borderStyle;
        return bottomStyle;
    case tc_border_side::bs_left:
        if (leftStyle == stroke_style::bs_not_set)
            return borderStyle;
        return leftStyle;
    case tc_border_side::bs_right:
        if (rightStyle == stroke_style::bs_not_set)
            return borderStyle;
        return rightStyle;
    case tc_border_side::bs_top:
        if (rightStyle == stroke_style::bs_not_set)
            return borderStyle;
        return rightStyle;
    }
    return borderStyle;
}

void TcBorderData::ClearCorner(tc_border_side side)
{
    switch (side)
    {
    case tc_border_side::bs_all:
        ClearCorner(tc_border_side::bs_bottom);
        ClearCorner(tc_border_side::bs_left);
        ClearCorner(tc_border_side::bs_right);
        ClearCorner(tc_border_side::bs_top);
        break;
    case tc_border_side::bs_bottom:
        if (bottomRight)
            delete bottomRight;
        bottomRight = nullptr;
        break;
    case tc_border_side::bs_left:
        if (bottomLeft)
            delete bottomLeft;
        bottomLeft = nullptr;
        break;
    case tc_border_side::bs_right:
        if (topRight)
            delete topRight;
        topRight = nullptr;
        break;
    case tc_border_side::bs_top:
        if (topLeft)
            delete topLeft;
        topLeft = nullptr;
        break;
    }
}

