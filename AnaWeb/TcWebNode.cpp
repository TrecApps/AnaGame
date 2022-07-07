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

TDataMap<R_Message_Type> messageTypeMap;

bool GetMessageType(const TString& messageStr, R_Message_Type& message)
{
    if (!messageTypeMap.count())
    {
        messageTypeMap.addEntry(L"onkeypress", R_Message_Type::On_Char);
        messageTypeMap.addEntry(L"", R_Message_Type::On_check);
        messageTypeMap.addEntry(L"onclick", R_Message_Type::On_Click);
        messageTypeMap.addEntry(L"", R_Message_Type::On_Click_Release);
        messageTypeMap.addEntry(L"", R_Message_Type::On_Flyout);
        messageTypeMap.addEntry(L"", R_Message_Type::On_Focus);
        messageTypeMap.addEntry(L"", R_Message_Type::On_Hold_Click);
        messageTypeMap.addEntry(L"onmousemove", R_Message_Type::On_Hover);
        messageTypeMap.addEntry(L"onmouseover", R_Message_Type::On_Hover_Enter);
        messageTypeMap.addEntry(L"onmouseout", R_Message_Type::On_Hover_Leave);
        messageTypeMap.addEntry(L"ondblclick", R_Message_Type::On_LDoubleClick);
        messageTypeMap.addEntry(L"", R_Message_Type::On_Lose_Focus);
        messageTypeMap.addEntry(L"onmousedown", R_Message_Type::On_L_Button_Down);

        messageTypeMap.addEntry(L"onmouseup", R_Message_Type::On_L_Button_Up);
        messageTypeMap.addEntry(L"", R_Message_Type::On_radio_change);
        messageTypeMap.addEntry(L"onresize", R_Message_Type::On_Resized);
        messageTypeMap.addEntry(L"", R_Message_Type::On_Right_Click);
        messageTypeMap.addEntry(L"", R_Message_Type::On_Right_Release);
        messageTypeMap.addEntry(L"", R_Message_Type::On_R_Button_Down);
        messageTypeMap.addEntry(L"", R_Message_Type::On_R_Button_Up);
        messageTypeMap.addEntry(L"", R_Message_Type::On_Scrolled);
        messageTypeMap.addEntry(L"", R_Message_Type::On_Select_Scroller);
        messageTypeMap.addEntry(L"", R_Message_Type::On_sel_change);
        messageTypeMap.addEntry(L"", R_Message_Type::On_SubmitDrag);
        messageTypeMap.addEntry(L"", R_Message_Type::On_Text_Change);
        messageTypeMap.addEntry(L"", R_Message_Type::On_L_Button_Down);
    }

    return messageStr.GetSize() && messageTypeMap.retrieveEntry(messageStr, message);
}


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
    if(!textElement.Get())
    return D2D1_RECT_F();
    return textElement->GetLocation();
}

bool TcWebNode::TcTextNodeElement::IsFirst()
{
    return isFirst;
}

