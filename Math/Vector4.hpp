#ifndef _VECTOR4_HPP_
#define _VECTOR4_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include "../TypeDef.hpp"

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Math
{
	class Vector4
	{
	public:
		static const Vector4 Zero;
		static const Vector4 UnitX;
		static const Vector4 UnitY;
		static const Vector4 UnitZ;
		static const Vector4 UnitW;
		static const Vector4 One;
		
		Vector4();
		Vector4(float32 _fX, float32 _fY, float32 _fZ, float32 _fW);
		Vector4(const Vector4& _vOther);
		
		inline void			Set				(float32 _fX, float32 _fY, float32 _fZ, float32 _fW) { m_fX = _fX; m_fY = _fY; m_fZ = _fZ; m_fW = _fW; }
		inline void			SetX			(float32 _fX) { m_fX = _fX; }
		inline void			SetY			(float32 _fY) { m_fY = _fY; }
		inline void			SetZ			(float32 _fZ) { m_fZ = _fZ; }
		inline void			SetW			(float32 _fW) { m_fW = _fW; }

		inline void			Add				(float32 _fX, float32 _fY, float32 _fZ, float32 _fW) { m_fX += _fX; m_fY += _fY; m_fZ += _fZ; m_fW += _fW; }
		inline void			AddX			(float32 _fX) { m_fX += _fX; }
		inline void			AddY			(float32 _fY) { m_fY += _fY; }
		inline void			AddZ			(float32 _fZ) { m_fZ += _fZ; }
		inline void			AddW			(float32 _fW) { m_fW += _fW; }

		inline void			Sub				(float32 _fX, float32 _fY, float32 _fZ, float32 _fW) { m_fX -= _fX; m_fY -= _fY; m_fZ -= _fZ; m_fW -= _fW; }
		inline void			SubX			(float32 _fX) { m_fX -= _fX; }
		inline void			SubY			(float32 _fY) { m_fY -= _fY; }
		inline void			SubZ			(float32 _fZ) { m_fZ -= _fZ; }
		inline void			SubW			(float32 _fW) { m_fW -= _fW; }

		inline void			Mul				(float32 _fX, float32 _fY, float32 _fZ, float32 _fW) { m_fX *= _fX; m_fY *= _fY; m_fZ *= _fZ; m_fW *= _fW; }
		inline void			MulX			(float32 _fX) { m_fX *= _fX; }
		inline void			MulY			(float32 _fY) { m_fY *= _fY; }
		inline void			MulZ			(float32 _fZ) { m_fZ *= _fZ; }
		inline void			MulW			(float32 _fW) { m_fW *= _fW; }
	
		inline void			Div				(float32 _fX, float32 _fY, float32 _fZ, float32 _fW) { m_fX /= _fX; m_fY /= _fY; m_fZ /= _fZ; m_fW /= _fW; }
		inline void			DivX			(float32 _fX) { m_fX /= _fX; }
		inline void			DivY			(float32 _fY) { m_fY /= _fY; }
		inline void			DivZ			(float32 _fZ) { m_fZ /= _fZ; }
		inline void			DivW			(float32 _fW) { m_fW /= _fW; }

		inline float32		LengthSquare	() const { return (m_fX * m_fX) + (m_fY * m_fY) + (m_fZ * m_fZ) + (m_fW * m_fW); }
		float32				Length			() const;
		inline void			Normalize		() { float32 fLengthInverse = 1.0f / Length(); Mul(fLengthInverse, fLengthInverse, fLengthInverse,fLengthInverse); }

		inline bool			operator==		(const Vector4& _vOther) const { return m_fX == _vOther.GetX() && m_fY == _vOther.GetY() && m_fZ == _vOther.GetZ() && m_fW == _vOther.GetW(); }
		inline bool			operator!=		(const Vector4& _vOther) const { return m_fX != _vOther.GetX() || m_fY != _vOther.GetY() || m_fZ != _vOther.GetZ() && m_fW != _vOther.GetW(); }

		inline Vector4		operator+		(const Vector4& _vOther) const { return Vector4(m_fX + _vOther.GetX(), m_fY + _vOther.GetY(), m_fZ + _vOther.GetZ(), m_fW + _vOther.GetW()); }
		inline Vector4		operator+		(float32 _fValue) const { return Vector4(m_fX + _fValue, m_fY + _fValue, m_fZ + _fValue, m_fW + _fValue); }
		inline Vector4&		operator+=		(const Vector4& _vOther) { Add(_vOther.GetX(), _vOther.GetY(), _vOther.GetZ(), _vOther.GetW());  return *this; }
		inline Vector4&		operator+=		(float32 _fValue) { Add(_fValue, _fValue, _fValue, _fValue);  return *this; }

		inline Vector4		operator-		(const Vector4& _vOther) const { return Vector4(m_fX - _vOther.GetX(), m_fY - _vOther.GetY(), m_fZ - _vOther.GetZ(), m_fW - _vOther.GetW()); }
		inline Vector4		operator-		(float32 _fValue) const { return Vector4(m_fX - _fValue, m_fY - _fValue, m_fZ - _fValue, m_fW - _fValue); }
		inline Vector4&		operator-=		(const Vector4& _vOther) { Sub(_vOther.GetX(), _vOther.GetY(), _vOther.GetZ(), _vOther.GetW());  return *this; }
		inline Vector4&		operator-=		(float32 _fValue) { Sub(_fValue, _fValue, _fValue, _fValue);  return *this; }

		inline Vector4		operator*		(const Vector4& _vOther) const { return Vector4(m_fX * _vOther.GetX(), m_fY * _vOther.GetY(), m_fZ * _vOther.GetZ(), m_fW * _vOther.GetW()); }
		inline Vector4		operator*		(float32 _fValue) const { return Vector4(m_fX * _fValue, m_fY * _fValue, m_fZ * _fValue, m_fW * _fValue); }
		inline Vector4&		operator*=		(const Vector4& _vOther) { Mul(_vOther.GetX(), _vOther.GetY(), _vOther.GetZ(), _vOther.GetW());  return *this; }
		inline Vector4&		operator*=		(float32 _fValue) { Mul(_fValue, _fValue, _fValue, _fValue);  return *this; }

		inline Vector4		operator/		(const Vector4& _vOther) const { return Vector4(m_fX / _vOther.GetX(), m_fY / _vOther.GetY(), m_fZ / _vOther.GetZ(), m_fW / _vOther.GetW()); }
		inline Vector4		operator/		(float32 _fValue) const { float32 fInverse = 1.0f / _fValue; return Vector4(m_fX * fInverse, m_fY * fInverse, m_fZ * fInverse, m_fW * fInverse); }
		inline Vector4&		operator/=		(const Vector4& _vOther) { Div(_vOther.GetX(), _vOther.GetY(), _vOther.GetZ(), _vOther.GetW());  return *this; }
		inline Vector4&		operator/=		(float32 _fValue) { float32 fInverse = 1.0f / _fValue; Mul(fInverse, fInverse, fInverse, fInverse);  return *this; }

		inline float32		operator[]		(uint32 _uiIndex) const { return *((float32*)this) + _uiIndex; }

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