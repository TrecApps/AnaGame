#include "pch.h"
#include "TLanguage.h"
#include <stack>
#include <DirectoryInterface.h>

static TString languageFolder = GetDirectoryWithSlash(CentralDirectories::cd_Executable) + TString(L"Languages");
TString tStringSemiColon(L";");

TrecPointer<TLanguage> TLanguage::getLanguage(TString& langName)
{
	TString folderTarget = languageFolder + langName + L"\\";

	TString langDef = folderTarget + TString(L"language.properties");
	TFile langFile;


	if (!langFile.Open(langDef, TFile::t_file_read))
	{
		return TrecPointer<TLanguage>();
	}



	TDataArray<TString> lines;
	TString lineString;
	while (langFile.ReadString(lineString))
	{
		lines.push_back(lineString);
	}

	TMap<TString> maps;
	for (UINT Rust = 0; Rust < lines.Size(); Rust++)
	{
		auto sections = lines[Rust].split(L":");
		if (!sections.Get() || sections->Size() < 2)
			continue;

		TString value = sections->at(1);
		for (UINT c = 2; c < sections->Size(); c++)
		{
			value.Append(L":");
			value.Append(sections->at(c));
		}

		if (!value.GetSize())
			continue;

		if (value.GetAt(0) == L'\"')
			value.Set(value.SubString(1));

		if (!value.GetSize())
			continue;

		if (value.GetAt(value.GetSize() - 1) == L'\"')
			value.Set(value.SubString(0, value.GetSize() - 1));


		maps.addEntry(sections->at(0), TrecPointerKey::GetNewTrecPointer<TString>(value));
	}


	return getLanguage(maps, TrecPointerKey::GetNewTrecPointer<TLanguage>(langName, TString()));
}

TrecPointer<TLanguage> TLanguage::getLanguage(TString& langName, TString& langVersion)
{
	TString folderTarget = languageFolder + langName + L"\\" + langVersion + L"\\";

	TString langDef = folderTarget + TString(L"language.properties");
	TFile langFile;


	if (!langFile.Open(langDef, TFile::t_file_read))
	{
		return TrecPointer<TLanguage>();
	}



	TDataArray<TString> lines;
	TString lineString;
	while (langFile.ReadString(lineString))
	{
		lines.push_back(lineString);
	}

	TMap<TString> maps;
	for (UINT Rust = 0; Rust < lines.Size(); Rust++)
	{
		auto sections = lines[Rust].split(L":");
		if (!sections.Get() || sections->Size() < 2)
			continue;

		TString value = sections->at(1);
		for (UINT c = 2; c < sections->Size(); c++)
		{
			value.Append(L":");
			value.Append(sections->at(c));
		}

		if (!value.GetSize())
			continue;

		if (value.GetAt(0) == L'\"')
			value.Set(value.SubString(1));

		if (!value.GetSize())
			continue;

		if (value.GetAt(value.GetSize() - 1) == L'\"')
			value.Set(value.SubString(0, value.GetSize() - 1));


		maps.addEntry(sections->at(0), TrecPointerKey::GetNewTrecPointer<TString>(value));
	}


	return getLanguage(maps, TrecPointerKey::GetNewTrecPointer<TLanguage>(langName, langVersion));
}

UINT TLanguage::PreProcessFile(TrecPointer<TFile>& sourceFile)
{
	TString nextFile;
	if (!RunCommentFilter(sourceFile, nextFile))
		return 1;

	// Comment Filter reports no issues, Proceed to run the Block filter pipeline so that blocks
	// Consist of Curly Brackets rather than indentatins (python) or key words
	sourceFile->Close();
	if(!sourceFile->Open(nextFile, TFile::t_file_open_always | TFile::t_file_read)) return 2;

	if (!RunBlockFilter(sourceFile, nextFile))
		return 3;

	// If Block Filter passes, proceed to the next step

	return 0;
}

const TMap<TString>& TLanguage::GetPrimitiveTypes()
{
	return primitiveTypeMap;
}

