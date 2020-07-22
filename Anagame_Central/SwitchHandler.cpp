#include "SwitchHandler.h"
#include <Page.h>
#include <TWindow.h>
#include <DirectoryInterface.h>

SwitchHandler::SwitchHandler(TrecPointer<TInstance> ins): EventHandler(ins)
{ 
	//ZeroMemory(&changeControl, sizeof(changeControl));
	
	rootLayout = nullptr;

	eventNameID enid;

	enid.eventID = 0;
	enid.name.Set(L"OnSelect1");
	events.push_back(enid);
	handlers[enid.eventID] = &SwitchHandler::OnSelect1;

	enid.eventID = 1;
	enid.name.Set(L"OnSelect2");
	events.push_back(enid);
	handlers[enid.eventID] = &SwitchHandler::OnSelect2;

	enid.eventID = 2;
	enid.name.Set(L"OnSelect3");
	events.push_back(enid);
	handlers[enid.eventID] = &SwitchHandler::OnSelect3;

	enid.eventID = 3;
	enid.name.Set(L"OnSelectRows");
	events.push_back(enid);
	handlers[enid.eventID] = &SwitchHandler::OnSelectRows;

	enid.eventID = 4;
	enid.name.Set(L"OnSelectColunm");
	events.push_back(enid);
	handlers[enid.eventID] = &SwitchHandler::OnSelectColumns;

	enid.eventID = 5;
	enid.name.Set(L"OnSelectGrid");
	events.push_back(enid);
	handlers[enid.eventID] = &SwitchHandler::OnSelectGrid;

	enid.eventID = 6;
	enid.name.Set(L"OnSelectLayers");
	events.push_back(enid);
	handlers[enid.eventID] = &SwitchHandler::OnSelectLayers;

	enid.eventID = 7;
	enid.name.Set(L"OnSelectGadget");
	events.push_back(enid);
	handlers[enid.eventID] = &SwitchHandler::OnSelectGadget;

	enid.eventID = 8;
	enid.name.Set(L"OnSelectText");
	events.push_back(enid);
	handlers[enid.eventID] = &SwitchHandler::OnSelectText;

	enid.eventID = 9;
	enid.name.Set(L"OnSelectCombobox");
	events.push_back(enid);
	handlers[enid.eventID] = &SwitchHandler::OnSelectCombo;

	enid.eventID = 10;
	enid.name.Set(L"OnSelectScroll");
	events.push_back(enid);
	handlers[enid.eventID] = &SwitchHandler::OnSelectScroll;

	enid.eventID = 11;
	enid.name.Set(L"OnSelectGif");
	events.push_back(enid);
	handlers[enid.eventID] = &SwitchHandler::OnSelectGif;

}

SwitchHandler::~SwitchHandler()
{
}

void SwitchHandler::Initialize(TrecPointer<Page> page)
{
	if (!page.Get())
		throw L"Error! Expected an actual Page Pointer to be provided!";

	this->page = page;

	TrecPointer<TControl> rootControl = page->GetRootControl();

	rootLayout = dynamic_cast<TLayout*>(rootControl.Get());

	if (!rootLayout)
		throw L"Error! Root Control of App expected to be a Layout";

	changeControl = rootLayout->GetLayoutChild(1, 0);

	if (!changeControl.Get())
	{
		throw L"Error! Expected A TControl to change";
	}

	TString gifStr(L"GifRunner");
	page->AddStory(gifStr,true);

	auto gifBuilder = TrecPointerKey::GetNewTrecPointer<AnimationBuilder>();
	gifBuilder->SetName(TString(L"Gif"));
	gifBuilder->SetType(TString(L"Gif"));
	gifBuilder->SetAttribute(TString(L"|RefreshRate"), TrecPointerKey::GetNewTrecPointer<TString>(L"100"));
	page->AddAnimations(gifBuilder);
}

