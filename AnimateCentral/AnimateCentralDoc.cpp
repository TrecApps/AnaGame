
// AnimateCentralDoc.cpp : implementation of the CAnimateCentralDoc class
//
#include <iostream>
#include <typeinfo.h>
#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "AnimateCentral.h"
#endif

#include "AnimateCentralDoc.h"
#include <d2d1.h>
#include <AnafaceParser.h>
#include <TML_Reader_.h>

#include <propkey.h>
#include <TDataArray.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAnimateCentralDoc

IMPLEMENT_DYNCREATE(CAnimateCentralDoc, CDocument)

BEGIN_MESSAGE_MAP(CAnimateCentralDoc, CDocument)
	ON_COMMAND(ID_SETACONTROLTEST_1, &CAnimateCentralDoc::OnSetacontroltest1)

	ON_COMMAND(ID_SETUPTCONTROL_1, &CAnimateCentralDoc::OnSetuptcontrol1)
	ON_COMMAND(ID_SETUPTCONTROL_2, &CAnimateCentralDoc::OnSetuptcontrol2)
	ON_COMMAND(ID_SETUPTCONTROL_3, &CAnimateCentralDoc::OnSetuptcontrol3)

	ON_COMMAND(ID_SETUPTCONTROL_4, &CAnimateCentralDoc::OnSetuptcontrol4)
	ON_COMMAND(ID_SETUPTCONTROL_5, &CAnimateCentralDoc::OnSetuptcontrol5)
	ON_COMMAND(ID_SETUPTCONTROL_6, &CAnimateCentralDoc::OnSetuptcontrol6)
	ON_COMMAND(ID_SETUPTCONTROL_LAYEREDTLAYOUT, &CAnimateCentralDoc::OnSetuptcontrolLayeredtlayout)
	ON_COMMAND(ID_SETUPTCONTROL_LAYOUT, &CAnimateCentralDoc::OnSetuptcontrolLayoutGadget)
	ON_COMMAND(ID_SETUPTCONTROL_TEXTBOX, &CAnimateCentralDoc::OnSetuptcontrolTextbox)
	ON_COMMAND(ID_SETUPTCONTROL_COMBOBOX, &CAnimateCentralDoc::OnSetuptcontrolCombobox)
	ON_COMMAND(ID_SETUPTCONTROL_SCROLLTEST, &CAnimateCentralDoc::OnSetuptcontrolScrolltest)
END_MESSAGE_MAP()


// CAnimateCentralDoc construction/destruction

CAnimateCentralDoc::CAnimateCentralDoc()
{
	drawFlyout = nullptr;

	// TODO: add one-time construction code here
	D2D1_FACTORY_OPTIONS ops;

	ops.debugLevel = D2D1_DEBUG_LEVEL_WARNING;
	TrecComPointer<ID2D1Factory>::TrecComHolder f;
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, ops, f.GetPointerAddress());
	factory = f.Extract();
	ASSERT(SUCCEEDED(hr));
	if (!SUCCEEDED(hr))
	{
		printf("Factory not Created\n");
		return;
	}
	TrecComPointer<ID2D1DCRenderTarget>::TrecComHolder tempDC;
	hr = factory->CreateDCRenderTarget(&(D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,D2D1_ALPHA_MODE_IGNORE),
		0,0,
		D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE,
		D2D1_FEATURE_LEVEL_DEFAULT
		)), tempDC.GetPointerAddress());
	ASSERT(SUCCEEDED(hr));
	if (!SUCCEEDED(hr))
	{
		factory.Nullify();
		printf("RenderTarget failed to generate\n");
		return;
	}
	rt = TrecPointerKey::GetComPointer<ID2D1RenderTarget, ID2D1DCRenderTarget>( tempDC);

	rootControl.Nullify();
	//rootLayout = NULL;
	firstDraw = false;
	WindowSet = false;
	CWnd* win = AfxGetMainWnd();
	if (win)
	{
		windowHandle = win->GetSafeHwnd();
		WindowSet = true;
	}
}

CAnimateCentralDoc::~CAnimateCentralDoc()
{
	rootControl.Delete();
	rt.Delete();
	factory.Delete();
}

BOOL CAnimateCentralDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CAnimateCentralDoc serialization

void CAnimateCentralDoc::Serialize(CArchive& ar)
{
	if (!WindowSet)
	{
		CWnd* win = AfxGetMainWnd();
		if (win)
		{
			windowHandle = win->GetSafeHwnd();
			WindowSet = true;
		}
		else return;
	}

}

void CAnimateCentralDoc::setView(CView * cv)
{
	attachedView = cv;
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CAnimateCentralDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CAnimateCentralDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CAnimateCentralDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CAnimateCentralDoc diagnostics

#ifdef _DEBUG
void CAnimateCentralDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAnimateCentralDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}

void CAnimateCentralDoc::SetRightClick(CPoint p)
{
	rightClickPoint = p;
}

#endif //_DEBUG
void CAnimateCentralDoc::setUpTControl(CDC * pCD)
{	
	if (!rt.Get() || !pCD)
		return;
	if (pCD)
	{
		windowHandle = pCD->GetWindow()->m_hWnd;
		WindowSet = true;
	}
	int c = 0;
	flush:
	UINT boundResult = pCD->GetBoundsRect(&area, 0);
	GetClientRect(WindowFromDC(pCD->m_hDC), &area);


	//if (typeid(tempRt) != typeid(ID2D1DCRenderTarget*))
	//	return;
	//ID2D1DCRenderTarget* tempDC = dynamic_cast<ID2D1DCRenderTarget*>(tempRt);
	ID2D1RenderTarget* rtRaw = rt.Get();
	ID2D1DCRenderTarget* rtDc = reinterpret_cast<ID2D1DCRenderTarget*>(rtRaw);

	HRESULT hr = rtDc->BindDC(pCD->GetSafeHdc(), &area);

	int er;
	if (!SUCCEEDED(hr))
	{
		er = GetLastError();
		c++;
		if (c < 5)
		{
			rt->Flush();
			goto flush;
		}
	}
//	ASSERT(SUCCEEDED(hr));
}

void CAnimateCentralDoc::onDraw()
{
	rt->BeginDraw();
	//rt->Clear();
	if (rootControl.Get())
		rootControl->onDraw();
	TFlyout* fly = TFlyout::DoDraw();
	if(fly)
	{
		try
		{
			TContextMenu* cMenu = dynamic_cast<TContextMenu*>(fly);
			if (cMenu)
			{
				cMenu->onDraw(rightClickPoint);
			}
			else
				fly->onDraw();
		}
		catch (std::bad_cast b)
		{
			fly->onDraw();
		}
	}
	rt->EndDraw();
}

bool CAnimateCentralDoc::getFirstDraw()
{
	return firstDraw;
}

void CAnimateCentralDoc::setDrawn()
{
	firstDraw = false;
}

RECT CAnimateCentralDoc::getLocation()
{
	if (rootControl.Get() == NULL)
		return RECT();
	else
		return rootControl->getLocation();
}

// CAnimateCentralDoc commands


void CAnimateCentralDoc::OnSetacontroltest1()
{
	// TODO: Add your command handler code here
}