TrecPointer<TTextElement> TcWebNode::TcTextNodeElement::GetTextElement()
{
    return textElement;
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

void TcWebNode::AddFile(TrecPointer<TFileShell> file)
{
    if (!file.Get() || file->IsDirectory())return;

    TString ext(file->GetName());
    ext.Set(ext.SubString(ext.FindLast(L'.')+1).GetLower().GetTrim());
    if (!ext.Compare(L"png") 
        || !ext.Compare(L"jpeg")
        || !ext.Compare(L"jpg")
        || !ext.Compare(L"gif"))
    {
        TrecSubPointer<TBrush, TBitmapBrush> bitBrush = this->drawingBoard->GetBrush(file, area);
        if (!bitBrush.Get())
            return;

        D2D1_SIZE_F imageSize = bitBrush->GetDefaultSize();

        if (!contentData.size.height)
            contentData.size.height = imageSize.height;
        if (!contentData.size.width)
            contentData.size.width = imageSize.width;
        contentData.bitmap = TrecSubToTrec<>(bitBrush);
    }
}

TcWebNode::TcWebNode(TrecPointer<DrawingBoard> board): TPage(board)
{

    outsideDisplay = TcWebNodeDisplayOutside::wndo_block;
    insideDisplay = TcWebNodeDisplayInside::wndi_flex;
    internalDisplay = TcWebNodeDisplayInternal::wndi_not_set;
    win = board->GetWindowHandle();
    doDisplay = shouldDraw = true;
    isListItem = false;
    listForward = doShrink = true;
    columnSpan = rowSpan = 1;

    listStyle = list_style::ls_none;

    innerMargin = outerMargin = { 0,0,0,0 };

    onLClick = onMouse = onRClick = false;
}

bool TcWebNode::HandlesEvents()
{
    return false;
}

void TcWebNode::Draw(TrecPointer<TVariable> object)
{
    if (!shouldDraw)
        return;

    // drawingBoard->AddLayer(area);

    //To-Do: Addres more complex backgrounds
    if (contentData.brush.Get())
        contentData.brush->FillRectangle(area);

    if (contentData.bitmap.Get())
    {
        auto picLoc = area;
        picLoc.bottom = contentData.size.height ? (picLoc.top + contentData.size.height) : area.bottom;
        picLoc.right = contentData.size.width ? (picLoc.left + contentData.size.width) : area.right;

        while (picLoc.top < area.bottom)
        {
            while (picLoc.left < area.right)
            {
                contentData.bitmap->FillRectangle(picLoc);

                if (!contentData.repeatX || !contentData.size.width)
                    break;
                picLoc.left += contentData.size.width;
                picLoc.right += contentData.size.width;
            }
            if (!contentData.repeatY || !contentData.size.height)
                break;

            picLoc.top += contentData.size.height;
            picLoc.bottom += contentData.size.height;
            picLoc.right = contentData.size.width ? (picLoc.left + contentData.size.width) : area.right;
        }
    }

    DrawBorder();

    // Now draw the ChildNodes
    for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
    {
        switch (childNodes[Rust]->GetElementType())
        {
        case NodeContainerType::nct_reg_node:
        case NodeContainerType::nct_tex_node:
            dynamic_cast<TcWebNodeElement*>(childNodes[Rust].Get())->GetWebNode()->Draw(object);
            continue;
        case NodeContainerType::net_tab_node:
            dynamic_cast<TcTableNodeElement*>(childNodes[Rust].Get())->GetWebNode()->Draw(object);
        }

    }

    // Draw Text
    if(tagName.CompareNoCase(L"img") || !contentData.bitmap.Get())
    for (UINT Rust = 0; Rust < textElements.Size(); Rust++)
    {
        textElements[Rust]->text->OnDraw(object);
    }
    //drawingBoard->PopLayer();
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

ag_msg void TcWebNode::OnRButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>& cred)
{
    EventPropagater prop;
    if (onRClick && handlers.retrieveEntry(L"onclick", prop))
    {
        EventID_Cred c;
        c.control = TrecPointerKey::GetTrecPointerFromSoft<>(self);
        c.expression.Set(prop.event);
        c.eventType = R_Message_Type::On_Right_Click;
        cred.push_back(c);
    }

    onRClick = false;
    if (!IsContained(point, area))
    {
        return;
    }

    bool hasEventProp = handlers.retrieveEntry(L"onmouseup", prop);

    TDataArray<TPoint> points;
    for (UINT Rust = 0; Rust < textElements.Size(); Rust++)
    {
        UINT i = 0;
        if (textElements[Rust]->text->GetClickIndex(i, point))
            points.push_back(TPoint(Rust, i));
    }

    UINT curCredCount = cred.Size();

    if (!points.Size())
    {
        if (outsideDisplay == TcWebNodeDisplayOutside::wndo_inline)
            hasEventProp = false;
        for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
        {
            switch (childNodes[Rust]->GetElementType())
            {
            case NodeContainerType::nct_reg_node:
            case NodeContainerType::nct_tex_node:
                dynamic_cast<TcWebNodeElement*>(childNodes[Rust].Get())->GetWebNode()->OnRButtonUp(nFlags, point, mOut, cred);
                break;
            case NodeContainerType::net_tab_node:
                dynamic_cast<TcTableNodeElement*>(childNodes[Rust].Get())->GetWebNode()->OnRButtonUp(nFlags, point, mOut, cred);
            }
        }
    }
    else
    {
        // To-Do: Handle scenario where second (or third...) Complext Text Element is used
        UINT indexStart = 0;
        FishForTextEvents(L"ommouseup", indexStart, points[0].y, cred);
    }


    

    if (hasEventProp)
    {
        EventID_Cred c;
        c.control = TrecPointerKey::GetTrecPointerFromSoft<>(self);
        c.expression.Set(prop.event);
        c.eventType = R_Message_Type::On_R_Button_Up;

        curCredCount = cred.Size() - curCredCount;

        if (prop.useCapture && curCredCount)
        {
            cred.InsertAt(c, cred.Size() - (curCredCount + 1));
        }
        else
            cred.push_back(c);
    }
}

ag_msg void TcWebNode::OnRButtonDown(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>& cred)
{
    if (!IsContained(point, area))
    {
        return;
    }

    onRClick = true;

    EventPropagater prop;
    bool hasEventProp = handlers.retrieveEntry(L"onmousedown", prop);

    TDataArray<TPoint> points;
    for (UINT Rust = 0; Rust < textElements.Size(); Rust++)
    {
        UINT i = 0;
        if (textElements[Rust]->text->GetClickIndex(i, point))
            points.push_back(TPoint(Rust, i));
    }

    UINT curCredCount = cred.Size();

    if (!points.Size())
    {
        if (outsideDisplay == TcWebNodeDisplayOutside::wndo_inline)
            hasEventProp = false;
        for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
        {
            switch (childNodes[Rust]->GetElementType())
            {
            case NodeContainerType::nct_reg_node:
            case NodeContainerType::nct_tex_node:
                dynamic_cast<TcWebNodeElement*>(childNodes[Rust].Get())->GetWebNode()->OnRButtonDown(nFlags, point, mOut, cred);
                break;
            case NodeContainerType::net_tab_node:
                dynamic_cast<TcTableNodeElement*>(childNodes[Rust].Get())->GetWebNode()->OnRButtonDown(nFlags, point, mOut, cred);
            }
        }
    }
    else
    {
        // To-Do: Handle scenario where second (or third...) Complext Text Element is used
        UINT indexStart = 0;
        FishForTextEvents(L"onmousedown", indexStart, points[0].y, cred);
    }




    if (hasEventProp)
    {
        EventID_Cred c;
        c.control = TrecPointerKey::GetTrecPointerFromSoft<>(self);
        c.expression.Set(prop.event);
        c.eventType = R_Message_Type::On_R_Button_Up;

        curCredCount = cred.Size() - curCredCount;

        if (prop.useCapture && curCredCount)
        {
            cred.InsertAt(c, cred.Size() - (curCredCount + 1));
        }
        else
            cred.push_back(c);
    }

}

ag_msg void TcWebNode::OnLButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>& cred)
{
    EventPropagater prop;
    if (onLClick && handlers.retrieveEntry(L"onclick", prop))
    {
        EventID_Cred c;
        c.control = TrecPointerKey::GetTrecPointerFromSoft<>(self);
        c.expression.Set(prop.event);
        c.eventType = R_Message_Type::On_Click;
        cred.push_back(c);
    }

    onLClick = false;
    if (!IsContained(point, area))
    {
        return;
    }

    bool hasEventProp = handlers.retrieveEntry(L"onmouseup", prop);

    TDataArray<TPoint> points;
    for (UINT Rust = 0; Rust < textElements.Size(); Rust++)
    {
        UINT i = 0;
        if (textElements[Rust]->text->GetClickIndex(i, point))
            points.push_back(TPoint(Rust, i));
    }

    UINT curCredCount = cred.Size();

    if (!points.Size())
    {
        if (outsideDisplay == TcWebNodeDisplayOutside::wndo_inline)
            hasEventProp = false;
        for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
        {
            switch (childNodes[Rust]->GetElementType())
            {
            case NodeContainerType::nct_reg_node:
            case NodeContainerType::nct_tex_node:
                dynamic_cast<TcWebNodeElement*>(childNodes[Rust].Get())->GetWebNode()->OnLButtonUp(nFlags, point, mOut, cred);
                break;
            case NodeContainerType::net_tab_node:
                dynamic_cast<TcTableNodeElement*>(childNodes[Rust].Get())->GetWebNode()->OnLButtonUp(nFlags, point, mOut, cred);
            }
        }
    }
    else
    {
        // To-Do: Handle scenario where second (or third...) Complext Text Element is used
        UINT indexStart = 0;
        FishForTextEvents(L"ommouseup", indexStart, points[0].y, cred);
        indexStart = 0;
        FishForTextEvents(L"onclick", indexStart, points[0].y, cred);
    }

    if (hasEventProp)
    {
        EventID_Cred c;
        c.control = TrecPointerKey::GetTrecPointerFromSoft<>(self);
        c.expression.Set(prop.event);
        c.eventType = R_Message_Type::On_L_Button_Up;

        curCredCount = cred.Size() - curCredCount;

        if (prop.useCapture && curCredCount)
        {
            cred.InsertAt(c, cred.Size() - (curCredCount + 1));
        }
        else
            cred.push_back(c);
    }
}

ag_msg void TcWebNode::OnLButtonDown(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>& cred)
{
    if (!IsContained(point, area))
    {
        return;
    }

    onLClick = true;

    EventPropagater prop;
    bool hasEventProp = handlers.retrieveEntry(L"onmousedown", prop);

    TDataArray<TPoint> points;
    for (UINT Rust = 0; Rust < textElements.Size(); Rust++)
    {
        UINT i = 0;
        if (textElements[Rust]->text->GetClickIndex(i, point))
            points.push_back(TPoint(Rust, i));
    }

    UINT curCredCount = cred.Size();

    if (!points.Size())
    {
        if (outsideDisplay == TcWebNodeDisplayOutside::wndo_inline)
            hasEventProp = false;
        for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
        {
            switch (childNodes[Rust]->GetElementType())
            {
            case NodeContainerType::nct_reg_node:
            case NodeContainerType::nct_tex_node:
                dynamic_cast<TcWebNodeElement*>(childNodes[Rust].Get())->GetWebNode()->OnLButtonDown(nFlags, point, mOut, cred);
                break;
            case NodeContainerType::net_tab_node:
                dynamic_cast<TcTableNodeElement*>(childNodes[Rust].Get())->GetWebNode()->OnLButtonDown(nFlags, point, mOut, cred);
            }
        }
    }
    else
    {
        // To-Do: Handle scenario where second (or third...) Complext Text Element is used
        UINT indexStart = 0;
        FishForTextEvents(L"onmousedown", indexStart, points[0].y, cred);
    }

    if (hasEventProp)
    {
        EventID_Cred c;
        c.control = TrecPointerKey::GetTrecPointerFromSoft<>(self);
        c.expression.Set(prop.event);
        c.eventType = R_Message_Type::On_R_Button_Up;

        curCredCount = cred.Size() - curCredCount;

        if (prop.useCapture && curCredCount)
        {
            cred.InsertAt(c, cred.Size() - (curCredCount + 1));
        }
        else
            cred.push_back(c);
    }
}

