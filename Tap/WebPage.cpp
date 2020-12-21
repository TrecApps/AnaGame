#include "WebPage.h"
#include <TObjectVariable.h>
#include "TWindow.h"
#include <SSGenerator.h>
#include <DirectoryInterface.h>

WebPage::WebPage(TrecPointer<DrawingBoard> board, TrecPointerSoft<TWindow> win): Page(board)
{
	windowHandle = win;
}

TString WebPage::GetType()
{
	return TString(L"WebPage;") + Page::GetType();
}

WebPage::~WebPage()
{
}

void WebPage::SetEnvironment(TrecPointer<TEnvironment> env)
{
	environment = env;

	if (environment.Get())
	{
		// Set up access to the DOM
		environment->AddVariable(L"document", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TObjectVariable>(
			TrecPointerKey::GetTrecObjectPointer<Page>(TrecPointerKey::GetTrecPointerFromSoft<Page>(self))));

		// Set up Access to the BOM
		environment->AddVariable(L"window", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TObjectVariable>(
			TrecPointerKey::GetTrecObjectPointer<TWindow>(TrecPointerKey::GetTrecPointerFromSoft<TWindow>(windowHandle))));


		htmlBuilder = TrecPointerKey::GetNewTrecPointer<HtmlBuilder>(environment, this->drawingBoard);

	}

	
}

int WebPage::SetAnaface(TrecPointer<TFile> file, TrecPointer<EventHandler> eh)
{
	if (!file.Get() || !file->IsOpen())
		return 1;

	if (!environment.Get())
		return 2;

	if (file->GetFileName().EndsWith(L".html") || file->GetFileName().EndsWith(L".htm"))
	{
		if (!htmlBuilder.Get())
			return 3;

		TString res(htmlBuilder->BuildPage(file));

		if (res.GetSize())
		{
			TrecPointer<TWindow> win = TrecPointerKey::GetTrecPointerFromSoft<TWindow>(windowHandle);

			MessageBoxExW(win->GetWindowHandle(), res.GetConstantBuffer(), L"Error Creating WebPage", 0, 0);
			return 4;
		}

		res.Set(SetUpCSS());

		TrecPointer<HtmlBody> hBody = htmlBuilder->RetrieveBody();

		rootNode = hBody.Get() ? hBody->RetrieveWebNode(): TrecPointer<TWebNode>();
		
		if (!rootNode.Get())
		{
			TrecPointer<TWindow> win = TrecPointerKey::GetTrecPointerFromSoft<TWindow>(windowHandle);

			MessageBoxExW(win->GetWindowHandle(), L"No Web Page Body Detected after attempted Generation!", L"Error Creating WebPage", 0, 0);
			return 5;
		}
		TrecPointer<TWindow> win = TrecPointerKey::GetTrecPointerFromSoft<TWindow>(windowHandle);
		rootNode->PreCreate(TrecPointerSoft<TWebNode>());
		UINT createResult = rootNode->CreateWebNode(area, win->GetWindowEngine(), styles, win->GetWindowHandle());
		
	}
	else if (file->GetFileName().EndsWith(L".tml"))
		return Page::SetAnaface(file, eh);

	return 0;
}

void WebPage::Draw(TWindowEngine* twe)
{
}

TrecPointer<TWebNode> WebPage::GetElementById(const TString& id)
{
	return TrecPointer<TWebNode>();
}

TDataArray<TrecPointer<TWebNode>> WebPage::GetElementsByTag(const TString& id)
{
	return TDataArray<TrecPointer<TWebNode>>();
}

TDataArray<TrecPointer<TWebNode>> WebPage::GetElementsByClass(const TString& id)
{
	return TDataArray<TrecPointer<TWebNode>>();
}

void WebPage::Open()
{
}

void WebPage::Write(const TString& markup)
{
}

void WebPage::Close()
{
}

void WebPage::OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TrecPointer<TFlyout> fly)
{
	if (rootNode.Get())
	{
		TDataArray<TString> script;
		TDataArray<TrecObjectPointer> objects;

		rootNode->OnLButtonDown(script, objects, clickNodes, point);

	}
}

void WebPage::OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TrecPointer<TFlyout> fly)
{
	if (rootNode.Get())
	{
		TDataArray<TString> script;
		TDataArray<TrecObjectPointer> objects;

		rootNode->OnMouseMove(script, objects, moveNodes, point);


		for (UINT Rust = 0; Rust < moveNodes.Size(); Rust++)
		{
			auto node = moveNodes[Rust];
			if (!node.Get())
				continue;
			node->OnMouseMove(script, objects, moveNodes, point);
		}
	}
}

void WebPage::OnLButtonDblClk(UINT nFlags, TPoint point, messageOutput* mOut)
{
	if (rootNode.Get())
	{
		TDataArray<TString> script;
		TDataArray<TrecObjectPointer> objects;

		rootNode->OnLButtonDblClck(script, objects, point);

	}
}