void CAnimateCentralDoc::OnSetuptcontrol1()
{
	// TODO: Add your command handler code here
	// TODO: Add your command update UI handler code here

	TrecComPointer<ID2D1RenderTarget> rtb = rt;

	rootControl.Delete();
	
	rootControl = TrecPointerKey::GetNewSelfTrecPointer<TControl>(rtb, table);
	//TControl* tcrootControl = rootControl.Get();
	TrecPointer<TString> value = TrecPointerKey::GetNewTrecPointer<TString>("10,10,30,30");
	rootControl->addAttribute(TString(L"|Margin"), value);
	TrecPointer<TString> value2 = TrecPointerKey::GetNewTrecPointer<TString>("2.0");
	rootControl->addAttribute(TString(L"|BorderThickness"), value2);
	TrecPointer<TString> value3 = TrecPointerKey::GetNewTrecPointer<TString>("0.1,0.1,1.0,1.0");
	rootControl->addAttribute(TString(L"|BorderColor"), value3);

	TrecPointer<TString> value4 = TrecPointerKey::GetNewTrecPointer<TString>("0.2,0.2,0.5,1.0");
	rootControl->addAttribute(TString(L"|ContentColor"), value4);
	

	TrecPointer<TString> value13 = TrecPointerKey::GetNewTrecPointer<TString>("0.1,0.6,1.0,1.0");
	rootControl->addAttribute(TString(L"|ClickBorderColor"), value13);

	TrecPointer<TString> value14 = TrecPointerKey::GetNewTrecPointer<TString>("0.1,0.1,0.7,1.0");
	rootControl->addAttribute(TString(L"|ClickContentColor"), value14);

	TrecPointer<TString> value5 = TrecPointerKey::GetNewTrecPointer<TString>("50,50,100,100");
	rootControl->addAttribute(TString(L"|ContainerLoc"), value5);


	// Prepare Child TControl
	TrecPointer<TControl> childControl = TrecPointerKey::GetNewSelfTrecPointer<TControl>(rtb, table);
	TControl* tcchildControl = childControl.Get();

	TrecPointer<TString> value6 = TrecPointerKey::GetNewTrecPointer<TString>("5.0");
	tcchildControl->addAttribute(TString(L"|BorderThickness"), value6);
	TrecPointer<TString> value7 = TrecPointerKey::GetNewTrecPointer<TString>("0.1,1.0,0.1,1.0");
	tcchildControl->addAttribute(TString(L"|BorderColor"), value7);
	TrecPointer<TString> value8 = TrecPointerKey::GetNewTrecPointer<TString>("0.8,0.2,0.2,1.0");
	tcchildControl->addAttribute(TString(L"|ContentColor"), value8);
	TrecPointer<TString> value9 = TrecPointerKey::GetNewTrecPointer<TString>("3,3,3,3");
	tcchildControl->addAttribute(TString(L"|Margin"), value9);


	tcchildControl->addAttribute(TString(L"|ClickBorderThickness"), value6);
	TrecPointer<TString> value17 = TrecPointerKey::GetNewTrecPointer<TString>("0.1,1.0,1.1,0.9");
	tcchildControl->addAttribute(TString(L"|ClickBorderColor"), value17);
	TrecPointer<TString> value18 = TrecPointerKey::GetNewTrecPointer<TString>("0.3,0.8,0.2,1.0");
	tcchildControl->addAttribute(TString(L"|ClickContentColor"), value18);


	//TrecPointer<TContainer> contain = new TContainer();

	//contain->setTControl();
	rootControl->addChild(childControl);

	rootControl->onCreate(area);
	firstDraw = true;
	this->UpdateAllViews(NULL);
	//firstDraw = false;
}


void CAnimateCentralDoc::OnSetuptcontrol2()
{
	rootControl.Delete();
	// TODO: Add your command handler code here
	TrecComPointer<ID2D1RenderTarget> rtb = rt;
	rootControl = TrecPointerKey::GetNewSelfTrecPointer<TControl>(rtb,table);
	//TControl* tcrootControl = rootControl.Get();

	TrecPointer<TString> value = TrecPointerKey::GetNewTrecPointer<TString>("50,50,80,80");
	rootControl->addAttribute(TString(L"|Margin"), value);
	TrecPointer<TString> value2 = TrecPointerKey::GetNewTrecPointer<TString>("3.0");
	rootControl->addAttribute(TString(L"|BorderThickness"), value2);
	TrecPointer<TString> value3 = TrecPointerKey::GetNewTrecPointer<TString>("0.1,1.0,0.1,1.0");
	rootControl->addAttribute(TString(L"|BorderColor"), value3);

	TrecPointer<TString> value4 = TrecPointerKey::GetNewTrecPointer<TString>("0.2,0.5,0.2,1.0");
	rootControl->addAttribute(TString(L"|ContentColor"), value4);

	TrecPointer<TString> value5 = TrecPointerKey::GetNewTrecPointer<TString>("DirectWrite");
	rootControl->addAttribute(TString(L"|Caption"), value5);
	TrecPointer<TString> value6 = TrecPointerKey::GetNewTrecPointer<TString>("0.9,0.3,0.2,0.7");
	rootControl->addAttribute(TString(L"|FontColor"), value6);
	//TrecPointer<TString> value7 = TrecPointerKey::GetNewTrecPointer<TString>("Ariel");
	//rootControl->addAttribute(TString(L"|Font"), value7);
	TrecPointer<TString> value8 = TrecPointerKey::GetNewTrecPointer<TString>("48.0");
	rootControl->addAttribute(TString(L"|FontSize"), value8);




	rootControl->addAttribute(TString(L"|HoverBorderThickness"), value2);

	rootControl->addAttribute(TString(L"|HoverBorderColor"), value3);

	TrecPointer<TString> value9 = TrecPointerKey::GetNewTrecPointer<TString>("0.2,0.9,0.2,1.0");
	rootControl->addAttribute(TString(L"|HoverContentColor"), value9);

	TrecPointer<TString> value10 = TrecPointerKey::GetNewTrecPointer<TString>("DirectWrite");
	rootControl->addAttribute(TString(L"|HoverCaption"), value10);
	TrecPointer<TString> value11 = TrecPointerKey::GetNewTrecPointer<TString>("0.1,0.1,0.9,0.8");
	rootControl->addAttribute(TString(L"|HoverFontColor"), value11);
	//TrecPointer<TString> value7 = new TString("Ariel");
	//rootControl->addAttribute(TString(L"|Font"), value7);



	rootControl->onCreate(area);
	firstDraw = true;
	this->UpdateAllViews(NULL);
	//firstDraw = false;
}


