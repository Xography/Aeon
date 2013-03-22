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
#ifndef Vector2_h__
#define Vector2_h__

#include <Aeon/AeonTypes.h>

namespace Aeon {
	/**
	 * @brief Container class for a 2-dimensional Vector Attribute.
	 */
	class AEON_EXPORT Vector2 
	{
	public:
		/**
		 * 
		 */
		Vector2(Real x, Real y):
			x(x),
			y(y)
		{

		}
		
		/**
		 * 
		 */
		Vector2(const Vector2& rhs):
			x(rhs.x),
			y(rhs.y)
		{
	
		}

	public:	
		/**
		 * 
		 */
		static Vector2 Zero()
		{
			static Vector2 zero(0.f,0.f);
			return zero;
		}

		/**
		 * 
		 */
		Real GetX() const
		{
			return x;
		}

		/**
		 * 
		 */
		Real GetY() const
		{
			return y;
		}


		/**
		 * 
		 */
		void SetX(Real x)
		{
			this->x = x;
		}

		/**
		 * 
		 */
		void SetY(Real y)
		{
			this->y = y;
		}
			
	protected:
		/**
		 * 
		 */
		Real x, y;
	};
}

#endif // Vector2_h__