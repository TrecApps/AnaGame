#include "stdafx.h"
#include "AnafaceParser.h"
//#include "TLayoutEx.h"

//using namespace ControlTypeSafety;

/*
* Method: AnafaceParser - 
* Purpose:
* Parameters:
* Returns:
*/






/*
* Method: (AnafaceParser) (Constructor)
* Purpose: Sets up the Anaface Parser
* Parameters: TrecComPointer<ID2D1RenderTarget> rt - the render target the controls will be using 
*				HWND hWin - the Window Handle the control would be using
*				TString directory - the directory the file originated in
* Returns: void
*/
AnafaceParser::AnafaceParser(TrecComPointer<ID2D1RenderTarget> rt, HWND hWin,TString directory)
{
	ASSERT(rt.get());
	renderer = rt;
	rootObj = nullptr;
	currentObj = nullptr;
	baseObj = nullptr;
	fileLoc.Set(directory);
	windowHandle = hWin;
	events = NULL;
	classList = new TArray<styleTable>();
	currentStyle = nullptr;
}

/*
* Method: (AnafaceParser) (Destructor)
* Purpose: Cleans up the parser
* Parameters: void
* Returns: void
*/
AnafaceParser::~AnafaceParser()
{
	rootObj.Boost();
	renderer.Boost();
}
/*
* Method: AnafaceParser - setEventSystem
* Purpose: Sets up the event system being used
* Parameters: TDataArray<eventNameID>& e - The list of events to use
* Returns: void
*/
void AnafaceParser::setEventSystem(TDataArray<eventNameID>& e)
{
	events = &e;
}

