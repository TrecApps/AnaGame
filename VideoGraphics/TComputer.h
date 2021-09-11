#pragma once

#include <TrecReference.h>
#include <d3d11.h>
#include <TString.h>


/**
 * Class: TComputer
 * Purpose: Leverages Compute Shaders to perform common computational tasks on a large scale
 */
class TComputer : public TObject
{
public:
	/**
	 * Method: TComputer::TComputer
	 * Purpose: Constructor
	 * Parameters: TrecComPointer<ID3D11DeviceContext> devContext - the interface to run compute shaders from
	 * Returns: new TComputer object
	 */
	TComputer(TrecComPointer<ID3D11DeviceContext> devContext);

	/**
	 * Method: TComputer::ConvertNV12ToRGBA8
	 * Purpose: Converts Data from the NV12 Format to the RGBA (1 byte per channel)
	 * Parameters: BYTE* nv12 - bytes in nv12 format
	 *				UINT byteLength - number of bytes in the format
	 *				BYTE** rgba8 - bytes in the ourput
	 *				UINT& newByteLength - length of the new Byte
	 * Returns: TString - error message (empty if no error)
	 */
	TString ConvertNV12ToRGBA8(BYTE* nv12, UINT byteLength, BYTE** rgba8, UINT& newByteLength);
protected:

	/**
	 * The Device Context to Use
	 */
	TrecComPointer<ID3D11DeviceContext> devContext;

	/**
	 * nv12 to rgba8 Compute shader
	 */
	TrecComPointer<ID3D11ComputeShader> nv12toRgba8;
};