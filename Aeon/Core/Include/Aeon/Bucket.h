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
#ifndef Bucket_h__
#define Bucket_h__

#include <Aeon/AeonTypes.h>
#include <Aeon/Export.h>

namespace Aeon {
	// TODO: Implement this (iteration 3, 4)
	typedef String BucketID;
	
	/**
	 * @brief Abstract bucket superclass.
	 */
	class AEON_EXPORT Bucket : public ITimeSubscriber
	{
	public:
		/**
		 * Sets the ID for this bucket.
		 */
		SetID(const BucketID& id)
		{
			this->id = id;
		}

		/**
		 * Retrieves the ID for this bucket.
		 */
		const BucketID& GetID() const
		{
			return id;
		}

		// Implements ITimeSubscriber
	public:
		virtual void OnTimePassed(uint32_t msecs) 
		{

		}
	protected:
		virtual void UpdateBucket(float delta) = 0;


	private:
		/**
		 * The ID for this bucket.
		 */
		BucketID id;

		/**
		 * List of in this bucket.
		 */
		std::deque<Component*> components;
	};
}

#endif // Bucket_h__
