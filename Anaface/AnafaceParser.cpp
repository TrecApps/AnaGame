#include <cassert>
#include "AnafaceParser.h"
//#include "TLayoutEx.h"

//using namespace ControlTypeSafety;

/*
* Method: AnafaceParser::
* Purpose:
* Parameters:
* Returns:
*/


bool isLayout(TrecPointer<TControl> cont);



/*
* Method: AnafaceParser::AnafaceParser
* Purpose: Sets up the Anaface Parser
* Parameters: TrecPointer<DrawingBoard> rt - the render target the controls will be using 
*				HWND hWin - the Window Handle the control would be using
*				TString directory - the directory the file originated in
* Returns: void
*/
AnafaceParser::AnafaceParser(TrecPointer<DrawingBoard> rt, HWND hWin,TString directory)
{
	assert(rt.Get());
	renderer = rt;

	fileLoc.Set(directory);
	windowHandle = hWin;
	events = NULL;
	classList = TrecPointerKey::GetNewTrecPointer<TArray<styleTable>>();
	parserMode = anaface_parser_mode::anaface_parser_mode_normal;
}

/*
* Method: AnafaceParser::AnafaceParser
* Purpose: Destructor
* Parameters: void
* Returns: void
*/
AnafaceParser::~AnafaceParser()
{

}
/*
* Method: AnafaceParser::setEventSystem
* Purpose: Sets up the event system being used
* Parameters: TDataArray<eventNameID>& e - The list of events to use
* Returns: void
*/
void AnafaceParser::setEventSystem(TDataArray<eventNameID>& e)
{
	events = &e;
}

