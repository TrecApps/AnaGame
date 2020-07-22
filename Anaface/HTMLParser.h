#pragma once
#include "Parser_.h"
#include "AnafaceUI.h"
class _ANAFACE_DLL HTMLParser :	public Parser_
{
public:
	HTMLParser(TrecPointer<DrawingBoard> resources, HWND window, TString location, bool onDrive);
	~HTMLParser();

	// for the initial object type
	bool Obj(TString *v) override;
	// for the attribute name
	bool Attribute(TString* v, TString e)override;
	bool Attribute(TrecPointer<TString> v, TString& e)override;
	// for the attribute value (here it may be good that TStrings are used)

	bool submitType(TString v)override;
	virtual bool submitEdition(TString v)override;

	bool goChild()override;
	void goParent()override;
	TString GetTitle();

	TrecPointer<TControl> GetRootControl();

	virtual UCHAR* GetAnaGameType()override;

private:
	TrecPointer<TControl> rootObj;
	TrecPointer<TControl> currentObj;
	TrecPointer<TControl> baseObj;

	TrecPointer<DrawingBoard> renderer;
	TrecPointer<TArray<styleTable>> classList;

	TTextField* GetTextField();

	void AddToTree();

	TString fileLoc;
	HWND windowHandle;
	bool onDrive;

	// Attributes to handle HTML specific materials
	bool boldMode, italicMode, pMode;
	int headerLevel;
	USHORT currentColunm, maxColunm; // When creating Tables
	USHORT row;
	/*
	The following attribute will be used to condense a series of states through which 
	the parser can keep track of which mode is active and which one is not. The following bits:
	0: boldMode
	1: italicMode
	2: pMode
	3: OList Mode
	4: UList Mode
	5: Link Mode, for importing other Files
	6: Title Mode
	7: List Item Mode
	*/
	unsigned char mode;
	USHORT listCount;

	/* Use for monitoring tables
	0: Grid - base object should be a grid
	1: Header Mode - Add columns
	2: Column Mode - use Columns
	3: Row Mode - Add Rows
	4: Whether a new element is needed during a "caption" call, usually if a caption is provided after a td or th object call
	5: if 4 is set, the 5 being set means that it is a <th> call
	*/
	UCHAR tableMode;

	/*
	The following attribute will be used for reference states for the HTML document:
	0: Link mode, for the link Object
	1: CSS Link, when to expect a CSS document
	2: 
	*/
	unsigned char linkMode;
	TString pageTitle;
	TString listClass;

	void AppendAppropriateClasses();
};