/*
* Method: AnafaceParser - Obj
* Purpose: Creates a new Control to parse
* Parameters: TString* va -  the object type
* Returns: bool - whether a valid string was provided
*/
bool AnafaceParser::Obj(TString* va)
{
	setLayoutParam();

	positionSet = false;

	TrecPointer<TString> v = va;
	v.hold();
	TrecPointer<TArray<TString>> strings;

	if (v->Find(L"("))                  // first check if there is additional information on location
	{
		strings = v->split(L"(");


		if (strings->Count() > 1)       // IF true, then so fr, so good
		{
			v = strings->ElementAt(0);
			
			TrecPointer<TArray<TString>> numbers = strings->ElementAt(1)->split(L",)");

			if (numbers->Count() > 1)  // expecting at least two strings, which should represent numbers
			{
				short firstNum, secondNum;
				int xValue = 0, yValue= 0; // declare numbers to be set nd assign them something logical

				

				firstNum = numbers->ElementAt(0)->ConvertToInt(&xValue);
				secondNum = numbers->ElementAt(1)->ConvertToInt(&yValue);
				if (firstNum == 0 && secondNum == 0) // YES! we now know where to place this object
				{
					positionSet = true;
					xPosition = xValue;
					yPosition = yValue;
				}

				// Now Check for the next two numbers
				if (numbers->Count() > 3)
				{
					firstNum = numbers->ElementAt(2)->ConvertToInt(&xValue);
					secondNum = numbers->ElementAt(3)->ConvertToInt(&yValue);
					if (firstNum == 0 && secondNum == 0) // YES! we now know where to place this object
					{
						extendPos = true;
						x2Pos = xValue;
						y2Pos = yValue;
					}
					else
						extendPos = false;
				}
				else
					extendPos = false;
			}
			
		}
		else
		{
			singleParam.Set(strings->ElementAt(0).get());
			singleParam.Remove(L')');
		}
	}


	if (currentStyle.get())
	{
		classList->Add(currentStyle);
		currentStyle = nullptr;
	}


	if (v->Compare(L"ContextMenu") == 0)
	{
		if (!baseObj.get())
		{
			v.clean();
			return false;
		}
		currentObj = TrecPointer<TControl>((new TContextMenu(renderer, classList)));
		if (!currentObj.get())
		{
			v.clean();
			return false;
		}
		return baseObj->SetContextMenu(currentObj);
	}
	else if (v->Compare(L"TVideo") == 0)
	{
		currentObj = TrecPointer<TControl>((new TVideo(renderer, classList,windowHandle)));
		addToTree(currentObj);
	}
	else if (v->Compare(L"TArena") == 0)
	{
		currentObj = TrecPointer<TControl>((new TArena(renderer, classList, windowHandle)));
		addToTree(currentObj);
	}
	else if (v->Compare(L"TGraphic")==0)
	{

		addToTree(currentObj);
	}
	else if (v->Compare(L"TGrid")==0)
	{
		currentObj = TrecPointer<TControl>((new TLayout(renderer, classList)));
		dynamic_cast<TLayout*>(currentObj.get())->setLayout(grid);
		addToTree(currentObj);
	}
	else if (!v->Compare(L"TGridEx") || !v->Compare(L"GridEx"))
	{
		currentObj = TrecPointer<TControl>((new TLayoutEx(renderer, classList)));
		dynamic_cast<TLayout*>(currentObj.get())->setLayout(grid);
		addToTree(currentObj);
	}
	else if (v->Compare(L"TStack") == 0)
	{
		currentObj = TrecPointer<TControl>((new TLayout(renderer, classList)));
		dynamic_cast<TLayout*>(currentObj.get())->setLayout(VMix);
		addToTree(currentObj);
	}
	else if (!v->Compare(L"TStackEx") || !v->Compare(L"StackEx"))
	{
		currentObj = TrecPointer<TControl>(new TLayoutEx(renderer, classList));
		dynamic_cast<TLayout*>(currentObj.get())->setLayout(VMix);
		addToTree(currentObj);
	}
	else if (v->Compare(L"TGallery") == 0)
	{
		currentObj = TrecPointer<TControl>((new TLayout(renderer, classList)));
		dynamic_cast<TLayout*>(currentObj.get())->setLayout(HMix);
		addToTree(currentObj);
	}
	else if (!v->Compare(L"TGalleryEx") || !v->Compare(L"GalleryEx"))
	{
		currentObj = TrecPointer<TControl>((new TLayoutEx(renderer, classList)));
		dynamic_cast<TLayout*>(currentObj.get())->setLayout(HMix);
		addToTree(currentObj);
	}
	else if (!v->Compare(L"ComboBox") || !v->Compare(L"TComboBox"))
	{
		currentObj = TrecPointer<TControl>((new TComboBox(renderer, classList)));
		addToTree(currentObj);
	}
	else if (v->Compare(L"Flyout") == 0)
	{
		currentObj = TrecPointer<TControl>((new TFlyout(renderer, classList)));
		addToTree(currentObj);
	}
	else if (v->Compare(L"List") == 0)
	{
		currentObj = TrecPointer<TControl>((new TLayout(renderer, classList)));
		addToTree(currentObj);
	}
	else if (!v->Compare(L"TextField") || !v->Compare(L"TTextField"))
	{
		currentObj = TrecPointer<TControl>((new TTextField(renderer, classList, windowHandle)));
		addToTree(currentObj);
	}
	else if (v->Compare(L"NumberField") == 0)
	{
		currentObj = TrecPointer<TControl>((new TTextField(renderer, classList, windowHandle)));
		dynamic_cast<TTextField*>(currentObj.get())->isNumber = true;
		addToTree(currentObj);
	}
	else if (v->Compare(L"RadioButton") == 0 || v->Compare(L"TRadioButton") == 0)
	{

		currentObj = TrecPointer<TControl>((new TRadioButton(renderer, classList)));
		addToTree(currentObj);
	}
	else if (v->Compare(L"CheckBox") == 0)
	{
		currentObj = TrecPointer<TControl>((new TCheckBox(renderer, classList)));
		addToTree(currentObj);
	}
	else if (!v->Compare(L"TUI") || !v->Compare(L"AnafaceUI"))
	{
		currentObj = TrecPointer<TControl>(new AnafaceUI(renderer, classList, windowHandle));
		addToTree(currentObj);
	}
	else if (!v->Compare(L"Spreadsheet") || !v->Compare(L"TSpreadsheet"))
	{
		currentObj = TrecPointer<TControl>(new TSpreadSheet(renderer, classList, windowHandle));
		addToTree(currentObj);
	}
	else if (!v->Compare(L"Image") || !!v->Compare(L"TImage"))
	{
		currentObj = TrecPointer<TControl>(new TImage(renderer, classList));
		addToTree(currentObj);
	}
	else if (!v->Compare(L"TDataBind") || !v->Compare(L"DataLayout"))
	{
		currentObj = TrecPointer<TControl>((new TDataBind(renderer, classList)));
		
		if(!singleParam.Compare(L"Horizontal"))
			dynamic_cast<TLayout*>(currentObj.get())->setLayout(HMix);
		else if(!singleParam.Compare(L"Grid"))
			dynamic_cast<TLayout*>(currentObj.get())->setLayout(grid);
		else
			dynamic_cast<TLayout*>(currentObj.get())->setLayout(VMix);
		addToTree(currentObj);
	}
	else if (!v->Compare(L"Class"))
	{
		currentStyle = new styleTable();
		addToTree(currentObj);
		currentObj = null<TControl>();
	}
	else
	{

		currentObj = TrecPointer<TControl>(new TControl(renderer, classList));
		addToTree(currentObj);

		
	}
	v.clean();
	return true;
}

