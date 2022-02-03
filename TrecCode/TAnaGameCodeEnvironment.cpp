#include "TAnaGameCodeEnvironment.h"
#include "TAnascriptInterpretor.h"
#include "TJavaScriptInterpretor.h"
#include <TFileNode.h>
#include "TcJavaScriptInterpretor.h"
#include <TStringVariable.h>
#include <TBlankNode.h>
#include <TCodeHandler.h>
#include <AnafacePage.h>
#include <DirectoryInterface.h>
#include <TerminalHandler.h>


class AnagameCodePageHandlerBuilder : public TPageEnvironment::PageHandlerBuilder {
private:
	TDataMap<TrecPointer<TPage>> singularPages;
	TrecPointer<TFileShell> directory;
public:

	AnagameCodePageHandlerBuilder(TrecPointer<TFileShell> directory)
	{
		assert(directory.Get() && directory->IsDirectory());
		this->directory = directory;
	}

	virtual void RetrievePageAndHandler(const TString& name, TrecPointer<TPage>& page, TrecPointer<TPage::EventHandler>& handler,
		TrecPointer<DrawingBoard> board, TrecPointer<TProcess> proc, const D2D1_RECT_F& loc)
	{
		if (!name.Compare(L"ag_ce_code"))
		{
			handler = TrecPointerKey::GetNewSelfTrecPointerAlt<TPage::EventHandler, TCodeHandler>(proc);
			TrecSubPointer<TPage, AnafacePage> aPage = TrecPointerKey::GetNewSelfTrecSubPointer<TPage, AnafacePage>(board);


			aPage->PrepPage(TFileShell::GetFileInfo(GetDirectoryWithSlash(CentralDirectories::cd_Executable) + L"Resources\\LineTextEditor.json"), handler);
			page = TrecSubToTrec(aPage);
		}

		if (!name.Compare(L"ag_ce_fBrowser"))
		{
			if (!singularPages.retrieveEntry(name, page))
			{
				handler = TrecPointerKey::GetNewSelfTrecPointerAlt<TPage::EventHandler, TCodeHandler>(proc);
				TrecSubPointer<TPage, AnafacePage> aPage = TrecPointerKey::GetNewSelfTrecSubPointer<TPage, AnafacePage>(board);


				aPage->PrepPage(TFileShell::GetFileInfo(GetDirectoryWithSlash(CentralDirectories::cd_Executable) + L"Resources\\FileBrowser.json"), handler);
				page = TrecSubToTrec(aPage);
				singularPages.addEntry(name, page);
			}
			else handler = page->GetHandler();
		}

		if (!name.Compare(L"ag_ce_i_console"))
		{
			handler = TrecPointerKey::GetNewSelfTrecPointerAlt<TPage::EventHandler, TerminalHandler>(proc, directory);
			TrecSubPointer<TPage, AnafacePage> aPage = TrecPointerKey::GetNewSelfTrecSubPointer<TPage, AnafacePage>(board);


			aPage->PrepPage(TFileShell::GetFileInfo(GetDirectoryWithSlash(CentralDirectories::cd_Executable) + L"Resources\\InputConsole.json"), handler);
			page = TrecSubToTrec(aPage);
		}

		if (!name.Compare(L"ag_ce_o_console"))
		{
			handler = TrecPointerKey::GetNewSelfTrecPointerAlt<TPage::EventHandler, TerminalHandler>(proc, directory);
			TrecSubPointer<TPage, AnafacePage> aPage = TrecPointerKey::GetNewSelfTrecSubPointer<TPage, AnafacePage>(board);


			aPage->PrepPage(TFileShell::GetFileInfo(GetDirectoryWithSlash(CentralDirectories::cd_Executable) + L"Resources\\OutputConsole.json"), handler);
			page = TrecSubToTrec(aPage);
		}

		// This object only uses Anaface Pages
		if(page.Get())
		dynamic_cast<AnafacePage*>(page.Get())->Create(loc, board->GetWindowEngine());
		
	}
};



TAnaGameCodeEnvironment::TAnaGameCodeEnvironment(TrecPointer<TFileShell> shell): TPageEnvironment(shell)
{
	targetMachine = TargetAnagameMachine::tam_object_register;
	compileErrorHandling = CompileErrorHandling::ceh_stop;

	codePageBuilder = TrecPointerKey::GetNewTrecPointerAlt<TPageEnvironment::PageHandlerBuilder, AnagameCodePageHandlerBuilder>(shell);
}

