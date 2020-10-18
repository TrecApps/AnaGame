#include "WebPage.h"
#include <TObjectVariable.h>

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
	}
}

int WebPage::SetAnaface(TrecPointer<TFile> file, TrecPointer<EventHandler> eh)
{
	if (!file.Get() || !file->IsOpen())
		return 1;

	if (!environment.Get())
		return 2;


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



void WebPageHolder::SetPage(TrecPointer<Page> p)
{
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