ag_msg void TcWebNode::OnMouseMove(UINT nFlags, TPoint point, message_output& mOut, TDataArray<EventID_Cred>& cred)
{
    EventPropagater prop;


    if (!IsContained(point, area))
    {
        if (onMouse && handlers.retrieveEntry(L"onmouseout", prop))
        {
            EventID_Cred c;
            c.control = TrecPointerKey::GetTrecPointerFromSoft<>(self);
            c.expression.Set(prop.event);
            c.eventType = R_Message_Type::On_Hover_Leave;
            cred.push_back(c);
        }
        onMouse = false;
        return;
    }
    // in nFlags is 0, then the call is directly due to an event. if 1, then the window is cleaning up pages from a previous call
    if (nFlags)
        return;

    TString eventToUse(onMouse ? L"onmousemove" : L"onmouseover");
    onMouse = true;

    bool hasEventProp = handlers.retrieveEntry(eventToUse, prop);

    TDataArray<TPoint> points;
    for (UINT Rust = 0; Rust < textElements.Size(); Rust++)
    {
        UINT i = 0;
        if (textElements[Rust]->text->GetClickIndex(i, point))
            points.push_back(TPoint(Rust, i));
    }

    UINT curCredCount = cred.Size();

    if (!points.Size())
    {
        if (outsideDisplay == TcWebNodeDisplayOutside::wndo_inline)
            hasEventProp = false;
        for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
        {
            switch (childNodes[Rust]->GetElementType())
            {
            case NodeContainerType::nct_reg_node:
            case NodeContainerType::nct_tex_node:
                dynamic_cast<TcWebNodeElement*>(childNodes[Rust].Get())->GetWebNode()->OnMouseMove(nFlags, point, mOut, cred);
                break;
            case NodeContainerType::net_tab_node:
                dynamic_cast<TcTableNodeElement*>(childNodes[Rust].Get())->GetWebNode()->OnMouseMove(nFlags, point, mOut, cred);
            }
        }
    }
    else
    {
        // To-Do: Handle scenario where second (or third...) Complext Text Element is used
        UINT indexStart = 0;
        FishForTextEvents(eventToUse, indexStart, points[0].y, cred);
    }

    if (hasEventProp)
    {
        EventID_Cred c;
        c.control = TrecPointerKey::GetTrecPointerFromSoft<>(self);
        c.expression.Set(prop.event);
        c.eventType = R_Message_Type::On_L_Button_Up;

        curCredCount = cred.Size() - curCredCount;

        if (prop.useCapture && curCredCount)
        {
            cred.InsertAt(c, cred.Size() - (curCredCount + 1));
        }
        else
            cred.push_back(c);
    }
}

ag_msg void TcWebNode::OnLButtonDblClk(UINT nFlags, TPoint point, message_output& mOut, TDataArray<EventID_Cred>& cred)
{
    if (!IsContained(point, area))
    {
        return;
    }

    EventPropagater prop;
    bool hasEventProp = handlers.retrieveEntry(L"ondblclick", prop);

    TDataArray<TPoint> points;
    for (UINT Rust = 0; Rust < textElements.Size(); Rust++)
    {
        UINT i = 0;
        if (textElements[Rust]->text->GetClickIndex(i, point))
            points.push_back(TPoint(Rust, i));
    }

    UINT curCredCount = cred.Size();

    if (!points.Size())
    {
        if (outsideDisplay == TcWebNodeDisplayOutside::wndo_inline)
            hasEventProp = false;
        for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
        {
            switch (childNodes[Rust]->GetElementType())
            {
            case NodeContainerType::nct_reg_node:
            case NodeContainerType::nct_tex_node:
                dynamic_cast<TcWebNodeElement*>(childNodes[Rust].Get())->GetWebNode()->OnLButtonDblClk(nFlags, point, mOut, cred);
                break;
            case NodeContainerType::net_tab_node:
                dynamic_cast<TcTableNodeElement*>(childNodes[Rust].Get())->GetWebNode()->OnLButtonDblClk(nFlags, point, mOut, cred);
            }
        }
    }
    else
    {
        // To-Do: Handle scenario where second (or third...) Complext Text Element is used
        UINT indexStart = 0;
        FishForTextEvents(L"ondblclick", indexStart, points[0].y, cred);
    }

    if (hasEventProp)
    {
        EventID_Cred c;
        c.control = TrecPointerKey::GetTrecPointerFromSoft<>(self);
        c.expression.Set(prop.event);
        c.eventType = R_Message_Type::On_R_Button_Up;

        curCredCount = cred.Size() - curCredCount;

        if (prop.useCapture && curCredCount)
        {
            cred.InsertAt(c, cred.Size() - (curCredCount + 1));
        }
        else
            cred.push_back(c);
    }
}

ag_msg void TcWebNode::OnResize(D2D1_RECT_F& newLoc, UINT nFlags, TDataArray<EventID_Cred>& eventAr)
{
    UINT curCredCount = eventAr.Size();
    for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
    {
        switch (childNodes[Rust]->GetElementType())
        {
        case NodeContainerType::nct_reg_node:
        case NodeContainerType::nct_tex_node:
            dynamic_cast<TcWebNodeElement*>(childNodes[Rust].Get())->GetWebNode()->OnResize(newLoc, nFlags, eventAr);
            break;
        case NodeContainerType::net_tab_node:
            dynamic_cast<TcTableNodeElement*>(childNodes[Rust].Get())->GetWebNode()->OnResize(newLoc, nFlags, eventAr);
        }
    }
    EventPropagater prop;
    bool hasEventProp = handlers.retrieveEntry(L"onresize", prop);

    if (hasEventProp)
    {
        EventID_Cred c;
        c.control = TrecPointerKey::GetTrecPointerFromSoft<>(self);
        c.expression.Set(prop.event);
        c.eventType = R_Message_Type::On_R_Button_Up;

        curCredCount = eventAr.Size() - curCredCount;

        if (prop.useCapture && curCredCount)
        {
            eventAr.InsertAt(c, eventAr.Size() - (curCredCount + 1));
        }
        else
            eventAr.push_back(c);
    }

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
    {
        TString printableText;
        if (!tagName.CompareNoCase(L"img") && attributes.retrieveEntry(L"alt", printableText) && printableText.GetSize())
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
        return 0;
    }
    return ProcessInnerHtml(html, start, win, styles);

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
            assert(webCh.GetBase());

            if (webCh.Get() && webCh->GetWebNode()->insideDisplay == TcWebNodeDisplayInside::wndi_table)
            {
                TrecSubPointer<TcNodeElement, TcTableNodeElement> tabCh = TrecPointerKey::GetNewTrecSubPointer<TcNodeElement, TcTableNodeElement>(webCh->GetWebNode());
                childNodes[Rust] = TrecSubToTrec<>(tabCh);
            }
            else if(webCh.Get())
            webCh->SetVariable(L"NodeType", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(
                webCh->GetWebNode()->IsText() ? L"Text" : L"Web"));
        }
    }
}

