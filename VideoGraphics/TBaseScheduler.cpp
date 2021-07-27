#include "TBaseScheduler.h"

TBaseScheduler::TBaseScheduler()
{
	frameRate = 1.0f;
}

/**
 * Method: TBaseScheduler::SetClockRate
 * Purpose: Sets the clock rate
 * Parameters: float fRate -  the new rate to use
 * Returns: HRESULT - S_OK
 */
HRESULT TBaseScheduler::SetClockRate(float fRate)
{
	frameRate = fRate;
	return S_OK;
}