TrecPointer<TLanguage> TLanguage::getLanguage(TMap<TString>& langProps, TrecPointer<TLanguage> lang)
{
	// first deduce how single line comments are recognized
	TrecPointer<TString> val = langProps.retrieveEntry(TString(L"Single-Line Comment Start"));
	if (val.Get())
	{
		lang->singleComment = *val->split(tStringSemiColon).Get();
	}

	val = langProps.retrieveEntry(TString(L"Multi-line Comment Start"));
	if (val.Get())
	{
		lang->startComment = *val->split(tStringSemiColon).Get();
	}

	val = langProps.retrieveEntry(TString(L"Multi-line Comment End"));
	if (val.Get())
	{
		lang->endComment = *val->split(tStringSemiColon).Get();
	}

	val = langProps.retrieveEntry(TString(L"Single-Line String Tokens"));
	if (val.Get())
	{
		lang->string = *val->split(tStringSemiColon).Get();
	}

	val = langProps.retrieveEntry(TString(L"Multi-Line String Tokens"));
	if (val.Get())
	{
		lang->multiLineString = *val->split(tStringSemiColon).Get();
	}

	val = langProps.retrieveEntry(TString(L"Default String Encoding"));
	lang->defaultencoding = (val.Get() && !val->Compare(L"Ascii")) ? LanguageDefaultStringEncoding::ldse_acsii : LanguageDefaultStringEncoding::ldse_unicode;

	if(lang->defaultencoding == LanguageDefaultStringEncoding::ldse_acsii)
		val = langProps.retrieveEntry(TString(L"Unicode Marker"));
	else
		val = langProps.retrieveEntry(TString(L"ACSII Marker"));

	if (val.Get())
		lang->switchMarker.Set(val.Get());

	// Aftre String Encoding
	val = langProps.retrieveEntry(TString(L"Block Type"));
	if (!val.Get())
		lang->blockType = LanguageBlockType::lbt_curly;
	else
	{
		val->Trim();
		if (!val->Compare(L"Indent"))
			lang->blockType = LanguageBlockType::lbt_indent;
		else if (!val->Compare(L"Tokens"))
			lang->blockType = LanguageBlockType::lbt_tokens;
		else
			lang->blockType = LanguageBlockType::lbt_curly;
	}

	val = langProps.retrieveEntry(TString(L"External File Access"));
	lang->externalFileName = val.Get() && !val->Compare(L"Filename");

	val = langProps.retrieveEntry(TString(L"Require Module Header"));
	
	if (val.Get())
	{
		auto reqModuleHeaders = val->split(tStringSemiColon);
		for (UINT Rust = 0; Rust < reqModuleHeaders->Size(); Rust++)
		{
			if (!reqModuleHeaders->at(Rust).Compare(L"Yes"))
				lang->requireModHeader = RequireModuleHeader::rmh_yes;
			else if (!reqModuleHeaders->at(Rust).Compare(L"No"))
				lang->requireModHeader = RequireModuleHeader::rmh_yes;
			else if (!reqModuleHeaders->at(Rust).Compare(L"NoinDefault"))
				lang->requireModHeader = RequireModuleHeader::rmh_yes;
			else if (!reqModuleHeaders->at(Rust).Compare(L"First"))
				lang->moduleHeaderFirst = true;
			else if (!reqModuleHeaders->at(Rust).Compare(L"Block"))
				lang->supportModuleBlock = true;
			else if (!reqModuleHeaders->at(Rust).Compare(L"UseForPublic"))
				lang->useForPublic = true;
			
		}
	}

	val = langProps.retrieveEntry(TString(L"Inheritance"));
	if (!val.Get())
		lang->inheritenceModel = LanguageInheritence::li_not_supported;
	else
	{
		val->Trim();
		if (!val->Compare(L"Multiple"))
			lang->inheritenceModel = LanguageInheritence::li_mulitple;
		else if (!val->Compare(L"MultipleInterface"))
			lang->inheritenceModel = LanguageInheritence::li_multiple_interface;
		else if (!val->Compare(L"Single"))
			lang->inheritenceModel = LanguageInheritence::li_single;
		else
			lang->inheritenceModel = LanguageInheritence::li_not_supported;
	}

	val = langProps.retrieveEntry(TString(L"Enum Implementation"));
	if (!val.Get())
		lang->enumModel = EnumImplementation::ei_not_supported;
	else
	{
		val->Trim();
		if (!val->Compare(L"Primitive"))
			lang->enumModel = EnumImplementation::ei_primitive;
		else if (!val->Compare(L"Class"))
			lang->enumModel = EnumImplementation::ei_class;
		else if (!val->Compare(L"LabelledUnion "))
			lang->enumModel = EnumImplementation::ei_labelledUnion;
		else
			lang->enumModel = EnumImplementation::ei_not_supported;
	}

	val = langProps.retrieveEntry(TString(L"Method Implementation"));
	if (val.Get())
	{
		auto reqModuleHeaders = val->split(tStringSemiColon);
		for (UINT Rust = 0; Rust < reqModuleHeaders->Size(); Rust++)
		{
			if (!reqModuleHeaders->at(Rust).Compare(L"Inline"))
				lang->methodImplementation += 0x01;
			else if (!reqModuleHeaders->at(Rust).Compare(L"Block"))
				lang->methodImplementation += 0x02;
			else if (!reqModuleHeaders->at(Rust).Compare(L"DefaultVirtual"))
				lang->methodImplementation += 0x04;
			else if (!reqModuleHeaders->at(Rust).Compare(L"RequireSelfParam"))
				lang->methodImplementation += 0x08;
		}
	}

	// Go through Primitive types supported by the language and get their aliases for the Environment to process

	// Signed Integers
	val = langProps.retrieveEntry(TString(L"s8"));
	if (val.Get())
		lang->primitiveTypeMap.addEntry(TString(L"s8"), val);

	val = langProps.retrieveEntry(TString(L"s16"));
	if (val.Get())
		lang->primitiveTypeMap.addEntry(TString(L"s16"), val);

	val = langProps.retrieveEntry(TString(L"s32"));
	if (val.Get())
		lang->primitiveTypeMap.addEntry(TString(L"s32"), val);

	val = langProps.retrieveEntry(TString(L"s64"));
	if (val.Get())
		lang->primitiveTypeMap.addEntry(TString(L"s64"), val);

	// Unsigned Integers
	val = langProps.retrieveEntry(TString(L"u8"));
	if (val.Get())
		lang->primitiveTypeMap.addEntry(TString(L"u8"), val);

	val = langProps.retrieveEntry(TString(L"u16"));
	if (val.Get())
		lang->primitiveTypeMap.addEntry(TString(L"u16"), val);

	val = langProps.retrieveEntry(TString(L"u32"));
	if (val.Get())
		lang->primitiveTypeMap.addEntry(TString(L"u32"), val);

	val = langProps.retrieveEntry(TString(L"u64"));
	if (val.Get())
		lang->primitiveTypeMap.addEntry(TString(L"u64"), val);

	// Floating Point types
	val = langProps.retrieveEntry(TString(L"f32"));
	if (val.Get())
		lang->primitiveTypeMap.addEntry(TString(L"f32"), val);

	val = langProps.retrieveEntry(TString(L"f64"));
	if (val.Get())
		lang->primitiveTypeMap.addEntry(TString(L"f64"), val);

	val = langProps.retrieveEntry(TString(L"f96"));
	if (val.Get())
		lang->primitiveTypeMap.addEntry(TString(L"f96"), val);

	// char types
	val = langProps.retrieveEntry(TString(L"c8"));
	if (val.Get())
		lang->primitiveTypeMap.addEntry(TString(L"c8"), val);

	val = langProps.retrieveEntry(TString(L"c16"));
	if (val.Get())
		lang->primitiveTypeMap.addEntry(TString(L"c16"), val);

	val = langProps.retrieveEntry(TString(L"c32"));
	if (val.Get())
		lang->primitiveTypeMap.addEntry(TString(L"c32"), val);

	// Boolean
	val = langProps.retrieveEntry(TString(L"b"));
	if (val.Get())
		lang->primitiveTypeMap.addEntry(TString(L"b"), val);

	return TrecPointer<TLanguage>();
}