void SwitchHandler::HandleEvents(TDataArray<EventID_Cred>& eventAr)
{
	for (UINT Rust = 0; Rust < eventAr.Size(); Rust++)
	{
		auto cont = eventAr[Rust].control;
		if (!cont.Get()) continue;

		EventArgs ea = cont->getEventArgs();
		

		int ea_id = ea.methodID;

		if (ea_id > -1 && ea_id < SWITCH_HANDLER_METHOD_COUNT)
		{
			if (handlers[ea_id])
				(this->*handlers[ea_id])(cont, ea);
		}
		cont->resetArgs();
	}
}

void SwitchHandler::ProcessMessage(TrecPointer<HandlerMessage> message)
{
}

void SwitchHandler::OnSelect1(TrecPointer<TControl> tc, EventArgs ea)
{
	TrecPointer<DrawingBoard> rtb = page->GetWindowHandle()->GetDrawingBoard();

	// changeControl.Delete();

	changeControl = TrecPointerKey::GetNewSelfTrecPointer<TControl>(rtb, TrecPointer<TArray<styleTable>>());
	//TrecPointer<TControl> tcchangeControl = changeControl.Get();
	TrecPointer<TString> value = TrecPointerKey::GetNewTrecPointer<TString>("10,10,30,30");
	changeControl->addAttribute(TString(L"|Margin"), value);
	TrecPointer<TString> value2 = TrecPointerKey::GetNewTrecPointer<TString>("2.0");
	changeControl->addAttribute(TString(L"|BorderThickness"), value2);
	TrecPointer<TString> value3 = TrecPointerKey::GetNewTrecPointer<TString>("0.1,0.1,1.0,1.0");
	changeControl->addAttribute(TString(L"|BorderColor"), value3);

	TrecPointer<TString> value4 = TrecPointerKey::GetNewTrecPointer<TString>("0.2,0.2,0.5,1.0");
	changeControl->addAttribute(TString(L"|ContentColor"), value4);


	TrecPointer<TString> value13 = TrecPointerKey::GetNewTrecPointer<TString>("0.1,0.6,1.0,1.0");
	changeControl->addAttribute(TString(L"|ClickBorderColor"), value13);

	TrecPointer<TString> value14 = TrecPointerKey::GetNewTrecPointer<TString>("0.1,0.1,0.7,1.0");
	changeControl->addAttribute(TString(L"|ClickContentColor"), value14);

	TrecPointer<TString> value5 = TrecPointerKey::GetNewTrecPointer<TString>("50,50,100,100");
	changeControl->addAttribute(TString(L"|ContainerLoc"), value5);


	// Prepare Child TControl
	TrecPointer<TControl> childControl = TrecPointerKey::GetNewSelfTrecPointer<TControl>(rtb, TrecPointer<TArray<styleTable>>());
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
	changeControl->addChild(childControl);

	rootLayout->addChild(changeControl, 1, 0);
	page->CreateLayout();
}

