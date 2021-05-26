#include "TVideo.h"

TVideo::TVideo(TrecPointer<DrawingBoard> rt, TrecPointer<TArray<styleTable>> st, HWND win) :TControl(rt, st)
{
	if (!win)
		throw 1;
	window = win;
	HRESULT res;
	player = TPlayer::CreateInstance(res, rt);
}

TVideo::~TVideo()
{
	if (player.Get())
		player->Shutdown();
}

bool TVideo::setVideo(const WCHAR* str)
{
	ThreadLock();
	bool ret = player.Get() && SUCCEEDED(player->OpenURL(str, windowEngine));
	ThreadRelease();
	return ret;
}

bool TVideo::Play()
{
	ThreadLock();
	bool ret = player.Get() && SUCCEEDED(player->Play());
	ThreadRelease();
	return ret;
}

bool TVideo::Pause()
{
	ThreadLock();
	bool ret = player.Get() && SUCCEEDED(player->Pause());
	ThreadRelease();
	return ret;
}

bool TVideo::Stop()
{
	ThreadLock();
	bool ret = player.Get() && SUCCEEDED(player->Stop());
	ThreadRelease();
	return ret;
}

bool TVideo::Speed(float r)
{
	return false;
}

bool TVideo::onCreate(D2D1_RECT_F r, TrecPointer<TWindowEngine> d3d)
{
	ThreadLock();
	if (!player.Get())
	{
		ThreadRelease();
		return false;
	}
	windowEngine = d3d;

	// Don't want to override non-direct 2D Content
	drawBackground = false;

	bool returnable = TControl::onCreate(r,d3d);
	TrecPointer<TString> valpoint = attributes.retrieveEntry(TString(L"|MediaSource"));
	if (valpoint.Get())
	{
		if (!setVideo(valpoint->GetConstantBuffer().getBuffer()))
			returnable = false;
	} 
	RECT rr = convertD2DRectToRECT(location);
	player->ResizeVideo(rr);
	ThreadRelease();
	return returnable;
}

void TVideo::onDraw(TObject* obj)
{
	ThreadLock();
	TControl::onDraw(obj);
	if (player.Get())
		player->Repaint();
	ThreadRelease();
}

void TVideo::Resize(D2D1_RECT_F& r)
{
	ThreadLock();
	if (player.Get())
	{
		RECT rr = convertD2DRectToRECT(r);
		player->ResizeVideo(rr);
	}
	TControl::Resize(r);
	ThreadRelease();
}

TrecPointer<TControl> TVideo::QueryVideoControl()
{
	return TrecPointerKey::GetTrecPointerFromSoft<TControl>(tThis);
}

TrecComPointer<TPlayer> TVideo::GetPlayer()
{
	ThreadLock();
	auto ret = player;
	ThreadRelease();
	return ret;
}

void TVideo::QueryMediaControl(TDataArray<TrecPointer<TControl>>& mediaControls)
{
	ThreadLock();
	mediaControls.push_back(TrecPointerKey::GetTrecPointerFromSoft<TControl>(tThis));
	TControl::QueryMediaControl(mediaControls);
	ThreadRelease();
}
