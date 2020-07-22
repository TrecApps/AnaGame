#pragma once
#include "TArenaEngine.h"
#include "ArenaModel.h"

// Macros for managing Look At vs Look To camera
#define CAMERA_TYPE bool
#define LOOK_TO true
#define LOOK_AT false

#define IF_IS_LOOK_TO(camType)   if(camType)
#define IF_IS_LOOK_AT(camType)   if(!camType)
class _VIDEO_GRAPHICS TCamera :
	public TObject
{
public:
	TCamera();

	bool setEngine(TrecPointer<TArenaEngine> e);

	void removeEngine();

	void Rotate(float leftRight, float upDown);
	void Translate(float degree, DirectX::XMFLOAT3 direction);

	void PanX(float x);
	void PanY(float y);

	void ChangeToLookAt();
	void ChangeToLookAt(ArenaModel& am, float distance, bool changeAngle);
	void ChangeToLookTo();
	void ChangeToLookTo(float x, float y, float z);

	virtual UCHAR* GetAnaGameType()override;

	void UpdatePos(float f, UCHAR dir);
	void UpdateDir(float f, UCHAR dir);

	DirectX::XMFLOAT3 GetCameraLocation();
	DirectX::XMFLOAT3 GetCameraDirection();

protected:

	TrecPointer<TArenaEngine> arenaEngine;
	HWND windowHandle;
	HINSTANCE instanceHandle;

	void RefreshVectors();
	void UpdateCamera();

	// View Attributes
	D3D11_VIEWPORT* viewport;
	DirectX::XMMATRIX projector;

	//float rotX, rotY;

	// Since this effectively holds the camera, attributes for the camera matrix
	CAMERA_TYPE cameraType;
	DirectX::XMMATRIX camera; // camera matrix (view in model-view-projection)
	bool lookTo;              // use a look to if true, a look at if false
	DirectX::XMFLOAT3 location_3, direction_3, up_3;
	DirectX::XMVECTOR location, direction, up;
};