/*
* Method: AnafaceParser - Attribute
* Purpose: Sets the attributes of the current Object
* Parameters: TrecPointer<TString> v - the value of the attribute
*				TString& e - the name of the attribute
* Returns: bool - whether the condition of the parser was adequate
*/
bool AnafaceParser::Attribute(TrecPointer<TString> v, TString& e)
{
	if (!currentObj.get() && !currentStyle.get() )
		return false;

	// Check Style first
	if (currentStyle.get())
	{
		if (!wcscmp(e, L"|Class"))
		{
			if (!v.get())
				return false;
			currentStyle->style.Set(v.get());
				return true;
		}

		currentStyle->names.addEntry(TString(e), v);
		return true;
	}

	int value;
	if (wcscmp(e, L"|ColumnWidth") == 0)
	{
		bool flex = v->Find(L"*") != -1;
		v->Remove(L'*');
		if (v.get()->ConvertToInt(&value))
			return false;
		else
		{
			columnwidth.push_back(value);
			columnFlex.push_back(flex);
		}
	}
	else if (wcscmp(e, L"|RowHeight") == 0)
	{
		bool flex = v->Find(L"*") != -1;
		v->Remove(L'*');
		if (v.get()->ConvertToInt(&value))
			return false;
		else
		{
			rowHeight.push_back(value);
			rowFlex.push_back(flex);
		}
	}
	else if (wcscmp(e, L"|ImageSource") == 0 ||
		wcscmp(e, L"|HoverImageSource") == 0 ||
		wcscmp(e, L"|ClickImageSource") == 0 ||
		wcscmp(e, L"|MediaSource") == 0)
	{
		v->Insert(0, fileLoc.GetBuffer());
		fileLoc.ReleaseBuffer();
		currentObj->addAttribute(TString(e), v);
	}
	else if(!handleEventAttribute(v, e))
		currentObj->addAttribute(TString(e), v);
	return true;
}

/*
* Method: AnafaceParser - submitType
* Purpose: Whether the TML file is written for this parser
* Parameters: TString v - the Parser type being checked
* Returns: bool - whether the TML type is compatible
*/
bool AnafaceParser::submitType(TString v)
{
	if (v == "Anaface_UI")
		return true;
	return false;
}