TLanguage::TLanguage(TString& name, TString& version)
{
	blockStartersAtBeginning = false;
	this->language.Set(name);
	this->version.Set(version);
	moduleHeaderFirst = supportModuleBlock = useForPublic = false;
	methodImplementation = 0;
}

TLanguage::~TLanguage()
{
}

bool TLanguage::RunCommentFilter(TrecPointer<TFile> file, TString& newFileName)
{
	if (!file.Get() || !file->IsOpen())
		return false;

	TString sourceName = file->GetFileDirectory();
	newFileName = TString(L"_NO_COM_") + file->GetFileName();


	TrecPointer<TFile> sourceFile = TrecPointerKey::GetNewTrecPointer<TFile>();

	if (!sourceFile->Open(sourceName + newFileName, TFile::t_file_write | TFile::t_file_create_always))
	{
		return false;
	}
	TString inBuffer;

	TString outbuffer;

	CodeMode sourceMode = CodeMode::cm_reg;

	UINT readData = 0, strIndex = 0;
	int  nextSinCom = -1, nextLine = -1, nextMulCom = -1, endMulCom = -1;
	//DoubIndex nextSinStr = { -1, 0 }, nextMulStr = { -1,0 };
	int endString = -1;
	DoubIndex sinCom, newL, multiCom, multiComE, sinStr, mulStr;

	while (readData = file->ReadString(inBuffer, (UINT)100))
	{
		UpdateDoubIndex(inBuffer, 0, sinCom, newL, multiCom, multiComE, sinStr, mulStr);
		outbuffer.Set(L"");
		for (UINT c = 0; c < inBuffer.GetSize(); c++)
		{

			switch (sourceMode)
			{
			case CodeMode::cm_reg:
				if (c == sinCom.strInd)
				{
					sourceMode = CodeMode::cm_sinCom;
				}
				else if (c == multiCom.strInd)
				{
					sourceMode = CodeMode::cm_mulCom;
				}
				else if (c == sinStr.strInd)
				{
					sourceMode = CodeMode::cm_sinStr;
					outbuffer += sinStr.stringQ;
					c += sinStr.stringQ.GetSize() - 1;
					sinStr = getNextSingleString(inBuffer, c + 1);
				}
				else if (c == mulStr.strInd)
				{
					sourceMode = CodeMode::cm_mulStr;
					outbuffer += mulStr.stringQ;
					c += mulStr.stringQ.GetSize() - 1;
					mulStr = getNextMultiString(inBuffer, c + 1);
				}
				else
					outbuffer += inBuffer[c];
				break;
			case CodeMode::cm_sinCom:
				if (newL.strInd == -1 || newL.strInd < c)
					newL = getNextNewline(inBuffer, c);
				if (newL.strInd != -1)
				{
					outbuffer += L'\n';
					c = newL.strInd;
					sourceMode = CodeMode::cm_reg;
					UpdateDoubIndex(inBuffer, c + 1, sinCom, newL, multiCom, multiComE, sinStr, mulStr);
				}
				break;
			case CodeMode::cm_mulCom:
				newL = getNextNewline(inBuffer, c);
				while (newL.strInd != -1 && (multiComE.strInd == -1 || newL.strInd < multiComE.strInd))
				{
					outbuffer += L'\n';
					c = newL.strInd;
					newL = getNextNewline(inBuffer, c + 1);
				}
				if (multiComE.strInd != -1)
				{
					c = multiComE.strInd + multiComE.stringQ.GetSize() - 1;
					sourceMode = CodeMode::cm_reg;
					UpdateDoubIndex(inBuffer, c + 1, sinCom, newL, multiCom, multiComE, sinStr, mulStr);
				}
				break;
			case CodeMode::cm_sinStr:
				if (c == sinStr.strInd)
				{
					outbuffer += sinStr.stringQ;
					c += sinStr.stringQ.GetSize() - 1;
					sourceMode = CodeMode::cm_reg;
					UpdateDoubIndex(inBuffer, c + 1, sinCom, newL, multiCom, multiComE, sinStr, mulStr);
				}
				else
				{
					if (c == newL.strInd)
					{
						sinStr = getNextSingleString(inBuffer, c);
						sourceMode = CodeMode::cm_reg;
						UpdateDoubIndex(inBuffer, c + 1, sinCom, newL, multiCom, multiComE, sinStr, mulStr);
					}
					outbuffer += inBuffer[c];
				}

				break;
			case CodeMode::cm_mulStr:
				if (c == mulStr.strInd)
				{
					outbuffer += mulStr.stringQ;
					c += mulStr.stringQ.GetSize() - 1;
					sourceMode = CodeMode::cm_reg;
					UpdateDoubIndex(inBuffer, c + 1, sinCom, newL, multiCom, multiComE, sinStr, mulStr);
				}
				else
				{
					outbuffer += inBuffer[c];
				}
			} // End of Switch
		} // end of for


		sourceFile->WriteString(outbuffer);



	}
	sourceFile->Close();

	return true;
}

