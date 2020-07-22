#pragma once
#include "TObject.h"
#include <d2d1.h>
class Dragable :
	public TObject
{
public: 
	Dragable();

	virtual void MovePoint(float x, float y)=0;
};