void SwitchHandler::OnSelect2(TrecPointer<TControl> tc, EventArgs ea)
{
	TrecPointer<DrawingBoard> rtb = page->GetWindowHandle()->GetDrawingBoard();
	// changeControl.Delete();
	changeControl = TrecPointerKey::GetNewSelfTrecPointer<TControl>(rtb,TrecPointer<TArray<styleTable>>());
	//TrecPointer<TControl> tcchangeControl = changeControl.Get();

	TrecPointer<TString> value = TrecPointerKey::GetNewTrecPointer<TString>("50,50,80,80");
	changeControl->addAttribute(TString(L"|Margin"), value);
	TrecPointer<TString> value2 = TrecPointerKey::GetNewTrecPointer<TString>("3.0");
	changeControl->addAttribute(TString(L"|BorderThickness"), value2);
	TrecPointer<TString> value3 = TrecPointerKey::GetNewTrecPointer<TString>("0.1,1.0,0.1,1.0");
	changeControl->addAttribute(TString(L"|BorderColor"), value3);

	TrecPointer<TString> value4 = TrecPointerKey::GetNewTrecPointer<TString>("0.2,0.5,0.2,1.0");
	changeControl->addAttribute(TString(L"|ContentColor"), value4);

	TrecPointer<TString> value5 = TrecPointerKey::GetNewTrecPointer<TString>("DirectWrite");
	changeControl->addAttribute(TString(L"|Caption"), value5);
	TrecPointer<TString> value6 = TrecPointerKey::GetNewTrecPointer<TString>("0.9,0.3,0.2,0.7");
	changeControl->addAttribute(TString(L"|FontColor"), value6);
	//TrecPointer<TString> value7 = TrecPointerKey::GetNewTrecPointer<TString>("Ariel");
	//changeControl->addAttribute(TString(L"|Font"), value7);
	TrecPointer<TString> value8 = TrecPointerKey::GetNewTrecPointer<TString>("48.0");
	changeControl->addAttribute(TString(L"|FontSize"), value8);




	changeControl->addAttribute(TString(L"|HoverBorderThickness"), value2);

	changeControl->addAttribute(TString(L"|HoverBorderColor"), value3);

	TrecPointer<TString> value9 = TrecPointerKey::GetNewTrecPointer<TString>("0.2,0.9,0.2,1.0");
	changeControl->addAttribute(TString(L"|HoverContentColor"), value9);

	TrecPointer<TString> value10 = TrecPointerKey::GetNewTrecPointer<TString>("DirectWrite");
	changeControl->addAttribute(TString(L"|HoverCaption"), value10);
	TrecPointer<TString> value11 = TrecPointerKey::GetNewTrecPointer<TString>("0.1,0.1,0.9,0.8");
	changeControl->addAttribute(TString(L"|HoverFontColor"), value11);
	rootLayout->addChild(changeControl, 1, 0);
	page->CreateLayout();
}

void SwitchHandler::OnSelect3(TrecPointer<TControl> tc, EventArgs ea)
{
	TrecPointer<DrawingBoard> rtb = page->GetWindowHandle()->GetDrawingBoard();
	// changeControl.Delete();
	changeControl = TrecPointerKey::GetNewSelfTrecPointer<TControl>(rtb,TrecPointer<TArray<styleTable>>());
	//TControl *tcchangeControl = changeControl.Get();

	TrecPointer<TString> value = TrecPointerKey::GetNewTrecPointer<TString>("20,20,30,30");
	changeControl->addAttribute(TString(L"|Margin"), value);
	TrecPointer<TString> value2 = TrecPointerKey::GetNewTrecPointer<TString>("3.0");
	changeControl->addAttribute(TString(L"|BorderThickness"), value2);
	TrecPointer<TString> value3 = TrecPointerKey::GetNewTrecPointer<TString>("0.1,1.0,0.1,1.0");
	changeControl->addAttribute(TString(L"|BorderColor"), value3);

	TrecPointer<TString> value4 = TrecPointerKey::GetNewTrecPointer<TString>("0.2,0.5,0.2,1.0");
	changeControl->addAttribute(TString(L"|ContentColor"), value4);

	TrecPointer<TString> value13 = TrecPointerKey::GetNewTrecPointer<TString>("0.6,1.0,0.6,1.0");
	changeControl->addAttribute(TString(L"|HoverBorderColor"), value3);

	TrecPointer<TString> value14 = TrecPointerKey::GetNewTrecPointer<TString>("0.1,0.6,0.1,1.0");
	changeControl->addAttribute(TString(L"|HoverContentColor"), value4);



	TrecPointer<TString> value5 = TrecPointerKey::GetNewTrecPointer<TString>("50,50,100,100");
	changeControl->addAttribute(TString(L"|ContainerLoc"), value5);


	// Prepare Child TControl
	TrecPointer<TControl> childControl = TrecPointerKey::GetNewSelfTrecPointer<TControl>(rtb,TrecPointer<TArray<styleTable>>());


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
	changeControl->addChild(childControl);
	rootLayout->addChild(changeControl, 1, 0);
	page->CreateLayout();
}

