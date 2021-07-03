#pragma once
#include "TControl.h"
#include <TPlayer.h>
class _ANAFACE_DLL  TVideo :
    public TControl
{
public:
	TVideo(TrecPointer<DrawingBoard>, TrecPointer<TArray<styleTable>> ta, HWND win);
	~TVideo();
	bool setVideo(const WCHAR* str);
	bool Play();
	bool Pause();
	bool Stop();
	bool Speed(float);
	// Time Functions

	bool onCreate(D2D1_RECT_F r, TrecPointer<TWindowEngine> d3d)override;
	void onDraw(TObject* obj = nullptr)override;

	/*
	 * Method: TVideo::Resize
	 * Purpose: Resizes the control upon the window being resized
	 * Parameters: RECT r - the new location for the control
	 * Returns: void
	 *
	 * Attributes: override
	 */
	virtual void Resize(D2D1_RECT_F&) override;

	/**
	 * Method: TControl::QueryVideoControl
	 * Purpose: Retrieves a specified video Player
	 * Parameters: void
	 * Returns: TrecSubPointer<TControl, TVideo> - the video player requested
	 */
	virtual TrecPointer<TControl> QueryVideoControl()override;

	TrecComPointer<TPlayer> GetPlayer();

	virtual void QueryMediaControl(TDataArray<TrecPointer<TControl>>& mediaControls)override;

private:
	TrecComPointer<TPlayer> player;
	TrecPointer<TWindowEngine> windowEngine;
	HWND window;
};

