#pragma once
#include "TControl.h"
#include <TCamera.h>


/*
 * Class: TArenaView
 * Purpose: Holds the Camera for view into a 3D Arena
 *
 * SuperClass: TControl, TCamera
 */
class TArenaView :
    public TControl, public TCamera
{
public:

	/**
	 * Method: TArenaView::GetType
	 * Purpose: Returns a String Representation of the object type
	 * Parameters: void
	 * Returns: TString - representation of the object type
	 */
	virtual TString GetType()override;

	/*
	* Method: TArenaView::TArenaView
	* Purpose: Constructor
	* Parameters: TrecPointer<DrawingBoard> rt - the render target to draw to
	*				TrecPointer<TArray<styleTable>> st - list of Anaface styles
	*				HWND h - the window to attach to
	*				CAMERA_TYPE type - type of camera to start as
	* Returns: void
	*/
	TArenaView(TrecPointer<DrawingBoard>, TrecPointer<TArray<styleTable>> ta, HWND h, CAMERA_TYPE lookTo = LOOK_TO);
	/*
	* Method: TArenaView::~TArenaView
	* Purpose: Destructor
	* Parameters: void
	* Returns: void
	*/
	~TArenaView();


	/*
	* Method: TArenaView::getEngine
	* Purpose: Retrieves the engine the Arena control is attached to
	* Parameters: void
	* Returns: TrecPointer<ArenaEngine> - the engine arena control is attached to
	*/
	TrecPointer<TArenaEngine> getEngine();


	/**
	 * Method: TArenaView::onCreate
	 * Purpose: Allows the Control To contstruct itself based off of the location it has and the
	 *		screen space it is given
	 * Parameters: const D2D1_RECT_F& loc- the location this control is bound by
	 * Returns: bool - success
	 * Note: You Must call this on the Root Control before any control can be drawn on sreen
	 *
	 * Attributes: virtual
	 */
	virtual bool onCreate(const D2D1_RECT_F& loc, TrecPointer<TWindowEngine> d3d);

	/**
	 * Method: TArenaView::Draw
	 * Purpose: Draws the Page to the Window
	 * Parameters: TrecPointer<TVariable> object - Memory Safe means of enabling Data-Binding, if the Page has to tailor it's drawing to data provided by this parameter
	 * Returns: void
	 *
	 * Attributes: override
	 */
	virtual void Draw(TrecPointer<TVariable> object) override;


	/**
	 * Method: TArenaView::OnResize
	 * Purpose: Resizes the Page
	 * Parameters: D2D1_RECT_F& newLoc - the new regoin of the Page
	 *				UINT nFlags - flags associated with the move
	 *				TrecPointer<TWindowEngine> - the 3D Engine to work with
	 *				TDataArray<EventID_Cred>& eventAr - list of events
	 * Returns: void
	 *
	 * Note: May be Deprecated soon once the MiniHandler is removed from the library
	 *
	 * Attributes: message; override
	 */
	ag_msg virtual void OnResize(D2D1_RECT_F& newLoc, UINT nFlags, TDataArray<EventID_Cred>& eventAr, TDataArray<EventArgs>&) override;


	/**
	 * Method: TArenaView::OnScroll
	 * Purpose: Sends Scroll Command to controls
	 * Parameters: const TPoint& point - point of the mouse
	 *				const TPoint& direction - how far to send the scroll
	 * Returns: bool - whether message was recieved
	 *
	 * Attributes: message; override
	 */
	ag_msg virtual bool OnScroll(bool, const TPoint& point, const TPoint& direction, TDataArray<EventArgs>&) override;

};

