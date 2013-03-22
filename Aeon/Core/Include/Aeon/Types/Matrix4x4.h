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
#ifndef Matrix4x4_h__
#define Matrix4x4_h__

#include <Aeon/Types/Vector4.h>

namespace Aeon {
	/**
	 * @brief Container class for a 4x4 Matrix.
	 */
	class AEON_EXPORT Matrix4x4
	{
	public:
		/**
		 *
		 */
		Matrix4x4(const Vector4& x0, const Vector4& x1, const Vector4& x2, const Vector4& x3):
			x0(x0),
			x1(x1),
			x2(x2),
			x3(x3)
		{

		}

		/**
		 *
		 */
		Matrix4x4(	Real x00, Real x01, Real x02, Real x03,
		            Real x10, Real x11, Real x12, Real x13,
		            Real x20, Real x21, Real x22, Real x23,
		            Real x30, Real x31, Real x32, Real x33):
			x0(x00,x01,x02,x03),
			x1(x10,x11,x12,x13),
			x2(x20,x21,x22,x23),
			x3(x30,x31,x32,x33)
		{

		}

	public:
		static Matrix4x4 Zero()
		{
			static Matrix4x4 zero(
			    0.f,0.f,0.f,0.f,
			    0.f,0.f,0.f,0.f,
			    0.f,0.f,0.f,0.f,
			    0.f,0.f,0.f,0.f);
			return zero;
		}
		/**
		 *
		 */
		const Vector4 GetX0() const
		{
			return x0;
		}

		/**
		 *
		 */
		const Vector4 GetX1() const
		{
			return x1;
		}

		/**
		 *
		 */
		const Vector4 GetX2() const
		{
			return x2;
		}

		/**
		 *
		 */
		const Vector4 GetX3() const
		{
			return x3;
		}

		/**
		 *
		 */
		void SetX0(const Vector4& x0)
		{
			this->x0 = x0;
		}

		/**
		 *
		 */
		void SetX1(const Vector4& x1)
		{
			this->x1 = x1;
		}

		/**
		 *
		 */
		void SetX2(const Vector4& x2)
		{
			this->x2 = x2;
		}

		/**
		 *
		 */
		void SetX3(const Vector4& x3)
		{
			this->x3 = x3;
		}
	private:

		/**
		 *
		 */
		Vector4 x0, x1, x2, x3;
	};
}

#endif // Matrix4x4_h__