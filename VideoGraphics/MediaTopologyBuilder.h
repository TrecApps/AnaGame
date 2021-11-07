#pragma once
#include <mfapi.h>
#include <TDataArray.h>
#include <TrecReference.h>
#include <mftransform.h>
typedef struct MediaTopologyLink
{
	GUID input;
	GUID output;
	GUID transformer;
	void operator=(const MediaTopologyLink& source);
}MediaTopologyLink;



TDataArray<MediaTopologyLink> GetLink(GUID input, GUID output);

void ConvertTopologyLinkToTransforms(IMFMediaType* type, TDataArray<MediaTopologyLink>& links, TDataArray<TrecComPointer<IMFTransform>>& transforms);