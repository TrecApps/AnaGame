#include "pch.h"
#include "HtmlHeader.h"

HtmlHeader::HtmlHeader(TrecPointer<TEnvironment> env)
{
    this->env = env;
}

TString HtmlHeader::ProcessHtml(TrecPointer<TFile> file, const TString& data)
{
    int closeBracket = data.FindOutOfQuotes(L'>', data.Find(L"head"));

    bool continueAtts = (closeBracket == -1);

    UINT attsStart = data.Find(L"head") + 5;

    WCHAR quote = L'\0';

    bool attMode = true;
    TString attribute, value;
    UINT backslashCount = 0;
    for (UINT Rust = attsStart; Rust < data.GetSize(); Rust++)
    {
		WCHAR ch = data.GetAt(Rust);
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

		if (ch != L'\\') backslashCount = 0;
    }
	TString headData;

	if (continueAtts)
	{
		quote = L'\0';

		attMode = true;
		attribute.Empty(), value.Empty();
		backslashCount = 0;
		file->ReadString(headData, L">", 7);

		for (UINT Rust = 0; Rust < headData.GetSize(); Rust++)
		{
			WCHAR ch = headData.GetAt(Rust);
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

			if (ch != L'\\') backslashCount = 0;
		}
	}

	if(data.EndsWith(L"/>"))
		return TString(L"Premature End to header!");

	while (file->ReadString(headData, L">", 7))
	{
		headData.Trim();
		if (!headData.EndsWith(L">")) return L"Premature End to header!";

		if (!headData.StartsWith(L"<")) return L"Malformed HTML, '<' Expected within head block!";
		headData.Set(headData.SubString(1));
		headData.Trim();

		if (headData.StartsWith(L"meta", true, true))
		{
			headData.Delete(0, 5);
			headData.Trim();
			TrecPointer<TDataArray<TString>> tokens = headData.split(L" \t\n=", 3);

			if (tokens->Size() == 2)
			{
				meta.addEntry(tokens->at(0), tokens->at(1));
			}
			else if (tokens->Size() == 4)
			{
				if (!tokens->at(2).CompareNoCase(L"content"))
				{
					// The presumed case, but check to make sure the html writer put content next
					meta.addEntry(tokens->at(1), tokens->at(3));
				}
				else
				{
					// Turns out, the user placed the contents tag first
					meta.addEntry(tokens->at(3), tokens->at(1));
				}
			}
		}
		else if (headData.StartsWith(L"title", true, false))
		{
			if (title.GetSize()) return L"Cannot have more than 1 title block!";
			if (!file->ReadString(headData, L">", 7)) return L"Error Parsing title tag in Header block";

			int endStyle = headData.FindLast(L"</");
			if (endStyle == -1) return L"Error Parsing styleTage in title block, expected '</' token";
			title.Set(headData.SubString(0, endStyle).GetTrim());
		}
		else if (headData.StartsWith(L"style", true, true))
		{
			if(!file->ReadString(headData, L">", 7)) return L"Error Parsing style tag in Header block";

			int endStyle = headData.FindLast(L"</");
			if (endStyle == -1) return L"Error Parsing styleTage in Header block, expected '</' token";
			css.Append(headData.SubString(0, endStyle));

		}
		else if (headData.StartsWith(L"link", true, true))
		{
			headData.Delete(0, 4);
			TrecPointer<HtmlLink> link = TrecPointerKey::GetNewTrecPointer<HtmlLink>(env);

			TString res(link->ProcessHtml(file, headData));
			if (res.GetSize()) return res;
			links.push_back(link);
		}
		else if (headData.StartsWith(L"script", true, true))
		{
			TrecPointer<HtmlScriptBlock> script = TrecPointerKey::GetNewTrecPointer<HtmlScriptBlock>(env);
			TString res(script->ProcessHtml(file, headData));
			if (res.GetSize())
				return res;
			scripts.push_back(script);
		}
		else if (headData.StartsWith(L"base", true, true))
		{
			if (base.isSet)
				return L"Only one base Element can be set!";

			headData.Delete(0, 5);
			headData.Trim();
			TrecPointer<TDataArray<TString>> tokens = headData.split(L" \t\n=", 3);

			if (tokens->Size() >= 2)
			{
				getBaseElement(tokens->at(0), tokens->at(1));
			}
			if (tokens->Size() >= 4)
			{
				getBaseElement(tokens->at(2), tokens->at(3));
			}
			base.isSet = true;
		}
		else if (headData.StartsWith(L"/") && headData.Find(L"head") != -1)
		{
			break;
		}
	}



    return TString();
}

void HtmlHeader::getBaseElement(const TString& att, const TString& val)
{
	if (!att.Compare(L"href"))
		base.url.Set(val);
	else if (!att.Compare(L"target"))
		base.target.Set(val);
}

HtmlHeaderBase::HtmlHeaderBase()
{
	isSet = false;
}
