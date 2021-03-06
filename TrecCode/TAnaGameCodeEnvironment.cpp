#include "TAnaGameCodeEnvironment.h"
#include "TAnascriptInterpretor.h"
#include "TJavaScriptInterpretor.h"
#include <TFileNode.h>
#include <TPromptControl.h>
#include "TcJavaScriptInterpretor.h"

TAnaGameCodeEnvironment::TAnaGameCodeEnvironment(TrecPointer<TFileShell> shell): TEnvironment(shell)
{
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


					shellRunner->Print(resultStr);
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

	auto node = TrecPointerKey::GetNewSelfTrecPointerAlt<TObjectNode, TFileNode>(0);
	auto path(rootDirectory->GetPath());
	node->Initialize(path);
	return node;
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
		shellRunner->Print(input);
		return true;
	}
	return false;
}

bool TAnaGameCodeEnvironment::PrintLine(const TString& input)
{
	if (shellRunner.Get())
	{
		shellRunner->PrintLine(input);
		return true;
	}
	return false;
}
