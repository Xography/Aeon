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
#ifndef Vector3_h__
#define Vector3_h__

#include <Aeon/AeonTypes.h>
#include <Aeon/Types/Vector2.h>

namespace Aeon {
	/**
	 * @brief Container class for a 3-dimensional Vector Attribute.
	 */
	class AEON_EXPORT Vector3 : public Vector2
	{
	public:
		/**
		 * 
		 */
		Vector3(Real x, Real y, Real z):
			Vector2(x,y),
			z(z)
		{

		}
			
		/**
		 * 
		 */
		Vector3(const Vector3& rhs):
			Vector2(rhs.x,rhs.y),
			z(rhs.z)
		{
	
		}

	public:		
		/**
		 * 
		 */
		static Vector3 Zero()
		{
			static Vector3 zero(0.f,0.f,0.f);
			return zero;
		}

		/**
		 * 
		 */
		Real GetZ() const
		{
			return z;
		}

		/**
		 * 
		 */
		void SetZ(Real z)
		{
			this->z = z;
		}
		
	protected:
		/**
		 * 
		 */
		Real z;
	};
}

#endif // Vector3_h__