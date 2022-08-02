#pragma once
#include "TString.h"

typedef struct ExStringSlice
{
	WCHAR* start;
	UINT length;
}ExStringSlice;

class _TREC_LIB_DLL TStringEx : public TString
{
protected:
	TDataArray<IndexRange> collapes;

public:

	TStringEx();

	virtual ~TStringEx();

	TStringEx(UINT num);

	TStringEx(float num);

	TStringEx(const TString*);

	TStringEx(const char*);

	TStringEx(const WCHAR*);

	TStringEx(const TString& c);

	TStringEx(const TStringEx& c);

	TStringEx(std::string& str);

	TStringEx(WCHAR c);

	virtual int Delete(UINT index, int count = 1) override;

	virtual int Insert(int index, const TString& subStr)override;

	bool AddCollapsePoint(int start, int end);

	bool ToggleCollapsePoint(int start);

	bool RemoveCollapsePoint(int start);

	void GetSlice(ExStringSlice& slice, UINT& start);
};