void SwitchHandler::OnSelectRows(TrecPointer<TControl> tc, EventArgs ea)
{
	TrecPointer<DrawingBoard> rtb = page->GetWindowHandle()->GetDrawingBoard();
	// changeControl.Delete();
	changeControl = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TLayout>(rtb, TrecPointer<TArray<styleTable>>());
	TLayout* rootLayout = dynamic_cast<TLayout*>(changeControl.Get());
	D2D1_RECT_F tLoc = this->rootLayout->getRawSectionLocation(0,1);


	rootLayout->setLocation(tLoc);
	rootLayout->setLayout(orgLayout::HStack);

	TrecPointer<TString> value = TrecPointerKey::GetNewTrecPointer<TString>("10,10,10,10");
	rootLayout->addAttribute(TString(L"|Margin"), value);

	TrecPointer<TString> value2 = TrecPointerKey::GetNewTrecPointer<TString>("3.0");
	rootLayout->addAttribute(TString(L"|BorderThickness"), value2);
	TrecPointer<TString> value3 = TrecPointerKey::GetNewTrecPointer<TString>("0.1,1.0,0.1,1.0");
	rootLayout->addAttribute(TString(L"|BorderColor"), value3);

	TrecPointer<TString> value4 = TrecPointerKey::GetNewTrecPointer<TString>("0.2,0.5,0.2,1.0");
	rootLayout->addAttribute(TString(L"|ContentColor"), value4);

	rootLayout->addColunm(150, true);

	rootLayout->addColunm(260, true);
	TrecPointer<TControl> childControl = TrecPointerKey::GetNewSelfTrecPointer<TControl>(rtb,TrecPointer<TArray<styleTable>>());
	TrecPointer<TString> value6 = TrecPointerKey::GetNewTrecPointer<TString>("5.0");
	childControl->addAttribute(TString(L"|BorderThickness"), value6);
	TrecPointer<TString> value7 = TrecPointerKey::GetNewTrecPointer<TString>("0.1,1.0,0.1,1.0");
	childControl->addAttribute(TString(L"|BorderColor"), value7);
	TrecPointer<TString> value8 = TrecPointerKey::GetNewTrecPointer<TString>("0.8,0.2,0.2,1.0");
	childControl->addAttribute(TString(L"|ContentColor"), value8);
	TrecPointer<TString> value9 = TrecPointerKey::GetNewTrecPointer<TString>("3,3,3,3");
	childControl->addAttribute(TString(L"|Margin"), value9);



	rootLayout->addChild(childControl, 0, 0);
	childControl = TrecPointerKey::GetNewSelfTrecPointer<TControl>(rtb,TrecPointer<TArray<styleTable>>());

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
	//changeControl->addAttribute(TString(L"|Font"), value7);
	TrecPointer<TString> value18 = TrecPointerKey::GetNewTrecPointer<TString>("48.0");
	childControl->addAttribute(TString(L"|FontSize"), value18);
	this->rootLayout->addChild(changeControl, 1, 0);

	page->CreateLayout();
}

