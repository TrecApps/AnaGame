#pragma once
#include "TGadgetControl.h"
class _ANAFACE_DLL TCheckBox :
	public TGadgetControl
{
public:
	TCheckBox(TrecComPointer<ID2D1RenderTarget> rt, TrecPointer<TArray<styleTable>> ta);
	~TCheckBox();

	bool onCreate(RECT);
	void onDraw(TObject* obj = nullptr) override;
	bool IsClicked();
	//virtual void storeInTML(CArchive* ar, int childLevel)override;

	afx_msg void OnLButtonDown(UINT nFlags, TPoint, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedControl);
	afx_msg void OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);
	virtual UCHAR* GetAnaGameType()override;
private:

	bool isClicked;
	int width, height;
	bool onClick;

};