bool TLanguage::RunBlockFilter(TrecPointer<TFile> file, TString& newFileName)
{
	if (!file.Get() || !file->IsOpen())
		return false;

	if (blockType == LanguageBlockType::lbt_curly)
	{
		newFileName = file->GetFileName();
	}

	if (blockType == LanguageBlockType::lbt_indent)
	{
		TString sourceName = file->GetFileDirectory();
		newFileName = file->GetFileName();

		newFileName.Replace(TString(L"_NO_COM_"), TString(L"_RBLOCK_"));

		TrecPointer<TFile> sourceFile = TrecPointerKey::GetNewTrecPointer<TFile>();

		if (!sourceFile->Open(sourceName + newFileName, TFile::t_file_write | TFile::t_file_create_always))
		{
			return false;
		}


		TString line;
		file->SeekToBegin();

		std::stack<UINT> indentCount;
		indentCount.push(0);
		
		WCHAR indent = L'\0';
		while (file->ReadString(line))
		{
			line.TrimRight();
			
			if (line.GetSize() == 0)
				continue;

			if (indent == L'\0')
			{
				if (line[0] == L'\t')
					indent = L'\t';
				else if (line[0] == L'\s')
					indent = L'\s';
			}
			UINT Rust;
			for (Rust = 0; Rust < line.GetSize(); Rust++)
			{
				if (line[Rust] != indent)
					break;
			}

			int loc = 0;

			for (UINT C = 0; C < blockStarters.Size(); C++)
			{
				loc = line.FindOutOfQuotes(blockStarters[C]);
				if (loc == -1)
					continue;

				if (loc + blockStarters[C].GetSize() == line.GetSize())
				{
					break;
				}
				loc = -1;
			}

			while (!indentCount.empty() && indentCount.top() > Rust)
			{
				TString nLine(L'}');
				line.Set(nLine + line);
				indentCount.pop();
			}

			if (Rust != indentCount.top())
				return false;

			if (loc != -1)
			{
				if(!blockStartersAtBeginning)
					line.SubString(0, loc);
				line.AppendChar(L'{');
				indentCount.push(Rust);
			}

			sourceFile->WriteString(line);
		}


		sourceFile->Close();
	}

	if (blockType == LanguageBlockType::lbt_indent)
	{
		TString sourceName = file->GetFileDirectory();
		newFileName = file->GetFileName();

		newFileName.Replace(TString(L"_NO_COM_"), TString(L"_RBLOCK_"));

		TrecPointer<TFile> sourceFile = TrecPointerKey::GetNewTrecPointer<TFile>();

		if (!sourceFile->Open(sourceName + newFileName, TFile::t_file_write | TFile::t_file_create_always))
		{
			return false;
		}


		TString line;
		file->SeekToBegin();


		while (file->ReadString(line))
		{
			for (UINT C = 0; C < blockStarters.Size(); C++)
			{
				int loc = line.FindOutOfQuotes(blockStarters[C]);
				if (loc == -1)
					continue;

				if (blockStartersAtBeginning)
					line.AppendChar(L'{');
				else
				{
					line.Insert(loc + blockStarters[C].GetSize(), L'{');
				}
				break;
			}


			for (UINT C = 0; C < blockEnders.Size(); C++)
			{
				int loc = line.FindOutOfQuotes(blockStarters[C]);
				if (loc == -1)
					continue;

				line.Insert(loc, L'}');
			}

			sourceFile->WriteString(line);
		}
		sourceFile->Close();
	}
	return true;
}

