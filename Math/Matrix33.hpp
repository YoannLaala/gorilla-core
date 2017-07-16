#ifndef _CORE_MATRIX33_HPP_
#define _CORE_MATRIX33_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Math/Vector2.hpp>
#include <Core/Math/Vector3.hpp>
#include <Core/Math/Quaternion.hpp>

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Math
{
	class Matrix33
	{
	public:
		static const Matrix33 Identity;

		Matrix33();
		Matrix33(const Matrix33& _mOther);

		// Translation
		inline const Vector3&	GetTranslation		() const { return *reinterpret_cast<const Vector3*>(m_aValue[2]); }
		inline void				Translate			(const Vector2& _vValue) { Translate(_vValue.GetX(), _vValue.GetY()); }
		inline void				Translate			(float32 _fValue) { Translate(_fValue, _fValue); }
		inline void				Translate			(float32 _fValueX, float32 _fValueY) { m_aValue[2][0] += _fValueX;  m_aValue[2][1] += _fValueY; }
		inline void				SetTranslation		(const Vector2& _vValue) { SetTranslation(_vValue.GetX(), _vValue.GetY()); }
		inline void				SetTranslation		(float32 _fValue) { SetTranslation(_fValue, _fValue); }
		inline void				SetTranslation		(float32 _fValueX, float32 _fValueY) { m_aValue[2][0] = _fValueX; m_aValue[2][1] = _fValueY; }

		// Scale
		inline void				Scale				(const Vector2& _vValue) { Scale(_vValue.GetX(), _vValue.GetY()); }
		inline void				Scale				(float32 _fValue) { Scale(_fValue, _fValue); }
		inline void				Scale				(float32 _fValueX, float32 _fValueY) { m_aValue[0][0] *= _fValueX;  m_aValue[0][1] *= _fValueY; }
		inline void				SetScale			(const Vector2& _vValue) { SetScale(_vValue.GetX(), _vValue.GetY()); }
		inline void				SetScale			(float32 _fValue) { SetScale(_fValue, _fValue); }
		inline void				SetScale			(float32 _fValueX, float32 _fValueY) { m_aValue[0][0] = _fValueX;m_aValue[1][1] = _fValueY; }

		// Rotation
		void				SetRotation				(const Quaternion& _qRotation);

		void				Transform				(const Vector2& _vPosition, const Quaternion& _qRotation, const Vector2& _vScale);
		void				Transform				(const Matrix33& _mOther);

		inline float32*		operator[]				(uint32 _uiIndex) { return m_aValue[_uiIndex]; }
		inline const float32* operator[]			(uint32 _uiIndex) const { return m_aValue[_uiIndex]; }

		inline Matrix33		operator*				(const Matrix33& _mOther) const { Matrix33 mResult(*this); mResult.Transform(_mOther); return mResult; }
		inline Matrix33&	operator*=				(const Matrix33& _mOther) { Transform(_mOther); }

	private:
		float32 m_aValue[3][3];
	};
}}

#endif