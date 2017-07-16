#ifndef _QUATERNION_HPP_
#define _QUATERNION_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include "Vector3.hpp"

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Math
{
	class Quaternion
	{
	public:
		static const Quaternion Identity;

		Quaternion();
		Quaternion(float32 _fX, float32 _fY, float32 _fZ, float32 _fW);
		Quaternion(const Vector3& _vAxis, float32 _fAngle);
		Quaternion(const Quaternion& _qOther);
		
		inline void			Set				(float32 _fX, float32 _fY, float32 _fZ, float32 _fW) { m_fX = _fX; m_fY = _fY; m_fZ = _fZ; m_fW = _fW; }
		inline void			SetX			(float32 _fX) { m_fX = _fX; }
		inline void			SetY			(float32 _fY) { m_fY = _fY; }
		inline void			SetZ			(float32 _fZ) { m_fZ = _fZ; }
		inline void			SetW			(float32 _fW) { m_fW = _fW; }
		
		inline float32		LengthSquare	() const { return (m_fX * m_fX) + (m_fY * m_fY) + (m_fZ * m_fZ) + (m_fW * m_fW); }
		float32				Length			() const;
		inline void			Normalize		() { float32 fLengthInverse = 1.0f / Length(); m_fX *= fLengthInverse; m_fY *= fLengthInverse; m_fZ *= fLengthInverse; m_fW *= fLengthInverse; }

		inline void			Conjugate		() { m_fX = -m_fX; m_fY = -m_fY; m_fZ = -m_fZ; }
		inline void			Inverse			() { float32 fLengthSquareInverse = 1.0f / LengthSquare(); Conjugate(); *this *= fLengthSquareInverse; }
		void				Rotate			(const Vector3& _vRotation);
		void				Rotate			(float32 _fPitchAngle, float32 _fYawAngle, float32 _fRollAngle);
		void				Rotate			(const Vector3& _vAxis, float32 _fRadian);

		inline Quaternion	operator+		(const Quaternion& _qOther) const { return Quaternion(m_fX + _qOther.GetX(), m_fY + _qOther.GetY(), m_fZ + _qOther.GetZ(), m_fW + _qOther.GetW()); }
		inline Quaternion&	operator+=		(const Quaternion& _qOther) { m_fX += _qOther.GetX(); m_fY += _qOther.GetY(); m_fZ += _qOther.GetZ(); m_fW += _qOther.GetW(); return *this;}

		inline Quaternion	operator-		(const Quaternion& _qOther) const { return Quaternion(m_fX - _qOther.GetX(), m_fY - _qOther.GetY(), m_fZ - _qOther.GetZ(), m_fW - _qOther.GetW()); }
		inline Quaternion&	operator-=		(const Quaternion& _qOther) { m_fX -= _qOther.GetX(); m_fY -= _qOther.GetY(); m_fZ -= _qOther.GetZ(); m_fW -= _qOther.GetW(); return *this;}

		inline Quaternion	operator*		(const Quaternion& _qOther) const { Quaternion qThis(*this) ; return qThis *= _qOther; }
		Vector3				operator*		(const Vector3& _vOther) const;
		inline Quaternion	operator*		(float32 _fValue) const { return Quaternion(m_fX * _fValue, m_fY * _fValue, m_fZ * _fValue, m_fW * _fValue); }
		Quaternion&			operator*=		(const Quaternion& _qOther);
		inline Quaternion&	operator*=		(float32 _fValue) { m_fX *= _fValue; m_fY *= _fValue; m_fZ *= _fValue; m_fW *= _fValue; return *this; }

		inline Quaternion	operator/	(float32 _fValue) const { float32 fInverse = 1.0f / _fValue; return Quaternion(m_fX * fInverse, m_fY * fInverse, m_fZ * fInverse, m_fW * _fValue); }
		inline Quaternion&	operator/=	(float32 _fValue) { float32 fInverse = 1.0f / _fValue; return *this *=  fInverse; }

		inline float32		GetX			() const { return m_fX; }
		inline float32		GetY			() const { return m_fY; }
		inline float32		GetZ			() const { return m_fZ; }
		inline float32		GetW			() const { return m_fW; }

	private:
		float32 m_fX;
		float32 m_fY;
		float32 m_fZ;
		float32 m_fW;
	};
}}

#endif