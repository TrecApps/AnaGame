#pragma once
#include "TLayout.h"

typedef enum DropMenuType
{
	dmt_File,
	dmt_Class,
}DropMenuType;


class DropMenuNode: public TObject
{
	friend class TDropMenu;
private:
	TString caption;

	TArray<DropMenuNode> children;
	bool hasChildren, childrenActive;
};

class TDropMenu :public TControl
{
public:
	TDropMenu(TrecComPointer<ID2D1RenderTarget>, TrecPointer<TArray<styleTable>> styles);
	~TDropMenu();

	bool onCreate(RECT l) override;
	void onDraw(TObject* obj = nullptr) override;

	bool SetFolderAsRoot(TString& folder);
	virtual UCHAR* GetAnaGameType()override;
protected:
	TrecPointer<DropMenuNode> rootNode;
	void DrawNode(long& top,TrecPointer<DropMenuNode> node);
	UINT nodeHeight;
	ID2D1SolidColorBrush* dotBrush;
};
