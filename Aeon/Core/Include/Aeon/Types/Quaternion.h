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
#ifndef Quaternion_h__
#define Quaternion_h__

#include <Aeon/Types/Vector4.h>

namespace Aeon {
	/**
	 * @brief Quaternion container. Until we have math in place, this should do.
	 */
	class AEON_EXPORT Quaternion
	{
	public:
		/**
		 * 
		 */
		Quaternion(Real x, Real y, Real z, Real w):
			data(x,y,z,w)
		{

		}

	public:
		/**
		 * 
		 */
		static Quaternion Zero()
		{
			static Quaternion zero(0.f,0.f,0.f,0.f);
			return zero;
		}

		/**
		 * 
		 */
		void SetVector4(const Vector4& data)
		{
			this->data = data;
		}

		/**
		 * 
		 */
		const Vector4& GetVector4() const
		{
			return data;
		}

	private:
		/**
		 * 
		 */
		Vector4 data;
	};
}

#endif // Quaternion_h__
