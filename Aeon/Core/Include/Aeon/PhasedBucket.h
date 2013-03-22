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
#ifndef PhasedBucket_h__
#define PhasedBucket_h__

#include <map>

#include <Aeon/Export.h>
#include <Aeon/Bucket.h>

// TODO: Implement this (iteration 3, 4)
typedef int Phase;

namespace Aeon {
	/**
	 * @brief Phased bucket implementation.
	 */
	class AEON_EXPORT PhasedBucket
	{
		typedef std:map<const Phase, Bucket> BucketMap;

	public:


	private:
		/**
		 * Bucket map.
		 */
		BucketMap buckets;
	};
}

#endif // PhasedBucket_h__