void SwitchHandler::OnSelectColumns(TrecPointer<TControl> tc, EventArgs ea)
{
	TrecPointer<DrawingBoard> rtb = page->GetWindowHandle()->GetDrawingBoard();
	// changeControl.Delete();
	changeControl = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TLayout>(rtb, TrecPointer<TArray<styleTable>>());
	TLayout* rootLayout = dynamic_cast<TLayout*>(changeControl.Get());

	TrecPointer<TString> value = TrecPointerKey::GetNewTrecPointer<TString>("10,10,10,10");
	rootLayout->addAttribute(TString(L"|Margin"), value);

	rootLayout->setLayout(orgLayout::VStack);
	TrecPointer<TString> value1 = TrecPointerKey::GetNewTrecPointer<TString>("5,5,5,5");
	rootLayout->addAttribute(TString(L"|Margin"), value1);
	TrecPointer<TString> value2 = TrecPointerKey::GetNewTrecPointer<TString>("3.0");
	rootLayout->addAttribute(TString(L"|BorderThickness"), value2);
	TrecPointer<TString> value3 = TrecPointerKey::GetNewTrecPointer<TString>("0.1,1.0,0.1,1.0");
	rootLayout->addAttribute(TString(L"|BorderColor"), value3);

	TrecPointer<TString> value4 = TrecPointerKey::GetNewTrecPointer<TString>("0.2,0.5,0.2,1.0");
	rootLayout->addAttribute(TString(L"|ContentColor"), value4);

	rootLayout->addRow(250, true);

	rootLayout->addRow(260, true);
	TrecPointer<TControl> childControl = TrecPointerKey::GetNewSelfTrecPointer<TControl>(rtb,TrecPointer<TArray<styleTable>>());
	TrecPointer<TString> value6 = TrecPointerKey::GetNewTrecPointer<TString>("5.0");
	childControl->addAttribute(TString(L"|BorderThickness"), value6);
	TrecPointer<TString> value7 = TrecPointerKey::GetNewTrecPointer<TString>("0.1,1.0,0.1,1.0");
	childControl->addAttribute(TString(L"|BorderColor"), value7);
	TrecPointer<TString> value8 = TrecPointerKey::GetNewTrecPointer<TString>("0.8,0.2,0.2,1.0");
	childControl->addAttribute(TString(L"|ContentColor"), value8);
	TrecPointer<TString> value9 = TrecPointerKey::GetNewTrecPointer<TString>("3,3,3,3");
	childControl->addAttribute(TString(L"|Margin"), value9);

	rootLayout->addChild(childControl, 0, 0);
	childControl = TrecPointerKey::GetNewSelfTrecPointer<TControl>(rtb,TrecPointer<TArray<styleTable>>());

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
	//changeControl->addAttribute(TString(L"|Font"), value7);
	TrecPointer<TString> value18 = TrecPointerKey::GetNewTrecPointer<TString>("48.0");
	childControl->addAttribute(TString(L"|FontSize"), value18);
	rootLayout->addChild(childControl, 0, 1);
	this->rootLayout->addChild(changeControl, 1, 0);
	page->CreateLayout();
}

void SwitchHandler::OnSelectGrid(TrecPointer<TControl> tc, EventArgs ea)
{
	TrecPointer<DrawingBoard> rtb = page->GetWindowHandle()->GetDrawingBoard();
	// changeControl.Delete();
	changeControl = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TLayout>(rtb, TrecPointer<TArray<styleTable>>());
	TLayout* rootLayout = dynamic_cast<TLayout*>(changeControl.Get());
	rootLayout->setLayout(orgLayout::grid);


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
	TrecPointer<TControl> childControl = TrecPointerKey::GetNewSelfTrecPointer<TControl>(rtb,TrecPointer<TArray<styleTable>>());
	TrecPointer<TString> value6 = TrecPointerKey::GetNewTrecPointer<TString>("5.0");
	childControl->addAttribute(TString(L"|BorderThickness"), value6);
	TrecPointer<TString> value7 = TrecPointerKey::GetNewTrecPointer<TString>("0.1,1.0,0.1,1.0");
	childControl->addAttribute(TString(L"|BorderColor"), value7);
	TrecPointer<TString> value8 = TrecPointerKey::GetNewTrecPointer<TString>("0.8,0.2,0.2,1.0");
	childControl->addAttribute(TString(L"|ContentColor"), value8);
	TrecPointer<TString> value9 = TrecPointerKey::GetNewTrecPointer<TString>("3,3,3,3");
	childControl->addAttribute(TString(L"|Margin"), value9);

	rootLayout->addChild(childControl, 0, 0);
	childControl = TrecPointerKey::GetNewSelfTrecPointer<TControl>(rtb,TrecPointer<TArray<styleTable>>());

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
	//changeControl->addAttribute(TString(L"|Font"), value7);
	TrecPointer<TString> value18 = TrecPointerKey::GetNewTrecPointer<TString>("48.0");
	childControl->addAttribute(TString(L"|FontSize"), value18);
	rootLayout->addChild(childControl, 0, 1);

	childControl = TrecPointerKey::GetNewSelfTrecPointer<TControl>(rtb,TrecPointer<TArray<styleTable>>());
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
	//changeControl->addAttribute(TString(L"|Font"), value7);
	TrecPointer<TString> value28 = TrecPointerKey::GetNewTrecPointer<TString>("36.0");
	childControl->addAttribute(TString(L"|FontSize"), value28);
	rootLayout->addChild(childControl, 1, 0);

	childControl = TrecPointerKey::GetNewSelfTrecPointer<TControl>(rtb,TrecPointer<TArray<styleTable>>());
	TrecPointer<TString> value36 = TrecPointerKey::GetNewTrecPointer<TString>("6.0");
	childControl->addAttribute(TString(L"|BorderThickness"), value36);
	TrecPointer<TString> value37 = TrecPointerKey::GetNewTrecPointer<TString>("0.1,1.0,1.0,1.0");
	childControl->addAttribute(TString(L"|BorderColor"), value37);
	TrecPointer<TString> value38 = TrecPointerKey::GetNewTrecPointer<TString>("0.2,0.8,0.2,1.0");
	childControl->addAttribute(TString(L"|ContentColor"), value38);
	TrecPointer<TString> value39 = TrecPointerKey::GetNewTrecPointer<TString>("3,3,3,3");
	childControl->addAttribute(TString(L"|Margin"), value39);

	rootLayout->addChild(childControl, 1, 1);
	this->rootLayout->addChild(changeControl, 1, 0);
	page->CreateLayout();
}

