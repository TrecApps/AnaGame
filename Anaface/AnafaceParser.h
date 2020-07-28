#pragma once
#include <Parser_.h>
#include "Anaface.h"
#include "AnafaceUI.h"
#include <TDataArray.h>
#include "TSpreadSheet.h"
#include "TDataBind.h"
#include "AnimationBuilder.h"


typedef struct _ANAFACE_DLL eventNameID
{
	TString name;
	int eventID;
}eventNameID;

typedef enum class anaface_parser_mode
{
	anaface_parser_mode_normal,
	anaface_parser_mode_normal_story,
	anaface_parser_mode_persistant_story
};


/**
 * Class: AnafaceParser
 * Purpose: Generates a series of TControls in a tree from a compliant Mark-up file
 */
class _ANAFACE_DLL AnafaceParser :
	public Parser_
{
public:

	/*
	* Method: AnafaceParser::AnafaceParser
	* Purpose: Sets up the Anaface Parser
	* Parameters: TrecPointer<DrawingBoard> rt - the render target the controls will be using
	*				HWND hWin - the Window Handle the control would be using
	*				TString directory - the directory the file originated in
	* Returns: void
	*/
	AnafaceParser(TrecPointer<DrawingBoard>, HWND,TString);

	/*
	* Method: AnafaceParser::AnafaceParser
	* Purpose: Destructor
	* Parameters: void
	* Returns: void
	*/
	~AnafaceParser();
	/*
	 * Method: AnafaceParser::setEventSystem
	 * Purpose: Sets up the event system being used
	 * Parameters: TDataArray<eventNameID>& e - The list of events to use
	 * Returns: void
	 */
	void setEventSystem(TDataArray<eventNameID>& e);

	// for the initial object type

	/*
	 * Method: AnafaceParser::Obj
	 * Purpose: Creates a new Control to parse
	 * Parameters: TString* va -  the object type
	 * Returns: bool - whether a valid string was provided
	 */
	virtual bool Obj(TString& v) override;

	/*
	* Method: AnafaceParser::Attribute
	* Purpose: Sets the attributes of the current Object
	* Parameters: TrecPointer<TString> v - the value of the attribute
	*				TString& e - the name of the attribute
	* Returns: bool - whether the condition of the parser was adequate
	*/
	virtual bool Attribute(TrecPointer<TString> v, TString & e)override;
	// for the attribute name

	// for the attribute value (here it may be good that TStrings are used)

	/*
	 * Method: AnafaceParser::submitType
	 * Purpose: Whether the TML file is written for this parser
	 * Parameters: TString v - the Parser type being checked
	 * Returns: bool - whether the TML type is compatible
	 */
	virtual bool submitType(TString v)override;

	/*
	* Method: AnafaceParser::submitEdition
	* Purpose: Returns version compatibility
	* Parameters: TString - the version string
	* Returns: bool - whether the version is compatible
	*/
	virtual bool submitEdition(TString v)override;

	/*
	* Method: AnafaceParser::goChild
	* Purpose: Puts focus of the parser onto a child control
	* Parameters: void
	* Returns: bool - true
	*/
	virtual bool goChild()override;
	/*
	* Method: AnafaceParser::goParent
	* Purpose: Returns parsing focus to the parent control
	* Parameters: void
	* Returns: void
	*/
	virtual void goParent()override;

	/*
	* Method: AnafaceParser::getRootControl
	* Purpose: Retrieves the root control of the
	* Parameters: void
	* Returns: TrecPointer<TControl> - the root control
	*/
	TrecPointer<TControl> getRootControl();

	virtual UCHAR* GetAnaGameType() override;


	// Animation Methods

	/**
	 * Method: AnafaceParser::GetAnimations
	 * Purpose: Retrieves the List of Animation Builders specified in the Mark-up file
	 * Parameters: void
	 * Returns: TDataArray<TrecPointer<AnimationBuilder>> - list of AnimationBuilders
	 */
	TDataArray<TrecPointer<AnimationBuilder>> GetAnimations();

	/**
	 * Method: AnafaceParser::GetStoryBoards
	 * Purpose: Retrieves the List of Animation Builders specified in the Mark-up file
	 * Parameters: void
	 * Returns: TDataArray<TrecPointer<AnimationBuilder>> - list of AnimationBuilders
	 */
	TDataArray<TString> GetStoryBoards();

	/**
	 * Method: AnafaceParser::GetPersistentStoryBoards
	 * Purpose: Retrieves the List of Animation Builders specified in the Mark-up file
	 * Parameters: void
	 * Returns: TDataArray<TrecPointer<AnimationBuilder>> - list of AnimationBuilders
	 */
	TDataArray<TString> GetPersistentStoryBoards();

private:
	TrecPointer<TControl> rootObj;
	TrecPointer<TControl> currentObj;
	TrecPointer<TControl> baseObj;
//	TLayout* layoutObject; // use if object is a layout so we have access to TLayout methods
	//TTextField* textObject; // use if object is a textField so we have access to textField methods
	TrecPointer<DrawingBoard> renderer;
	TDataArray<int> columnwidth, rowHeight;
	TDataArray<bool> columnFlex, rowFlex;
	HWND windowHandle;

	TrecPointer<TArray<styleTable>> classList;
	TrecPointer<styleTable> currentStyle;

	TDataArray<TrecPointer<AnimationBuilder>> animations;
	TrecPointer<AnimationBuilder> currentAnimation;
	TDataArray<TString> basicStoryBoards;
	TDataArray<TString> persistantStoryBoards;
	anaface_parser_mode parserMode;

	int xPosition, yPosition;
	int x2Pos, y2Pos;
	bool positionSet, extendPos;

	void setLayoutParam();

	TDataArray<eventNameID>* events;
	//void setLayoutPointer();

	void addToTree(TrecPointer<TControl>);
	TString fileLoc;
	TString singleParam;

	void AddToEventList(R_Message_Type rmt, int id);
	bool handleEventAttribute(TrecPointer<TString>& v, TString & e);

};