void TLanguage::UpdateDoubIndex(TString& buff, UINT index, DoubIndex& sinCom, DoubIndex& newLine, DoubIndex& mulCom, DoubIndex& mulComE, DoubIndex& sinStr, DoubIndex& mulStr)
{
	sinCom = getNextSingleComment(buff, index);
	newLine = getNextNewline(buff, index);
	mulCom = getNextMultiComment(buff, index);
	mulComE = getNextMultiCommentEnd(buff, index);
	sinStr = getNextSingleString(buff, index);
	mulStr = getNextMultiString(buff, index);
}

DoubIndex TLanguage::getNextSingleComment(TString& buff, UINT index)
{
	DoubIndex ret;
	ret.strInd = -1;

	for (UINT rust = 0; rust < singleComment.Size(); rust++)
	{
		int loc = buff.Find(singleComment[rust], index);
		if (loc != -1 && (ret.strInd == -1 || loc < ret.strInd))
		{
			ret.stringQ.Set(singleComment[rust]);
			ret.strInd = loc;
			break;
		}
	}

	return ret;
}

DoubIndex TLanguage::getNextNewline(TString& buff, UINT index)
{
	DoubIndex ret{ -1, TString(L"\n") };

	ret.strInd = buff.Find(ret.stringQ, index);

	return ret;
}

