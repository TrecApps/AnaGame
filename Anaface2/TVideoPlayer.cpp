#include "TVideoPlayer.h"

TVideoPlayer::TVideoPlayer(TrecPointer<DrawingBoard> rt, TrecPointer<TArray<styleTable>> ta): TControl(rt, ta)
{
	HRESULT res;
	player = TPlayer::CreateInstance(res, rt);
	assert(SUCCEEDED(res));
}

TVideoPlayer::~TVideoPlayer()
{
}

bool TVideoPlayer::setVideo(const TString& str)
{
	TObjectLocker lock(&thread);
	return player.Get() && SUCCEEDED(player->OpenURL(str, this->windowEngine));

}

bool TVideoPlayer::Play()
{
	TObjectLocker lock(&thread);
	return player.Get() && SUCCEEDED(player->Play());
}

bool TVideoPlayer::Pause()
{
	TObjectLocker lock(&thread);
	return player.Get() && SUCCEEDED(player->Pause());
}

bool TVideoPlayer::Stop()
{
	TObjectLocker lock(&thread);
	return player.Get() && SUCCEEDED(player->Stop());
}

bool TVideoPlayer::Speed(float)
{
	TObjectLocker lock(&thread);
	return false;
}

bool TVideoPlayer::onCreate(const D2D1_RECT_F& loc, TrecPointer<TWindowEngine> d3d, TrecPointer<TFileShell> d)
{
	TObjectLocker lock(&thread);
	if (!player.Get())
	{
		return false;
	}
	windowEngine = d3d;

	// Don't want to override non-direct 2D Content

	bool returnable = TControl::onCreate(loc, d3d, d);
	TString valpoint;;
	if (attributes.retrieveEntry(L"MediaSource", valpoint))
	{
		if (!setVideo(valpoint))
			returnable = false;
	}
	RECT rr;
	rr.top = area.top;
	rr.left = area.left;
	rr.right = area.right;
	rr.bottom = area.bottom;

	player->ResizeVideo(rr);
	return returnable;
}

void TVideoPlayer::Draw(TrecPointer<TVariable> object)
{
	TObjectLocker lock(&thread);
	TControl::Draw(object);
	if (player.Get())
		player->Repaint();
}

void TVideoPlayer::OnResize(D2D1_RECT_F& newLoc, UINT nFlags, TDataArray<EventID_Cred>& eventAr)
{
	TObjectLocker lock(&thread);
	TControl::OnResize(newLoc, nFlags, eventAr);
	if (player.Get())
	{
		RECT rr;
		rr.top = area.top;
		rr.left = area.left;
		rr.right = area.right;
		rr.bottom = area.bottom;
		player->ResizeVideo(rr);
	}
}

TrecComPointer<TPlayer> TVideoPlayer::GetPlayer()
{
	TObjectLocker lock(&thread);
	return player;
}