void SwitchHandler::OnSelectLayers(TrecPointer<TControl> tc, EventArgs ea)
{
	TrecPointer<DrawingBoard> rtb = page->GetWindowHandle()->GetDrawingBoard();
	// changeControl.Delete();
	changeControl = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TLayout>(rtb, TrecPointer<TArray<styleTable>>());
	TLayout* rootLayout = dynamic_cast<TLayout*>(changeControl.Get());


	rootLayout->setLayout(orgLayout::VStack);
	TrecPointer<TString> value1 = TrecPointerKey::GetNewTrecPointer<TString>("5,5,5,5");
	rootLayout->addAttribute(TString(L"|Margin"), value1);
	TrecPointer<TString> value2 = TrecPointerKey::GetNewTrecPointer<TString>("3.0");
	rootLayout->addAttribute(TString(L"|BorderThickness"), value2);
	TrecPointer<TString> value3 = TrecPointerKey::GetNewTrecPointer<TString>("0.1,1.0,0.1,1.0");
	rootLayout->addAttribute(TString(L"|BorderColor"), value3);

	TrecPointer<TString> value4 = TrecPointerKey::GetNewTrecPointer<TString>("0.2,0.5,0.2,1.0");
	rootLayout->addAttribute(TString(L"|ContentColor"), value4);

	rootLayout->addRow(300, true);

	rootLayout->addRow(600, true);

	TrecPointer<TControl> childControl = TrecPointerKey::GetNewSelfTrecPointer<TControl>(rtb,TrecPointer<TArray<styleTable>>());

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
	childLayout->setLayout(orgLayout::grid);

	D2D1_RECT_F tLoc;
	D2D1_RECT_F area = this->rootLayout->getRawSectionLocation(0, 1);

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

	childControl = TrecPointerKey::GetNewSelfTrecPointer<TControl>(rtb,TrecPointer<TArray<styleTable>>());
	TrecPointer<TString> value46 = TrecPointerKey::GetNewTrecPointer<TString>("5.0");
	childControl->addAttribute(TString(L"|BorderThickness"), value46);
	TrecPointer<TString> value47 = TrecPointerKey::GetNewTrecPointer<TString>("0.1,1.0,0.1,1.0");
	childControl->addAttribute(TString(L"|BorderColor"), value47);
	TrecPointer<TString> value48 = TrecPointerKey::GetNewTrecPointer<TString>("0.8,0.2,0.2,1.0");
	childControl->addAttribute(TString(L"|ContentColor"), value48);
	TrecPointer<TString> value49 = TrecPointerKey::GetNewTrecPointer<TString>("3,3,3,3");
	childControl->addAttribute(TString(L"|Margin"), value49);

	childLayout->addChild(childControl, 0, 0);
	childControl = TrecPointerKey::GetNewSelfTrecPointer<TControl>(rtb,TrecPointer<TArray<styleTable>>());

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
	//changeControl->addAttribute(TString(L"|Font"), value7);
	TrecPointer<TString> value18 = TrecPointerKey::GetNewTrecPointer<TString>("48.0");
	childControl->addAttribute(TString(L"|FontSize"), value18);
	childLayout->addChild(childControl, 0, 1);

	childControl = TrecPointerKey::GetNewSelfTrecPointer<TControl>(rtb,TrecPointer<TArray<styleTable>>());
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
	//changeControl->addAttribute(TString(L"|Font"), value7);
	TrecPointer<TString> value28 = TrecPointerKey::GetNewTrecPointer<TString>("36.0");
	childControl->addAttribute(TString(L"|FontSize"), value28);
	childLayout->addChild(childControl, 1, 0);

	childControl = TrecPointerKey::GetNewSelfTrecPointer<TControl>(rtb,TrecPointer<TArray<styleTable>>());
	TrecPointer<TString> value36 = TrecPointerKey::GetNewTrecPointer<TString>("6.0");
	childControl->addAttribute(TString(L"|BorderThickness"), value36);
	TrecPointer<TString> value37 = TrecPointerKey::GetNewTrecPointer<TString>("0.1,1.0,1.0,1.0");
	childControl->addAttribute(TString(L"|BorderColor"), value37);
	TrecPointer<TString> value38 = TrecPointerKey::GetNewTrecPointer<TString>("0.2,0.8,0.2,1.0");
	childControl->addAttribute(TString(L"|ContentColor"), value38);
	TrecPointer<TString> value39 = TrecPointerKey::GetNewTrecPointer<TString>("3,3,3,3");
	childControl->addAttribute(TString(L"|Margin"), value39);

	childLayout->addChild(childControl, 1, 1);
	rootLayout->addChild(childControl2, 0, 1);
	this->rootLayout->addChild(changeControl, 1, 0);
	page->CreateLayout();
}

