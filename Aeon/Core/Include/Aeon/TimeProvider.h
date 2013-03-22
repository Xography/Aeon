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
#ifndef TimeProvider_h__
#define TimeProvider_h__

#include <deque>
#include <Aeon/AeonTypes.h>
#include <Aeon/Singleton.h>

namespace Aeon {
	/**
	 * @brief Interface for time subscriber classes, use this class if you want to be notified of timer events by the time manager.
	 */
	class AEON_EXPORT ITimeSubscriber
	{
	public:
		/**
		 * Automatically called by the time manager when the time specified during subscription has passed.
		 *
		 * @param msecs: The amount of time that has passed, this is always >= the time you subscribed for. It can be larger when lag occurred or updating was delayed for some reason.
		 */
		virtual void OnTimePassed(uint32_t msecs) = 0;
	};

	/**
	 *  @brief Singleton that manages time for other objects and can send notifications to subscribers when a specific amount of time passes.
	 */
	class AEON_EXPORT TimeProvider : public Singleton<TimeProvider>
	{	
		friend class Singleton<TimeProvider>;

	private:
		/**
		 * Defines a subscription, only used internally.
		 */
		struct Subscription
		{
			/**
			 * The time in milliseconds after which this subscription has to have its timer fired.
			 */
			uint32_t time;
		
			/**
			 * Calculated variable that is used to keep track of how much time has passed already (should always be < time outside of IncreaseTime).
			 */
			uint32_t timePassed;

			/**
			 * Whether this is a one-shot subscription or not.
			 */
			bool oneShot;

			/**
			 * Whether this subscriber always wants notifications, regardless of the game's state.
			 */
			bool alwaysFire;
		
			/**
			 * A pointer to the subscriber object.
			 */
			ITimeSubscriber *subscriber;
		};

	private:
		/**
		  * Indicates if the time manager is paused or not.
		  */
		bool paused;

		/**
		 * List of subscribers.
		 */
		std::deque<Subscription> subscriptions;

	protected:
		/**
		 * Constructor.
		 */
		TimeProvider();

		/**
		 * Copy constructor.
		 */
		TimeProvider(const TimeProvider&);

	public:
		
		/**
		 * Returns whether the time manager is paused or not.
		 */
		bool IsPaused() const;

		/**
		 * Pauses the time manager, stopping any timers from being fired (except those that have the alwaysFire parameter set).
		 */
		void Pause();

		/**
		 * Resumes the time manager.
		 */
		void Resume();

	public:
		/**
		 * Lets the specified subscriber subscribe to the time manager. The subscriber will be notified when the specified time has passed.
		 *
		 * @param s: The object subscribing to timer events.
		 * @param msecs: How frequently the subscriber should be notified (in milliseconds).
		 * @param oneShot: If true, the subscriber will be notified once and then unsubscribed. If false, a notification will be sent every specified amount of milliseconds.
		 * @param alwaysRun: If true, the subscriber will always be notified, even when the time manager is in the "paused" state (i.e. because a game menu is showing).
		 */
		void Subscribe(ITimeSubscriber *s, uint32_t msecs, bool oneShot = true, bool alwaysFire = false);

		/**
		 * Used by the game mechanics to indicate that a specific amount of time has passed (should be called each game update).
		 *
		 * @param msecs: The amount of milliseconds that has passed since the last call.
		 */
		void IncreaseTime(uint32_t msecs);
	};
}

#endif // TimeProvider_h__
