#include "TStringSlice.h"

TStringSlice::TStringSlice(TrecPointer<TString>, TrecPointerSoft<TSliceManager> parent, UINT start, UINT count)
{
	this->start = start;
	this->count = count;
}

void TStringSlice::Invalidate()
{
	valid = false;
}

TStringSliceManager::TStringSliceManager(TrecPointer<TString> string)
{
	this->string = string;
}

TStringSliceManager::TStringSliceManager(const TString& string)
{
	this->string = TrecPointerKey::GetNewTrecPointer<TString>(string);
}

bool TStringSliceManager::isValid()
{
	return string.Get() != nullptr;
}

TString* TStringSliceManager::operator->()
{
	return string.Get();
}

void TStringSliceManager::Insert(UINT start, UINT count)
{
	for (UINT Rust = 0; Rust < this->slices.Size(); Rust++)
	{
		auto slice = slices[Rust].Get();
		if (slice->start > start)
			slice->start += count;

		if (slice->start < start && (slice->start + slice->count > start))
			slice->count += count;
	}
}

void TStringSliceManager::Delete(UINT start, UINT count)
{
	UINT end = start + count;
	for (UINT Rust = 0; Rust < this->slices.Size(); Rust++)
	{
		auto slice = slices[Rust].Get();

		UINT sliceEnd = slice->count + slice->start;

		if (slice->start > end)
			slice->start -= count;

		if (slice->start >= start && slice->start < end)
		{
			if (sliceEnd <= end)
			{
				// Here the entire slice is getting deleted
				slice->Invalidate();
				slices.RemoveAt(Rust--);
				continue;
			}
			slice->count = sliceEnd - end;
			slice->start = start;
			continue;
		}

		if (slice->start < start && sliceEnd > start)
		{
			if (sliceEnd >= end)
				slice->count -= count;
			else
			{
				slice->count -= (sliceEnd - start);
			}
		}
	}
}

TrecPointer<TStringSlice> TStringSliceManager::GetSlice(UINT start, UINT count)
{
	if(!string.Get())
		return TrecPointer<TStringSlice>();
	if(start + count >= string->GetSize())
		return TrecPointer<TStringSlice>();
	return TrecPointerKey::GetNewTrecPointer<TStringSlice>(string, self, start, count);
}