/*
* Method: AnafaceParser::Obj
* Purpose: Creates a new Control to parse
* Parameters: TString* va -  the object type
* Returns: bool - whether a valid string was provided
*/
bool AnafaceParser::Obj(TString& va)
{
	setLayoutParam();

	positionSet = false;

	TString v(va);
	v.Trim();

	if (!v.Compare(L"PersistentStoryBoard"))
	{
		addToTree(currentObj);
		currentObj.Nullify();
		parserMode = anaface_parser_mode::anaface_parser_mode_persistant_story;
		return true;
	}
	if (!v.Compare(L"StoryBoard"))
	{
		addToTree(currentObj);
		currentObj.Nullify();
		parserMode = anaface_parser_mode::anaface_parser_mode_normal_story;
		return true;
	}

	parserMode = anaface_parser_mode::anaface_parser_mode_normal;
	currentAnimation.Nullify();

	TrecPointer<TDataArray<TString>> strings;

	if (v.Find(L"(") != -1)                  // first check if there is additional information on location
	{
		strings = v.split(L"(");


		if (strings->Size() > 1)       // IF true, then so fr, so good
		{
			v = strings->at(0);
			
			TrecPointer<TDataArray<TString>> numbers = strings->at(1).split(L",)");

			if (numbers->Size() > 1)  // expecting at least two strings, which should represent numbers
			{
				short firstNum, secondNum;
				int xValue = 0, yValue= 0; // declare numbers to be set nd assign them something logical

				

				firstNum = numbers->at(0).ConvertToInt(xValue);
				secondNum = numbers->at(1).ConvertToInt(yValue);
				if (firstNum == 0 && secondNum == 0) // YES! we now know where to place this object
				{
					positionSet = true;
					xPosition = xValue;
					yPosition = yValue;
				}

				// Now Check for the next two numbers
				if (numbers->Size() > 3)
				{
					firstNum = numbers->at(2).ConvertToInt(xValue);
					secondNum = numbers->at(3).ConvertToInt(yValue);
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
			singleParam.Set(strings->at(0));
			singleParam.Remove(L')');
		}
	}


	if (currentStyle.Get())
	{
		classList->Add(currentStyle);
		currentStyle.Nullify();
	}


	//if (v.Compare(L"ContextMenu") == 0)
	//{
	//	if (!baseObj.Get())
	//	{
	//		return false;
	//	}
	//	currentObj = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TContextMenu>(renderer, classList);   // <TControl>((new TContextMenu()));
	//	if (!currentObj.Get())
	//	{
	//		return false;
	//	}
	//	return baseObj->SetContextMenu(currentObj);
	//}
	/*else if (v.Compare(L"TVideo") == 0)
	{
		currentObj = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TVideo>(renderer, classList, windowHandle);// TrecPointer<TControl>((new TVideo(renderer, classList,windowHandle)));
		addToTree(currentObj);
	}*/
	if (v.Compare(L"TArena") == 0)
	{
		currentObj = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TArena>(renderer, classList, windowHandle);// TrecPointer<TControl>((new TArena(renderer, classList, windowHandle)));
		addToTree(currentObj);
	}
	else if (v.Compare(L"TGraphic")==0)
	{

		addToTree(currentObj);
	}
	else if (v.Compare(L"TGrid")==0)
	{
		currentObj = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TLayout>(renderer, classList);// TrecPointer<TControl>((new TLayout(renderer, classList)));
		dynamic_cast<TLayout*>(currentObj.Get())->setLayout(orgLayout::grid);
		addToTree(currentObj);
	}
	else if (!v.Compare(L"TGridEx") || !v.Compare(L"GridEx"))
	{
		currentObj = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TLayoutEx>(renderer, classList);// TrecPointer<TControl>((new TLayoutEx(renderer, classList)));
		dynamic_cast<TLayout*>(currentObj.Get())->setLayout(orgLayout::grid);
		addToTree(currentObj);
	}
	else if (v.Compare(L"TStack") == 0)
	{
		currentObj = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TLayout>(renderer, classList);// TrecPointer<TControl>((new TLayout(renderer, classList)));
		dynamic_cast<TLayout*>(currentObj.Get())->setLayout(orgLayout::VMix);
		addToTree(currentObj);
	}
	else if (!v.Compare(L"TStackEx") || !v.Compare(L"StackEx"))
	{
		currentObj = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TLayoutEx>(renderer, classList);// TrecPointer<TControl>(new TLayoutEx(renderer, classList));
		dynamic_cast<TLayout*>(currentObj.Get())->setLayout(orgLayout::VMix);
		addToTree(currentObj);
	}
	else if (v.Compare(L"TGallery") == 0)
	{
		currentObj = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TLayout>(renderer, classList);// TrecPointer<TControl>((new TLayout(renderer, classList)));
		dynamic_cast<TLayout*>(currentObj.Get())->setLayout(orgLayout::HMix);
		addToTree(currentObj);
	}
	else if (!v.Compare(L"TGalleryEx") || !v.Compare(L"GalleryEx"))
	{
		currentObj = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TLayout>(renderer, classList);// TrecPointer<TControl>((new TLayoutEx(renderer, classList)));
		dynamic_cast<TLayout*>(currentObj.Get())->setLayout(orgLayout::HMix);
		addToTree(currentObj);
	}
	else if (!v.Compare(L"ComboBox") || !v.Compare(L"TComboBox"))
	{
		currentObj = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TComboBox>(renderer, classList);// TrecPointer<TControl>((new TComboBox(renderer, classList)));
		addToTree(currentObj);
	}

	else if (!v.Compare(L"TTerminal") || !v.Compare(L"TCommandPrompt") || !v.Compare(L"TPrompt"))
	{
		currentObj = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TPromptControl>(renderer, classList, windowHandle);
		addToTree(currentObj);
	}
	else if (v.Compare(L"List") == 0)
	{
		currentObj = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TLayout>(renderer, classList);// TrecPointer<TControl>((new TLayout(renderer, classList)));
		addToTree(currentObj);
	}
	else if (!v.Compare(L"TextField") || !v.Compare(L"TTextField"))
	{
		currentObj = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TTextField>(renderer, classList, windowHandle);// TrecPointer<TControl>((new TTextField(renderer, classList, windowHandle)));
		addToTree(currentObj);
	}
	else if (v.Compare(L"NumberField") == 0)
	{
		currentObj = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TTextField>(renderer, classList, windowHandle);// TrecPointer<TControl>((new TTextField(renderer, classList, windowHandle)));
		dynamic_cast<TTextField*>(currentObj.Get())->isNumber = true;
		addToTree(currentObj);
	}
	else if (v.Compare(L"RadioButton") == 0 || v.Compare(L"TRadioButton") == 0)
	{

		currentObj = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TRadioButton>(renderer, classList);// TrecPointer<TControl>((new TRadioButton(renderer, classList)));
		addToTree(currentObj);
	}
	else if (v.Compare(L"CheckBox") == 0)
	{
		currentObj = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TCheckBox>(renderer, classList);// TrecPointer<TControl>((new TCheckBox(renderer, classList)));
		addToTree(currentObj);
	}
	else if (!v.Compare(L"TUI") || !v.Compare(L"AnafaceUI"))
	{
		currentObj = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, AnafaceUI>(renderer, classList, windowHandle);// TrecPointer<TControl>(new AnafaceUI(renderer, classList, windowHandle));
		addToTree(currentObj);
	}
	else if (!v.Compare(L"Spreadsheet") || !v.Compare(L"TSpreadsheet"))
	{
		currentObj = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TSpreadSheet>(renderer, classList, windowHandle);// TrecPointer<TControl>(new TSpreadSheet(renderer, classList, windowHandle));
		addToTree(currentObj);
	}
	else if (!v.Compare(L"Image") || !v.Compare(L"TImage"))
	{
		currentObj = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TImage>(renderer, classList);// TrecPointer<TControl>(new TImage(renderer, classList));
		addToTree(currentObj);
	}
	else if (!v.Compare(L"TDataBind") || !v.Compare(L"DataLayout"))
	{
		currentObj = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TDataBind>(renderer, classList);// TrecPointer<TControl>((new TDataBind(renderer, classList)));
		addToTree(currentObj);
	}
	else if (!v.Compare(L"TTreeDataBind"))
	{
		currentObj = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TTreeDataBind>(renderer, classList);
		addToTree(currentObj);
	}
	else if (!v.Compare(L"Class"))
	{
	currentStyle = TrecPointerKey::GetNewTrecPointer<styleTable>(); //  new styleTable();
		addToTree(currentObj);
		currentObj.Nullify();//  null<TControl>();
	}
	else if (!v.Compare(L"Animation"))
	{
		currentAnimation = TrecPointerKey::GetNewTrecPointer<AnimationBuilder>();
		animations.push_back(currentAnimation);
		addToTree(currentObj);
		currentObj.Nullify();//  null<TControl>();
	}
	else
	{

		currentObj = TrecPointerKey::GetNewSelfTrecPointer<TControl>(renderer, classList);
		addToTree(currentObj);

		
	}
	return true;
}

/*
* Method: AnafaceParser::Attribute
* Purpose: Sets the attributes of the current Object
* Parameters: TrecPointer<TString> v - the value of the attribute
*				TString& e - the name of the attribute
* Returns: bool - whether the condition of the parser was adequate
*/
bool AnafaceParser::Attribute(TrecPointer<TString> v, TString& e)
{
	if (!e.CompareNoCase(L"Name") && parserMode == anaface_parser_mode::anaface_parser_mode_normal_story)
	{
		if (!v.Get())
			return false;
		basicStoryBoards.push_back(*v.Get());
		return true;
	}
	if (!e.CompareNoCase(L"Name") && parserMode == anaface_parser_mode::anaface_parser_mode_persistant_story)
	{
		if (!v.Get())
			return false;
		persistantStoryBoards.push_back(*v.Get());
		return true;
	}

	if (currentAnimation.Get())
	{
		if (!e.CompareNoCase(L"|type"))
			currentAnimation->SetType(v->SubString(0));
		else if (!e.CompareNoCase(L"|name"))
			currentAnimation->SetName(v->SubString(0));
		else
		{
			currentAnimation->SetAttribute(e, v);
		}
		return true;
	}

	if (!currentObj.Get() && !currentStyle.Get() )
		return false;

	// Check Style first
	if (currentStyle.Get())
	{
		if (!TString::Compare(e, L"|Class"))
		{
			if (!v.Get())
				return false;
			currentStyle->style.Set(v.Get());
				return true;
		}

		currentStyle->names.addEntry(TString(e), v);
		return true;
	}

	int value;
	if (TString::Compare(e, L"|ColumnWidth") == 0)
	{
		if (isLayout(currentObj))
		{
			bool flex = v->Find(L"*") != -1;
			v->Remove(L'*');
			if (v.Get()->ConvertToInt(value))
				return false;
			else
			{
				columnwidth.push_back(value);
				columnFlex.push_back(flex);
			}
		}
		else if (currentObj.Get())
			currentObj->addAttribute(TString(e), v);
	}
	else if (TString::Compare(e, L"|RowHeight") == 0)
	{
		if (isLayout(currentObj))
		{
			bool flex = v->Find(L"*") != -1;
			v->Remove(L'*');
			if (v.Get()->ConvertToInt(value))
				return false;
			else
			{
				rowHeight.push_back(value);
				rowFlex.push_back(flex);
			}
		}
		else if (currentObj.Get())
		{
			currentObj->addAttribute(TString(e), v);
		}
	}
	else if (TString::Compare(e, L"|ImageSource") == 0 ||
		TString::Compare(e, L"|HoverImageSource") == 0 ||
		TString::Compare(e, L"|ClickImageSource") == 0 ||
		TString::Compare(e, L"|MediaSource") == 0)
	{
		v->Insert(0, fileLoc);
		currentObj->addAttribute(TString(e), v);
	}
	else if(!handleEventAttribute(v, e))
		currentObj->addAttribute(TString(e), v);
	return true;
}

/*
* Method: AnafaceParser::submitType
* Purpose: Whether the TML file is written for this parser
* Parameters: TString v - the Parser type being checked
* Returns: bool - whether the TML type is compatible
*/
bool AnafaceParser::submitType(TString v)
{
	if (!v.Compare(L"Anaface_UI"))
		return true;
	return false;
}

/*
* Method: AnafaceParser::submitEdition
* Purpose: Returns version compatibility
* Parameters: TString - the version string
* Returns: bool - whether the version is compatible
*/
bool AnafaceParser::submitEdition(TString v)
{
	return false;
}

/*
* Method: AnafaceParser::goChild
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
* Method: AnafaceParser::goParent
* Purpose: Returns parsing focus to the parent control
* Parameters: void
* Returns: void
*/
void AnafaceParser::goParent()
{
	currentObj = baseObj;
	if (baseObj.Get())
		baseObj = baseObj->getParent();
	
}

/*
* Method: AnafaceParser::getRootControl
* Purpose: Retrieves the root control of the 
* Parameters: void
* Returns: TrecPointer<TControl> - the root control
*/
TrecPointer<TControl> AnafaceParser::getRootControl()
{
	return rootObj;
}

/*
* Method: AnafaceParser::GetAnaGameType
* Purpose: Retrieves the Object type as represented in AnaGame
* Parameters: void
* Returns: UCHAR* - the type of object in AnaGame form
*/
UCHAR * AnafaceParser::GetAnaGameType()
{
	return nullptr;
}

TDataArray<TrecPointer<AnimationBuilder>> AnafaceParser::GetAnimations()
{
	TrecPointer<AnimationBuilder> gifBuilder;

	for (UINT c = 0; c < animations.Size(); c++)
	{
		if (animations[c].Get() && animations[c]->GetName().CompareNoCase(TString(L"Gif")) && animations[c]->GetType().CompareNoCase(TString(L"Gif")))
		{
			gifBuilder = animations[c];
			break;
		}
	}

	if (!gifBuilder.Get())
	{
		gifBuilder = TrecPointerKey::GetNewTrecPointer<AnimationBuilder>();
		gifBuilder->SetName(TString(L"Gif"));
		gifBuilder->SetType(TString(L"Gif"));
		gifBuilder->SetAttribute(TString(L"|RefreshRate"), TrecPointerKey::GetNewTrecPointer<TString>(L"10"));
	}

	animations.push_back(gifBuilder);

	return animations;
}

TDataArray<TString> AnafaceParser::GetStoryBoards()
{
	return basicStoryBoards;
}

TDataArray<TString> AnafaceParser::GetPersistentStoryBoards()
{
	bool hasGif = false;

	for (UINT c = 0; c < persistantStoryBoards.Size(); c++)
	{
		if (!persistantStoryBoards[c].Compare(L"GifRunner"))
		{
			hasGif = true;
			break;
		}
	}

	if (!hasGif)
		persistantStoryBoards.push_back(TString(L"GifRunner"));
	return persistantStoryBoards;
}

/*
* Method: AnafaceParser::setLayoutParam
* Purpose: Sets the rows and columns of a Layout control
* Parameters: void
* Returns: void
*/
void AnafaceParser::setLayoutParam()
{
	bool result;
	int res = 4;
	TLayout* layoutObject = NULL;
	if (currentObj.Get())
	{
		try
		{
			layoutObject = dynamic_cast<TLayout*>(currentObj.Get());
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



bool isLayout(TrecPointer<TControl> cont)
{
	if (!cont.Get())
		return false;
	return !strcmp(typeid(*(cont.Get())).name(), typeid(TLayout).name()) ||
		!strcmp(typeid(*(cont.Get())).name(), typeid(TLayoutEx).name()) ||
		!strcmp(typeid(*(cont.Get())).name(), typeid(TSpreadSheet).name());
}

/*
* Method: AnafaceParser::addToTree
* Purpose: Adds the curren control to the tree of controls for the Anaface UI
* Parameters: TrecPointer<TControl> tc - the control to add
* Returns: void
*/
void AnafaceParser::addToTree(TrecPointer<TControl> tc)
{
	if (!rootObj.Get())
	{
		rootObj = tc;
		baseObj = rootObj;
		currentObj = rootObj;
		return;
	}
	if (baseObj.Get() && isLayout(baseObj) && positionSet)
	{
		TLayout* lBase = dynamic_cast<TLayout*>(baseObj.Get());
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
		//cont->setTControl(tc);
		assert(baseObj.Get()); // It should mean something, if it doesn't, gracefully crash
		baseObj->addChild(tc);
	}
}

/*
* Method: AnafaceParser::AddToEventList
* Purpose: Adds the event handler to the Control
* Parameters: R_Message_Type rmt - the message type
*				int id - the Event handler to repond to
* Returns: void
*/
void AnafaceParser::AddToEventList(R_Message_Type rmt, int id)
{
	if (currentObj.Get())
		currentObj->addEventID(rmt, id);
}

/*
* Method: AnafaceParser::handleEventAttribute
* Purpose: Sets up a Controls reaction to an event
* Parameters: TrecPointer<TString>& v - Id of the event handler
*			TString& e - event type
* Returns:
*/
bool AnafaceParser::handleEventAttribute(TrecPointer<TString>& v, TString& e)
{
	// Quick NULL checks, don't proceed if any are NULL
	if (!events || !currentObj.Get())
		return false;

	bool returnable = false;

	int eventID = -1;

	if (!v.Get())
		return false;
	
	
	for (int c = 0; c < events->Size();c++)
	{
		if (events->at(c).name.GetSize() && !v->Compare(events->at(c).name))
		{
			returnable = true;
			eventID = events->at(c).eventID;
			break;
		}
	}

	

	if (eventID == -1)
		return false;

	if (!TString::Compare(e, L"|EventOnLButtonDown"))
	{
		AddToEventList(R_Message_Type::On_L_Button_Down, eventID);
		return true;
	}
	if (!TString::Compare(e, L"|EventOnLButtonUp"))
	{
		AddToEventList(R_Message_Type::On_L_Button_Up, eventID);
		return true;
	}
	if (!TString::Compare(e, L"|EventOnRButtonDown"))
	{
		AddToEventList(R_Message_Type::On_R_Button_Down, eventID);
		return true;
	}
	if (!TString::Compare(e, L"|EventOnRButtonUp"))
	{
		AddToEventList(R_Message_Type::On_R_Button_Up, eventID);
		return true;
	}
	else if (!TString::Compare(e, L"|EventOnClick"))
	{
		AddToEventList(R_Message_Type::On_Click, eventID);
		return true;
	}
	else if (!TString::Compare(e, L"|EventOnHover"))
	{
		AddToEventList(R_Message_Type::On_Hover, eventID);
		return true;
	}
	else if (!TString::Compare(e, L"|EventOnHoverLeave"))
	{
		AddToEventList(R_Message_Type::On_Hover_Leave, eventID);
		return true;
	}
	else if (!TString::Compare(e, L"|EventOnRightClick"))
	{
		AddToEventList(R_Message_Type::On_Right_Click, eventID);
		return true;
	}
	else if (!TString::Compare(e, L"|EventOnRelease"))
	{
		AddToEventList(R_Message_Type::On_Click_Release, eventID);
		return true;
	}
	else if (!TString::Compare(e, L"|EventOnRightRelease"))
	{
		AddToEventList(R_Message_Type::On_Right_Release, eventID);
		return true;
	}
	else if (!TString::Compare(e, L"|EventOnChar"))
	{
		AddToEventList(R_Message_Type::On_Char, eventID);
		return true;
	}
	else if (!TString::Compare(e, L"|EventOnDblClick"))
	{
		AddToEventList(R_Message_Type::On_LDoubleClick, eventID);
		return true;
	}
	else if (!TString::Compare(e, L"|EventOnRadioChange"))
	{
		AddToEventList(R_Message_Type::On_radio_change, eventID);
		return true;
	}
	else if (!TString::Compare(e, L"|EventCheck"))
	{
		AddToEventList(R_Message_Type::On_check, eventID);
		return true;
	}
	else if (!TString::Compare(e, L"|EventSelectionChange"))
	{
		AddToEventList(R_Message_Type::On_sel_change, eventID);
		return true;
	}
	else if (!TString::Compare(e, L"|EventOnTextChange"))
	{
		AddToEventList(R_Message_Type::On_Text_Change, eventID);
		return true;
	}
	else if (!TString::Compare(e, L"|OnFocus"))
	{
		AddToEventList(R_Message_Type::On_Focus, eventID);
		return true;
	}
	else if (!TString::Compare(e, L"|OnLoseFocus"))
	{
		AddToEventList(R_Message_Type::On_Lose_Focus, eventID);
		return true;
	}
	
	return false;
}
