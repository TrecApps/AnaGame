#include "MediaTopologyBuilder.h"
#include <wmcodecdsp.h>
#include <mfapi.h>
#include <Wmcodecdsp.h>
#include <Camerauicontrol.h>
#include <mfidl.h>
#include <TLinkedList.h>

class TransformBuilder
{
public:
	TransformBuilder(REFCLSID id):id(id) {
		transformer = nullptr;



		HRESULT ret = CoCreateInstance(id, nullptr, CLSCTX_INPROC_SERVER, __uuidof(IMFTransform), (void**)&transformer);
		if (FAILED(ret))
			transformer = nullptr;
	}


	~TransformBuilder() {
		if (transformer)
			transformer->Release();
		transformer = nullptr;
	}

	bool TransformerAvailable() {
		return transformer != nullptr;
	}

	IMFMediaType* GetOutputType(MediaTopologyLink link, IMFMediaType* input) {
		if (!input || (id != link.transformer) || !transformer)
			return nullptr;
		IMFMediaType* output = nullptr, * newInput = nullptr;
		assert(SUCCEEDED(MFCreateMediaType(&output)));

		assert(SUCCEEDED(input->CopyAllItems(output)));
		assert(SUCCEEDED(MFCreateMediaType(&newInput)));

		assert(SUCCEEDED(input->CopyAllItems(newInput)));

		assert(SUCCEEDED(output->SetGUID(MF_MT_SUBTYPE, link.output)));
		assert(SUCCEEDED(newInput->SetGUID(MF_MT_SUBTYPE, link.input)));


		IMFMediaType* ret = nullptr;


		if (id == CLSID_CMSH264DecoderMFT)
		{
			assert(SUCCEEDED(transformer->SetInputType(0, newInput, 0)));
			if (SUCCEEDED(transformer->SetOutputType(0, output, 0)))
			{
				ret = output;
				ret->AddRef();
			}
		}
		else if (id == CLSID_CMSH264EncoderMFT)
		{
			output->SetUINT32(MF_MT_AVG_BITRATE, 10);
			HRESULT hret = transformer->SetOutputType(0, output, 0);
			assert(SUCCEEDED(hret));

			//
			//MFSetAttributeRatio(output, MF_MT_FRAME_RATE, 30, 1);


			if (SUCCEEDED(transformer->SetInputType(0, newInput, 0)))
			{
				ret = output;
				ret->AddRef();
			}
		}
		else if (id == CLSID_CMpeg4DecMediaObject)
		{
			assert(SUCCEEDED(transformer->SetInputType(0, newInput, 0)));
			if (SUCCEEDED(transformer->SetOutputType(0, output, 0)))
			{
				ret = output;
				ret->AddRef();
			}
		}
		else if (id == CLSID_VideoProcessorMFT)
		{
			assert(SUCCEEDED(transformer->SetInputType(0, newInput, 0)));
			if (SUCCEEDED(transformer->SetOutputType(0, output, 0)))
			{
				ret = output;
				ret->AddRef();
			}
		}
		else if (id == CLSID_CMpeg4sDecMFT)
		{
			HRESULT inputHr = transformer->SetInputType(0, newInput, 0);
			assert(SUCCEEDED(inputHr));
			if (SUCCEEDED(transformer->SetOutputType(0, output, 0)))
			{
				ret = output;
				ret->AddRef();
			}
		}

		newInput->Release();
		newInput = nullptr;
		output->Release();
		output = nullptr;
		return ret;

	}

	TrecComPointer<IMFTransform> GetTransform()
	{
		TrecComPointer<IMFTransform> ret;

		if (transformer)
		{
			TrecComPointer<IMFTransform>::TrecComHolder holder;

			*holder.GetPointerAddress() = transformer;
			transformer->AddRef();
			ret = holder.Extract();
		}


		return ret;
	}


private:
	REFCLSID id;
	IMFTransform* transformer;
};

bool isOutoutInputToAnotherNode(TDataArray<TLinkedList< MediaTopologyLink>>& options, GUID output);