void CAnimateCentralDoc::OnSetuptcontrol3()
{
	rootControl.Delete();
	// TODO: Add your command handler code here
	TrecComPointer<ID2D1RenderTarget> rtb = rt;
	rootControl = TrecPointerKey::GetNewSelfTrecPointer<TControl>(rtb, table);
	//TControl *tcrootControl = rootControl.Get();

	TrecPointer<TString> value = TrecPointerKey::GetNewTrecPointer<TString>("20,20,30,30");
	rootControl->addAttribute(TString(L"|Margin"), value);
	TrecPointer<TString> value2 = TrecPointerKey::GetNewTrecPointer<TString>("3.0");
	rootControl->addAttribute(TString(L"|BorderThickness"), value2);
	TrecPointer<TString> value3 = TrecPointerKey::GetNewTrecPointer<TString>("0.1,1.0,0.1,1.0");
	rootControl->addAttribute(TString(L"|BorderColor"), value3);

	TrecPointer<TString> value4 = TrecPointerKey::GetNewTrecPointer<TString>("0.2,0.5,0.2,1.0");
	rootControl->addAttribute(TString(L"|ContentColor"), value4);

	TrecPointer<TString> value13 = TrecPointerKey::GetNewTrecPointer<TString>("0.6,1.0,0.6,1.0");
	rootControl->addAttribute(TString(L"|HoverBorderColor"), value3);

	TrecPointer<TString> value14 = TrecPointerKey::GetNewTrecPointer<TString>("0.1,0.6,0.1,1.0");
	rootControl->addAttribute(TString(L"|HoverContentColor"), value4);



	TrecPointer<TString> value5 = TrecPointerKey::GetNewTrecPointer<TString>("50,50,100,100");
	rootControl->addAttribute(TString(L"|ContainerLoc"),value5);


	// Prepare Child TControl
	TrecPointer<TControl> childControl = TrecPointerKey::GetNewSelfTrecPointer<TControl>(rtb, table);


	TrecPointer<TString> value6 = TrecPointerKey::GetNewTrecPointer<TString>("5.0");
	childControl->addAttribute(TString(L"|BorderThickness"), value6);
	TrecPointer<TString> value7 = TrecPointerKey::GetNewTrecPointer<TString>("0.1,1.0,0.1,1.0");
	childControl->addAttribute(TString(L"|BorderColor"), value7);
	TrecPointer<TString> value8 = TrecPointerKey::GetNewTrecPointer<TString>("0.8,0.2,0.2,1.0");
	childControl->addAttribute(TString(L"|ContentColor"), value8);
	TrecPointer<TString> value9 = TrecPointerKey::GetNewTrecPointer<TString>("3,3,3,3");
	childControl->addAttribute(TString(L"|Margin"), value9);


	childControl->addAttribute(TString(L"|HoverBorderThickness"), value6);
	TrecPointer<TString> value17 = TrecPointerKey::GetNewTrecPointer<TString>("0.1,1.0,1.1,0.9");
	childControl->addAttribute(TString(L"|HoverBorderColor"), value17);
	TrecPointer<TString> value18 = TrecPointerKey::GetNewTrecPointer<TString>("0.3,0.8,0.2,1.0");
	childControl->addAttribute(TString(L"|HoverContentColor"), value18);





	//TrecPointer<TContainer> contain = new TContainer();

	//contain->setTControl();
	rootControl->addChild(childControl);

	rootControl->onCreate(area);
	firstDraw = true;
	this->UpdateAllViews(NULL);
	//firstDraw = false;
}




void CAnimateCentralDoc::OnSetuptcontrol4()
{
	// TODO: Add your command handler code here
	rootControl.Delete();
	TrecComPointer<ID2D1RenderTarget> rtb = rt;

	rootControl = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TLayout>(rtb, TrecPointer<TArray<styleTable>>());
	TLayout* rootLayout = dynamic_cast<TLayout*>(rootControl.Get());
	RECT tLoc;

	tLoc.top = area.top;
	tLoc.bottom = area.bottom;
	tLoc.right = area.right;
	tLoc.left = area.left;
	rootLayout->setLocation(tLoc);
	rootLayout->setLayout(HStack);

	TrecPointer<TString> value = TrecPointerKey::GetNewTrecPointer<TString>("10,10,10,10");
	rootLayout->addAttribute(TString(L"|Margin"), value);

	TrecPointer<TString> value2 = TrecPointerKey::GetNewTrecPointer<TString>("3.0");
	rootLayout->addAttribute(TString(L"|BorderThickness"), value2);
	TrecPointer<TString> value3 = TrecPointerKey::GetNewTrecPointer<TString>("0.1,1.0,0.1,1.0");
	rootLayout->addAttribute(TString(L"|BorderColor"), value3);

	TrecPointer<TString> value4 = TrecPointerKey::GetNewTrecPointer<TString>("0.2,0.5,0.2,1.0");
	rootLayout->addAttribute(TString(L"|ContentColor"), value4);

	rootLayout->addColunm(150,true);
	
	rootLayout->addColunm(260,true);
	TrecPointer<TControl> childControl = TrecPointerKey::GetNewSelfTrecPointer<TControl>(rtb, table);
	TrecPointer<TString> value6 = TrecPointerKey::GetNewTrecPointer<TString>("5.0");
	childControl->addAttribute(TString(L"|BorderThickness"), value6);
	TrecPointer<TString> value7 = TrecPointerKey::GetNewTrecPointer<TString>("0.1,1.0,0.1,1.0");
	childControl->addAttribute(TString(L"|BorderColor"), value7);
	TrecPointer<TString> value8 = TrecPointerKey::GetNewTrecPointer<TString>("0.8,0.2,0.2,1.0");
	childControl->addAttribute(TString(L"|ContentColor"), value8);
	TrecPointer<TString> value9 = TrecPointerKey::GetNewTrecPointer<TString>("3,3,3,3");
	childControl->addAttribute(TString(L"|Margin"), value9);



	rootLayout->addChild(childControl , 0, 0);
	childControl = TrecPointerKey::GetNewSelfTrecPointer<TControl>(rtb, table);

	TrecPointer<TString> value11 = TrecPointerKey::GetNewTrecPointer<TString>("20,20,30,30");
	childControl->addAttribute(TString(L"|Margin"), value11);
	TrecPointer<TString> value12 = TrecPointerKey::GetNewTrecPointer<TString>("3.0");
	childControl->addAttribute(TString(L"|BorderThickness"), value12);
	TrecPointer<TString> value13 = TrecPointerKey::GetNewTrecPointer<TString>("0.1,1.0,0.1,1.0");
	childControl->addAttribute(TString(L"|BorderColor"), value13);

	TrecPointer<TString> value14 = TrecPointerKey::GetNewTrecPointer<TString>("0.2,0.5,0.2,1.0");
	childControl->addAttribute(TString(L"|ContentColor"), value14);

	TrecPointer<TString> value15 = TrecPointerKey::GetNewTrecPointer<TString>("DirectWrite");
	childControl->addAttribute(TString(L"|Caption"), value15);
	TrecPointer<TString> value16 = TrecPointerKey::GetNewTrecPointer<TString>("0.9,0.3,0.2,0.7");
	childControl->addAttribute(TString(L"|FontColor"), value16);
	//TrecPointer<TString> value7 = TrecPointerKey::GetNewTrecPointer<TString>("Ariel");
	//rootControl->addAttribute(TString(L"|Font"), value7);
	TrecPointer<TString> value18 = TrecPointerKey::GetNewTrecPointer<TString>("48.0");
	childControl->addAttribute(TString(L"|FontSize"), value18);
	rootLayout->addChild( childControl , 1, 0);

	rootLayout->onCreate(area);
	firstDraw = true;
	this->UpdateAllViews(NULL);
	//firstDraw = false;
}


