#pragma once
#include "TControl.h"
#include <Dragable.h>

class TTabBar;

class Tab 
{
	friend class TTabBar;
public:
	Tab();




	void SetBrush(TrecPointer<TBrush> brush);
	void SetText(const TString& text);
	D2D1_RECT_F SetLocation(const D2D1_RECT_F& location);

	void Draw();

	virtual void MovePoint(float x, float y);

private:
	TrecPointer<TText> text;
	TrecPointer<TContent> content1, content2;
	TrecPointer<TBrush> brush;
	D2D1_RECT_F location;
	TrecPointer<TTabBar> currentTabBar;
	bool draw1;
	bool isAdd;
	//float verticalResistance;
	//TPoint clickPoint;
};



class TTabBar :
	public TControl
{
public:
	TTabBar(TrecPointer<DrawingBoard> db,  TrecPointer<TArray<styleTable>> styles);
	~TTabBar();

	virtual bool onCreate(D2D1_RECT_F, TrecPointer<TWindowEngine> d3d)override;
	virtual void Resize(D2D1_RECT_F&)override;
	virtual void onDraw(TObject* obj = nullptr);

	afx_msg virtual void OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& hoverControls)override;
	afx_msg virtual void OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedButtons)override;
	afx_msg virtual void OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)override;


	void AddTab(const TString& text);

protected:
	void SetTabSizes();

	UINT startTab;
	TDataArray<TrecPointer<Tab>> tabs;

	Tab leftTab, rightTab;

	UINT selectionSize;
	bool haveAdd;

	bool onClick;
};

