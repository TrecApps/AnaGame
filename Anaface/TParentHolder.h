#pragma once
#include <TObject.h>
#include <TrecReference.h>
#include "Anaface.h"
class TControl;

/**
 * Class: TParentHolder
 * Purpose: Provide's the TControl an abstract Interface for interacting with it's parent, whether that parent is a TControl
 *      or another object that holds a TControl
 */
class _ANAFACE_DLL TParentHolder : public TObject
{
public:
    /**
     * Method: TParentHolder::TParentHolder
     * Purpose: Constructor
     * Parameters: void
     * Returns: New TParentHolder
     */
	TParentHolder();

    /**
     * Method: TParentHolder::SwitchChildControl
     * Purpose: Allows the TControl to insert a new control between itse;f and it's parent (most likely a TScrollerControl)
     * Parameters: TrecPointerSoft<TControl> c1 - the current child control (lets the parent control know which control to replace it with
     *              TrecPointer<TControl> c2 - the control to swap it with
     * Returns: void
     */
	virtual void SwitchChildControl(TrecPointerSoft<TControl> c1, TrecPointer<TControl> c2) = 0;

    /**
     * Method: TParentHolder::GetParent
     * Purpose: Allows the Retrieval of the Parent Control (if the holder is holding a control)
     * Parameters: void
     * Returns: TrecPointer<TControl> - the Parent (the default returns null but the TControlParentHolder will return the parent)
     */
	virtual TrecPointer<TControl> GetParent();

    /**
     * Method: TParentHolder::IsScroller
     * Purpose: Reports to the Child whether the parent holding it is a Scroller Control
     * Parameters: void
     * Returns: bool - whether or not the parent is a Scroller Control (default is false, but the Holder that holds a control can determine otherwise)
     */
    virtual bool IsScroller();
};