void CAnimateCentralDoc::OnSetuptcontrol5()
{
	rootControl.Delete();
	// TODO: Add your command handler code here
	TrecComPointer<ID2D1RenderTarget> rtb = rt;
	rootControl = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TLayout>(rtb, TrecPointer<TArray<styleTable>>());
	TLayout* rootLayout = dynamic_cast<TLayout*>(rootControl.Get());

	TrecPointer<TString> value = TrecPointerKey::GetNewTrecPointer<TString>("10,10,10,10");
	rootLayout->addAttribute(TString(L"|Margin"), value);

	rootLayout->setLayout(VStack);
	TrecPointer<TString> value1 = TrecPointerKey::GetNewTrecPointer<TString>("5,5,5,5");
	rootLayout->addAttribute(TString(L"|Margin"), value1);
	TrecPointer<TString> value2 = TrecPointerKey::GetNewTrecPointer<TString>("3.0");
	rootLayout->addAttribute(TString(L"|BorderThickness"), value2);
	TrecPointer<TString> value3 = TrecPointerKey::GetNewTrecPointer<TString>("0.1,1.0,0.1,1.0");
	rootLayout->addAttribute(TString(L"|BorderColor"), value3);

	TrecPointer<TString> value4 = TrecPointerKey::GetNewTrecPointer<TString>("0.2,0.5,0.2,1.0");
	rootLayout->addAttribute(TString(L"|ContentColor"), value4);

	rootLayout->addRow(250,true);

	rootLayout->addRow(260,true);
	TrecPointer<TControl> childControl = TrecPointerKey::GetNewSelfTrecPointer<TControl>(rtb, table);
	TrecPointer<TString> value6 = TrecPointerKey::GetNewTrecPointer<TString>("5.0");
	childControl->addAttribute(TString(L"|BorderThickness"), value6);
	TrecPointer<TString> value7 = TrecPointerKey::GetNewTrecPointer<TString>("0.1,1.0,0.1,1.0");
	childControl->addAttribute(TString(L"|BorderColor"), value7);
	TrecPointer<TString> value8 = TrecPointerKey::GetNewTrecPointer<TString>("0.8,0.2,0.2,1.0");
	childControl->addAttribute(TString(L"|ContentColor"), value8);
	TrecPointer<TString> value9 = TrecPointerKey::GetNewTrecPointer<TString>("3,3,3,3");
	childControl->addAttribute(TString(L"|Margin"), value9);

	rootLayout->addChild(childControl, 0, 0);
	childControl = TrecPointerKey::GetNewSelfTrecPointer<TControl>(rtb, table);

	TrecPointer<TString> value11 = TrecPointerKey::GetNewTrecPointer<TString>("4,4,6,6");
	childControl->addAttribute(TString(L"|Margin"), value11);
	TrecPointer<TString> value12 = TrecPointerKey::GetNewTrecPointer<TString>("3.0");
	childControl->addAttribute(TString(L"|BorderThickness"), value12);
	TrecPointer<TString> value13 = TrecPointerKey::GetNewTrecPointer<TString>("0.1,1.0,0.1,1.0");
	childControl->addAttribute(TString(L"|BorderColor"), value13);

	TrecPointer<TString> value14 = TrecPointerKey::GetNewTrecPointer<TString>("0.2,0.5,0.2,1.0");
	childControl->addAttribute(TString(L"|ContentColor"), value14);

	TrecPointer<TString> value15 = TrecPointerKey::GetNewTrecPointer<TString>("DirectWrite");
	childControl->addAttribute(TString(L"|Caption"), value15);
	TrecPointer<TString> value16 = TrecPointerKey::GetNewTrecPointer<TString>("0.9,0.3,0.2,0.7");
	childControl->addAttribute(TString(L"|FontColor"), value16);
	//TrecPointer<TString> value7 = TrecPointerKey::GetNewTrecPointer<TString>("Ariel");
	//rootControl->addAttribute(TString(L"|Font"), value7);
	TrecPointer<TString> value18 = TrecPointerKey::GetNewTrecPointer<TString>("48.0");
	childControl->addAttribute(TString(L"|FontSize"), value18);
	rootLayout->addChild(childControl, 0, 1);


	rootLayout->onCreate(area);
	firstDraw = true;
	this->UpdateAllViews(NULL);
	//firstDraw = false;
}


void CAnimateCentralDoc::OnSetuptcontrol6()
{
	// TODO: Add your command handler code here
	rootControl.Delete();
	TrecComPointer<ID2D1RenderTarget> rtb = rt;
	rootControl = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TLayout>(rtb, TrecPointer<TArray<styleTable>>());
	TLayout* rootLayout = dynamic_cast<TLayout*>(rootControl.Get());
	rootLayout->setLayout(grid);


	TrecPointer<TString> value1 = TrecPointerKey::GetNewTrecPointer<TString>("5,5,5,5");
	rootLayout->addAttribute(TString(L"|Margin"), value1);
	TrecPointer<TString> value2 = TrecPointerKey::GetNewTrecPointer<TString>("3.0");
	rootLayout->addAttribute(TString(L"|BorderThickness"), value2);
	TrecPointer<TString> value3 = TrecPointerKey::GetNewTrecPointer<TString>("0.1,1.0,0.1,1.0");
	rootLayout->addAttribute(TString(L"|BorderColor"), value3);

	TrecPointer<TString> value4 = TrecPointerKey::GetNewTrecPointer<TString>("0.2,0.5,0.2,1.0");
	rootLayout->addAttribute(TString(L"|ContentColor"), value4);

	TDataArray<int> col, row;
	col.push_back(500);
	col.push_back(600);
	row.push_back(400);
	row.push_back(500);

	rootLayout->setGrid(col, row);
	TrecPointer<TControl> childControl = TrecPointerKey::GetNewSelfTrecPointer<TControl>(rtb, table);
	TrecPointer<TString> value6 = TrecPointerKey::GetNewTrecPointer<TString>("5.0");
	childControl->addAttribute(TString(L"|BorderThickness"), value6);
	TrecPointer<TString> value7 = TrecPointerKey::GetNewTrecPointer<TString>("0.1,1.0,0.1,1.0");
	childControl->addAttribute(TString(L"|BorderColor"), value7);
	TrecPointer<TString> value8 = TrecPointerKey::GetNewTrecPointer<TString>("0.8,0.2,0.2,1.0");
	childControl->addAttribute(TString(L"|ContentColor"), value8);
	TrecPointer<TString> value9 = TrecPointerKey::GetNewTrecPointer<TString>("3,3,3,3");
	childControl->addAttribute(TString(L"|Margin"), value9);

	rootLayout->addChild( childControl , 0, 0);
	childControl = TrecPointerKey::GetNewSelfTrecPointer<TControl>(rtb, table);

	TrecPointer<TString> value11 = TrecPointerKey::GetNewTrecPointer<TString>("2,2,3,3");
	childControl->addAttribute(TString(L"|Margin"), value11);
	TrecPointer<TString> value12 = TrecPointerKey::GetNewTrecPointer<TString>("3.0");
	childControl->addAttribute(TString(L"|BorderThickness"), value12);
	TrecPointer<TString> value13 = TrecPointerKey::GetNewTrecPointer<TString>("0.1,1.0,0.1,1.0");
	childControl->addAttribute(TString(L"|BorderColor"), value13);

	TrecPointer<TString> value14 = TrecPointerKey::GetNewTrecPointer<TString>("0.2,0.5,0.2,1.0");
	childControl->addAttribute(TString(L"|ContentColor"), value14);

	TrecPointer<TString> value15 = TrecPointerKey::GetNewTrecPointer<TString>("DirectWrite");
	childControl->addAttribute(TString(L"|Caption"), value15);
	TrecPointer<TString> value16 = TrecPointerKey::GetNewTrecPointer<TString>("0.9,0.3,0.2,0.7");
	childControl->addAttribute(TString(L"|FontColor"), value16);
	//TrecPointer<TString> value7 = TrecPointerKey::GetNewTrecPointer<TString>("Ariel");
	//rootControl->addAttribute(TString(L"|Font"), value7);
	TrecPointer<TString> value18 = TrecPointerKey::GetNewTrecPointer<TString>("48.0");
	childControl->addAttribute(TString(L"|FontSize"), value18);
	rootLayout->addChild(childControl, 0, 1);

	childControl = TrecPointerKey::GetNewSelfTrecPointer<TControl>(rtb, table);
	TrecPointer<TString> value21 = TrecPointerKey::GetNewTrecPointer<TString>("2,2,3,3");
	childControl->addAttribute(TString(L"|Margin"), value21);
	TrecPointer<TString> value22 = TrecPointerKey::GetNewTrecPointer<TString>("1.5");
	childControl->addAttribute(TString(L"|BorderThickness"), value22);
	TrecPointer<TString> value23 = TrecPointerKey::GetNewTrecPointer<TString>("0.4,1.0,0.1,1.0");
	childControl->addAttribute(TString(L"|BorderColor"), value23);

	TrecPointer<TString> value24 = TrecPointerKey::GetNewTrecPointer<TString>("0.2,0.5,0.6,1.0");
	childControl->addAttribute(TString(L"|ContentColor"), value24);

	TrecPointer<TString> value25 = TrecPointerKey::GetNewTrecPointer<TString>("Direct2D");
	childControl->addAttribute(TString(L"|Caption"), value25);
	TrecPointer<TString> value26 = TrecPointerKey::GetNewTrecPointer<TString>("0.9,0.9,0.2,0.7");
	childControl->addAttribute(TString(L"|FontColor"), value26);
	//TrecPointer<TString> value7 = TrecPointerKey::GetNewTrecPointer<TString>("Ariel");
	//rootControl->addAttribute(TString(L"|Font"), value7);
	TrecPointer<TString> value28 = TrecPointerKey::GetNewTrecPointer<TString>("36.0");
	childControl->addAttribute(TString(L"|FontSize"), value28);
	rootLayout->addChild(childControl , 1, 0);

	childControl = TrecPointerKey::GetNewSelfTrecPointer<TControl>(rtb, table);
	TrecPointer<TString> value36 = TrecPointerKey::GetNewTrecPointer<TString>("6.0");
	childControl->addAttribute(TString(L"|BorderThickness"), value36);
	TrecPointer<TString> value37 = TrecPointerKey::GetNewTrecPointer<TString>("0.1,1.0,1.0,1.0");
	childControl->addAttribute(TString(L"|BorderColor"), value37);
	TrecPointer<TString> value38 = TrecPointerKey::GetNewTrecPointer<TString>("0.2,0.8,0.2,1.0");
	childControl->addAttribute(TString(L"|ContentColor"), value38);
	TrecPointer<TString> value39 = TrecPointerKey::GetNewTrecPointer<TString>("3,3,3,3");
	childControl->addAttribute(TString(L"|Margin"), value39);

	rootLayout->addChild(childControl, 1, 1);

	rootLayout->onCreate(area);
	firstDraw = true;
	this->UpdateAllViews(NULL);
	//firstDraw = false;
}