/// <summary>
/// List of supported video Conversions. Note: Commented out options were converstions that were unable to actually provide the Transform and thus
/// are assumed to be unsupported.
/// </summary>
MediaTopologyLink links[] = {
	// CLSID_CMSH264DecoderMFT - Video Decoder for H.264 (Needs Testing)
	{MFVideoFormat_H264_ES, MFVideoFormat_I420, CLSID_CMSH264DecoderMFT},
	{MFVideoFormat_H264_ES, MFVideoFormat_IYUV, CLSID_CMSH264DecoderMFT},
	{MFVideoFormat_H264_ES, MFVideoFormat_NV12, CLSID_CMSH264DecoderMFT},
	{MFVideoFormat_H264_ES, MFVideoFormat_YUY2, CLSID_CMSH264DecoderMFT},
	{MFVideoFormat_H264_ES, MFVideoFormat_YV12, CLSID_CMSH264DecoderMFT},
	{MFVideoFormat_H264, MFVideoFormat_I420, CLSID_CMSH264DecoderMFT},
	{MFVideoFormat_H264, MFVideoFormat_IYUV, CLSID_CMSH264DecoderMFT},
	{MFVideoFormat_H264, MFVideoFormat_NV12, CLSID_CMSH264DecoderMFT},
	{MFVideoFormat_H264, MFVideoFormat_YUY2, CLSID_CMSH264DecoderMFT},
	{MFVideoFormat_H264, MFVideoFormat_YV12, CLSID_CMSH264DecoderMFT},

	// CLSID_CMSH264EncoderMFT - Video Encoder for H.264 (Needs Testing)
	//{MFVideoFormat_I420, MFVideoFormat_H264, CLSID_CMSH264EncoderMFT},
	{MFVideoFormat_IYUV, MFVideoFormat_H264, CLSID_CMSH264EncoderMFT},
	{MFVideoFormat_NV12, MFVideoFormat_H264, CLSID_CMSH264EncoderMFT},
	{MFVideoFormat_YUY2, MFVideoFormat_H264, CLSID_CMSH264EncoderMFT},
	{MFVideoFormat_YV12, MFVideoFormat_H264, CLSID_CMSH264EncoderMFT},

	// To-Do: Create Mapping for H.265 / HEVC Encoder/Decoder once CLSID for them is found

	// CLSID_CMpeg4DecMediaObject - Decoder for MPEG4 V1/2
	{MEDIASUBTYPE_MPG4, MFVideoFormat_YUY2, CLSID_CMpeg4DecMediaObject},
	{MEDIASUBTYPE_MPG4, MFVideoFormat_UYVY, CLSID_CMpeg4DecMediaObject},
	{MEDIASUBTYPE_MPG4, MFVideoFormat_RGB32, CLSID_CMpeg4DecMediaObject},
	{MEDIASUBTYPE_MPG4, MFVideoFormat_RGB24, CLSID_CMpeg4DecMediaObject},
	{MEDIASUBTYPE_MPG4, MFVideoFormat_RGB565, CLSID_CMpeg4DecMediaObject},
	//{MEDIASUBTYPE_MPG4, MFVideoFormat_RGB8, CLSID_CMpeg4DecMediaObject},
	{MEDIASUBTYPE_MPG4, MFVideoFormat_RGB555, CLSID_CMpeg4DecMediaObject},

	{MEDIASUBTYPE_mpg4, MFVideoFormat_YUY2, CLSID_CMpeg4DecMediaObject},
	{MEDIASUBTYPE_mpg4, MFVideoFormat_UYVY, CLSID_CMpeg4DecMediaObject},
	{MEDIASUBTYPE_mpg4, MFVideoFormat_RGB32, CLSID_CMpeg4DecMediaObject},
	{MEDIASUBTYPE_mpg4, MFVideoFormat_RGB24, CLSID_CMpeg4DecMediaObject},
	{MEDIASUBTYPE_mpg4, MFVideoFormat_RGB565, CLSID_CMpeg4DecMediaObject},
	//{MEDIASUBTYPE_mpg4, MFVideoFormat_RGB8, CLSID_CMpeg4DecMediaObject},
	{MEDIASUBTYPE_mpg4, MFVideoFormat_RGB555, CLSID_CMpeg4DecMediaObject},

	{MEDIASUBTYPE_MP42, MFVideoFormat_YUY2, CLSID_CMpeg4DecMediaObject},
	{MEDIASUBTYPE_MP42, MFVideoFormat_UYVY, CLSID_CMpeg4DecMediaObject},
	{MEDIASUBTYPE_MP42, MFVideoFormat_RGB32, CLSID_CMpeg4DecMediaObject},
	{MEDIASUBTYPE_MP42, MFVideoFormat_RGB24, CLSID_CMpeg4DecMediaObject},
	{MEDIASUBTYPE_MP42, MFVideoFormat_RGB565, CLSID_CMpeg4DecMediaObject},
	//{MEDIASUBTYPE_MP42, MFVideoFormat_RGB8, CLSID_CMpeg4DecMediaObject},
	{MEDIASUBTYPE_MP42, MFVideoFormat_RGB555, CLSID_CMpeg4DecMediaObject},

	{MEDIASUBTYPE_mp42, MFVideoFormat_YUY2, CLSID_CMpeg4DecMediaObject},
	{MEDIASUBTYPE_mp42, MFVideoFormat_UYVY, CLSID_CMpeg4DecMediaObject},
	{MEDIASUBTYPE_mp42, MFVideoFormat_RGB32, CLSID_CMpeg4DecMediaObject},
	{MEDIASUBTYPE_mp42, MFVideoFormat_RGB24, CLSID_CMpeg4DecMediaObject},
	{MEDIASUBTYPE_mp42, MFVideoFormat_RGB565, CLSID_CMpeg4DecMediaObject},
	//{MEDIASUBTYPE_mp42, MFVideoFormat_RGB8, CLSID_CMpeg4DecMediaObject},
	{MEDIASUBTYPE_mp42, MFVideoFormat_RGB555, CLSID_CMpeg4DecMediaObject},

	// CLSID_CMpeg4sDecMFT - Decoder for the MPEG-4 P2 Format (Needs Testing)
	{MFVideoFormat_MP4V, MFVideoFormat_NV12, CLSID_CMpeg4sDecMFT},
	{MFVideoFormat_MP4V, MFVideoFormat_NV12, CLSID_CMpeg4sDecMFT},

	// CLSID_VideoProcessorMFT - Processor of Video Frames
	{ MFVideoFormat_ARGB32, MFVideoFormat_ARGB32, CLSID_VideoProcessorMFT}, // Same
	{ MFVideoFormat_ARGB32, MFVideoFormat_AYUV, CLSID_VideoProcessorMFT},
	{ MFVideoFormat_ARGB32, MFVideoFormat_I420, CLSID_VideoProcessorMFT},
	{ MFVideoFormat_ARGB32, MFVideoFormat_IYUV, CLSID_VideoProcessorMFT},
	{ MFVideoFormat_ARGB32, MFVideoFormat_NV12, CLSID_VideoProcessorMFT},
	{ MFVideoFormat_ARGB32, MFVideoFormat_RGB24, CLSID_VideoProcessorMFT},
	{ MFVideoFormat_ARGB32, MFVideoFormat_RGB32, CLSID_VideoProcessorMFT},
	{ MFVideoFormat_ARGB32, MFVideoFormat_RGB555, CLSID_VideoProcessorMFT},
	{ MFVideoFormat_ARGB32, MFVideoFormat_RGB565, CLSID_VideoProcessorMFT},
	//{ MFVideoFormat_ARGB32, MFVideoFormat_UYVY, CLSID_VideoProcessorMFT},
	//{ MFVideoFormat_ARGB32, MFVideoFormat_Y216, CLSID_VideoProcessorMFT},
	{ MFVideoFormat_ARGB32, MFVideoFormat_YUY2, CLSID_VideoProcessorMFT},
	{ MFVideoFormat_ARGB32, MFVideoFormat_YV12, CLSID_VideoProcessorMFT},

	{ MFVideoFormat_AYUV, MFVideoFormat_ARGB32, CLSID_VideoProcessorMFT},
	{ MFVideoFormat_AYUV, MFVideoFormat_AYUV, CLSID_VideoProcessorMFT}, // Same
	{ MFVideoFormat_AYUV, MFVideoFormat_I420, CLSID_VideoProcessorMFT},
	{ MFVideoFormat_AYUV, MFVideoFormat_IYUV, CLSID_VideoProcessorMFT},
	{ MFVideoFormat_AYUV, MFVideoFormat_NV12, CLSID_VideoProcessorMFT},
	//{ MFVideoFormat_AYUV, MFVideoFormat_RGB24, CLSID_VideoProcessorMFT},
	{ MFVideoFormat_AYUV, MFVideoFormat_RGB32, CLSID_VideoProcessorMFT},
	//{ MFVideoFormat_AYUV, MFVideoFormat_RGB555, CLSID_VideoProcessorMFT},
	//{ MFVideoFormat_AYUV, MFVideoFormat_RGB565, CLSID_VideoProcessorMFT},
	{ MFVideoFormat_AYUV, MFVideoFormat_UYVY, CLSID_VideoProcessorMFT},
	//{ MFVideoFormat_AYUV, MFVideoFormat_Y216, CLSID_VideoProcessorMFT},
	{ MFVideoFormat_AYUV, MFVideoFormat_YUY2, CLSID_VideoProcessorMFT},
	{ MFVideoFormat_AYUV, MFVideoFormat_YV12, CLSID_VideoProcessorMFT},

	{ MFVideoFormat_I420, MFVideoFormat_ARGB32, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_I420, MFVideoFormat_AYUV, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_I420, MFVideoFormat_I420, CLSID_VideoProcessorMFT }, // Same
	//{ MFVideoFormat_I420, MFVideoFormat_IYUV, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_I420, MFVideoFormat_NV12, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_I420, MFVideoFormat_RGB24, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_I420, MFVideoFormat_RGB32, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_I420, MFVideoFormat_RGB555, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_I420, MFVideoFormat_RGB565, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_I420, MFVideoFormat_UYVY, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_I420, MFVideoFormat_Y216, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_I420, MFVideoFormat_YUY2, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_I420, MFVideoFormat_YV12, CLSID_VideoProcessorMFT },

	{ MFVideoFormat_IYUV, MFVideoFormat_ARGB32, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_IYUV, MFVideoFormat_AYUV, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_IYUV, MFVideoFormat_I420, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_IYUV, MFVideoFormat_IYUV, CLSID_VideoProcessorMFT }, // Same
	{ MFVideoFormat_IYUV, MFVideoFormat_NV12, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_IYUV, MFVideoFormat_RGB24, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_IYUV, MFVideoFormat_RGB32, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_IYUV, MFVideoFormat_RGB555, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_IYUV, MFVideoFormat_RGB565, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_IYUV, MFVideoFormat_UYVY, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_IYUV, MFVideoFormat_Y216, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_IYUV, MFVideoFormat_YUY2, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_IYUV, MFVideoFormat_YV12, CLSID_VideoProcessorMFT },

	{ MFVideoFormat_NV11, MFVideoFormat_ARGB32, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_NV11, MFVideoFormat_AYUV, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_NV11, MFVideoFormat_I420, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_NV11, MFVideoFormat_IYUV, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_NV11, MFVideoFormat_NV12, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_NV11, MFVideoFormat_RGB24, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_NV11, MFVideoFormat_RGB32, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_NV11, MFVideoFormat_RGB555, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_NV11, MFVideoFormat_RGB565, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_NV11, MFVideoFormat_UYVY, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_NV11, MFVideoFormat_Y216, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_NV11, MFVideoFormat_YUY2, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_NV11, MFVideoFormat_YV12, CLSID_VideoProcessorMFT },

	{ MFVideoFormat_RGB24, MFVideoFormat_ARGB32, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_RGB24, MFVideoFormat_AYUV, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_RGB24, MFVideoFormat_I420, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_RGB24, MFVideoFormat_IYUV, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_RGB24, MFVideoFormat_NV12, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_RGB24, MFVideoFormat_RGB24, CLSID_VideoProcessorMFT }, // Same
	{ MFVideoFormat_RGB24, MFVideoFormat_RGB32, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_RGB24, MFVideoFormat_RGB555, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_RGB24, MFVideoFormat_RGB565, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_RGB24, MFVideoFormat_UYVY, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_RGB24, MFVideoFormat_Y216, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_RGB24, MFVideoFormat_YUY2, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_RGB24, MFVideoFormat_YV12, CLSID_VideoProcessorMFT },

	{ MFVideoFormat_NV12, MFVideoFormat_ARGB32, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_NV12, MFVideoFormat_AYUV, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_NV12, MFVideoFormat_I420, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_NV12, MFVideoFormat_IYUV, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_NV12, MFVideoFormat_NV12, CLSID_VideoProcessorMFT }, // Same
	{ MFVideoFormat_NV12, MFVideoFormat_RGB24, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_NV12, MFVideoFormat_RGB32, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_NV12, MFVideoFormat_RGB555, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_NV12, MFVideoFormat_RGB565, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_NV12, MFVideoFormat_UYVY, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_NV12, MFVideoFormat_Y216, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_NV12, MFVideoFormat_YUY2, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_NV12, MFVideoFormat_YV12, CLSID_VideoProcessorMFT },

	{ MFVideoFormat_RGB32, MFVideoFormat_ARGB32, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_RGB32, MFVideoFormat_AYUV, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_RGB32, MFVideoFormat_I420, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_RGB32, MFVideoFormat_IYUV, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_RGB32, MFVideoFormat_NV12, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_RGB32, MFVideoFormat_RGB24, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_RGB32, MFVideoFormat_RGB32, CLSID_VideoProcessorMFT }, // Same
	{ MFVideoFormat_RGB32, MFVideoFormat_RGB555, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_RGB32, MFVideoFormat_RGB565, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_RGB32, MFVideoFormat_UYVY, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_RGB32, MFVideoFormat_Y216, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_RGB32, MFVideoFormat_YUY2, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_RGB32, MFVideoFormat_YV12, CLSID_VideoProcessorMFT },

	{ MFVideoFormat_RGB555, MFVideoFormat_ARGB32, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_RGB555, MFVideoFormat_AYUV, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_RGB555, MFVideoFormat_I420, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_RGB555, MFVideoFormat_IYUV, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_RGB555, MFVideoFormat_NV12, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_RGB555, MFVideoFormat_RGB24, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_RGB555, MFVideoFormat_RGB32, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_RGB555, MFVideoFormat_RGB555, CLSID_VideoProcessorMFT }, // Same
	//{ MFVideoFormat_RGB555, MFVideoFormat_RGB565, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_RGB555, MFVideoFormat_UYVY, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_RGB555, MFVideoFormat_Y216, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_RGB555, MFVideoFormat_YUY2, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_RGB555, MFVideoFormat_YV12, CLSID_VideoProcessorMFT },

	{ MFVideoFormat_RGB565, MFVideoFormat_ARGB32, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_RGB565, MFVideoFormat_AYUV, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_RGB565, MFVideoFormat_I420, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_RGB565, MFVideoFormat_IYUV, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_RGB565, MFVideoFormat_NV12, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_RGB565, MFVideoFormat_RGB24, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_RGB565, MFVideoFormat_RGB32, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_RGB565, MFVideoFormat_RGB555, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_RGB565, MFVideoFormat_RGB565, CLSID_VideoProcessorMFT }, // Same
	//{ MFVideoFormat_RGB565, MFVideoFormat_UYVY, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_RGB565, MFVideoFormat_Y216, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_RGB565, MFVideoFormat_YUY2, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_RGB565, MFVideoFormat_YV12, CLSID_VideoProcessorMFT },

	{ MFVideoFormat_RGB8, MFVideoFormat_ARGB32, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_RGB8, MFVideoFormat_AYUV, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_RGB8, MFVideoFormat_I420, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_RGB8, MFVideoFormat_IYUV, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_RGB8, MFVideoFormat_NV12, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_RGB8, MFVideoFormat_RGB24, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_RGB8, MFVideoFormat_RGB32, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_RGB8, MFVideoFormat_RGB555, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_RGB8, MFVideoFormat_RGB565, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_RGB8, MFVideoFormat_UYVY, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_RGB8, MFVideoFormat_Y216, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_RGB8, MFVideoFormat_YUY2, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_RGB8, MFVideoFormat_YV12, CLSID_VideoProcessorMFT },

	{ MFVideoFormat_UYVY, MFVideoFormat_ARGB32, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_UYVY, MFVideoFormat_AYUV, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_UYVY, MFVideoFormat_I420, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_UYVY, MFVideoFormat_IYUV, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_UYVY, MFVideoFormat_NV12, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_UYVY, MFVideoFormat_RGB24, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_UYVY, MFVideoFormat_RGB32, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_UYVY, MFVideoFormat_RGB555, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_UYVY, MFVideoFormat_RGB565, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_UYVY, MFVideoFormat_UYVY, CLSID_VideoProcessorMFT }, // Same
	{ MFVideoFormat_UYVY, MFVideoFormat_Y216, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_UYVY, MFVideoFormat_YUY2, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_UYVY, MFVideoFormat_YV12, CLSID_VideoProcessorMFT },

	//{ MFVideoFormat_v410, MFVideoFormat_ARGB32, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_v410, MFVideoFormat_AYUV, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_v410, MFVideoFormat_I420, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_v410, MFVideoFormat_IYUV, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_v410, MFVideoFormat_NV12, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_v410, MFVideoFormat_RGB24, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_v410, MFVideoFormat_RGB32, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_v410, MFVideoFormat_RGB555, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_v410, MFVideoFormat_RGB565, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_v410, MFVideoFormat_UYVY, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_v410, MFVideoFormat_Y216, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_v410, MFVideoFormat_YUY2, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_v410, MFVideoFormat_YV12, CLSID_VideoProcessorMFT },

	//{ MFVideoFormat_Y216, MFVideoFormat_ARGB32, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_Y216, MFVideoFormat_AYUV, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_Y216, MFVideoFormat_I420, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_Y216, MFVideoFormat_IYUV, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_Y216, MFVideoFormat_NV12, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_Y216, MFVideoFormat_RGB24, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_Y216, MFVideoFormat_RGB32, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_Y216, MFVideoFormat_RGB555, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_Y216, MFVideoFormat_RGB565, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_Y216, MFVideoFormat_UYVY, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_Y216, MFVideoFormat_Y216, CLSID_VideoProcessorMFT }, // Same
	{ MFVideoFormat_Y216, MFVideoFormat_YUY2, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_Y216, MFVideoFormat_YV12, CLSID_VideoProcessorMFT },

	//{ MFVideoFormat_Y41P, MFVideoFormat_ARGB32, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_Y41P, MFVideoFormat_AYUV, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_Y41P, MFVideoFormat_I420, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_Y41P, MFVideoFormat_IYUV, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_Y41P, MFVideoFormat_NV12, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_Y41P, MFVideoFormat_RGB24, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_Y41P, MFVideoFormat_RGB32, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_Y41P, MFVideoFormat_RGB555, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_Y41P, MFVideoFormat_RGB565, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_Y41P, MFVideoFormat_UYVY, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_Y41P, MFVideoFormat_Y216, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_Y41P, MFVideoFormat_YUY2, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_Y41P, MFVideoFormat_YV12, CLSID_VideoProcessorMFT },

	//{ MFVideoFormat_Y41T, MFVideoFormat_ARGB32, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_Y41T, MFVideoFormat_AYUV, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_Y41T, MFVideoFormat_I420, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_Y41T, MFVideoFormat_IYUV, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_Y41T, MFVideoFormat_NV12, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_Y41T, MFVideoFormat_RGB24, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_Y41T, MFVideoFormat_RGB32, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_Y41T, MFVideoFormat_RGB555, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_Y41T, MFVideoFormat_RGB565, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_Y41T, MFVideoFormat_UYVY, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_Y41T, MFVideoFormat_Y216, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_Y41T, MFVideoFormat_YUY2, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_Y41T, MFVideoFormat_YV12, CLSID_VideoProcessorMFT },

	//{ MFVideoFormat_Y42T, MFVideoFormat_ARGB32, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_Y42T, MFVideoFormat_AYUV, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_Y42T, MFVideoFormat_I420, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_Y42T, MFVideoFormat_IYUV, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_Y42T, MFVideoFormat_NV12, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_Y42T, MFVideoFormat_RGB24, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_Y42T, MFVideoFormat_RGB32, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_Y42T, MFVideoFormat_RGB555, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_Y42T, MFVideoFormat_RGB565, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_Y42T, MFVideoFormat_UYVY, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_Y42T, MFVideoFormat_Y216, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_Y42T, MFVideoFormat_YUY2, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_Y42T, MFVideoFormat_YV12, CLSID_VideoProcessorMFT },

	{ MFVideoFormat_YUY2, MFVideoFormat_ARGB32, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_YUY2, MFVideoFormat_AYUV, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_YUY2, MFVideoFormat_I420, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_YUY2, MFVideoFormat_IYUV, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_YUY2, MFVideoFormat_NV12, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_YUY2, MFVideoFormat_RGB24, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_YUY2, MFVideoFormat_RGB32, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_YUY2, MFVideoFormat_RGB555, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_YUY2, MFVideoFormat_RGB565, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_YUY2, MFVideoFormat_UYVY, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_YUY2, MFVideoFormat_Y216, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_YUY2, MFVideoFormat_YUY2, CLSID_VideoProcessorMFT }, // Same
	{ MFVideoFormat_YUY2, MFVideoFormat_YV12, CLSID_VideoProcessorMFT },

	{ MFVideoFormat_YV12, MFVideoFormat_ARGB32, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_YV12, MFVideoFormat_AYUV, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_YV12, MFVideoFormat_I420, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_YV12, MFVideoFormat_IYUV, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_YV12, MFVideoFormat_NV12, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_YV12, MFVideoFormat_RGB24, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_YV12, MFVideoFormat_RGB32, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_YV12, MFVideoFormat_RGB555, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_YV12, MFVideoFormat_RGB565, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_YV12, MFVideoFormat_UYVY, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_YV12, MFVideoFormat_Y216, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_YV12, MFVideoFormat_YUY2, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_YV12, MFVideoFormat_YV12, CLSID_VideoProcessorMFT },  // Same

	{ MFVideoFormat_YVYU, MFVideoFormat_ARGB32, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_YVYU, MFVideoFormat_AYUV, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_YVYU, MFVideoFormat_I420, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_YVYU, MFVideoFormat_IYUV, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_YVYU, MFVideoFormat_NV12, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_YVYU, MFVideoFormat_RGB24, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_YVYU, MFVideoFormat_RGB32, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_YVYU, MFVideoFormat_RGB555, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_YVYU, MFVideoFormat_RGB565, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_YVYU, MFVideoFormat_UYVY, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_YVYU, MFVideoFormat_Y216, CLSID_VideoProcessorMFT },
	//{ MFVideoFormat_YVYU, MFVideoFormat_YUY2, CLSID_VideoProcessorMFT },
	{ MFVideoFormat_YVYU, MFVideoFormat_YV12, CLSID_VideoProcessorMFT }
};

