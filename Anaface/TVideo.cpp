#include "TVideo.h"

TVideo::TVideo(TrecPointer<DrawingBoard> rt, TrecPointer<TArray<styleTable>> st, HWND win) :TControl(rt, st)
{
	if (!win)
		throw 1;
	window = win;
	HRESULT res;
	player = TPlayer::CreateInstance(res, window, window);
}

TVideo::~TVideo()
{
	if (player.Get())
		player->Shutdown();
}

bool TVideo::setVideo(const WCHAR* str)
{
	if (!player.Get())
		return false;
	if (FAILED(player->OpenURL(str, windowEngine)))
		return false;
	return true;
}

bool TVideo::Play()
{
	if (!player.Get())
		return false;
	if (FAILED(player->Play()))
		return false;
	return true;
}

bool TVideo::Pause()
{
	if (!player.Get())
		return false;
	if (FAILED(player->Pause()))
		return false;
	return true;
}

bool TVideo::Stop()
{
	if (!player.Get())
		return false;
	if (FAILED(player->Stop()))
		return false;
	return true;
}

bool TVideo::Speed(float r)
{
	return false;
}

bool TVideo::onCreate(D2D1_RECT_F r, TrecPointer<TWindowEngine> d3d)
{
	if (!player.Get())
		return false;

	windowEngine = d3d;

	// Don't want to override non-direct 2D Content
	drawBackground = false;

	bool returnable = TControl::onCreate(r,d3d);
	TrecPointer<TString> valpoint = attributes.retrieveEntry(TString(L"|MediaSource"));
	if (valpoint.Get())
	{
		if (!setVideo(valpoint->GetConstantBuffer()))
			returnable = false;
	} 
	RECT rr = convertD2DRectToRECT(location);
	player->ResizeVideo(rr);

	return returnable;
}

void TVideo::onDraw(TObject* obj)
{
	TControl::onDraw(obj);
	if (player.Get())
		player->Repaint();
}

void TVideo::Resize(D2D1_RECT_F& r)
{
	if (player.Get())
	{
		RECT rr = convertD2DRectToRECT(r);
		player->ResizeVideo(rr);
	}
	TControl::Resize(r);
}

TrecPointer<TControl> TVideo::QueryVideoControl()
{
	return TrecPointerKey::GetTrecPointerFromSoft<TControl>(tThis);
}

TrecComPointer<TPlayer> TVideo::GetPlayer()
{
	return player;
}

void TVideo::QueryMediaControl(TDataArray<TrecPointer<TControl>>& mediaControls)
{
	mediaControls.push_back(TrecPointerKey::GetTrecPointerFromSoft<TControl>(tThis));
	TControl::QueryMediaControl(mediaControls);
}