void SwitchHandler::OnSelectGadget(TrecPointer<TControl> tc, EventArgs ea)
{
	TrecPointer<DrawingBoard> rtb = page->GetWindowHandle()->GetDrawingBoard();

	// changeControl.Delete();

	// TODO: Add your command handler code here
	changeControl = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TLayout>(rtb, TrecPointer<TArray<styleTable>>());
	TLayout* rootLayout = dynamic_cast<TLayout*>(changeControl.Get());
	rootLayout->setLayout(orgLayout::grid);

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
	this->rootLayout->addChild(changeControl, 1, 0);
	page->CreateLayout();
}

void SwitchHandler::OnSelectText(TrecPointer<TControl> tc, EventArgs ea)
{
	TrecPointer<DrawingBoard> rtb = page->GetWindowHandle()->GetDrawingBoard();
	// changeControl.Delete();
	HWND windowHandle = page->GetWindowHandle()->GetWindowHandle();
	changeControl = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TLayout>(rtb, TrecPointer<TArray<styleTable>>());
	TLayout* rootLayout = dynamic_cast<TLayout*>(changeControl.Get());
	rootLayout->setLayout(orgLayout::grid);

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
	this->rootLayout->addChild(changeControl, 1, 0);
	page->CreateLayout();
}

void SwitchHandler::OnSelectCombo(TrecPointer<TControl> tc, EventArgs ea)
{
	TrecPointer<DrawingBoard> rtb = page->GetWindowHandle()->GetDrawingBoard();
	// changeControl.Delete();
	// TODO: Add your command handler code here
	changeControl = TrecPointerKey::GetNewSelfTrecPointer<TControl>(rtb,TrecPointer<TArray<styleTable>>());
	//TrecPointer<TControl> tcchangeControl = changeControl.Get();


	TrecPointer<TString> value = TrecPointerKey::GetNewTrecPointer<TString>("20,20,30,30");
	changeControl->addAttribute(TString(L"|Margin"), value);
	TrecPointer<TString> value2 = TrecPointerKey::GetNewTrecPointer<TString>("3.0");
	changeControl->addAttribute(TString(L"|BorderThickness"), value2);
	TrecPointer<TString> value3 = TrecPointerKey::GetNewTrecPointer<TString>("0.1,1.0,0.1,1.0");
	changeControl->addAttribute(TString(L"|BorderColor"), value3);

	TrecPointer<TString> value4 = TrecPointerKey::GetNewTrecPointer<TString>("0.2,0.5,0.2,1.0");
	changeControl->addAttribute(TString(L"|ContentColor"), value4);

	changeControl->addAttribute(TString(L"|ContainerLoc"), TrecPointerKey::GetNewTrecPointer<TString>("70,70,200,100"));

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
	changeControl->addChild(TrecPointer<TControl>(combo));
	this->rootLayout->addChild(changeControl, 1, 0);
	page->CreateLayout();
}