void CAnimateCentralDoc::OnSetuptcontrolLayeredtlayout()
{
	// TODO: Add your command handler code here
	rootControl.Delete();
	TrecComPointer<ID2D1RenderTarget> rtb = rt;

	rootControl = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TLayout>(rtb, TrecPointer<TArray<styleTable>>());
	TLayout* rootLayout = dynamic_cast<TLayout*>(rootControl.Get());


	rootLayout->setLayout(VStack);
	TrecPointer<TString> value1 = TrecPointerKey::GetNewTrecPointer<TString>("5,5,5,5");
	rootLayout->addAttribute(TString(L"|Margin"), value1);
	TrecPointer<TString> value2 = TrecPointerKey::GetNewTrecPointer<TString>("3.0");
	rootLayout->addAttribute(TString(L"|BorderThickness"), value2);
	TrecPointer<TString> value3 = TrecPointerKey::GetNewTrecPointer<TString>("0.1,1.0,0.1,1.0");
	rootLayout->addAttribute(TString(L"|BorderColor"), value3);

	TrecPointer<TString> value4 = TrecPointerKey::GetNewTrecPointer<TString>("0.2,0.5,0.2,1.0");
	rootLayout->addAttribute(TString(L"|ContentColor"), value4);

	rootLayout->addRow(300,true);

	rootLayout->addRow(600,true);

	TrecPointer<TControl> childControl = TrecPointerKey::GetNewSelfTrecPointer<TControl>(rtb, table);

	TrecPointer<TString> value5 = TrecPointerKey::GetNewTrecPointer<TString>("5,5,5,5");
	childControl->addAttribute(TString(L"|Margin"), value5);
	TrecPointer<TString> value6 = TrecPointerKey::GetNewTrecPointer<TString>("3.0");
	childControl->addAttribute(TString(L"|BorderThickness"), value6);
	TrecPointer<TString> value7 = TrecPointerKey::GetNewTrecPointer<TString>("0.1,1.0,0.1,1.0");
	childControl->addAttribute(TString(L"|BorderColor"), value7);

	TrecPointer<TString> value8 = TrecPointerKey::GetNewTrecPointer<TString>("0.2,0.5,0.2,1.0");
	childControl->addAttribute(TString(L"|ContentColor"), value8);

	rootLayout->addChild(childControl, 0, 0);

	TrecPointer<TControl>childControl2 = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TLayout>(rtb, TrecPointer<TArray<styleTable>>());
	TLayout* childLayout = dynamic_cast<TLayout*>(childControl2.Get());
	childLayout->setLayout(grid);

	RECT tLoc;


	tLoc.top = area.top + 310;
	tLoc.bottom = area.bottom - 10;
	tLoc.right = area.right - 10;
	tLoc.left = area.left + 10;
	childLayout->setLocation(tLoc);

	TrecPointer<TString> value9 = TrecPointerKey::GetNewTrecPointer<TString>("3,3,3,3");
	childLayout->addAttribute(TString(L"|Margin"), value9);

	TDataArray<int> col, row;
	col.push_back(500);
	col.push_back(600);
	row.push_back(300);
	row.push_back(200);

	childLayout->setGrid(col, row);

	childControl = TrecPointerKey::GetNewSelfTrecPointer<TControl>(rtb, table);
	TrecPointer<TString> value46 = TrecPointerKey::GetNewTrecPointer<TString>("5.0");
	childControl->addAttribute(TString(L"|BorderThickness"), value46);
	TrecPointer<TString> value47 = TrecPointerKey::GetNewTrecPointer<TString>("0.1,1.0,0.1,1.0");
	childControl->addAttribute(TString(L"|BorderColor"), value47);
	TrecPointer<TString> value48 = TrecPointerKey::GetNewTrecPointer<TString>("0.8,0.2,0.2,1.0");
	childControl->addAttribute(TString(L"|ContentColor"), value48);
	TrecPointer<TString> value49 = TrecPointerKey::GetNewTrecPointer<TString>("3,3,3,3");
	childControl->addAttribute(TString(L"|Margin"), value49);

	childLayout->addChild( childControl , 0, 0);
	childControl = TrecPointerKey::GetNewSelfTrecPointer<TControl>(rtb, table);

	TrecPointer<TString> value11 = TrecPointerKey::GetNewTrecPointer<TString>("2,2,3,3");
	childControl->addAttribute(TString(L"|Margin"), value11);
	TrecPointer<TString> value12 = TrecPointerKey::GetNewTrecPointer<TString>("3.0");
	childControl->addAttribute(TString(L"|BorderThickness"), value12);
	TrecPointer<TString> value13 = TrecPointerKey::GetNewTrecPointer<TString>("0.1,1.0,0.1,1.0");
	childControl->addAttribute(TString(L"|BorderColor"), value13);

	TrecPointer<TString> value14 = TrecPointerKey::GetNewTrecPointer<TString>("0.2,0.5,0.2,1.0");
	childControl->addAttribute(TString(L"|ContentColor"), value14);

	TrecPointer<TString> value15 = TrecPointerKey::GetNewTrecPointer<TString>("DirectWrite");
	childControl->addAttribute(TString(L"|Caption"), value15);
	TrecPointer<TString> value16 = TrecPointerKey::GetNewTrecPointer<TString>("0.9,0.3,0.2,0.7");
	childControl->addAttribute(TString(L"|FontColor"), value16);
	//TrecPointer<TString> value7 = TrecPointerKey::GetNewTrecPointer<TString>("Ariel");
	//rootControl->addAttribute(TString(L"|Font"), value7);
	TrecPointer<TString> value18 = TrecPointerKey::GetNewTrecPointer<TString>("48.0");
	childControl->addAttribute(TString(L"|FontSize"), value18);
	childLayout->addChild( childControl , 0, 1);

	childControl = TrecPointerKey::GetNewSelfTrecPointer<TControl>(rtb, table);
	TrecPointer<TString> value21 = TrecPointerKey::GetNewTrecPointer<TString>("2,2,3,3");
	childControl->addAttribute(TString(L"|Margin"), value21);
	TrecPointer<TString> value22 = TrecPointerKey::GetNewTrecPointer<TString>("1.5");
	childControl->addAttribute(TString(L"|BorderThickness"), value22);
	TrecPointer<TString> value23 = TrecPointerKey::GetNewTrecPointer<TString>("0.4,1.0,0.1,1.0");
	childControl->addAttribute(TString(L"|BorderColor"), value23);

	TrecPointer<TString> value24 = TrecPointerKey::GetNewTrecPointer<TString>("0.2,0.5,0.6,1.0");
	childControl->addAttribute(TString(L"|ContentColor"), value24);

	TrecPointer<TString> value25 = TrecPointerKey::GetNewTrecPointer<TString>("Direct2D");
	childControl->addAttribute(TString(L"|Caption"), value25);
	TrecPointer<TString> value26 = TrecPointerKey::GetNewTrecPointer<TString>("0.9,0.9,0.2,0.7");
	childControl->addAttribute(TString(L"|FontColor"), value26);
	//TrecPointer<TString> value7 = TrecPointerKey::GetNewTrecPointer<TString>("Ariel");
	//rootControl->addAttribute(TString(L"|Font"), value7);
	TrecPointer<TString> value28 = TrecPointerKey::GetNewTrecPointer<TString>("36.0");
	childControl->addAttribute(TString(L"|FontSize"), value28);
	childLayout->addChild(childControl, 1, 0);

	childControl = TrecPointerKey::GetNewSelfTrecPointer<TControl>(rtb, table);
	TrecPointer<TString> value36 = TrecPointerKey::GetNewTrecPointer<TString>("6.0");
	childControl->addAttribute(TString(L"|BorderThickness"), value36);
	TrecPointer<TString> value37 = TrecPointerKey::GetNewTrecPointer<TString>("0.1,1.0,1.0,1.0");
	childControl->addAttribute(TString(L"|BorderColor"), value37);
	TrecPointer<TString> value38 = TrecPointerKey::GetNewTrecPointer<TString>("0.2,0.8,0.2,1.0");
	childControl->addAttribute(TString(L"|ContentColor"), value38);
	TrecPointer<TString> value39 = TrecPointerKey::GetNewTrecPointer<TString>("3,3,3,3");
	childControl->addAttribute(TString(L"|Margin"), value39);

	childLayout->addChild(childControl , 1, 1);
	rootLayout->addChild(childControl2 , 0, 1);
	rootLayout->onCreate(area);
	firstDraw = true;
	this->UpdateAllViews(NULL);
	//firstDraw = false;
}