/*
* Method: AnafaceParser - submitEdition
* Purpose: Returns version compatibility
* Parameters: TString - the version string
* Returns: bool - whether the version is compatible
*/
bool AnafaceParser::submitEdition(TString v)
{
	return false;
}

/*
* Method: AnafaceParser - goChild
* Purpose: Puts focus of the parser onto a child control
* Parameters: void
* Returns: bool - true
*/
bool AnafaceParser::goChild()
{

	baseObj = currentObj;
	return true;
}

/*
* Method: AnafaceParser - goParent
* Purpose: Returns parsing focus to the parent control
* Parameters: void
* Returns: void
*/
void AnafaceParser::goParent()
{
	currentObj = baseObj;
	if (baseObj.get())
		baseObj = baseObj->getParent();
	
}

/*
* Method: AnafaceParser - getRootControl
* Purpose: Retrieves the root control of the 
* Parameters: void
* Returns: TrecPointer<TControl> - the root control
*/
TrecPointer<TControl> AnafaceParser::getRootControl()
{
	return rootObj;
}

/*
* Method: AnafaceParser - GetAnaGameType
* Purpose: Retrieves the Object type as represented in AnaGame
* Parameters: void
* Returns: UCHAR* - the type of object in AnaGame form
*/
UCHAR * AnafaceParser::GetAnaGameType()
{
	return nullptr;
}

/*
* Method: AnafaceParser - setLayoutParam
* Purpose: Sets the rows and columns of a Layout control
* Parameters: void
* Returns: void
*/
void AnafaceParser::setLayoutParam()
{
	bool result;
	int res = 4;
	TLayout* layoutObject = NULL;
	if (currentObj.get())
	{
		try
		{
			layoutObject = dynamic_cast<TLayout*>(currentObj.get());
		}
		catch (std::bad_cast e)
		{

		}
	}
	if (layoutObject)
	{
		if (columnwidth.Size() && rowHeight.Size())
		{
			for (int c = 0; c < columnwidth.Size(); c++)
			{
				result = layoutObject->addColunm(columnwidth[c], columnFlex[c]);
				res = res + 0;
			}
			for (int c = 0; c < rowHeight.Size(); c++)
			{
				result = layoutObject->addRow(rowHeight[c], rowFlex[c]);
				res = res + 0;
			}
		}
		else if (columnwidth.Size())
		{
			for (int c = 0; c < columnwidth.Size();c++)
			{
				result = layoutObject->addColunm(columnwidth[c],columnFlex[c]);
				res = res + 0;
			}
		}
		else if (rowHeight.Size())
		{
			for (int c = 0; c < rowHeight.Size();c++)
			{
				result = layoutObject->addRow(rowHeight[c],rowFlex[c]);
				res = res + 0;
			}
		}
	}

	// now that the TArrays of rows and Colunms have been used, clear them
	columnwidth.RemoveAll();
	rowHeight.RemoveAll();
	columnFlex.RemoveAll();
	rowFlex.RemoveAll();
}
/*/
void AnafaceParser::setLayoutPointer()
{
	layoutObject = new TLayout(renderer, NULL);
	addToTree(layoutObject);
}
*/


bool isLayout(TrecPointer<TControl> cont)
{
	return !strcmp(typeid(*(cont.get())).name(), typeid(TLayout).name()) ||
		!strcmp(typeid(*(cont.get())).name(), typeid(TLayoutEx).name()) ||
		!strcmp(typeid(*(cont.get())).name(), typeid(TSpreadSheet).name()) ||
		!strcmp(typeid(*(cont.get())).name(), typeid(TDataBind).name());
}

