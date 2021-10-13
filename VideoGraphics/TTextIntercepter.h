#pragma once
#include <TObject.h>
#include "VideoGraphics.h"
/**
 * Class: TTextIntercepter
 * Purpose: Retrieves a character from the message infrastructure and directs it towards some Text-based object
 */
class _VIDEO_GRAPHICS  TTextIntercepter : public TObject
{
public:
    /**
     * Method: TTextIntercepter::OnChar
     * Purpose: Takes a character and feeds it to its target
     * Parameters: WCHAR ch - the character to report
     *          UINT count number of instances of that character to feed
     *          UINT flags - flags (usually 0)
     * Returns: void
     * 
     * Attributes: abstract
     */
    virtual void OnChar(UINT ch, UINT count, UINT flags) = 0;

    /**
     * Method: TTextIntercepter::OnChar
     * Purpose: Takes a character and feeds it to its target
     * Parameters: WCHAR ch - the character to report
     *          UINT count number of instances of that character to feed
     *          UINT flags - flags (usually 0)
     * Returns: void
     *
     * Attributes: abstract
     */
    virtual void OnKey(UINT ch, UINT count, UINT flags) = 0;

    /**
     * Method: TTextIntercepter::OnLoseFocus
     * Purpose: Alerts the target that it will no longer be intercepting characters
     * Parameters: void
     * Returns: void
     *
     * Attributes: abstract
     */
    virtual void OnLoseFocus() = 0;

    /**
     * Method: TTextIntercepter::OnCopy
     * Purpose: Tells the target that CTRL-C was pressed
     * Parameters: void
     * Returns: void
     *
     * Attributes: abstract
     */
    virtual void OnCopy() = 0;
    /**
     * Method: TTextIntercepter::OnCut
     * Purpose: Tells the target that CTRL-X was pressed
     * Parameters: void
     * Returns: void
     *
     * Attributes: abstract
     */
    virtual void OnCut() = 0;

    /**
     * Method: TTextIntercepter::GetTarget
     * Purpose: Retrieves the target for operations, so TInstance can determine if it should direct input to a new target
     * Parameters: void
     * Returns: void* - raw pointer to target (only used to determine if Object operated on is the same
     * 
     * Attributes: abstract
     */
    virtual void* GetTarget() = 0;


    // virtual void OnPaste() = 0;
};