void CAnimateCentralDoc::OnSetuptcontrolLayoutGadget()
{
	rootControl.Delete();
	TrecComPointer<ID2D1RenderTarget> rtb = rt;

	// TODO: Add your command handler code here
	rootControl = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TLayout>(rtb, TrecPointer<TArray<styleTable>>());
	TLayout* rootLayout = dynamic_cast<TLayout*>(rootControl.Get());
	rootLayout->setLayout(grid);

	TDataArray<int> col, row;
	col.push_back(500);
	col.push_back(500);
	row.push_back(250);
	row.push_back(250);
	row.push_back(250);

	rootLayout->setGrid(col, row);

	
	
	TrecPointer<TString> TStrHeight = TrecPointerKey::GetNewTrecPointer<TString>("150");
	TrecPointer<TString> TStrWidth = TrecPointerKey::GetNewTrecPointer<TString>("300");


	TrecPointer<TString> TStrCap1 = TrecPointerKey::GetNewTrecPointer<TString>("Class 1, Button 1");
	TrecPointer<TString> TStrCap2 = TrecPointerKey::GetNewTrecPointer<TString>("Class 1, Button 2");
	TrecPointer<TString> TStrCap3 = TrecPointerKey::GetNewTrecPointer<TString>("Check 1");
	TrecPointer<TString> TStrCap4 = TrecPointerKey::GetNewTrecPointer<TString>("Class 2, Button 1");
	TrecPointer<TString> TStrCap5 = TrecPointerKey::GetNewTrecPointer<TString>("Class 2, Button 2");
	TrecPointer<TString> TStrCap6 = TrecPointerKey::GetNewTrecPointer<TString>("Check 2");

	TrecPointer<TString> TStrClass1 = TrecPointerKey::GetNewTrecPointer<TString>("Class1");
	TrecPointer<TString> TStrClass2 = TrecPointerKey::GetNewTrecPointer<TString>("Class2");

	TrecPointer<TString> value2 = TrecPointerKey::GetNewTrecPointer<TString>("2.0");
	
	TrecPointer<TString> value3 = TrecPointerKey::GetNewTrecPointer<TString>("0.1,0.1,1.0,1.0");
	


	TrecPointer<TControl> trb = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TRadioButton>(rtb, TrecPointer<TArray<styleTable>>());
	trb->addAttribute(TString(L"|Width"), TStrWidth);
	trb->addAttribute(TString(L"|Height"), TStrHeight);
	trb->addAttribute(TString(L"|RadioClass"), TStrClass1);
	trb->addAttribute(TString(L"|Caption"), TStrCap1);
	trb->addAttribute(TString(L"|BorderThickness"), value2);
	trb->addAttribute(TString(L"|BorderColor"), value3);

	rootLayout->addChild(trb, 0, 0);

	trb = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TRadioButton>(rtb, TrecPointer<TArray<styleTable>>());
	trb->addAttribute(TString(L"|Width"), TStrWidth);
	trb->addAttribute(TString(L"|Height"), TStrHeight);
	trb->addAttribute(TString(L"|RadioClass"), TStrClass1);
	trb->addAttribute(TString(L"|Caption"), TStrCap2);
	trb->addAttribute(TString(L"|BorderThickness"), value2);
	trb->addAttribute(TString(L"|BorderColor"), value3);
	rootLayout->addChild(trb, 0, 1);



	TrecPointer<TControl> tcb = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TCheckBox>(rtb, TrecPointer<TArray<styleTable>>());
	tcb->addAttribute(TString(L"|Width"), TStrWidth);
	tcb->addAttribute(TString(L"|Height"), TStrHeight);

	tcb->addAttribute(TString(L"|Caption"), TStrCap3);

	rootLayout->addChild(tcb, 0, 2);





	trb = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TRadioButton>(rtb, TrecPointer<TArray<styleTable>>());
	trb->addAttribute(TString(L"|Width"), TStrWidth);
	trb->addAttribute(TString(L"|Height"), TStrHeight);
	trb->addAttribute(TString(L"|RadioClass"), TStrClass2);
	trb->addAttribute(TString(L"|Caption"), TStrCap4);
	trb->addAttribute(TString(L"|BorderThickness"), value2);
	trb->addAttribute(TString(L"|BorderColor"), value3);

	rootLayout->addChild(trb, 1, 0);

	trb = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TRadioButton>(rtb, TrecPointer<TArray<styleTable>>());
	trb->addAttribute(TString(L"|Width"), TStrWidth);
	trb->addAttribute(TString(L"|Height"), TStrHeight);
	trb->addAttribute(TString(L"|RadioClass"), TStrClass2);
	trb->addAttribute(TString(L"|Caption"), TStrCap5);
	trb->addAttribute(TString(L"|BorderThickness"), value2);
	trb->addAttribute(TString(L"|BorderColor"), value3);

	rootLayout->addChild(trb, 1, 1);

	tcb = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TCheckBox>(rtb, TrecPointer<TArray<styleTable>>());
	tcb->addAttribute(TString(L"|Width"), TStrWidth);
	tcb->addAttribute(TString(L"|Height"), TStrHeight);

	tcb->addAttribute(TString(L"|Caption"), TStrCap6);

	rootLayout->addChild(tcb, 1, 2);

	rootLayout->onCreate(area);
	firstDraw = true;
	this->UpdateAllViews(NULL);
}


