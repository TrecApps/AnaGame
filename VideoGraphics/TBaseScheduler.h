#pragma once
#include <TObject.h>
class TBaseScheduler :
    public TObject
{
public:

    /**
     * Method: TBaseScheduler::SetClockRate
     * Purpose: Sets the clock rate
     * Parameters: float fRate -  the new rate to use
     * Returns: HRESULT - S_OK
     */
    HRESULT SetClockRate(float fRate);
protected:

    float frameRate;
};