void WebPage::OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TrecPointer<TFlyout> fly)
{
	if (rootNode.Get())
	{
		TDataArray<TString> script;
		TDataArray<TrecObjectPointer> objects;

		TrecPointer<TWebNode> newFocus;

		rootNode->OnLButtonUp(script, objects, clickNodes, newFocus, point);

		if (newFocus.Get() != focusNode.Get())
		{
			if (focusNode.Get())
			{
				TString script(focusNode->OnLoseFocus());
			}
			focusNode = newFocus;
		}
	}
}

TString WebPage::SetUpCSS()
{
	TrecPointer<HtmlHeader> header = htmlBuilder->RetrieveHeader();

	TDataArray<TrecPointer<TFileShell>> cssFiles;

	if (header.Get())
	{
		TFile cssFile(environment->GetRootDirectory()->GetName() + L"/Anagame_Internal.css", TFile::t_file_share_write | TFile::t_file_open_always);
		if (cssFile.IsOpen())
		{
			cssFiles.push_back(TFileShell::GetFileInfo(cssFile.GetFilePath()));

			cssFile.WriteString(header->RetrieveCss());
			cssFile.Close();
		}

		UINT linkIndex = 0;
		for (TrecPointer<HtmlLink> link = header->GetLink(linkIndex++); link.Get(); link = header->GetLink(linkIndex++))
		{
			TString relation(link->getVariableValueStr(L"rel"));

			if (!relation.CompareNoCase(L"stylesheet"))
			{
				// To-Do: Manage retrieval of Stylesheets in a URL
			}
		}

	}

	// To Do: Add File held to manage default attributes
	TString defaultCssFile(GetDirectoryWithSlash(CentralDirectories::cd_Executable) + L"Resources\\Web-Tours\\DefaultStyles.css");
	cssFiles.push_back(TFileShell::GetFileInfo(defaultCssFile));

	// Now Read the CSS Files into a Stylesheet table
	styles = TrecPointerKey::GetNewTrecPointer<TArray<styleTable>>();
	for (UINT Rust = 0; Rust < cssFiles.Size(); Rust++)
	{
		if (!cssFiles[Rust].Get())
			continue;
		TrecPointer<TArray<styleTable>> tempStyles;
		TFile cssFile(cssFiles[Rust]->GetPath(), TFile::t_file_open_always | TFile::t_file_read);
		if (cssFile.IsOpen())
		{
			CSSGenerator gen(cssFile);
			if (gen.Parse())
			{
				tempStyles = gen.GetStyles();
				if (tempStyles.Get())
				{
					for (UINT Rust = 0; Rust < tempStyles->Count(); Rust++)
					{
						if (tempStyles->ElementAt(Rust).Get())
							styles->Add(tempStyles->ElementAt(Rust));
					}
				}
			}
		}
	}



	return TString();
}



void WebPageHolder::SetPage(TrecPointer<Page> p)
{
	this->page = p;
}

WebPageHolder::WebPageHolder(TString name, TrecPointer<DrawingBoard> rt, UINT barSpace, TrecPointer<EventHandler> handler, TrecPointer<TWindow> win, D2D1_RECT_F initLoc)
{
	board = rt;

	text = TrecPointerKey::GetNewTrecPointer<TText>(rt, nullptr);
	text->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
	text->setNewFontSize(12.0f);
	text->setCaption(name);
	text->setNewHorizontalAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	text->setNewVerticalAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	text->onCreate(initLoc);
	bool w;
	float minWidth = text->GetMinWidth(w);
	if (w)
	{
		initLoc.right = initLoc.left + minWidth;
		text->SetLocation(initLoc);
	}
	location = initLoc;
}

WebPageHolder::~WebPageHolder()
{
}

bool WebPageHolder::SetImage(TrecPointer<TFileShell> file)
{
	image = TrecPointerKey::GetNewTrecSubPointer<TBrush, TBitmapBrush>(file, board, location);


	return image.Get() != nullptr;
}

TrecPointer<Page> WebPageHolder::GetBasePage()
{
	return page;
}


TString WebPageHolder::GetName()
{
	if (!text.Get())
		return TString();
	return text->getCaption();
}

D2D1_RECT_F WebPageHolder::GetLocation()
{
	return location;
}

D2D1_RECT_F WebPageHolder::SetLocation(const D2D1_RECT_F& newLoc)
{
	location = newLoc;
	if (text.Get())
	{
		text->SetLocation(newLoc);
		bool w;
		float width = text->GetMinWidth(w);
		location.right = location.left + width;

		text->SetLocation(location);

	}
	return location;
}

void WebPageHolder::Draw()
{
	if (text.Get())
		text->onDraw(location);

	if (image.Get())
		image->FillRectangle(location);
}

void WebPageHolder::Move(TPoint& moveBy)
{
	auto tempPoint = moveBy;
	moveBy.x -= curPoint.x;
	moveBy.y -= curPoint.y;

	location.bottom += moveBy.y;
	location.top += moveBy.y;
	location.left += moveBy.x;
	location.right += moveBy.x;

	curPoint = tempPoint;

	if (text.Get())
		text->SetLocation(location);
}

void WebPageHolder::SetCurrentPoint(TPoint& p)
{
	curPoint = p;
}