void CAnimateCentralDoc::OnSetuptcontrolTextbox()
{
	rootControl.Delete();
	TrecComPointer<ID2D1RenderTarget> rtb = rt;

	if (!windowHandle)
		return;
	rootControl = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TLayout>(rtb, TrecPointer<TArray<styleTable>>());
	TLayout* rootLayout = dynamic_cast<TLayout*>(rootControl.Get());
	rootLayout->setLayout(grid);

	TDataArray<int> col, row;
	col.push_back(500);
	col.push_back(500);
	row.push_back(250);
	row.push_back(250);
	row.push_back(250);

	rootLayout->setGrid(col, row);



	TrecPointer<TString> TStrHeight = TrecPointerKey::GetNewTrecPointer<TString>("150");
	TrecPointer<TString> TStrWidth = TrecPointerKey::GetNewTrecPointer<TString>("300");

	TrecPointer<TControl> field = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TTextField>(rtb, TrecPointer<TArray<styleTable>>(), windowHandle);
	field->addAttribute(TString(L"|IsPassword"), TrecPointerKey::GetNewTrecPointer<TString>("False"));
	field->addAttribute(TString(L"|CanEdit"), TrecPointerKey::GetNewTrecPointer<TString>("True"));
	field->addAttribute(TString(L"|BorderThickness"), TrecPointerKey::GetNewTrecPointer<TString>("0.9"));
	field->addAttribute(TString(L"|IsNumberControl"), TrecPointerKey::GetNewTrecPointer<TString>("False"));

	rootLayout->addChild(field, 0, 0);
	field = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TTextField>(rtb, TrecPointer<TArray<styleTable>>(), windowHandle);

	
	field->addAttribute(TString(L"|CanEdit"), TrecPointerKey::GetNewTrecPointer<TString>("True"));
	field->addAttribute(TString(L"|BorderThickness"), TrecPointerKey::GetNewTrecPointer<TString>("0.9"));
	field->addAttribute(TString(L"|IsPassword"), TrecPointerKey::GetNewTrecPointer<TString>("False"));
	field->addAttribute(TString(L"|IsNumberControl"), TrecPointerKey::GetNewTrecPointer<TString>("False"));

	rootLayout->addChild(TrecPointer<TControl>(field), 1, 0);
	field = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TTextField>(rtb, TrecPointer<TArray<styleTable>>(), windowHandle);


	field->addAttribute(TString(L"|CanEdit"), TrecPointerKey::GetNewTrecPointer<TString>("True"));
	field->addAttribute(TString(L"|BorderThickness"), TrecPointerKey::GetNewTrecPointer<TString>("0.9"));
	field->addAttribute(TString(L"|IsPassword"), TrecPointerKey::GetNewTrecPointer<TString>("False"));
	field->addAttribute(TString(L"|IsNumberControl"), TrecPointerKey::GetNewTrecPointer<TString>("True"));
	field->addAttribute(TString(L"|DefaultIncriment"), TrecPointerKey::GetNewTrecPointer<TString>("1.5"));

	rootLayout->addChild(TrecPointer<TControl>(field), 0, 1);
	field = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TTextField>(rtb, TrecPointer<TArray<styleTable>>(), windowHandle);

	field->addAttribute(TString(L"|CanEdit"), TrecPointerKey::GetNewTrecPointer<TString>("True"));
	field->addAttribute(TString(L"|BorderThickness"), TrecPointerKey::GetNewTrecPointer<TString>("0.9"));
	field->addAttribute(TString(L"|IsPassword"), TrecPointerKey::GetNewTrecPointer<TString>("False"));
	field->addAttribute(TString(L"|IsNumberControl"), TrecPointerKey::GetNewTrecPointer<TString>("True"));
	field->addAttribute(TString(L"|DefaultIncriment"), TrecPointerKey::GetNewTrecPointer<TString>("2"));

	rootLayout->addChild(TrecPointer<TControl>(field), 1, 1);
	field = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TTextField>(rtb, TrecPointer<TArray<styleTable>>(), windowHandle);

	field->addAttribute(TString(L"|CanEdit"), TrecPointerKey::GetNewTrecPointer<TString>("True"));
	field->addAttribute(TString(L"|BorderThickness"), TrecPointerKey::GetNewTrecPointer<TString>("0.9"));
	field->addAttribute(TString(L"|IsPassword"), TrecPointerKey::GetNewTrecPointer<TString>("True"));
	field->addAttribute(TString(L"|PasswordPeek"), TrecPointerKey::GetNewTrecPointer<TString>("False"));

	rootLayout->addChild(TrecPointer<TControl>(field), 0, 2);
	field = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TTextField>(rtb, TrecPointer<TArray<styleTable>>(), windowHandle);

	field->addAttribute(TString(L"|CanEdit"), TrecPointerKey::GetNewTrecPointer<TString>("True"));
	field->addAttribute(TString(L"|BorderThickness"), TrecPointerKey::GetNewTrecPointer<TString>("0.9"));
	field->addAttribute(TString(L"|IsPassword"), TrecPointerKey::GetNewTrecPointer<TString>("True"));
	field->addAttribute(TString(L"|PasswordPeek"), TrecPointerKey::GetNewTrecPointer<TString>("True"));

	rootLayout->addChild(TrecPointer<TControl>(field), 1, 2);
	field = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TTextField>(rtb, TrecPointer<TArray<styleTable>>(), windowHandle);


	rootLayout->onCreate(area);
	firstDraw = true;
	this->UpdateAllViews(NULL);
}

