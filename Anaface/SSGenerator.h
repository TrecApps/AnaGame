#pragma once
#include <TString.h>
#include "AnafaceUI.h"

/*
* class CSSGenerator
* Purpose: Parse CSS documents and returns a list of styles
*/
class CSSGenerator : public TObject
{
public:
	CSSGenerator(TFile&);
	CSSGenerator(TString);
	~CSSGenerator();

	bool Parse();
	void DeduceCharType(unsigned char * bytes);
	WCHAR ReturnWCharType(char c);
	bool isWhiteSpace(WCHAR c);

	TrecPointer<TArray<styleTable>> GetStyles();

	virtual UCHAR* GetAnaGameType()override;

private:
	TFile* Arch;
	TString parsable;
	TString piece;
	TrecPointer<TArray<styleTable>> styleList;

	bool charDeduced;
	bool usingWide;

	bool ParseArchive();
	bool ParseString();
	bool ParseGroup();
	bool BuildString(WCHAR w);
};

