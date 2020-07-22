#pragma once
#include <TCamera.h>
#include "TControl.h"

// Macros for managing Look At vs Look To camera
#define CAMERA_TYPE bool
#define LOOK_TO true
#define LOOK_AT false

#define IF_IS_LOOK_TO(camType)   if(camType)
#define IF_IS_LOOK_AT(camType)   if(!camType)

/*
* class TArena
* Purpose: Holds the Camera for view into a 3D Arena
*/
class _ANAFACE_DLL TArena : public TControl, public TCamera
{
public:
	/*
	* Method: TArena::TArena
	* Purpose: Constructor
	* Parameters: TrecPointer<DrawingBoard> rt - the render target to draw to
	*				TrecPointer<TArray<styleTable>> st - list of Anaface styles
	*				HWND h - the window to attach to
	*				CAMERA_TYPE type - type of camera to start as
	* Returns: void
	*/
	TArena(TrecPointer<DrawingBoard>, TrecPointer<TArray<styleTable>> ta, HWND h, CAMERA_TYPE lookTo = LOOK_TO);
	/*
	* Method: TArena::~TArena
	* Purpose: Destructor
	* Parameters: void
	* Returns: void
	*/
	~TArena();

	/*
	* Method: TArena::onCreate
	* Purpose: Sets up Arena specific attributes
	* Parameters: RECT r - the location on screen where arena is to show
	* Returns: bool - ignore
	*/
	bool onCreate(D2D1_RECT_F, TrecPointer<TWindowEngine> d3d) override;
	/*
	* Method: TArena::getEngine
	* Purpose: Retrieves the engine the Arena control is attached to
	* Parameters: void
	* Returns: TrecPointer<ArenaEngine> - the engine arena control is attached to
	*/
	TrecPointer<TArenaEngine> getEngine();


	/*
	* Method: TArena::onDraw
	* Purpose: Draws the Control as well as activates the 3D engine to draw
	* Parameters: void
	* Returns: void
	*/
	void onDraw(TObject* obj) override;

	/*
	* Method: TArena::GetAnaGameType
	* Purpose: Retrieves the AnaGame type for the TArena class
	* Parameters: void
	* Returns: UCHAR* - AnaGame representation of the type
	*/
	virtual UCHAR* GetAnaGameType()override;

	/*
	 * Method: TArena::Resize
	 * Purpose: Resizes the control upon the window being resized
	 * Parameters: D2D1_RECT_F& r - the new location for the control
	 * Returns: void
	 */
	void Resize(D2D1_RECT_F& r) override;


};

