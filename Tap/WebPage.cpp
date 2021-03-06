#include "WebPage.h"
#include <TObjectVariable.h>
#include "TWindow.h"
#include <SSGenerator.h>
#include <DirectoryInterface.h>
#include "WebHandler.h"


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
	ThreadLock();
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
	ThreadRelease();	
}

int WebPage::SetAnaface(TrecPointer<TFile> file, TrecPointer<EventHandler> eh)
{
	ThreadLock();
	if (!file.Get() || !file->IsOpen())
	{
		ThreadRelease();
		return 1;
	}
	if (!environment.Get())
	{
		ThreadRelease();
		return 2;
	}
	if (!windowHandle.Get())
	{
		ThreadRelease();
		return 5;
	}
	if (file->GetFileName().EndsWith(L".html") || file->GetFileName().EndsWith(L".htm"))
	{
		if (!htmlBuilder.Get())
		{
			ThreadRelease();
			return 3;
		}
		TString res(htmlBuilder->BuildPage(file, TrecPointerKey::GetTrecPointerFromSoft<TWindow>(windowHandle)->GetWindowHandle()));

		if (res.GetSize())
		{
			TrecPointer<TWindow> win = TrecPointerKey::GetTrecPointerFromSoft<TWindow>(windowHandle);

			MessageBoxExW(win->GetWindowHandle(), res.GetConstantBuffer().getBuffer(), L"Error Creating WebPage", 0, 0);
			ThreadRelease();
			return 4;
		}

		res.Set(SetUpCSS());

		TrecPointer<HtmlBody> hBody = htmlBuilder->RetrieveBody();

		rootNode = hBody.Get() ? hBody->RetrieveWebNode(): TrecPointer<TWebNode>();
		
		if (!rootNode.Get())
		{
			TrecPointer<TWindow> win = TrecPointerKey::GetTrecPointerFromSoft<TWindow>(windowHandle);

			MessageBoxExW(win->GetWindowHandle(), L"No Web Page Body Detected after attempted Generation!", L"Error Creating WebPage", 0, 0);
			ThreadRelease();
			return 5;
		}
		TrecPointer<TWindow> win = TrecPointerKey::GetTrecPointerFromSoft<TWindow>(windowHandle);
		rootNode->PreCreate(TrecPointerSoft<TWebNode>(), styles);
		D2D1_RECT_F tempArea = area;
		tempArea.bottom = 50000.0f;
		UINT createResult = rootNode->CreateWebNode(tempArea, win->GetWindowEngine(), win->GetWindowHandle());
		
	}
	else if (file->GetFileName().EndsWith(L".tml"))
	{
		auto ret =  Page::SetAnaface(file, eh);
		ThreadRelease();
		return ret;
	}
	ThreadRelease();
	return 0;
}

void WebPage::Draw(TWindowEngine* twe)
{
	ThreadLock();
	if (rootNode.Get())
	{
		rootNode->OnDraw();
	}
	else
		Page::Draw(twe);
	ThreadRelease();

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
	ThreadLock();
	if (rootNode.Get())
	{
		TDataArray<TString> script;
		TDataArray<TrecObjectPointer> objects;

		rootNode->OnLButtonDown(script, objects, clickNodes, point);

		if (dynamic_cast<WebHandler*>(handler.Get()))
		{
			for (UINT Rust = 0; Rust < script.Size() && Rust < objects.Size(); Rust++)
				dynamic_cast<WebHandler*>(handler.Get())->HandleWebEvents(script[Rust], TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TObjectVariable>(objects[Rust]));
		}
	}
	ThreadRelease();
}

void WebPage::OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TrecPointer<TFlyout> fly)
{
	ThreadLock();
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
		if (dynamic_cast<WebHandler*>(handler.Get()))
		{
			for (UINT Rust = 0; Rust < script.Size() && Rust < objects.Size(); Rust++)
				dynamic_cast<WebHandler*>(handler.Get())->HandleWebEvents(script[Rust], TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TObjectVariable>(objects[Rust]));
		}
	}
	ThreadRelease();
}

void WebPage::OnLButtonDblClk(UINT nFlags, TPoint point, messageOutput* mOut)
{
	ThreadLock();
	if (rootNode.Get())
	{
		TDataArray<TString> script;
		TDataArray<TrecObjectPointer> objects;

		rootNode->OnLButtonDblClck(script, objects, point);
	}
	ThreadRelease();
}

void WebPage::OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TrecPointer<TFlyout> fly)
{
	ThreadLock();
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
		if (dynamic_cast<WebHandler*>(handler.Get()))
		{
			for(UINT Rust = 0; Rust < script.Size() && Rust < objects.Size(); Rust++)
				dynamic_cast<WebHandler*>(handler.Get())->HandleWebEvents(script[Rust], TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TObjectVariable>(objects[Rust]));
		}
	}
	ThreadRelease();
}

/**
 * Method: WebPage::GetTitle
 * Purpose: Retirves the title of this web page
 * Parameters: void
 * Returns TString - the title of the web page
 *
 * Note: This value could be an empty String. It is up to the caller to check for that and handle it accordingly
 */
TString WebPage::GetTitle()
{
	ThreadLock();
	if (htmlBuilder.Get())
	{
		TrecPointer<HtmlHeader> head = htmlBuilder->RetrieveHeader();
		if (head.Get())
		{
			ThreadRelease();
			return head->RetrieveTitle();
		}
	}
	ThreadRelease();
	return TString();
}

TString WebPage::SetUpCSS()
{
	ThreadLock();
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
			TString val(link->getVariableValueStr(L"href"));

			if (!relation.CompareNoCase(L"stylesheet"))
			{
				// To-Do: Manage retrieval of Stylesheets in a URL
				TString fullPath(directory + val);
				TFile tcssFile(fullPath, TFile::t_file_read | TFile::t_file_open_always);

				if (!tcssFile.IsOpen())
				{

				}

				if (tcssFile.IsOpen())
				{
					cssFiles.push_back(TFileShell::GetFileInfo(tcssFile.GetFilePath()));
					tcssFile.Close();
				}
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
	ThreadRelease();
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