TString TAnaGameCodeEnvironment::GetType()
{
	return TString(L"TAnaGameCodeEnvironment;") + TEnvironment::GetType();
}

void TAnaGameCodeEnvironment::PreProcessSingleFile(TrecPointer<TFile> file)
{
	//if (!file.Get() || !file->IsOpen())
	//	return;

	//TString fileExt(file->GetFileExtension());

	//TrecPointer<TLanguage> lang = languages.retrieveEntry(fileExt);

	//if (!lang.Get())
	//{
	//	SetUpLanguageExtensionMapping();

	//	TString langName(retrieveLanguageByExtension(fileExt));
	//	if (!langName.GetSize())
	//		return;

	//	auto language = TLanguage::getLanguage(langName);
	//	if (!language.Get())
	//		return;

	//	for (UINT C = 0; C < languageList.Size(); C++)
	//	{
	//		if (languageList[C].language.Compare(langName))
	//			continue;
	//		for (UINT Rust = 0; Rust < languageList[C].fileExtensions.Size(); Rust++)
	//		{
	//			languages.addEntry(languageList[C].fileExtensions[Rust], language);
	//		}
	//		break;
	//	}

	//	lang = language;
	//}

	//lang->PreProcessFile(file);
}

UINT TAnaGameCodeEnvironment::RunTask(TString& task)
{
	auto fileTask = TFileShell::GetFileInfo(task);

	if (fileTask.Get())
	{
		if (task.EndsWith(L".ascrpt"))
		{
			// We have an Anascript file on our hands
			auto interpretor = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TAnascriptInterpretor>(TrecSubPointer<TVariable, TInterpretor>(), TrecPointerKey::GetTrecPointerFromSoft<TEnvironment>(self));

			TFile file(task, TFile::t_file_open_existing | TFile::t_file_read);

			
			if (file.IsOpen())
			{
				interpretor->SetCode(file);

				file.Close();

				auto result = interpretor->Run();

				if (this->shellRunner.Get())
				{
					TString resultStr(L"Program exited with code: ");
					resultStr.AppendFormat(L"%i\n", result.returnCode);

					if (result.returnCode)
					{
						resultStr.Append(result.errorMessage);

						for (UINT Rust = 0; Rust < result.stackTrace.Size(); Rust++)
						{
							resultStr.AppendFormat(L"\n\t%ws", result.stackTrace[Rust].GetConstantBuffer().getBuffer());
						}
						resultStr.AppendChar(L'\n');
					}


					shellRunner->Log(TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(resultStr));
				}
			}
		}
		else if (task.EndsWith(L".js"))
		{
			// We have an Anascript file on our hands
			auto interpretor = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TJavaScriptInterpretor>(TrecSubPointer<TVariable, TInterpretor>(), TrecPointerKey::GetTrecPointerFromSoft<TEnvironment>(self));

			TrecPointer<TFileShell> jsFile = TFileShell::GetFileInfo(task);
			Run(jsFile);

		}
	}
	return 0;
}

UINT TAnaGameCodeEnvironment::SetUpEnv()
{
	return 0;
}

void TAnaGameCodeEnvironment::Compile()
{
}

void TAnaGameCodeEnvironment::Compile(TrecPointer<TFile> logFile)
{
}

void TAnaGameCodeEnvironment::Log()
{
}

void TAnaGameCodeEnvironment::Run()
{
}

