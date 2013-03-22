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
#ifndef Range_h__
#define Range_h__

#include <Aeon/AeonTypes.h>

namespace Aeon {
	/**
	 * @brief Simple range class container to store values in a certain range.
	 */
	class AEON_EXPORT Range
	{
	public:
		/**
		 * 
		 */
		Range(Real lo, Real hi, Real val = 0.f):
			lo(lo),
			hi(hi),
			val(val)
		{

		}

		/**
		 * 
		 */
		Range(const Range& rhs):
			lo(rhs.lo),
			hi(rhs.hi),
			val(rhs.val)
		{

		}

	public:
		/**
		 * 
		 */
		Real GetLowerBound() const
		{
			return lo;
		}

		/**
		 * 
		 */
		Real GetUpperBound() const
		{
			return hi;
		}
		
		/**
		 * 
		 */
		Real GetValue() const
		{
			return val;
		}

		/**
		 * Assign lower bound value. Will only be set when the value is smaller than (or equal to) the upper bound.
		 */
		void SetLowerBound(Real lo)
		{
			if(lo <= hi)
			this->lo = lo;
		}

		/**
		 * Assign upper bound value. Will only be set when the value is greater than (or equal to) the lower bound.
		 */
		void SetUpperBound(Real hi)
		{
			if(hi >= lo)
				this->hi = hi;
		}

		/**
		 * Assign value. Will only be assigned when the value is within the lower and upper bounds.
		 */
		void SetValue(Real val)
		{
			if(val <= hi && val >= lo)
				this->val = val;
		}

	private:
		/**
		 * 
		 */
		Real lo, hi, val;
	};
}

#endif // Range_h__