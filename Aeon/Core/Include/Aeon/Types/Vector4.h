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
#ifndef Vector4_h__
#define Vector4_h__

#include <Aeon/AeonTypes.h>
#include <Aeon/Types/Vector3.h>

namespace Aeon {
	/**
	 * @brief Container class for a 4-dimensional Vector Attribute.
	 */
	class AEON_EXPORT Vector4 : public Vector3
	{
	public:
		/**
		 * 
		 */
		Vector4(Real x = 0, Real y = 0, Real z = 0, Real w = 0):
			Vector3(x,y,z),
			w(w)
		{

		}
			
		/**
		 * 
		 */
		Vector4(const Vector4& rhs):
			Vector3(rhs.x,rhs.y,rhs.w),
			w(rhs.w)
		{
	
		}

	public:		
		/**
		 * 
		 */
		static Vector4 Zero()
		{
			static Vector4 zero(0.f,0.f,0.f,0.f);
			return zero;
		}
		/**
		 * 
		 */
		Real GetW() const
		{
			return w;
		}

		/**
		 * 
		 */
		void SetW(Real w)
		{
			this->w = w;
		}

	protected:
		/**
		 * 
		 */
		Real w;
	};
}

#endif // Vector4_h__
