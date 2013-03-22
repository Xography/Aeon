/*******************************************************************************
 *
 * This file is part of the AEON Framework.
 *
 * -----------------------------------------------------------------------------
 * Copyright (C) 2012- The Aeon Development Team.
 * 
 * See LICENSE.txt for licensing details.
 * 
 ******************************************************************************/
#include <Aeon/TimeProvider.h>

namespace Aeon {
	// --------------------------------------------------------------------------
	TimeProvider::TimeProvider():
		paused(false)
	{
	
	}

	// --------------------------------------------------------------------------
	TimeProvider::TimeProvider(const TimeProvider&)
	{
	
	}

	// --------------------------------------------------------------------------
	bool TimeProvider::IsPaused() const
	{
		return paused;
	}

	// --------------------------------------------------------------------------
	void TimeProvider::Pause()
	{
		paused = true;
	}

	// --------------------------------------------------------------------------
	void TimeProvider::Resume()
	{
		paused = false;
	}

	// --------------------------------------------------------------------------
	void TimeProvider::Subscribe(ITimeSubscriber *s, uint32_t msecs, bool oneShot, bool alwaysFire)
	{
		if(s)
		{
			Subscription sub = {msecs, 0, oneShot, alwaysFire, s};
			subscriptions.push_back(sub);
		}
	}

	// --------------------------------------------------------------------------
	void TimeProvider::IncreaseTime(uint32_t msecs)
	{
		auto i = subscriptions.begin();

		while(i != subscriptions.end())
		{
			if(!paused || i->alwaysFire)
			{
				if((i->timePassed += msecs) >= i->time)
				{
					i->subscriber->OnTimePassed(i->timePassed);
					i->timePassed = 0;

					if(i->oneShot)
					{
						i = subscriptions.erase(i);
						continue;
					}
				}
			}

			++i;
		}
	}

}