int ExamineLink(UINT baseIndex, TDataArray<TLinkedList< MediaTopologyLink>>& options, TDataArray<bool>& viableOptions, GUID targetOutput)
{
	auto link = options[baseIndex].GetTailNode();
	// If node is not present, we failed to find a link
	if(!link.Get())
		return -2;

	// Get the output of the current tail node
	GUID output = link->data.output;

	// We found a successful link, return index
	if (output == targetOutput)
		return baseIndex;

	if (isOutoutInputToAnotherNode(options, output))
	{
		viableOptions[baseIndex] = false;
		return -1;
	}

	TDataArray<MediaTopologyLink> tempLinks;

	for (UINT Rust = 0; Rust < ARRAYSIZE(links); Rust++)
	{
		if (links[Rust].input == output && !isOutoutInputToAnotherNode(options, links[Rust].output))
		{
			tempLinks.push_back(links[Rust]);
		}
	}

	if (!tempLinks.Size())
	{
		viableOptions[baseIndex] = false;
		return -1;
	}

	for (UINT Rust = 1; Rust < tempLinks.Size(); Rust++)
	{
		UINT index = options.push_back(TLinkedList<MediaTopologyLink>());
		viableOptions.push_back(true);

		options[baseIndex].ToHead();
		do
		{
			options[index].Push(options[baseIndex].GetCurrentNode()->data);
		} while (options[baseIndex].Next());

		options[index].Push(tempLinks[Rust]);
	}

	options[baseIndex].Push(tempLinks[0]);

	return -1;
}

