#include "TWebForm.h"

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
