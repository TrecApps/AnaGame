#pragma once
class TControl;
#include "Anaface.h"

typedef enum class  _ANAFACE_DLL eventType
{
	onClick,
	onHover,
	onRelease,
	onClick_R,
	onRelease_R,
	onDoubleClick,
	onHold,
	onHold_R,
	onTextChange,
	onSelectionChange,
}eventType;

class _ANAFACE_DLL EventTarget
{
public:
	EventTarget();
	~EventTarget();
private:
	virtual void FireEvent(TControl* control, eventType t);
};

