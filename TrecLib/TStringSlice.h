#pragma once
#include "TString.h"
#include "TSliceManager.h"


class TStringSlice
{
	friend class TStringSliceManager;
public:
	TStringSlice(TrecPointer<TString>, TrecPointerSoft<TSliceManager> parent, UINT start, UINT count);
	void Invalidate();
protected:
	TrecPointer<TString> string;
	TrecPointerSoft<TSliceManager> sliceManager;
	UINT start, count;
	bool valid;
};

class TStringSliceManager : public TSliceManager
{
public:
	TStringSliceManager(TrecPointer<TString> string);
	TStringSliceManager(const TString& string);

	bool isValid();
	TString* operator->();


	virtual void Insert(UINT start, UINT count) override;
	virtual void Delete(UINT start, UINT count) override;

	TrecPointer<TStringSlice> GetSlice(UINT start, UINT count);

protected:
	TrecPointer<TString> string;
	TDataArray<TrecPointer<TStringSlice>> slices;
};





