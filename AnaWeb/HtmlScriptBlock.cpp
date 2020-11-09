#include "pch.h"
#include "HtmlScriptBlock.h"

HtmlScriptBlock::HtmlScriptBlock(TrecPointer<TEnvironment> env)
{
	this->env = env;
}

TString HtmlScriptBlock::ProcessHtml(TrecPointer<TFile> file, const TString& data)
{
	WCHAR quote = L'\0';

	bool attMode = true;
	TString attribute, value;
	UINT backslashCount = 0;
	for (UINT Rust = 7; Rust < data.GetSize(); Rust++)
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

	TString srcEntry;

	if (data.EndsWith(L"/>") && !attributes.retrieveEntry(L"src", srcEntry))
		return L"Error! Script block required either s src attribute or a script in it's body!";

	if (srcEntry.GetSize())
	{
		// To-Do: Make Web Request
	}
	else
	{
		int closeTag = -1, scriptTag = -1, closeTag2 = -1;

		UINT startSearch = 0;

		TString readable;

		while (closeTag == -1 && scriptTag == -1)
		{
			if (!file->ReadString(readable))
				return L"Reached end of file before finishing Script block!";
			if (srcEntry.GetSize())
				srcEntry.AppendChar(L'\n');
			closeTag = readable.FindOutOfQuotes(L"</", startSearch);
			scriptTag = readable.FindOutOfQuotes(L"script", startSearch);
			closeTag2 = readable.FindLast(L'>');

			if (scriptTag > closeTag && closeTag2 > scriptTag && closeTag != -1)
			{
				if (readable.SubString(0, closeTag).CountFinds(L'`') % 2 == 0)
				{
					// We're Done. Reset the file position and prepare to return
					UINT sub = readable.GetSize() - closeTag2;
					file->Seek(file->GetPosition() - sub, 0);
					srcEntry.Append(readable.SubString(0, closeTag));
					break;
				}
				else
				{
					// We encountered those symbols within a multi-line string
					startSearch = closeTag2;
				}
			}

			srcEntry.Append(readable);
		}

		TString language;
		if (!attributes.retrieveEntry(L"type", language))
			language.Set(L"javascript");

		TString fileLoc(env->GetRootDirectory()->GetPath() + L"\\html.");

		// Most Browsers support JavaScript, so check for JavaScript first
		if (language.EndsWith(L"javascript", true))
			fileLoc.Append(L"js");
		// Unlike most browsers, we plan on supporting multiple scripting languages, including Anascript, so check this out
		else if (language.EndsWith(L"anascript", true))
			fileLoc.Append(L"ascrpt");
		// To do: Else

		TFile scriptFile(fileLoc, TFile::t_file_open_always | TFile::t_file_write);

		if (scriptFile.IsOpen())
		{
			scriptFile.Seek(scriptFile.GetLength(), 0);

			scriptFile.WriteString(srcEntry);
			scriptFile.Close();
		}
	}

	return TString();
}
