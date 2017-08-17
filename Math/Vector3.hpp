#ifndef _VECTOR3_HPP_
#define _VECTOR3_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include "../TypeDef.hpp"

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Math
{
	class Vector3
	{
	public:
		static const Vector3 Zero;
		static const Vector3 UnitX;
		static const Vector3 UnitY;
		static const Vector3 UnitZ;
		static const Vector3 One;
		
		Vector3();
		Vector3(float32 _fX, float32 _fY, float32 _fZ);
		Vector3(const Vector3& _vOther);
		
		inline void			Set				(float32 _fX, float32 _fY, float32 _fZ) { m_fX = _fX; m_fY = _fY; m_fZ = _fZ; }
		inline void			SetX			(float32 _fX) { m_fX = _fX; }
		inline void			SetY			(float32 _fY) { m_fY = _fY; }
		inline void			SetZ			(float32 _fZ) { m_fZ = _fZ; }

		inline void			Add				(float32 _fX, float32 _fY, float32 _fZ) { m_fX += _fX; m_fY += _fY; m_fZ += _fZ; }
		inline void			AddX			(float32 _fX) { m_fX += _fX; }
		inline void			AddY			(float32 _fY) { m_fY += _fY; }
		inline void			AddZ			(float32 _fZ) { m_fZ += _fZ; }

		inline void			Sub				(float32 _fX, float32 _fY, float32 _fZ) { m_fX += _fX; m_fY += _fY; m_fZ -= _fZ; }
		inline void			SubX			(float32 _fX) { m_fX -= _fX; }
		inline void			SubY			(float32 _fY) { m_fY -= _fY; }
		inline void			SubZ			(float32 _fZ) { m_fZ -= _fZ; }

		inline void			Mul				(float32 _fX, float32 _fY, float32 _fZ) { m_fX *= _fX; m_fY *= _fY; m_fZ *= _fZ; }
		inline void			MulX			(float32 _fX) { m_fX *= _fX; }
		inline void			MulY			(float32 _fY) { m_fY *= _fY; }
		inline void			MulZ			(float32 _fZ) { m_fZ *= _fZ; }
	
		inline void			Div				(float32 _fX, float32 _fY, float32 _fZ) { m_fX /= _fX; m_fY /= _fY; m_fZ /= _fZ; }
		inline void			DivX			(float32 _fX) { m_fX /= _fX; }
		inline void			DivY			(float32 _fY) { m_fY /= _fY; }
		inline void			DivZ			(float32 _fZ) { m_fZ /= _fZ; }

		inline Vector3		Cross			(const Vector3& _vOther) const { return Vector3((m_fY * _vOther.GetZ()) - (m_fZ * _vOther.GetY()), (m_fZ * _vOther.GetX()) - (m_fX * _vOther.GetZ()), (m_fX * _vOther.GetY()) - (m_fY * _vOther.GetX())); }
		inline float32		Dot				(const Vector3& _vOther) const { return (m_fX * _vOther.GetX()) + (m_fY * _vOther.GetY()) + (m_fZ * _vOther.GetZ()); }
		inline float32		LengthSquare	() const { return (m_fX * m_fX) + (m_fY * m_fY) + (m_fZ * m_fZ); }
		float32				Length			() const;
		void				Normalize		();

		inline bool			operator==		(const Vector3& _vOther) const { return m_fX == _vOther.GetX() && m_fY == _vOther.GetY() && m_fZ == _vOther.GetZ(); }
		inline bool			operator!=		(const Vector3& _vOther) const { return m_fX != _vOther.GetX() || m_fY != _vOther.GetY() || m_fZ != _vOther.GetZ(); }

		inline Vector3		operator+		(const Vector3& _vOther) const { return Vector3(m_fX + _vOther.GetX(), m_fY + _vOther.GetY(), m_fZ + _vOther.GetZ()); }
		inline Vector3		operator+		(float32 _fValue) const { return Vector3(m_fX + _fValue, m_fY + _fValue, m_fZ + _fValue); }
		inline Vector3&		operator+=		(const Vector3& _vOther) { Add(_vOther.GetX(), _vOther.GetY(), _vOther.GetZ());  return *this; }
		inline Vector3&		operator+=		(float32 _fValue) { Add(_fValue, _fValue, _fValue);  return *this; }

		inline Vector3		operator-		(const Vector3& _vOther) const { return Vector3(m_fX - _vOther.GetX(), m_fY - _vOther.GetY(), m_fZ - _vOther.GetZ()); }
		inline Vector3		operator-		(float32 _fValue) const { return Vector3(m_fX - _fValue, m_fY - _fValue, m_fZ - _fValue); }
		inline Vector3&		operator-=		(const Vector3& _vOther) { Sub(_vOther.GetX(), _vOther.GetY(), _vOther.GetZ());  return *this; }
		inline Vector3&		operator-=		(float32 _fValue) { Sub(_fValue, _fValue, _fValue);  return *this; }

		inline Vector3		operator*		(const Vector3& _vOther) const { return Vector3(m_fX * _vOther.GetX(), m_fY * _vOther.GetY(), m_fZ * _vOther.GetZ()); }
		inline Vector3		operator*		(float32 _fValue) const { return Vector3(m_fX * _fValue, m_fY * _fValue, m_fZ * _fValue); }
		inline Vector3&		operator*=		(const Vector3& _vOther) { Mul(_vOther.GetX(), _vOther.GetY(), _vOther.GetZ());  return *this; }
		inline Vector3&		operator*=		(float32 _fValue) { Mul(_fValue, _fValue, _fValue);  return *this; }

		inline Vector3		operator/		(const Vector3& _vOther) const { return Vector3(m_fX / _vOther.GetX(), m_fY / _vOther.GetY(), m_fZ / _vOther.GetZ()); }
		inline Vector3		operator/		(float32 _fValue) const { float32 fInverse = 1.0f / _fValue; return Vector3(m_fX * fInverse, m_fY * fInverse, m_fZ * fInverse); }
		inline Vector3&		operator/=		(const Vector3& _vOther) { Div(_vOther.GetX(), _vOther.GetY(), _vOther.GetZ());  return *this; }
		inline Vector3&		operator/=		(float32 _fValue) { float32 fInverse = 1.0f / _fValue; Mul(fInverse, fInverse, fInverse);  return *this; }

		inline float32		operator[]		(uint32 _uiIndex) const { return reinterpret_cast<const float32*>(this)[_uiIndex]; }
		inline float32&		operator[]		(uint32 _uiIndex) { return reinterpret_cast<float32*>(this)[_uiIndex]; }

		inline float32		GetX			() const { return m_fX; }
		inline float32		GetY			() const { return m_fY; }
		inline float32		GetZ			() const { return m_fZ; }

	private:
		float32 m_fX;
		float32 m_fY;
		float32 m_fZ;
	};
}}

#endif