void TAnaGameCodeEnvironment::Run(TrecPointer<TFileShell> file)
{
	if (!file.Get() || file->IsDirectory())
	{
		this->PrintLine(L"Null File or a Directory has been provided!");
		return;
	}
	auto path = file->GetPath();


	if (path.GetSize() > 7 && path.FindLast(L".ascrpt") == path.GetSize() - 7)
	{
		// This is an anascript file, Run Anascript
		auto anascriptInterpreter = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TAnascriptInterpretor>(TrecSubPointer<TVariable, TInterpretor>(),
			TrecPointerKey::GetTrecPointerFromSoft<TEnvironment>(self));
		TFile actualFile(path, TFile::t_file_open_always | TFile::t_file_read);
		UINT setCodeResult = anascriptInterpreter->SetCode(actualFile);

		if (setCodeResult)
			return;

		auto runCodeResult = anascriptInterpreter->Run();

		int e = 3;
	}
	else if (path.GetSize() > 3 && path.EndsWith(L".js"))
	{
		auto javaScriptInterpretor = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TcJavaScriptInterpretor>(TrecSubPointer<TVariable, TcInterpretor>(),
			TrecPointerKey::GetTrecPointerFromSoft<TEnvironment>(self));
		ReturnObject ret;
		javaScriptInterpretor->SetFile(file, ret);

		currentRunner = TrecPointerKey::GetTrecPointerFromSub<>(javaScriptInterpretor);

		if (ret.returnCode)
		{
			TString message;
			message.Format(L"Java Script file Preprocessing exited with Error code: %d", ret.returnCode);
			this->PrintLine(message);
			this->PrintLine(ret.errorMessage);

			for (UINT Rust = 0; Rust < ret.stackTrace.Size(); Rust++)
			{
				this->Print(L'\t');
				this->PrintLine(ret.stackTrace[Rust]);
			}
			return;
		}

		javaScriptInterpretor->PreProcess(ret);
		if (ret.returnCode)
		{
			TString message;
			message.Format(L"Java Script '%ws' file Preprocessing exited with Error code: %d", path.GetConstantBuffer().getBuffer(), ret.returnCode);
			this->PrintLine(message);
			this->PrintLine(ret.errorMessage);

			for (UINT Rust = 0; Rust < ret.stackTrace.Size(); Rust++)
			{
				this->Print(L'\t');
				this->PrintLine(ret.stackTrace[Rust]);
			}
			return;
		}

		ret = javaScriptInterpretor->Run();
		if (ret.returnCode)
		{
			TString message;
			message.Format(L"Java Script '%ws' file Run exited with Error code: %d", path.GetConstantBuffer().getBuffer(), ret.returnCode);
			this->PrintLine(message);
			this->PrintLine(ret.errorMessage);

			for (UINT Rust = 0; Rust < ret.stackTrace.Size(); Rust++)
			{
				this->Print(L'\t');
				this->PrintLine(ret.stackTrace[Rust]);
			}
			return;
		}
		else
		{
			TString message;
			message.Format(L"Java Script '%ws' file Preprocessing exited with Error code 0", path.GetConstantBuffer().getBuffer());
			this->PrintLine(message);
		}
		int e = 3;
	}
}

TrecPointer<TObjectNode> TAnaGameCodeEnvironment::GetBrowsingNode()
{
	if(!rootDirectory.Get())
		return TrecPointer<TObjectNode>();

	auto node = TrecPointerKey::GetNewSelfTrecSubPointer<TObjectNode, TBlankNode>(0);

	UINT rootSize = rootDirectory->GetPath().GetSize();

	for (UINT Rust = 0; Rust < files.Size(); Rust++)
	{
		TString name(files[Rust]->GetPath());
		name.Delete(0, rootSize);
		int firstSlash = name.Find(L'\\');

		name.Set(rootDirectory->GetPath() + name.SubString(0, firstSlash));
		auto fNode = TrecPointerKey::GetNewSelfTrecPointerAlt<TObjectNode, TFileNode>(1);

		fNode->Initialize(name);
		node->AddNode(fNode);
	}

	return TrecPointerKey::GetTrecPointerFromSub<>(node);
}

bool TAnaGameCodeEnvironment::SupportsFileExt(const TString& ext)
{
	return false;
}

void TAnaGameCodeEnvironment::SupportsFileExt(TDataArray<TString>& ext)
{
}

bool TAnaGameCodeEnvironment::Print(const TString& input)
{
	if (shellRunner.Get())
	{
		shellRunner->Log(TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(input));
		RedrawWindow(nullptr, nullptr, nullptr, 0);
		return true;
	}
	return false;
}

bool TAnaGameCodeEnvironment::PrintLine(const TString& input)
{
	if (shellRunner.Get())
	{
		shellRunner->Log(TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(input));
		RedrawWindow(nullptr, nullptr, nullptr, 0);
		return true;
	}
	return false;
}