UINT TcWebNode::CreateWebNode(D2D1_RECT_F location, TrecPointer<TWindowEngine> d3dEngine, HWND window, TDataArray<FileRequest>& fileRequests, bool shrinkHeight, bool requestFiles)
{
    // Go through each bit of the child elements, determining where they lay
    if (this->doDisplay && (!TcIsVoidElement(tagName) || !tagName.CompareNoCase(L"img")))
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

    if (requestFiles)
    {
        if (contentData.file.GetSize())
        {
            FileRequest newRequest;
            newRequest.requesterNode = TrecPointerKey::GetTrecSubPointerFromTrec<TPage, TcWebNode>(TrecPointerKey::GetTrecPointerFromSoft<>(self));
            newRequest.fileRequest.Set(contentData.file);
            fileRequests.push_back(newRequest);
        }
    }

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

    //float curBottomMargin = 0;

    

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
                ProcessTextNodes(textNodes, textDataList, location, shrinkHeight);
                textNodes.RemoveAll();
                textDataList.RemoveAll();
            }
            TrecSubPointer<TPage, TcWebNode> childNode = dynamic_cast<TcWebNodeElement*>(element.Get())->GetWebNode();

            if (internalDisplay == TcWebNodeDisplayInternal::wndi_row)
            {
                
                if (!Rust)
                    location.right = location.left + columnSizes[0];
                else if (Rust < columnSizes.Size())
                {
                    location.left = location.right;
                    location.right = location.left + columnSizes[Rust];
                }
                childNode->CreateWebNode(location, d3dEngine, window, fileRequests, true, requestFiles);
            }
            else
            {
                //location.top -= min(curBottomMargin, childNode->outerMargin.top);
                childNode->CreateWebNode(location, d3dEngine, window, fileRequests,true, requestFiles);
                childNode->ShrinkHeight();
                if (childNode->GetArea().bottom > location.top)
                {
                    //curBottomMargin = childNode->outerMargin.bottom;
                    location.top = childNode->GetArea().bottom;
                }
            }


        }
        else if (element->GetElementType() == NodeContainerType::net_tab_node)
        {
            auto tableElement = TrecPointerKey::GetTrecSubPointerFromTrec<TcNodeElement, TcTableNodeElement>(element);
            if (internalDisplay == TcWebNodeDisplayInternal::wndi_row)
            {
                if (!Rust)
                    location.right = location.left + columnSizes[0];
                else if (Rust < columnSizes.Size())
                {
                    location.left = location.right;
                    location.right = location.left + columnSizes[Rust];
                }
                tableElement->PrepTable();
                tableElement->CreateTable(location, d3dEngine, window, fileRequests, requestFiles);
            }
            else
            {
               // location.top -= min(curBottomMargin, tableElement->GetOuterMargin().top);
                tableElement->PrepTable();
                tableElement->CreateTable(location, d3dEngine, window, fileRequests, requestFiles);
                location.top = tableElement->GetLocation().bottom;

                //curBottomMargin = tableElement->GetOuterMargin().bottom;
            }
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
    if(shrinkHeight)
        ShrinkHeight();
    if (insideDisplay == TcWebNodeDisplayInside::wndi_table)
    {
        TDataArray<TDataArray<float>> widthTable;
        for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
        {
            auto cNode = childNodes[Rust];
            TrecSubPointer<TPage, TcWebNode> webNode;
            if (cNode->GetElementType() == NodeContainerType::nct_reg_node || cNode->GetElementType() == NodeContainerType::nct_tex_node)
                webNode = dynamic_cast<TcWebNodeElement*>(cNode.Get())->GetWebNode();
            else if (cNode->GetElementType() == NodeContainerType::net_tab_node)
                webNode = dynamic_cast<TcTableNodeElement*>(cNode.Get())->GetWebNode();
            if (webNode.Get())
            {
                webNode->TableGetNeededWidth(widthTable);
            }
        }

        TDataArray<float> columnWidths;

        for (UINT Rust = 0; Rust < widthTable.Size(); Rust++)
        {
            for (UINT C = 0; C < widthTable[Rust].Size(); C++)
            {
                float val = widthTable[Rust][C];
                if (C >= columnWidths.Size())
                    columnWidths.push_back(val);
                else if (val > columnWidths[C])
                    columnWidths[C] = val;
            }
        }

        if (columnWidths.Size())
        {
            for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
            {
                auto cNode = childNodes[Rust];
                TrecSubPointer<TPage, TcWebNode> webNode;
                if (cNode->GetElementType() == NodeContainerType::nct_reg_node || cNode->GetElementType() == NodeContainerType::nct_tex_node)
                    webNode = dynamic_cast<TcWebNodeElement*>(cNode.Get())->GetWebNode();
                else if (cNode->GetElementType() == NodeContainerType::net_tab_node)
                    webNode = dynamic_cast<TcTableNodeElement*>(cNode.Get())->GetWebNode();
                if (webNode.Get())
                {
                    webNode->SetTableColWidth(columnWidths);
                }
            }
        }

        rowSizes.RemoveAll();

        CollectRowSizes(rowSizes);
        for (UINT Rust = 0, C = 0; Rust < childNodes.Size(); Rust++)
        {
            TrecSubPointer<TPage, TcWebNode> node;
            switch (childNodes[Rust]->GetElementType())
            {
            case NodeContainerType::nct_reg_node:
            case NodeContainerType::nct_tex_node:
                node = dynamic_cast<TcWebNodeElement*>(childNodes[Rust].Get())->GetWebNode();
                break;
            case NodeContainerType::net_tab_node:
                node = dynamic_cast<TcTableNodeElement*>(childNodes[Rust].Get())->GetWebNode();
            }
            node->HandleRowSpan(rowSizes, C, fileRequests, requestFiles);
        }


        //ShrinkWidth(0);
        //HandleRowSpan();
    }

    if (textNodes.Size())
    {
        ProcessTextNodes(textNodes, textDataList, location, shrinkHeight);
        textNodes.RemoveAll();
    }

    return 0;
}

void TcWebNode::HandleRowSpan(TDataArray<UINT>& sizes, UINT& row, TDataArray<FileRequest>& fileRequests, bool requestFiles)
{
    if (internalDisplay == TcWebNodeDisplayInternal::wndi_row)
    {
        for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
        {
            TrecSubPointer<TPage, TcWebNode> node;
            switch (childNodes[Rust]->GetElementType())
            {
            case NodeContainerType::nct_reg_node:
            case NodeContainerType::nct_tex_node:
                node = dynamic_cast<TcWebNodeElement*>(childNodes[Rust].Get())->GetWebNode();
                break;
            case NodeContainerType::net_tab_node:
                node = dynamic_cast<TcTableNodeElement*>(childNodes[Rust].Get())->GetWebNode();
            }
            D2D1_RECT_F loc = node->area;
            loc.top -= node->outerMargin.top;
            loc.left -= node->outerMargin.left;
            loc.right += node->outerMargin.right;
            loc.bottom += node->outerMargin.bottom;

            for (UINT C = 1, c = row + 1; C < node->rowSpan && c < sizes.Size(); C++, c++)
            {
                loc.bottom += sizes[c];
            }

            node->CreateWebNode(loc, d3dEngine, this->win, fileRequests, false, requestFiles);
        }
        row++;
    }
    else if (insideDisplay == TcWebNodeDisplayInside::wndi_table)
        return;
    else
    {
        for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
        {
            TrecSubPointer<TPage, TcWebNode> node;
            switch (childNodes[Rust]->GetElementType())
            {
            case NodeContainerType::nct_reg_node:
            case NodeContainerType::nct_tex_node:
                node = dynamic_cast<TcWebNodeElement*>(childNodes[Rust].Get())->GetWebNode();
                break;
            case NodeContainerType::net_tab_node:
                node = dynamic_cast<TcTableNodeElement*>(childNodes[Rust].Get())->GetWebNode();
            }
            node->HandleRowSpan(sizes, row, fileRequests, requestFiles);
        }
    }
}

