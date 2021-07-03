#include "pch.h"
#include "HtmlLink.h"

HtmlLink::HtmlLink(TrecPointer<TEnvironment> env)
{
    this->env = env;
}

TString HtmlLink::ProcessHtml(TrecPointer<TFile> file, const TString& data)
{
    TString aData(data.GetTrim());

    if (aData.EndsWith(L">"))
        aData.Delete(aData.GetSize() - 1);

    TrecPointer<TDataArray<TString>> fields = aData.split(L" =\t\n", 3);

    if (fields->Size() % 2)
        return L"Link Attribute expected to have an even number of fields during parsing! between whitespace and '='";


    for (UINT Rust = 0; Rust < fields->Size(); Rust += 2)
    {
        TString att(fields->at(Rust).GetTrim());
        TString val(fields->at(Rust + 1).GetTrim());

        if (val.GetSize() < 3)
            return L"No Active Value can be properly Parsed for Link Block";

        if (val[0] != val[val.GetSize() - 1])
            return L"Value needs to start and end with the same quotation types";

        if (val[0] != L'\'' && val[0] != L'\"')
            return L"Value needs to start and end with quotation marks!";

        val.Set(val.SubString(1, val.GetSize() - 1));

        if (!att.Compare(L"as"))
            as.Set(val);
        else if (!att.Compare(L"crossorigin"))
            crossorigin.Set(val);
        else if (!att.Compare(L"href"))
            href.Set(val);
        else if (!att.Compare(L"integrity"))
            integrity.Set(val);
        else if (!att.Compare(L"media"))
            media.Set(val);
        else if (!att.Compare(L"rel"))
            rel.Set(val);
    }

    return TString();
}

TString HtmlLink::getVariableValueStr(const TString& varName)
{
    TString ret;
    if (!varName.Compare(L"href"))
        ret.Set(href);
    else if (!varName.Compare(L"rel"))
        ret.Set(rel);
    else if (!varName.Compare(L"crossorigin"))
        ret.Set(crossorigin);
    else if (!varName.Compare(L"integrity"))
        ret.Set(integrity);
    else if (!varName.Compare(L"type"))
        ret.Set(type);
    else if (!varName.Compare(L"media"))
        ret.Set(media);
    else if (!varName.Compare(L"as"))
        ret.Set(as);

    return ret;
}