/*
* Method: AnafaceParser - addToTree
* Purpose: Adds the curren control to the tree of controls for the Anaface UI
* Parameters: TrecPointer<TControl> tc - the control to add
* Returns: void
*/
void AnafaceParser::addToTree(TrecPointer<TControl> tc)
{
	if (!rootObj.get())
	{
		rootObj = tc;
		baseObj = rootObj;
		currentObj = rootObj;
		return;
	}
	if (baseObj.get() && isLayout(baseObj) && positionSet)
	{
		TLayout* lBase = dynamic_cast<TLayout*>(baseObj.get());
		if (lBase)
		{
			if (extendPos)
				lBase->addChild(tc, xPosition, yPosition, x2Pos, y2Pos);
			else
				lBase->addChild(tc, xPosition, yPosition);
		}
	}
	else
	{
		//TrecPointer<TContainer> cont = new TContainer();
		//cont->setTControl(tc);
		ASSERT(baseObj.get()); // It should mean something, if it doesn't, gracefully crash
		baseObj->addChild(tc);
	}
}

/*
* Method: AnafaceParser - AddToEventList
* Purpose: Adds the event handler to the Control
* Parameters: R_Message_Type rmt - the message type
*				int id - the Event handler to repond to
* Returns: void
*/
void AnafaceParser::AddToEventList(R_Message_Type rmt, int id)
{
	if (currentObj.get())
		currentObj->addEventID(rmt, id);
}

/*
* Method: AnafaceParser - handleEventAttribute
* Purpose: Sets up a Controls reaction to an event
* Parameters: TrecPointer<TString>& v - Id of the event handler
*			TString& e - event type
* Returns:
*/
bool AnafaceParser::handleEventAttribute(TrecPointer<TString>& v, TString& e)
{
	// Quick NULL checks, don't proceed if any are NULL
	if (!events || !currentObj.get())
		return false;

	bool returnable = false;

	int eventID = -1;

	if (!v.get())
		return false;
	
	
	for (int c = 0; c < events->Size();c++)
	{
		if (events->at(c).name && !v->Compare(events->at(c).name))
		{
			returnable = true;
			eventID = events->at(c).eventID;
			break;
		}
	}

	

	if (eventID == -1)
		return false;

	if (!wcscmp(e, L"|EventOnClick"))
	{
		AddToEventList(On_Click, eventID);
		return true;
	}
	else if (!wcscmp(e, L"|EventOnHover"))
	{
		AddToEventList(On_Hover, eventID);
		return true;
	}
	else if (!wcscmp(e, L"|EventOnHoverLeave"))
	{
		AddToEventList(On_Hover_Leave, eventID);
		return true;
	}
	else if (!wcscmp(e, L"|EventOnRightClick"))
	{
		AddToEventList(On_Right_Click, eventID);
		return true;
	}
	else if (!wcscmp(e, L"|EventOnRelease"))
	{
		AddToEventList(On_Click_Release, eventID);
		return true;
	}
	else if (!wcscmp(e, L"|EventOnRightRelease"))
	{
		AddToEventList(On_Right_Release, eventID);
		return true;
	}
	else if (!wcscmp(e, L"|EventOnChar"))
	{
		AddToEventList(On_Char, eventID);
		return true;
	}
	else if (!wcscmp(e, L"|EventOnDblClick"))
	{

	}
	else if (!wcscmp(e, L"|EventOnRadioChange"))
	{
		AddToEventList(On_radio_change, eventID);
		return true;
	}
	else if (!wcscmp(e, L"|EventCheck"))
	{
		AddToEventList(On_check, eventID);
		return true;
	}
	else if (!wcscmp(e, L"|EventSelectionChange"))
	{
		AddToEventList(On_sel_change, eventID);
		return true;
	}
	else if (!wcscmp(e, L"|EventOnTextChange"))
	{
		AddToEventList(On_Text_Change, eventID);
		return true;
	}
	else if (!wcscmp(e, L"|OnFocus"))
	{
		AddToEventList(On_Focus, eventID);
		return true;
	}
	else if (!wcscmp(e, L"|OnLoseFocus"))
	{
		AddToEventList(On_Lose_Focus, eventID);
		return true;
	}
	
	return false;
}