DoubIndex TLanguage::getNextMultiComment(TString& buff, UINT index)
{
	DoubIndex ret;
	ret.strInd = -1;

	for (UINT rust = 0; rust < startComment.Size(); rust++)
	{
		int loc = buff.Find(startComment[rust], index);
		if (loc != -1 && (ret.strInd == -1 || loc < ret.strInd))
		{
			ret.stringQ.Set(startComment[rust]);
			ret.strInd = loc;
			break;
		}
	}

	return ret;
}

DoubIndex TLanguage::getNextMultiCommentEnd(TString& buff, UINT index)
{
	DoubIndex ret;
	ret.strInd = -1;

	for (UINT rust = 0; rust < endComment.Size(); rust++)
	{
		int loc = buff.Find(endComment[rust], index);
		if (loc != -1 && (ret.strInd == -1 || loc < ret.strInd))
		{
			ret.stringQ.Set(endComment[rust]);
			ret.strInd = loc;
			break;
		}
	}

	return ret;
}

DoubIndex TLanguage::getNextSingleString(TString& buff, UINT index)
{
	DoubIndex ret;
	ret.strInd = -1;

	for (UINT rust = 0; rust < string.Size(); rust++)
	{
		int loc = buff.Find(string[rust], index);
		if (loc != -1 && (ret.strInd == -1 || loc < ret.strInd))
		{
			ret.stringQ.Set(string[rust]);
			ret.strInd = loc;
			break;
		}
	}

	return ret;
}

DoubIndex TLanguage::getNextMultiString(TString& buff, UINT index)
{
	DoubIndex ret;
	ret.strInd = -1;

	for (UINT rust = 0; rust < multiLineString.Size(); rust++)
	{
		int loc = buff.Find(multiLineString[rust], index);
		if (loc != -1 && (ret.strInd == -1 || loc < ret.strInd))
		{
			ret.stringQ.Set(multiLineString[rust]);
			ret.strInd = loc;
			break;
		}
	}

	return ret;
}
