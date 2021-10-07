#pragma once

#include <TDataArray.h>

typedef struct MediaTopologyLink
{
	GUID input;
	GUID output;
	REFCLSID transformer;
}MediaTopologyLink;

TDataArray<MediaTopologyLink> GetLink(GUID input, GUID output);