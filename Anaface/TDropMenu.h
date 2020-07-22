#pragma once
#include "TLayout.h"

typedef enum class  DropMenuType
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

/**
 * DEPRECATED in favor of the TTreeDataBind control and it's TObjectNode
 */
class TDropMenu :public TControl
{
public:
	TDropMenu(TrecPointer<DrawingBoard>, TrecPointer<TArray<styleTable>> styles);
	~TDropMenu();

	bool onCreate(D2D1_RECT_F l, TrecPointer<TWindowEngine> d3d) override;
	void onDraw(TObject* obj = nullptr) override;


	bool SetFolderAsRoot(TString& folder);
	virtual UCHAR* GetAnaGameType()override;
protected:
	TrecPointer<DropMenuNode> rootNode;
	void DrawNode(float& top,TrecPointer<DropMenuNode> node);
	UINT nodeHeight;
	TrecPointer<TBrush> dotBrush;
};

