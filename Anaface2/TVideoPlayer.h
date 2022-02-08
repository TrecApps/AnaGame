#pragma once
#include "TControl.h"
#include <TPlayer.h>
class _ANAFACE_DLL2 TVideoPlayer :
    public TControl
{
public:
    TVideoPlayer(TrecPointer<DrawingBoard>, TrecPointer<TArray<styleTable>> ta);
    ~TVideoPlayer();
	bool setVideo(const TString& str);
    bool Play();
    bool Pause();
    bool Stop();
    bool Speed(float);
	/**
	 * Method: TControl::onCreate
	 * Purpose: Allows the Control To contstruct itself based off of the location it has and the
	 *		screen space it is given
	 * Parameters: const D2D1_RECT_F& loc- the location this control is bound by
	 * Returns: bool - success
	 * Note: You Must call this on the Root Control before any control can be drawn on sreen
	 *
	 * Attributes: virtual
	 */
	virtual bool onCreate(const D2D1_RECT_F& loc, TrecPointer<TWindowEngine> d3d, TrecPointer<TFileShell> d)override;

	/**
	 * Method: TControl::Draw
	 * Purpose: Draws the Page to the Window
	 * Parameters: TrecPointer<TVariable> object - Memory Safe means of enabling Data-Binding, if the Page has to tailor it's drawing to data provided by this parameter
	 * Returns: void
	 *
	 * Attributes: override
	 */
	virtual void Draw(TrecPointer<TVariable> object) override;

	/**
	 * Method: TControl::OnResize
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
	ag_msg virtual void OnResize(D2D1_RECT_F& newLoc, UINT nFlags, TDataArray<EventID_Cred>& eventAr) override;


    TrecComPointer<TPlayer> GetPlayer();
private:
    TrecComPointer<TPlayer> player;
    TrecPointer<TWindowEngine> windowEngine;
};

