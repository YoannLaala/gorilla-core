#ifndef _VECTOR2_HPP_
#define _VECTOR2_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include "../TypeDef.hpp"

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Math
{
	class Vector2
	{
	public:
		static const Vector2 Zero;
		static const Vector2 UnitX;
		static const Vector2 UnitY;
		static const Vector2 One;
		
		Vector2();
		Vector2(float32 _fX, float32 _fY);
		Vector2(const Vector2& _vOther);
		
		inline void			Set				(float32 _fX, float32 _fY) { m_fX = _fX; m_fY = _fY; }
		inline void			SetX			(float32 _fX) { m_fX = _fX; }
		inline void			SetY			(float32 _fY) { m_fY = _fY; }

		inline void			Add				(float32 _fX, float32 _fY) { m_fX += _fX; m_fY += _fY; }
		inline void			AddX			(float32 _fX) { m_fX += _fX; }
		inline void			AddY			(float32 _fY) { m_fY += _fY; }

		inline void			Sub				(float32 _fX, float32 _fY) { m_fX -= _fX; m_fY -= _fY; }
		inline void			SubX			(float32 _fX) { m_fX -= _fX; }
		inline void			SubY			(float32 _fY) { m_fY -= _fY; }

		inline void			Mul				(float32 _fX, float32 _fY) { m_fX *= _fX; m_fY *= _fY; }
		inline void			MulX			(float32 _fX) { m_fX *= _fX; }
		inline void			MulY			(float32 _fY) { m_fY *= _fY; }
	
		inline void			Div				(float32 _fX, float32 _fY) { m_fX /= _fX; m_fY /= _fY; }
		inline void			DivX			(float32 _fX) { m_fX /= _fX; }
		inline void			DivY			(float32 _fY) { m_fY /= _fY; }

		inline float32		Cross			(const Vector2& _vOther) const { return (m_fY * _vOther.GetX()) - (m_fX * _vOther.GetY()); }
		inline float32		Dot				(const Vector2& _vOther) const { return (m_fX * _vOther.GetX()) + (m_fY * _vOther.GetY()); }
		inline float32		LengthSquare	() const { return (m_fX * m_fX) + (m_fY * m_fY); }
		float32				Length			() const;
		inline void			Normalize		() { float32 fLengthInverse = 1.0f / Length(); Mul(fLengthInverse, fLengthInverse); }

		inline bool			operator==		(const Vector2& _vOther) const { return m_fX == _vOther.GetX() && m_fY == _vOther.GetY(); }
		inline bool			operator!=		(const Vector2& _vOther) const { return m_fX != _vOther.GetX() || m_fY != _vOther.GetY(); }

		inline Vector2		operator+		(const Vector2& _vOther) const { return Vector2(m_fX + _vOther.GetX(), m_fY + _vOther.GetY()); }
		inline Vector2		operator+		(float32 _fValue) const { return Vector2(m_fX + _fValue, m_fY + _fValue); }
		inline Vector2&		operator+=		(const Vector2& _vOther) { Add(_vOther.GetX(), _vOther.GetY());  return *this; }
		inline Vector2&		operator+=		(float32 _fValue) { Add(_fValue, _fValue);  return *this; }

		inline Vector2		operator-		(const Vector2& _vOther) const { return Vector2(m_fX - _vOther.GetX(), m_fY - _vOther.GetY()); }
		inline Vector2		operator-		(float32 _fValue) const { return Vector2(m_fX - _fValue, m_fY - _fValue); }
		inline Vector2&		operator-=		(const Vector2& _vOther) { Sub(_vOther.GetX(), _vOther.GetY());  return *this; }
		inline Vector2&		operator-=		(float32 _fValue) { Sub(_fValue, _fValue);  return *this; }

		inline Vector2		operator*		(const Vector2& _vOther) const { return Vector2(m_fX * _vOther.GetX(), m_fY * _vOther.GetY()); }
		inline Vector2		operator*		(float32 _fValue) const { return Vector2(m_fX * _fValue, m_fY * _fValue); }
		inline Vector2&		operator*=		(const Vector2& _vOther) { Mul(_vOther.GetX(), _vOther.GetY());  return *this; }
		inline Vector2&		operator*=		(float32 _fValue) { Mul(_fValue, _fValue);  return *this; }

		inline Vector2		operator/		(const Vector2& _vOther) const { return Vector2(m_fX / _vOther.GetX(), m_fY / _vOther.GetY()); }
		inline Vector2		operator/		(float32 _fValue) const { float32 fInverse = 1.0f / _fValue; return Vector2(m_fX * fInverse, m_fY * fInverse); }
		inline Vector2&		operator/=		(const Vector2& _vOther) { Div(_vOther.GetX(), _vOther.GetY());  return *this; }
		inline Vector2&		operator/=		(float32 _fValue) { float32 fInverse = 1.0f / _fValue; Mul(fInverse, fInverse);  return *this; }

		inline float32		operator[]		(uint32 _uiIndex) const { return *((float32*)this) + _uiIndex; }

		inline float32		GetX			() const { return m_fX; }
		inline float32		GetY			() const { return m_fY; }

	private:
		float32 m_fX;
		float32 m_fY;
	};
}}

#endif