void TcWebNode::CollectRowSizes(TDataArray<UINT>& sizes)
{

    for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
    {
        TrecSubPointer<TPage, TcWebNode> node;
        switch (childNodes[Rust]->GetElementType())
        {
        case NodeContainerType::nct_reg_node:
        case NodeContainerType::nct_tex_node:
            node = dynamic_cast<TcWebNodeElement*>(childNodes[Rust].Get())->GetWebNode();
            break;
        case NodeContainerType::net_tab_node:
            node = dynamic_cast<TcTableNodeElement*>(childNodes[Rust].Get())->GetWebNode();
        }

        if (node->insideDisplay == TcWebNodeDisplayInside::wndi_table)
            continue;
        if (node->internalDisplay == TcWebNodeDisplayInternal::wndi_row)
        {
            D2D1_RECT_F loc = node->area;
            loc.top -= node->outerMargin.top;
            loc.bottom += node->outerMargin.bottom;
            sizes.push_back(loc.bottom - loc.top);
        }
        else node->CollectRowSizes(sizes);
    }
}


void TcWebNode::ShrinkWidth(UINT minWidth)
{
    if (insideDisplay == TcWebNodeDisplayInside::wndi_table)
    {
        float width = NeedsWidth(0);

        for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
        {
            TrecPointer<TTextElement> field;
            TrecSubPointer<TPage, TcWebNode> node;
            switch (childNodes[Rust]->GetElementType())
            {
            case NodeContainerType::nct_raw_text:
                field = dynamic_cast<TcTextNodeElement*>(childNodes[Rust].Get())->GetTextElement();
                break;
            case NodeContainerType::nct_reg_node:
            case NodeContainerType::nct_tex_node:
                node = dynamic_cast<TcWebNodeElement*>(childNodes[Rust].Get())->GetWebNode();
                break;
            case NodeContainerType::net_tab_node:
                node = dynamic_cast<TcTableNodeElement*>(childNodes[Rust].Get())->GetWebNode();
            }

            if (field.Get())
            {
                auto fieldLoc = field->GetLocation();
                fieldLoc.right = fieldLoc.left + width;

                field->SetLocation(fieldLoc);
            }
            else if (node.Get())
            {
                if (node->internalDisplay == TcWebNodeDisplayInternal::wndi_row)
                {
                    node->SetColumnsAndRows(columnSizes, rowSizes);
                    node->ShrinkWidth(0);
                }
                else
                {
                    auto nodeLoc = node->GetArea();
                    nodeLoc.right = nodeLoc.left + width;
                    TDataArray<FileRequest> req;
                    node->CreateWebNode(nodeLoc, d3dEngine, nullptr, req);
                }
            }
        }
    }

    if (internalDisplay == TcWebNodeDisplayInternal::wndi_row)
    {
        auto tempLoc = area;
        bool w;
        if (columnSizes.Size())
            tempLoc.right = tempLoc.left + columnSizes[0];
        for (UINT Rust = 0, C = 0; Rust < columnSizes.Size() && C < childNodes.Size(); C++)
        {
            UINT curWidth = tempLoc.right - tempLoc.left;
            TrecPointer<TTextElement> field;
            TrecSubPointer<TPage, TcWebNode> node;
            switch (childNodes[C]->GetElementType())
            {
            case NodeContainerType::nct_raw_text:
                field = dynamic_cast<TcTextNodeElement*>(childNodes[C].Get())->GetTextElement();
                break;
            case NodeContainerType::nct_reg_node:
            case NodeContainerType::nct_tex_node:
                node = dynamic_cast<TcWebNodeElement*>(childNodes[C].Get())->GetWebNode();
                break;
            case NodeContainerType::net_tab_node:
                node = dynamic_cast<TcTableNodeElement*>(childNodes[C].Get())->GetWebNode();
            }

            if (field.Get())
            {
                field->SetLocation(tempLoc);
                Rust++;
            }
            else if (node.Get())
            {
                UINT span = 1;


                for (; span < node->columnSpan; span++)
                {
                    if ((Rust + span) < columnSizes.Size())
                        curWidth += columnSizes[Rust + span];
                }
                Rust += span;
                tempLoc.right = tempLoc.left + curWidth;
                TDataArray<FileRequest> req;
                node->CreateWebNode(tempLoc, d3dEngine, nullptr,req);
            }

            if ((Rust) < columnSizes.Size())
            {
                tempLoc.left = tempLoc.right;
                tempLoc.right += columnSizes[Rust];
            }
        }
    }

}

void TcWebNode::AffirmDownEvent(R_Message_Type rType)
{
    if (rType == R_Message_Type::On_Hover_Enter)
        onMouse = true;
    if (rType == R_Message_Type::On_R_Button_Down)
        onRClick = true;
    if (rType == R_Message_Type::On_L_Button_Down)
        onLClick = true;
}

bool TcWebNode::AffirmClickEvent(R_Message_Type rType)
{
    bool ret = true;
    if (rType == R_Message_Type::On_Click)
    {
        ret = onRClick || onLClick;
        onRClick = onLClick = false;
    }
    return ret;
}

UINT TcWebNode::FishForTextEvents(const TString& eventType, UINT& startText, UINT textIndex, TDataArray<EventID_Cred>& cred)
{
    if (startText > textIndex)
        return 0;
    textIndex -= startText;
    UINT textSize = 0;
    UINT found = 0;
    bool bFound = false;
    
    R_Message_Type type;
    bool foundEvent = GetMessageType(eventType, type);
    
    for (UINT Rust = 0; !found && !bFound && Rust < childNodes.Size(); Rust++)
    {
        switch (childNodes[Rust]->GetElementType())
        {
        case NodeContainerType::nct_raw_text:
            textSize = dynamic_cast<TcTextNodeElement*>(childNodes[Rust].Get())->data.text.GetSize();
            
            if (textIndex < textSize)
            {
                bFound = 1;

                

                break;
            }
            else
                textIndex -= textSize;

            break;
        case NodeContainerType::nct_tex_node:
            textSize = 0;
            found += dynamic_cast<TcWebNodeElement*>(childNodes[Rust].Get())->GetWebNode()->FishForTextEvents(eventType, textSize, textIndex, cred);

        }
        startText += textSize;
    }
    bool doOp = false;
    if (bFound || found)
    {
        AffirmDownEvent(type);
        doOp = AffirmClickEvent(type);
    }
    EventPropagater prop;
    bool hasEventProp = handlers.retrieveEntry(eventType, prop);

    if (hasEventProp && doOp)
    {
        EventArgs args;
        args.eventType = type;
        EventID_Cred cr;
        cr.eventType = type;
        cr.control = TrecPointerKey::GetTrecPointerFromSoft<>(self);
        cr.expression.Set(prop.event);

        if (found || bFound)
        {
            

            if (found && prop.useCapture)
            {
                EventID_Cred cr2;
                cred.InsertAt(cr, cred.Size() - (found + 1));
                found++;
            }
            else
                cred.push_back(cr);
        }
        else if (bFound)
        {
            cred.push_back(cr);
        }
    }

    return found + bFound;
}

