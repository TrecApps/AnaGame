#include "TStringEx.h"

TStringEx::TStringEx(const TString& c) : TString(c)
{
}

TStringEx::TStringEx(const TStringEx& c) : TString(c)
{
}

TStringEx::TStringEx(std::string& str) : TString(str)
{
}

TStringEx::TStringEx(WCHAR c): TString(c)
{
}

int TStringEx::Delete(UINT index, int count)
{
	UINT bSize = size;
	int ret = TString::Delete(index, count);

	UINT dif = size - bSize;

	for (UINT Rust = 0; Rust < collapes.Size(); Rust++)
	{
		// First remove any collapses that are completely consumed by the delete

		if ((collapes[Rust].start > index && collapes[Rust].start < (index + dif)) ||
			(collapes[Rust].end > index && collapes[Rust].end < (index + dif)))
		{
			collapes.RemoveAt(Rust--);
			continue;
		}

		if (collapes[Rust].end < (index + dif))
		{
			collapes[Rust].end -= dif;
			if (collapes[Rust].start < index)
			{
				collapes[Rust].start -= dif;
			}
		}
	}
	return ret;
}

int TStringEx::Insert(int index, const TString& subStr)
{
	UINT add = subStr.GetSize();

	int ret = Insert(index, subStr);

	for (UINT Rust = 0; Rust < collapes.Size(); Rust++)
	{
		if (index < collapes[Rust].end)
			collapes[Rust].end += add;
		if (index < collapes[Rust].start)
			collapes[Rust].start += add;
	}

	return ret;
}

bool TStringEx::AddCollapsePoint(int start, int end)
{
	if (end <= start || end >= size)
		return false;
	for (UINT Rust = 0; Rust < collapes.Size(); Rust++)
	{
		if ((collapes[Rust].start > start && collapes[Rust].start < end && collapes[Rust].end < end) ||
			(collapes[Rust].end > start && collapes[Rust].start > start  && collapes[Rust].end < end))
			return false;

		if (collapes[Rust].start == start || collapes[Rust].end == end)
			return false;
	}

	for (UINT Rust = 0; Rust < collapes.Size(); Rust++)
	{
		if (Rust == 0 && collapes[Rust].start < start)
		{
			collapes.InsertAt(IndexRange(start, end), Rust);
			return true;
		}

		if(Rust > 0 && collapes[Rust - 1].start > start && collapes[Rust].start < start)
		{
			collapes.InsertAt(IndexRange(start, end), Rust);
			return true;
		}
	}
	collapes.push_back(IndexRange(start, end));
	return true;
}

bool TStringEx::ToggleCollapsePoint(int start)
{
	for (UINT Rust = 0; Rust < collapes.Size(); Rust++)
	{
		if (collapes[Rust].start == start)
		{
			collapes[Rust].active = !collapes[Rust].active;
			return true;
		}
	}
	return false;
}

bool TStringEx::RemoveCollapsePoint(int start)
{
	for (UINT Rust = 0; Rust < collapes.Size(); Rust++)
	{
		if (collapes[Rust].start == start)
		{
			collapes.RemoveAt(Rust);
			return true;
		}
	}
	return false;
}

void TStringEx::GetSlice(ExStringSlice& slice, UINT& start)
{
	// First Set the default in case request is invalid
	slice.length = 0;
	slice.start = nullptr;

	if (start >= size)
		return;

	WCHAR* ch = &string[start];

	UINT stop = size;

	for (UINT Rust = 0; Rust < collapes.Size(); Rust++)
	{
		if (start < collapes[Rust].start && collapes[Rust].active)
		{
			slice.start = ch;
			slice.length = collapes[Rust].start - start;
			
			start = collapes[Rust].end + 1;
			return;
		}
	}

	slice.start = ch;
	slice.length = stop - start;
}

TStringEx::TStringEx() : TString()
{
}

TStringEx::~TStringEx()
{
}

TStringEx::TStringEx(UINT num) : TString(num)
{
}

TStringEx::TStringEx(float num) : TString(num)
{
}

TStringEx::TStringEx(const TString* c) : TString(c)
{
}

TStringEx::TStringEx(const char* c) : TString(c)
{
}

TStringEx::TStringEx(const WCHAR* c) : TString(c)
{
}