void SwitchHandler::OnSelectScroll(TrecPointer<TControl> tc, EventArgs ea)
{
	TrecPointer<DrawingBoard> rtb = page->GetWindowHandle()->GetDrawingBoard();
	// changeControl.Delete();
	changeControl = TrecPointerKey::GetNewSelfTrecPointer<TControl>(rtb,TrecPointer<TArray<styleTable>>());
	//TControl *tcchangeControl = changeControl.Get();

	TrecPointer<TString> value = TrecPointerKey::GetNewTrecPointer<TString>("20,20,30,30");
	changeControl->addAttribute(TString(L"|Margin"), value);
	TrecPointer<TString> value2 = TrecPointerKey::GetNewTrecPointer<TString>("3.0");
	changeControl->addAttribute(TString(L"|BorderThickness"), value2);
	TrecPointer<TString> value3 = TrecPointerKey::GetNewTrecPointer<TString>("0.1,1.0,0.1,1.0");
	changeControl->addAttribute(TString(L"|BorderColor"), value3);

	TrecPointer<TString> value4 = TrecPointerKey::GetNewTrecPointer<TString>("0.2,0.5,0.2,1.0");
	changeControl->addAttribute(TString(L"|ContentColor"), value4);

	TrecPointer<TString> value13 = TrecPointerKey::GetNewTrecPointer<TString>("0.6,1.0,0.6,1.0");
	changeControl->addAttribute(TString(L"|HoverBorderColor"), value3);

	TrecPointer<TString> value14 = TrecPointerKey::GetNewTrecPointer<TString>("0.1,0.6,0.1,1.0");
	changeControl->addAttribute(TString(L"|HoverContentColor"), value4);



	TrecPointer<TString> value5 = TrecPointerKey::GetNewTrecPointer<TString>("50,50,200,200");
	changeControl->addAttribute(TString(L"|ContainerLoc"), value5);


	// Prepare Child TControl
	TrecPointer<TControl> childControl = TrecPointerKey::GetNewSelfTrecPointer<TControl>(rtb,TrecPointer<TArray<styleTable>>());


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
	changeControl->addChild(TrecPointer<TControl>(childControl));
	this->rootLayout->addChild(changeControl, 1, 0);
	page->CreateLayout();
}

void SwitchHandler::OnSelectGif(TrecPointer<TControl> tc, EventArgs ea)
{
	auto window = page->GetWindowHandle();
	TrecPointer<DrawingBoard> rtb = window->GetDrawingBoard();

	changeControl = TrecPointerKey::GetNewSelfTrecPointer<TControl>(rtb, TrecPointer<TArray<styleTable>>());
	TString gifPath(GetDirectoryWithSlash(CentralDirectories::cd_Executable));
	gifPath.Append(L"Resources\\giphy.gif");

	changeControl->addAttribute(TString(L"|ImageSource"), TrecPointerKey::GetNewTrecPointer<TString>(gifPath));
	rootLayout->addChild(changeControl, 1, 0);
	page->CreateLayout();

	window->PrepAnimations(page);
}

bool SwitchHandler::ShouldProcessMessageByType(TrecPointer<HandlerMessage> message)
{
	return false;
}