void TcWebNode::TableGetNeededWidth(TDataArray<TDataArray<float>>& needed)
{
    if (internalDisplay == TcWebNodeDisplayInternal::wndi_row)
    {
        TDataArray<float> add;

        for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
        {
            bool w;
            TrecSubPointer<TPage, TcWebNode> node;
            switch (childNodes[Rust]->GetElementType())
            {
            case NodeContainerType::nct_reg_node:
            case NodeContainerType::nct_tex_node:
                node = dynamic_cast<TcWebNodeElement*>(childNodes[Rust].Get())->GetWebNode();
                break;
            case NodeContainerType::net_tab_node:
                node = dynamic_cast<TcTableNodeElement*>(childNodes[Rust].Get())->GetWebNode();
            }
            if (node.Get())
            {
                add.push_back(node->NeedsWidth(0));
                for (UINT C = 1; C < node->columnSpan; C++)
                    add.push_back(-1.0f);
            }
        }

        needed.push_back(add);
    }
}

void TcWebNode::SetTableColWidth(TDataArray<float>& use)
{
    auto loc = area;
    for (UINT Rust = 0, C = 0; Rust < childNodes.Size() && C < use.Size(); Rust++)
    {
        TrecSubPointer<TPage, TcWebNode> node;
        switch (childNodes[Rust]->GetElementType())
        {
        case NodeContainerType::nct_reg_node:
        case NodeContainerType::nct_tex_node:
            node = dynamic_cast<TcWebNodeElement*>(childNodes[Rust].Get())->GetWebNode();
            break;
        case NodeContainerType::net_tab_node:
            node = dynamic_cast<TcTableNodeElement*>(childNodes[Rust].Get())->GetWebNode();
        }
        if (!node.Get())continue;
        float width = 0.0f;
        for (UINT i = 0; i < node->columnSpan && C < use.Size(); i++)
        {
            width += use[C++];
        }

        loc.right = loc.left + width;
        TDataArray<FileRequest> req;
        node->CreateWebNode(loc,d3dEngine, nullptr, req);

        loc.left = loc.right;
    }
}