//    field->addAttribute(TString(L""), new TString(""));


void CAnimateCentralDoc::OnSetuptcontrolCombobox()
{
	rootControl.Delete();
	TrecComPointer<ID2D1RenderTarget> rtb = rt;

	// TODO: Add your command handler code here
	rootControl = TrecPointerKey::GetNewSelfTrecPointer<TControl>(rtb, table);
	//TControl* tcrootControl = rootControl.Get();


	TrecPointer<TString> value = TrecPointerKey::GetNewTrecPointer<TString>("20,20,30,30");
	rootControl->addAttribute(TString(L"|Margin"), value);
	TrecPointer<TString> value2 = TrecPointerKey::GetNewTrecPointer<TString>("3.0");
	rootControl->addAttribute(TString(L"|BorderThickness"), value2);
	TrecPointer<TString> value3 = TrecPointerKey::GetNewTrecPointer<TString>("0.1,1.0,0.1,1.0");
	rootControl->addAttribute(TString(L"|BorderColor"), value3);

	TrecPointer<TString> value4 = TrecPointerKey::GetNewTrecPointer<TString>("0.2,0.5,0.2,1.0");
	rootControl->addAttribute(TString(L"|ContentColor"), value4);

	rootControl->addAttribute(TString(L"|ContainerLoc"), TrecPointerKey::GetNewTrecPointer<TString>("70,70,200,100"));

	TrecPointer<TControl> combo = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TComboBox>(rtb, TrecPointer<TArray<styleTable>>());

	combo->addAttribute(TString(L"|Height"), TrecPointerKey::GetNewTrecPointer<TString>("80"));
	combo->addAttribute(TString(L"|BorderColor"), TrecPointerKey::GetNewTrecPointer<TString>("1.0,0.0,0.0,1.0"));
	combo->addAttribute(TString(L"|Width"), TrecPointerKey::GetNewTrecPointer<TString>("250"));
	combo->addAttribute(TString(L"|SubHeight"), TrecPointerKey::GetNewTrecPointer<TString>("70"));
	combo->addAttribute(TString(L"|DefaultText"), TrecPointerKey::GetNewTrecPointer<TString>("Make Selection"));
	combo->addAttribute(TString(L"|BoxEntry"), TrecPointerKey::GetNewTrecPointer<TString>("Sel 1"));
	combo->addAttribute(TString(L"|BoxEntry"), TrecPointerKey::GetNewTrecPointer<TString>("Sel 2"));
	combo->addAttribute(TString(L"|BoxEntry"), TrecPointerKey::GetNewTrecPointer<TString>("Sel 3"));

	//TrecPointer<TContainer> contain = new TContainer();

	//contain->setTControl();
	rootControl->addChild(TrecPointer<TControl>(combo));

	bool res = rootControl->onCreate(area);
	firstDraw = true;
	this->UpdateAllViews(NULL);
}


void CAnimateCentralDoc::OnSetuptcontrolScrolltest()
{
	rootControl.Delete();
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	TrecComPointer<ID2D1RenderTarget> rtb = rt;
	rootControl = TrecPointerKey::GetNewSelfTrecPointer<TControl>(rtb, table);
	//TControl *tcrootControl = rootControl.Get();

	TrecPointer<TString> value = TrecPointerKey::GetNewTrecPointer<TString>("20,20,30,30");
	rootControl->addAttribute(TString(L"|Margin"), value);
	TrecPointer<TString> value2 = TrecPointerKey::GetNewTrecPointer<TString>("3.0");
	rootControl->addAttribute(TString(L"|BorderThickness"), value2);
	TrecPointer<TString> value3 = TrecPointerKey::GetNewTrecPointer<TString>("0.1,1.0,0.1,1.0");
	rootControl->addAttribute(TString(L"|BorderColor"), value3);

	TrecPointer<TString> value4 = TrecPointerKey::GetNewTrecPointer<TString>("0.2,0.5,0.2,1.0");
	rootControl->addAttribute(TString(L"|ContentColor"), value4);

	TrecPointer<TString> value13 = TrecPointerKey::GetNewTrecPointer<TString>("0.6,1.0,0.6,1.0");
	rootControl->addAttribute(TString(L"|HoverBorderColor"), value3);

	TrecPointer<TString> value14 = TrecPointerKey::GetNewTrecPointer<TString>("0.1,0.6,0.1,1.0");
	rootControl->addAttribute(TString(L"|HoverContentColor"), value4);



	TrecPointer<TString> value5 = TrecPointerKey::GetNewTrecPointer<TString>("50,50,200,200");
	rootControl->addAttribute(TString(L"|ContainerLoc"), value5);


	// Prepare Child TControl
	TrecPointer<TControl> childControl = TrecPointerKey::GetNewSelfTrecPointer<TControl>(rtb, table);


	TrecPointer<TString> value6 = TrecPointerKey::GetNewTrecPointer<TString>("5.0");
	childControl->addAttribute(TString(L"|BorderThickness"), value6);
	TrecPointer<TString> value7 = TrecPointerKey::GetNewTrecPointer<TString>("0.1,1.0,0.1,1.0");
	childControl->addAttribute(TString(L"|BorderColor"), value7);
	TrecPointer<TString> value8 = TrecPointerKey::GetNewTrecPointer<TString>("0.8,0.2,0.2,1.0");
	childControl->addAttribute(TString(L"|ContentColor"), value8);
	TrecPointer<TString> value9 = TrecPointerKey::GetNewTrecPointer<TString>("3,3,3,3");
	childControl->addAttribute(TString(L"|Margin"), value9);


	childControl->addAttribute(TString(L"|HoverBorderThickness"), value6);
	TrecPointer<TString> value17 = TrecPointerKey::GetNewTrecPointer<TString>("0.1,1.0,1.1,0.9");
	childControl->addAttribute(TString(L"|HoverBorderColor"), value17);
	TrecPointer<TString> value18 = TrecPointerKey::GetNewTrecPointer<TString>("0.3,0.8,0.2,1.0");
	childControl->addAttribute(TString(L"|HoverContentColor"), value18);

	childControl->addAttribute(TString(L"|Height"), TrecPointerKey::GetNewTrecPointer<TString>("1000"));
	childControl->addAttribute(TString(L"|Width"), TrecPointerKey::GetNewTrecPointer<TString>("1000"));
	childControl->addAttribute(TString(L"|VerticalScroll"), TrecPointerKey::GetNewTrecPointer<TString>("True"));
	childControl->addAttribute(TString(L"|HorizontalScroll"), TrecPointerKey::GetNewTrecPointer<TString>("True"));



	//TrecPointer<TContainer> contain = new TContainer();

	//contain->setTControl();
	rootControl->addChild(TrecPointer<TControl>(childControl));

	rootControl->onCreate( area);
	firstDraw = true;
	this->UpdateAllViews(NULL);
	//firstDraw = false;
}