bool isOutoutInputToAnotherNode(TDataArray<TLinkedList< MediaTopologyLink>>& options, GUID output)
{
	for (UINT Rust = 0; Rust < options.Size(); Rust++)
	{
		options[Rust].ToHead();
		do
		{
			auto linkNode = options[Rust].GetCurrentNode();
			assert(linkNode.Get());
			if (linkNode->data.input == output)
				return true;
		}while (options[Rust].Next());
	}
	return false;
}


TDataArray<MediaTopologyLink> GetLink(GUID input, GUID output)
{
	TDataArray<TLinkedList< MediaTopologyLink>> options;
	TDataArray<bool> viableOptions;

	// Establish Base Link
	for (UINT Rust = 0; Rust < ARRAYSIZE(links); Rust++)
	{
		if (links[Rust].input == input)
		{
			UINT index = options.push_back(TLinkedList<MediaTopologyLink>());
			options[index].Push(links[Rust]);
			viableOptions.push_back(true);
		}
	}

	bool found = false;
	int foundable = -1;
	for (UINT Rust = 1; Rust < 5 && !found; Rust++)
	{
		for (UINT C = 0; C < options.Size(); C++)
		{
			if (options[C].GetSize() == Rust && viableOptions[C])
			{
				foundable = ExamineLink(C, options, viableOptions, output);
				if (foundable != -1)
				{
					found = true;
					break;
				}
			}
		}
	}

	TDataArray< MediaTopologyLink> ret;

	if (foundable > -1)
	{
		options[foundable].ToHead();
		do
		{
			ret.push_back(options[foundable].GetCurrentNode()->data);
		} while (options[foundable].Next());
	}
	return ret;
}

void ConvertTopologyLinkToTransforms(IMFMediaType* type, TDataArray<MediaTopologyLink>& links, TDataArray<TrecComPointer<IMFTransform>>& transforms)
{
	assert(type);

	for (UINT Rust = 0; Rust < links.Size(); Rust++)
	{
		MediaTopologyLink link = links[Rust];
		TransformBuilder builder(link.transformer);

		IMFMediaType* outputType = builder.GetOutputType(link, type);

		type->Release();
		type = outputType;

		transforms.push_back(builder.GetTransform());
	}
	type->Release();
}

void MediaTopologyLink::operator=(const MediaTopologyLink& source)
{
	this->input = source.input;
	this->output = source.output;
	this->transformer = source.transformer;
}