float TcWebNode::NeedsWidth(UINT column)
{
    if (internalDisplay == TcWebNodeDisplayInternal::wndi_row)
    {
        for (UINT Rust = 0, C = 0; Rust < childNodes.Size() && C <= column; Rust++)
        {
            float ret = 0.0f;
            UINT colSpan = 1;
            bool w;
            if (childNodes[Rust].Get())
            {
                TrecPointer<TTextElement> field;
                TrecSubPointer<TPage, TcWebNode> node;
                switch (childNodes[Rust]->GetElementType())
                {
                case NodeContainerType::nct_raw_text:
                    field = dynamic_cast<TcTextNodeElement*>(childNodes[Rust].Get())->GetTextElement();
                    break;
                case NodeContainerType::nct_reg_node:
                case NodeContainerType::nct_tex_node:
                    node = dynamic_cast<TcWebNodeElement*>(childNodes[Rust].Get())->GetWebNode();
                    break;
                case NodeContainerType::net_tab_node:
                    node = dynamic_cast<TcTableNodeElement*>(childNodes[Rust].Get())->GetWebNode();
                }

                if (field.Get())
                {
                    ret = field->GetMinWidth(w);
                }
                else if (node.Get())
                {
                    ret = node->NeedsWidth(0);
                    colSpan = node->columnSpan;
                }
            }
            if (C == column)
                return ret + 1 + this->innerMargin.left + this->innerMargin.right;
            if (!colSpan)
                colSpan = 1;
            C += colSpan;
        }
    }
    else if (insideDisplay == TcWebNodeDisplayInside::wndi_table)
    {
        float minNeeded = 0.0f;
        for (UINT Rust = 0; Rust < columnSizes.Size(); Rust++)
        {
            float minOfCol = 0;
            bool w;
            for (UINT C = 0; C < childNodes.Size(); C++)
            {
                TrecPointer<TTextElement> field;
                TrecSubPointer<TPage, TcWebNode> node;
                switch (childNodes[C]->GetElementType())
                {
                case NodeContainerType::nct_raw_text:
                    field = dynamic_cast<TcTextNodeElement*>(childNodes[C].Get())->GetTextElement();
                    break;
                case NodeContainerType::nct_reg_node:
                case NodeContainerType::nct_tex_node:
                    node = dynamic_cast<TcWebNodeElement*>(childNodes[C].Get())->GetWebNode();
                    break;
                case NodeContainerType::net_tab_node:
                    node = dynamic_cast<TcTableNodeElement*>(childNodes[C].Get())->GetWebNode();
                }
                if (field.Get())
                {
                    float needs = field->GetMinWidth(w);
                    if (minNeeded < needs)
                        minNeeded = needs;
                }
                else if (node.Get() && node->internalDisplay == TcWebNodeDisplayInternal::wndi_row)
                {
                    float needs = node->NeedsWidth(Rust);
                    if (minOfCol < needs)
                        minOfCol = needs;
                }
                else if (node.Get())
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
        bool w;
        for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
        {
            TrecPointer<TTextElement> field;
            TrecSubPointer<TPage, TcWebNode> node;
            switch (childNodes[Rust]->GetElementType())
            {
            case NodeContainerType::nct_raw_text:
                field = dynamic_cast<TcTextNodeElement*>(childNodes[Rust].Get())->GetTextElement();
                break;
            case NodeContainerType::nct_reg_node:
            case NodeContainerType::nct_tex_node:
                node = dynamic_cast<TcWebNodeElement*>(childNodes[Rust].Get())->GetWebNode();
                break;
            case NodeContainerType::net_tab_node:
                node = dynamic_cast<TcTableNodeElement*>(childNodes[Rust].Get())->GetWebNode();
            }
            if (field.Get())
            {
                float needs = field->GetMinWidth(w);
                if (ret < needs)
                    ret = needs;
            }
            else if (node.Get())
            {
                float needs = node->NeedsWidth(0);
                if (ret < needs)
                    ret = needs;
            }
        }
        if (ret) return ret + this->innerMargin.left + this->innerMargin.right;
    }

    float ret = 0.0f;
    for (UINT Rust = 0; Rust < textElements.Size(); Rust++)
    {
        bool w;
        float minW = textElements[Rust]->text->GetMinWidth(w);
        if (minW > ret)
            ret = minW;
    }
    return ret + this->innerMargin.left + this->innerMargin.right;
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

    if (atts.retrieveEntry(L"font-size", val))
    {
        float size = ConvertMeasurement(val);
        textData.fontSize = size;
        textData.fontSizeUpdated = true;
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

    if (atts.retrieveEntry(L"background", val))
        contentData.CompileAttributes(val, drawingBoard);
    if (atts.retrieveEntry(L"background-color", val))
        contentData.CompileColor(val, drawingBoard);
    if (atts.retrieveEntry(L"background-image", val))
        contentData.CompileImage(val);
    if (atts.retrieveEntry(L"background-repeat", val))
        contentData.CompileRepeat(val);
    if (atts.retrieveEntry(L"background-attachment", val))
        contentData.CompileAttachment(val);
    if (atts.retrieveEntry(L"background-position", val))
        contentData.CompilePosition(val);

    // Handle the img tag
    if (!tagName.CompareNoCase(L"img"))
    {
        if (atts.retrieveEntry(L"src", val))
            contentData.file.Set(val.GetTrim());
    }

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

    if (atts.retrieveEntry(L"height", val))
    {
        contentData.size.height = ConvertMeasurement(val);
    }
    if (atts.retrieveEntry(L"width", val))
    {
        contentData.size.width = ConvertMeasurement(val);
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
    if (!tagName.CompareNoCase(L"img"))
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

void TcWebNode::ProcessTextNodes(TDataArray<TrecPointer<TcNodeElement>>& textNodes, TDataArray<TcTextData>& textDataList, D2D1_RECT_F& location, bool shrink)
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

    UINT beginningIndex = 0;
    if (isListItem)
    {
        theString.Set(listInfo + L"  " + theString);
        beginningIndex = listInfo.GetSize() + 2;
    }


    element->SetText(theString);
    element->SetHorizontallignment(DWRITE_TEXT_ALIGNMENT_LEADING);
    element->SetVerticalAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    element->SetLocation(location);
    if (textData.textColorUpdated)
        element->SetColor(textData.textColor);
    element->SetBasicFormatting(getTextFormattingDetails(textData, drawingBoard));

    for (UINT Rust = 0; Rust < textNodes.Size(); Rust++)
    {
        TrecSubPointer<TcNodeElement, TcTextNodeElement> textElement = TrecPointerKey::GetTrecSubPointerFromTrec<TcNodeElement, TcTextNodeElement>(textNodes[Rust]);

        TextFormattingDetails details = getTextFormattingDetails(textDataList[Rust], drawingBoard);
        details.range = elementStructure->ranges[Rust];
        element->SetFormatting(details, details.range.startPosition + beginningIndex, details.range.startPosition + details.range.length, false);
    }

    D2D1_RECT_F loc = element->GetLocation();
    bool w;
    if (shrink)
    {
        loc.bottom = loc.top + element->GetMinHeight(w);
        element->SetLocation(loc);
    }
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
    if (!doShrink)
        return;
    // To-Do: Update Method once minimum height is tracked

    float curBottom = area.top;
    for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
    {
        if (!childNodes[Rust].Get() || childNodes[Rust]->GetElementType() == NodeContainerType::ntc_null)
        {
            childNodes.RemoveAt(Rust--);
            continue;
        }
        TrecPointer<TTextElement> textEle;
        D2D1_RECT_F loc = { 0,0,0,0 };
        switch (childNodes[Rust]->GetElementType())
        {
        case NodeContainerType::nct_raw_text:
            textEle = dynamic_cast<TcTextNodeElement*>(childNodes[Rust].Get())->GetTextElement();
            if (textEle.Get())
            {
                loc = textEle->GetLocation();
                bool worked = true;
                loc.bottom = loc.top + textEle->GetMinHeight(worked);
                textEle->SetLocation(loc);

            }
            break;
        case NodeContainerType::nct_reg_node:
        case NodeContainerType::nct_tex_node:
            dynamic_cast<TcWebNodeElement*>(childNodes[Rust].Get())->GetWebNode()->ShrinkHeight();
            loc = dynamic_cast<TcWebNodeElement*>(childNodes[Rust].Get())->GetWebNode()->GetArea();
            break;
        case NodeContainerType::net_tab_node:
            dynamic_cast<TcTableNodeElement*>(childNodes[Rust].Get())->GetWebNode()->ShrinkHeight();
            loc = dynamic_cast<TcTableNodeElement*>(childNodes[Rust].Get())->GetWebNode()->GetArea();
        }

        if (curBottom < loc.bottom)
            curBottom = loc.bottom;
    }
    if(curBottom <= area.top)
    {
        for (UINT Rust = 0; Rust < textElements.Size(); Rust++)
        {
            auto tLoc = textElements[Rust]->text->GetLocation();
            if (tLoc.bottom > curBottom)
                curBottom = tLoc.bottom;
        }
    }
    if (curBottom > area.top)
        area.bottom = curBottom + innerMargin.bottom;

    float curHeight = area.bottom - area.top;
    if (contentData.size.height && curHeight < contentData.size.height)
        area.bottom = area.top + contentData.size.height;
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
    // CompileProperties(styles);
    return 0;
}

void TcWebNode::PreEstablishTable(TDataArray<UINT>& cols, TDataArray<UINT>& rows)
{
    if (insideDisplay == TcWebNodeDisplayInside::wndi_table)
    {
        for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
        {
            if (childNodes[Rust]->GetElementType() != NodeContainerType::nct_reg_node)
                continue;
            TDataArray<UINT> tempCols;
            dynamic_cast<TcWebNodeElement*>(childNodes[Rust].Get())->GetWebNode()->PreEstablishTable(tempCols, rows);

            for (UINT C = cols.Size(); C < tempCols.Size(); C++)
                cols.push_back(0);
        }
        return;
    }

    switch (internalDisplay)
    {
    case TcWebNodeDisplayInternal::wndi_row:
        // To-Do: Call Add Row once Available
        rows.push_back(0);
    case TcWebNodeDisplayInternal::wndi_footer_group:
    case TcWebNodeDisplayInternal::wndi_header_group:
    case TcWebNodeDisplayInternal::wndi_column_group:
    case TcWebNodeDisplayInternal::wndi_row_group:
        for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
        {

            if (childNodes[Rust]->GetElementType() == NodeContainerType::nct_reg_node)
            {
                dynamic_cast<TcWebNodeElement*>(childNodes[Rust].Get())->GetWebNode()->PreEstablishTable(cols, rows);
                //if (childNodes[Rust]->webNode->columnSizes.Size())
                //    childNodes[Rust]->webNode->AddColumn(Rust + 1);
            }
        }

        //if (!cols.Size())
        //    cols.push_back(0);
        break;
        
    default:
        cols.push_back(0);
        return;
    }
}

void TcWebNode::SetColumnsAndRows(TDataArray<UINT>& cols, TDataArray<UINT>& rows)
{
    this->columnSizes = cols;
    this->rowSizes = rows;

    for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
    {
        switch (childNodes[Rust]->GetElementType())
        {
        case NodeContainerType::nct_reg_node:
        case NodeContainerType::nct_tex_node:
            dynamic_cast<TcWebNodeElement*>(childNodes[Rust].Get())->GetWebNode()->SetColumnsAndRows(cols, rows);
        }
    }
}

void TcWebNode::SetColumnProps(TDataArray<TDataMap<TString>>& colProps, TDataArray<UINT>& colSizes)
{
    switch (internalDisplay)
    {
    case TcWebNodeDisplayInternal::wndi_row_group:
        for (UINT Rust = 0; Rust < childNodes.Size(); Rust++)
        {
            if (childNodes[Rust]->GetElementType() != NodeContainerType::nct_reg_node)
                continue;
            auto webNode = dynamic_cast<TcWebNodeElement*>(childNodes[Rust].Get())->GetWebNode();
            webNode->SetColumnProps(colProps, colSizes);
        }
        break;
    case TcWebNodeDisplayInternal::wndi_row:
        columnSizes = colSizes;
        for (UINT Rust = 0; Rust < childNodes.Size() && Rust < colProps.Size(); Rust++)
        {
            if (childNodes[Rust]->GetElementType() != NodeContainerType::nct_reg_node)
                continue;
            auto webNode = dynamic_cast<TcWebNodeElement*>(childNodes[Rust].Get())->GetWebNode();
            webNode->CompileProperties(colProps[Rust]);
        }
    }
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
            sizeVal = sizeVal * 2.54f / dpif;
            break;
        case WebSizeUnit::wsu_em:
            sizeVal = sizeVal * 16.0f;
            break;
        case WebSizeUnit::wsu_in:
            sizeVal = sizeVal / dpif;
            break;
        case WebSizeUnit::wsu_mm:
            sizeVal = sizeVal * 0.254f / dpif;
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
            sizeVal = sizeVal * 2.54f / dpif;
            break;
        case WebSizeUnit::wsu_em:

            break;
        case WebSizeUnit::wsu_in:
            sizeVal = sizeVal / dpif;
            break;
        case WebSizeUnit::wsu_mm:
            sizeVal = sizeVal * 0.254f / dpif;
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
                vAtts[Rust] = vAtts[Rust] * 2.54f / dpif;
                break;
            case WebSizeUnit::wsu_em:

                break;
            case WebSizeUnit::wsu_in:
                vAtts[Rust] = vAtts[Rust] / dpif;
                break;
            case WebSizeUnit::wsu_mm:
                vAtts[Rust] = vAtts[Rust] * 0.254f / dpif;
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

TcWebNode::TcTableNodeElement::TcTableNodeElement(TrecSubPointer<TPage, TcWebNode> node)
{
    this->node = node;
}

TcWebNode::NodeContainerType TcWebNode::TcTableNodeElement::GetElementType()
{
    return NodeContainerType::net_tab_node;
}

D2D1_RECT_F TcWebNode::TcTableNodeElement::GetLocation()
{
    return node->area;
}

TrecSubPointer<TPage, TcWebNode> TcWebNode::TcTableNodeElement::GetWebNode()
{
    return node;
}

D2D1_RECT_F TcWebNode::TcTableNodeElement::GetOuterMargin()
{
    return node->outerMargin;
}

void TcWebNode::TcTableNodeElement::PrepTable()
{
    this->colProps.RemoveAll();

    TrecSubPointer<TPage, TcWebNode> colNode;
    for (UINT Rust = 0; Rust < node->childNodes.Size(); Rust++)
    {
        auto curNode = node->childNodes[Rust];
        if (curNode->GetElementType() != NodeContainerType::nct_reg_node)
            continue;
        auto assessNode = dynamic_cast<TcWebNodeElement*>(curNode.Get())->GetWebNode();
        if (assessNode->internalDisplay == TcWebNodeDisplayInternal::wndi_column_group)
        {
            colNode = assessNode;
            break;
        }
    }
    TString prop;
    if (colNode.Get())
    {
        for (UINT Rust = 0; Rust < colNode->childNodes.Size(); Rust++)
        {
            auto curNode = colNode->childNodes[Rust];
            if (curNode->GetElementType() != NodeContainerType::nct_reg_node)
                continue;
            auto assessNode = dynamic_cast<TcWebNodeElement*>(curNode.Get())->GetWebNode();
            if (assessNode->internalDisplay != TcWebNodeDisplayInternal::wndi_column) continue;

            auto tempAttributes = assessNode->attributes;
            UINT span = 1;
            if (tempAttributes.retrieveEntry(L"span", prop))
                TString::ConvertStringToUint(prop, span);

            // span and display are not attributes we need to apply to elements in the table itself
            tempAttributes.removeEntry(L"span", prop);
            tempAttributes.removeEntry(L"display", prop);
            for(UINT C =0; C < span; C++)
                colProps.push_back(tempAttributes);

        }
    }

    node->PreEstablishTable(cols, rows);
}

void TcWebNode::TcTableNodeElement::CreateTable(D2D1_RECT_F location, TrecPointer<TWindowEngine> d3dEngine, HWND window, TDataArray<FileRequest>& req, bool requestFile)
{
    // To-Do: Figure out the actual sizes of the columns. 

    // For the time being, assume that all columns are the same
    float totalWidth = location.right - location.left;
    for (UINT Rust = 0; Rust < cols.Size(); Rust++)
    {
        cols[Rust] = totalWidth / cols.Size();
    }

    // Now Set the columns and Rows of child nodes
    node->SetColumnsAndRows(this->cols, this->rows);

    for (UINT Rust = 0; Rust < node->childNodes.Size(); Rust++)
    {
        if (node->childNodes[Rust]->GetElementType() == NodeContainerType::nct_reg_node)
            dynamic_cast<TcWebNodeElement*>(node->childNodes[Rust].Get())->GetWebNode()->SetColumnProps(this->colProps, cols);

    }

    node->CreateWebNode(location, d3dEngine, window, req, true, requestFile);
    node->ShrinkHeight();
}

bool TcWebNode::TcTableNodeElement::GetVariable(const TString& name, TrecPointer<TVariable>& var)
{
    return false;
}

bool TcWebNode::TcTableNodeElement::SetVariable(const TString& name, TrecPointer<TVariable> var)
{
    return false;
}

TcWebNode::FileRequest::FileRequest()
{
}

TcWebNode::FileRequest::FileRequest(const FileRequest& copy)
{
    this->fileRequest.Set(copy.fileRequest);
    this->requesterNode = copy.requesterNode;
}

TcContentData::TcContentData()
{
    repeatX = repeatY = true;
    locked = false;
    this->size = { 0.0f,0.0f };
}

TcContentData::TcContentData(const TcContentData& copy)
{
    this->brush = copy.brush;
    this->bitmap = copy.bitmap;
    this->file = copy.file;
    this->locked = copy.locked;
    this->repeatX = copy.repeatX;
    this->repeatY = copy.repeatY;
    this->size = copy.size;
}

void TcContentData::CompileAttributes(TString& atts, TrecPointer<DrawingBoard> board)
{
    auto multiAtts = atts.split(L' ', 3);
    for (UINT Rust = 0; Rust < multiAtts->Size(); Rust++)
    {
        TString curAtt(multiAtts->at(Rust));
        (CompileImage(curAtt) ||
            CompileColor(curAtt, board) ||
            CompileAttachment(curAtt) ||
            CompilePosition(curAtt) ||
            CompileRepeat(curAtt));
           
    }
}

bool TcContentData::CompileImage(TString& atts)
{
    TString att(atts);
    if(!att.StartsWith(L"url", true))
        return false;
    att.Delete(0, 3);
    att.Trim();
    if (att.StartsWith(L'(') && att.EndsWith(L')'))
        att.Set(att.SubString(1, att.GetSize() - 1));
    else return false;

    att.Trim();
    if ((att.StartsWith(L'\'') && att.EndsWith(L'\'')) ||
        (att.StartsWith(L'\"') && att.EndsWith(L'\"')))
    {
        att.Set(att.SubString(1, att.GetSize() - 1));
        this->file.Set(att.GetTrim());
        return true;
    }
    return false;
}

bool TcContentData::CompileColor(TString& atts, TrecPointer<DrawingBoard> board)
{
    bool ret = false;
    TColor color = TColor::GetColorFromString(atts, ret);
    if (ret)
    {
        brush = board->GetBrush(color);
    }
    return ret;
}

bool TcContentData::CompileRepeat(TString& atts)
{
    if (!atts.CompareNoCase(L"no-repeat"))
        repeatX = repeatY = false;
    else if (!atts.CompareNoCase(L"repeat-x"))
        repeatY = false;
    else if (!atts.CompareNoCase(L"repeat-y"))
        repeatX = false;
    else return false;
    return true;
}

bool TcContentData::CompilePosition(TString& atts)
{
    return false;
}

bool TcContentData::CompileAttachment(TString& atts)
{
    return false;
}
