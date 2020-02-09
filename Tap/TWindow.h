#pragma once
#include <Drawer.h>
#include "Page.h"
#include <TWindowEngine.h>
#include "TAnimationManager.h"


class _TAP_DLL TWindow :	public Drawer
{
public:
	TWindow(TString& name, TString& winClass, UINT style, HWND parent, int commandShow, TrecPointer<TInstance> ins);
	virtual ~TWindow();

	virtual int PrepareWindow();

	int CompileView(TString& file, TrecPointer<EventHandler> eh);

	bool MovePageToTop(TrecPointer<Page> p);
	TString GetWinClass();
	TString GetWinName();
	// Action Listeners
	void Draw() override;
	void Draw(Page& draw);
	HWND GetWindowHandle();

	afx_msg void OnRButtonUp(UINT nFlags, TPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, TPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, TPoint);
	afx_msg void OnMouseMove(UINT nFlags, TPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, TPoint point);
	afx_msg virtual void OnLButtonUp(UINT nFlags, TPoint point);
	afx_msg bool OnChar(bool fromChar,UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnWindowResize(UINT width, UINT height);
	afx_msg virtual bool OnDestroy();

	TrecPointer<Page> GetHandlePage(bool singleton);
	TrecPointer<Page> GetHandlePage(const TString& name);
	TrecPointer<Page> Get3DPage(bool singleton, TString& engineId);
	TrecPointer<Page> Get3DPage(bool singleton, TrecPointer<TArenaEngine> engine);

	void LockWindow();
	void UnlockWindow();

	void SetSelf(TrecPointer<TWindow> win);

	TrecPointer<Page> GetPageByArea(RECT r);
	TrecPointer<TInstance> GetInstance();

	bool SetUp3D();
	void CleanUp();

	TrecPointer<TWindowEngine> GetWindowEngine();
	TrecPointer<TArenaEngine> GetNewArenaEngine();
	TrecPointer<TArenaEngine> GetNewArenaEngine(TString& name);

protected:
	HWND parent, currentWindow;
	TrecPointer<TInstance> windowInstance;
	int command;
	TrecPointer<Page> mainPage;
	TDataArray<TrecPointer<Page>> pages;
	TString name, winClass;
	TrecComPointer<ID2D1Factory1> directFactory;

	TMap<Page> keyPages; // Used when multiple objects might want to access the same page by ID

	// Singleton Pages
	TrecPointer<Page> _3DPage, handlePage;

	// Whether the Window should ignore user interactivity. Effectivley disables Windows at the AnaGame level without disabling them at the Windows level
	bool locked;

	UCHAR safeToDraw;

	//
	TrecPointerSoft<TWindow> self;
	TrecPointer<Page> deletePage;


	// 3D Resource
	TrecPointer<TWindowEngine> d3dEngine;
	TTrecPointerArray<TArenaEngine> engines;

	TAnimationManager animationCentral;
};

