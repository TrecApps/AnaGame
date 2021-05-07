#include "TWebForm.h"
#include <TCheckBox.h>
#include <TRadioButton.h>

TWebForm::TWebForm(TrecPointer<DrawingBoard> board) : TWebNode(board)
{
    doGet = true; // GET is default
}

UINT TWebForm::CreateWebNode(D2D1_RECT_F location, TrecPointer<TWindowEngine> d3dEngine, HWND window)
{
    // For now, just call parent
    return TWebNode::CreateWebNode(location, d3dEngine, window);
}

void TWebForm::CompileProperties(TDataMap<TString>& atts)
{
    TString valpoint;
    if (atts.retrieveEntry(L"method", valpoint))
    {
        if (!valpoint.CompareNoCase(L"post"))
            doGet = false;
    }

    if (atts.retrieveEntry(L"action", valpoint))
    {
        action.Set(valpoint);
    }
    TWebNode::CompileProperties(atts);
}


TWebInput::TWebInput(TrecPointer<DrawingBoard> board) : TWebNode(board)
{
    inputType = web_input_type::wit_text; // Text is the default
}

UINT TWebInput::CreateWebNode(D2D1_RECT_F location, TrecPointer<TWindowEngine> d3dEngine, HWND window)
{
    childNodes.RemoveAll();

    TrecPointer<TControl> control;

    switch (inputType)
    {
        // Type that just require a regular TControl
    case web_input_type::wit_button:
    case web_input_type::wit_reset:
    case web_input_type::wit_submit:
    case web_input_type::wit_color:
    case web_input_type::wit_date:
    case web_input_type::wit_datetime_local:
    case web_input_type::wit_file:
    case web_input_type::wit_image:
        control = TrecPointerKey::GetNewSelfTrecPointer<TControl>(board, TrecPointer<TArray<styleTable>>());
        break;
        // Requires a Checkbox
    case web_input_type::wit_checkbox:
        control = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TCheckBox>(board, TrecPointer<TArray<styleTable>>());
        break;
        // Requires Text Controls
    case web_input_type::wit_email:
    case web_input_type::wit_number:
    case web_input_type::wit_password:
    case web_input_type::wit_tel:
    case web_input_type::wit_text:
        control = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TTextField>(board, TrecPointer<TArray<styleTable>>());

        if (inputType == web_input_type::wit_number)
        {
            control->addAttribute(L"|IsNumberControl", TrecPointerKey::GetNewTrecPointer<TString>(L"true"));
        }
        else if (inputType == web_input_type::wit_password)
        {
            control->addAttribute(L"|IsPassword", TrecPointerKey::GetNewTrecPointer<TString>(L"true"));
        }

        break;
        // Requires Radio Button
    case web_input_type::wit_radio:
        control = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TRadioButton>(board, TrecPointer<TArray<styleTable>>());
        if (name.GetSize())
            control->addAttribute(L"|RadioClass", TrecPointerKey::GetNewTrecPointer<TString>(name));
    }
    if (control.Get())
    {
        control->addAttribute(L"|caption", TrecPointerKey::GetNewTrecPointer<TString>(value));


        childNodes.push_back(TrecPointerKey::GetNewTrecPointer<TWebNodeContainer>(control));
    }
    return TWebNode::CreateWebNode(location, d3dEngine, window);
}

void TWebInput::CompileProperties(TDataMap<TString>& atts)
{
    TString val;
    if (atts.retrieveEntry(L"type", val))
    {
        if (val.CompareNoCase(L"button"))
            inputType = web_input_type::wit_button;
        if (val.CompareNoCase(L"checkbox"))
            inputType = web_input_type::wit_checkbox;
        if (val.CompareNoCase(L"color"))
            inputType = web_input_type::wit_color;
        if (val.CompareNoCase(L"date"))
            inputType = web_input_type::wit_date;
        if (val.CompareNoCase(L"datetime-local"))
            inputType = web_input_type::wit_datetime_local;
        if (val.CompareNoCase(L"email"))
            inputType = web_input_type::wit_email;
        if (val.CompareNoCase(L"file"))
            inputType = web_input_type::wit_file;
        if (val.CompareNoCase(L"hidden"))
            inputType = web_input_type::wit_hidden;
        if (val.CompareNoCase(L"image"))
            inputType = web_input_type::wit_image;
        if (val.CompareNoCase(L"month"))
            inputType = web_input_type::wit_month;
        if (val.CompareNoCase(L"number"))
            inputType = web_input_type::wit_number;
        if (val.CompareNoCase(L"password"))
            inputType = web_input_type::wit_password;
        if (val.CompareNoCase(L"radio"))
            inputType = web_input_type::wit_radio;
        if (val.CompareNoCase(L"range"))
            inputType = web_input_type::wit_range;
        if (val.CompareNoCase(L"reset"))
            inputType = web_input_type::wit_reset;
        if (val.CompareNoCase(L"search"))
            inputType = web_input_type::wit_search;
        if (val.CompareNoCase(L"submit"))
            inputType = web_input_type::wit_submit;
        if (val.CompareNoCase(L"tel"))
            inputType = web_input_type::wit_tel;
        if (val.CompareNoCase(L"text"))
            inputType = web_input_type::wit_text;
        if (val.CompareNoCase(L"time"))
            inputType = web_input_type::wit_time;
        if (val.CompareNoCase(L"url"))
            inputType = web_input_type::wit_url;
        if (val.CompareNoCase(L"week"))
            inputType = web_input_type::wit_week;
    }

    TWebNode::CompileProperties(atts);
}
