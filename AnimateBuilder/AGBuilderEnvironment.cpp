#include "AGBuilderEnvironment.h"
#include <AnafacePage.h>
#include <DirectoryInterface.h>
#include "CodeRibbonHandler.h"

class AGBuilderPageHandlerBuilder : public TPageEnvironment::PageHandlerBuilder {
private:
	TDataMap<TrecPointer<TPage>> singularPages;

	TrecPointer<TPage> codeRibbonPage;

public:
	virtual void RetrievePageAndHandler(const TString& name, TrecPointer<TPage>& page, TrecPointer<TPage::EventHandler>& handler,
		TrecPointer<DrawingBoard> board, TrecPointer<TProcess> proc, const D2D1_RECT_F& loc)
	{
		if (!name.Compare(L"ag_builder_code"))
		{
			if (codeRibbonPage.Get())
			{
				page = codeRibbonPage;
				handler = page->GetHandler();
			}
			else
			{
				handler = TrecPointerKey::GetNewSelfTrecPointerAlt<TPage::EventHandler, CodeRibbonHandler>(proc);
				TrecSubPointer<TPage, AnafacePage> aPage = TrecPointerKey::GetNewSelfTrecSubPointer<TPage, AnafacePage>(board);


				aPage->PrepPage(TFileShell::GetFileInfo(GetDirectoryWithSlash(CentralDirectories::cd_Executable) + L"Resources\\CodeRibbon.json"), handler);
				page = TrecSubToTrec(aPage);
				codeRibbonPage = page;
			}
		}


		// This object only uses Anaface Pages
		if(page.Get())
		dynamic_cast<AnafacePage*>(page.Get())->Create(loc, board->GetWindowEngine());

	}
};


void AGBuilderEnvironment::GetPageAndHandler_(handler_type hType, const TString& name, TrecPointer<PageHandlerBuilder>& builder)
{
	if (!name.CompareNoCase(L"ag_builder_code") && hType == handler_type::ht_ribbon)
		builder = codePageBuilder;
}

void AGBuilderEnvironment::GetPageList_(handler_type hType, const TString& ext, TDataArray<TString>& extensions)
{
	if (!ext.CompareNoCase(L"code") && hType == handler_type::ht_ribbon)
		extensions.push_back(L"ag_builder_code");
}

TrecPointer<TObjectNode> AGBuilderEnvironment::GetProjectLyout()
{
	return TrecPointer<TObjectNode>();
}

UINT AGBuilderEnvironment::SetUpEnv()
{
	return 0;
}

void AGBuilderEnvironment::Compile()
{
}

void AGBuilderEnvironment::Compile(TrecPointer<TFile> logFile)
{
}

void AGBuilderEnvironment::Log()
{
}

void AGBuilderEnvironment::Run()
{
}

void AGBuilderEnvironment::Run(TrecPointer<TFileShell> shell)
{
}

UINT AGBuilderEnvironment::RunTask(TString& task)
{
	return 0;
}

TrecPointer<TObjectNode> AGBuilderEnvironment::GetBrowsingNode()
{
	return TrecPointer<TObjectNode>();
}

bool AGBuilderEnvironment::SupportsFileExt(const TString& ext)
{
	return false;
}

void AGBuilderEnvironment::SupportsFileExt(TDataArray<TString>& ext)
{
}

UINT AGBuilderEnvironment::SaveEnv()
{
	return 0;
}

void AGBuilderEnvironment::AddResource(TrecPointer<TFileShell> fileResource)
{
}

TString AGBuilderEnvironment::SetLoadFile(TrecPointer<TFileShell> file)
{
	return TString();
}

AGBuilderEnvironment::AGBuilderEnvironment(TrecPointer<TFileShell> shell): TPageEnvironment(shell)
{
	codePageBuilder = TrecPointerKey::GetNewTrecPointerAlt<TPageEnvironment::PageHandlerBuilder, AGBuilderPageHandlerBuilder>();
}
