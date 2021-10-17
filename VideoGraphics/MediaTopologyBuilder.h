#pragma once
#include <mfapi.h>
#include <TDataArray.h>
#include <TrecReference.h>

typedef struct MediaTopologyLink
{
	GUID input;
	GUID output;
	REFCLSID transformer;
}MediaTopologyLink;

TDataArray<MediaTopologyLink> GetLink(GUID input, GUID output);

void ConvertTopologyLinkToTransforms(IMFMediaType* type, TDataArray<MediaTopologyLink>& links, TDataArray<TrecComPointer<IMFTransform>>& transforms);