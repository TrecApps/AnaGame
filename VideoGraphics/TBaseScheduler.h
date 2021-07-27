#pragma once
#include <TObject.h>
class TBaseScheduler :
    public TObject
{
public:
    /**
     * Method: TBaseScheduuler::TBaseScheduler
     * Purpose: Default Constructor
     * Parameters: void
     * Returns: new Base Scheduler
     */
    TBaseScheduler();

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

