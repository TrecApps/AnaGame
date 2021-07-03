#pragma once
#include <TString.h>
#include "AnafaceUI.h"

/*
* Class: CSSGenerator
* Purpose: Parse CSS documents and returns a list of styles
* 
* SuperClass: TObject
*/
class _ANAFACE_DLL CSSGenerator : public TObject
{
public:
	CSSGenerator(TFile&);
	CSSGenerator(TString);
	~CSSGenerator();

	bool Parse();
	WCHAR ReturnWCharType(char c);
	bool isWhiteSpace(WCHAR c);

	TrecPointer<TArray<styleTable>> GetStyles();

	virtual UCHAR* GetAnaGameType()override;

private:
	TFile* Arch;
	TString parsable;
	TString piece;
	TrecPointer<TArray<styleTable>> styleList;

	bool ParseArchive();
	bool ParseString();
	bool ParseGroup();
	bool BuildString(WCHAR w);
};

