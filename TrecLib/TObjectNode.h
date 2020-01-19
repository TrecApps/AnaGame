#pragma once
#include "TObject.h"
#include "TrecReference.h"
class _TREC_LIB_DLL TObjectNode :
	public TObject
{
public:
	TObjectNode(UINT l);
	~TObjectNode();

	virtual TString GetContent() = 0;
	virtual bool IsExtendable() = 0;
	virtual bool Initialize() = 0;
	virtual bool Initialize(TString& value) = 0;
	virtual void Extend() = 0;
	virtual TrecPointer<TObjectNode> GetChildNodes(UINT) = 0;
	virtual void DropChildNodes() = 0;
	UINT GetLevel();

	void SetParent(TrecPointerSoft<TObjectNode> p);

protected:
	UINT level;
	TrecPointerSoft<TObjectNode> parentNode;
};

