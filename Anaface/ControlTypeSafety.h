#pragma once

#include "Anaface.h"
#include <d2d1.h>
#include "GeoGroup.h"
#include <TMap.h>

/**
 * Class: styleTable
 * Purpose: Holds the list of styles according to name, useful in declaring 'classes' in Anaface
 */
class _ANAFACE_DLL styleTable : public TObject
{
public:
	TString style;
	TMap<TString> names;
};