UINT TAnaGameCodeEnvironment::SaveEnv()
{
	if (!rootDirectory.Get())
		return 1;

	TFile file(rootDirectory->GetPath() + L"\\treccode.tml", TFile::t_file_open_always | TFile::t_file_write);

	if (!file.IsOpen())
		return 2;

	file.WriteString(L"->TML\n");
	file.WriteString(L"-|Type: Anacode\n");
	file.WriteString(L"-|Version: 0.0.1\n");
	file.WriteString(L"-/\n");

	file.WriteString(L"->Project\n");
	TString relatives;
	if(mainFile.Get() && mainFile->GetRelativePath(relatives, rootDirectory, false))
		file.WriteString(TString(L"-|MainFile: ") + relatives + L'\n');
	for (UINT Rust = 0; Rust < files.Size(); Rust++)
	{
		if(files[Rust].Get() && files[Rust]->GetRelativePath(relatives, rootDirectory, false))
		file.WriteString(TString(L"-|File: ") + relatives + L'\n');
	}


	TEnvironment::UpdateProjectRepo(TFileShell::GetFileInfo(file.GetFilePath()), L"Anagame", L"AnaCode", name);
	file.Close();
	return 0;
}

TrecPointer<TObjectNode> TAnaGameCodeEnvironment::GetProjectLyout()
{
	return TrecPointer<TObjectNode>();
}

void TAnaGameCodeEnvironment::AddResource(TrecPointer<TFileShell> fileResource)
{
	if (!fileResource.Get())
		return;

	// Need to make sure that we aren't adding effectively the same file
	bool canAdd = true;
	for (UINT Rust = 0; Rust < files.Size() && canAdd; Rust++)
	{
		TrecPointer<TFileShell> f = files[Rust];
		if (!fileResource->GetPath().CompareNoCase(f->GetPath()))
		{
			canAdd = false;
		}
	}

	if (canAdd)
		files.push_back(fileResource);
}

TString TAnaGameCodeEnvironment::SetLoadFile(TrecPointer<TFileShell> file)
{
	if (!file.Get())
		return L"Null Parameter";
	if (!rootDirectory.Get())
		return L"RootDirectory Not Set";

	TFile actFile(file->GetPath(), TFile::t_file_open_existing | TFile::t_file_read);

	if (!actFile.IsOpen())
		return L"File Failed to Open!";
	TString line;
	while (actFile.ReadString(line))
	{
		if (line.StartsWith(L"-|File:"))
		{
			line.Delete(0, 7);
			line.Trim();
			TString dLine(rootDirectory->GetPath());
			if (!dLine.EndsWith(L"\\"))
				dLine.AppendChar(L'\\');
			line.Set(dLine + line);

			TrecPointer<TFileShell> f = TFileShell::GetFileInfo(line);
			if (f.Get())
				this->files.push_back(f);
			continue;
		}
	}
	name.Set(file->GetDirectoryName());
	actFile.Close();
	return TString();
}

void TAnaGameCodeEnvironment::GetPageAndHandler_(handler_type hType, const TString& name, TrecPointer<PageHandlerBuilder>& builder)
{
	if (hType == handler_type::ht_file && !name.Compare(L"ag_ce_code"))
	{
		builder = codePageBuilder;
		return; 
	}

	if (hType == handler_type::ht_singular && !name.Compare(L"ag_ce_fBrowser"))
	{
		builder = codePageBuilder;
		return;
	}

	if (hType == handler_type::ht_i_console || hType == handler_type::ht_o_console)
	{
		builder = codePageBuilder;
		return;
	}
}

void TAnaGameCodeEnvironment::GetPageList_(handler_type hType, const TString& ext, TDataArray<TString>& extensions)
{
	switch (hType)
	{
	case handler_type::ht_file:
		if (!ext.CompareNoCase(L"js") ||
			!ext.CompareNoCase(L"py") ||
			!ext.CompareNoCase(L"ascrpt") ||
			!ext.GetSize())
		{
			extensions.push_back(L"ag_ce_code");
		}
		break;
	case handler_type::ht_singular:
		if (!ext.CompareNoCase(L"filebrowser"))
		{
			extensions.push_back(L"ag_ce_fBrowser");
		}
		break;

	case handler_type::ht_i_console:
		extensions.push_back(L"ag_ce_i_console");
		break;
	case handler_type::ht_o_console:
		extensions.push_back(L"ag_ce_o_console");
	//case handler_type::ht_ribbon:
	//	if (!ext.CompareNoCase(L"code"))
	//		extensions.push_back(L"ag_ce_code_blade");
	}
}
