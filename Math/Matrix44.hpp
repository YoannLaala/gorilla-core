#ifndef _MATRIX44_HPP_
#define _MATRIX44_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include "Vector3.hpp"
#include "Quaternion.hpp"

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Math
{
	class Matrix44
	{
	public:
		static const Matrix44 Identity;

		Matrix44();
		Matrix44(const Matrix44& _mOther);

		inline void			SetTranslation		(const Vector3& _vTranslation) { SetTranslation(_vTranslation.GetX(), _vTranslation.GetY(), _vTranslation.GetZ()); }
		inline void			SetTranslation		(float32 _fTranslation) { SetTranslation(_fTranslation, _fTranslation, _fTranslation); }
		inline void			SetTranslation		(float32 _fX, float32 _fY, float32 _fZ) { m_mValue[3][0] = _fX; m_mValue[3][1] = _fY; m_mValue[3][2] = _fZ; }

		inline void			ApplyTranslation	(const Vector3& _vTranslation) { ApplyTranslation(_vTranslation.GetX(), _vTranslation.GetY(), _vTranslation.GetZ()); }
		inline void			ApplyTranslation	(float32 _fTranslation) { ApplyTranslation(_fTranslation, _fTranslation, _fTranslation); }
		inline void			ApplyTranslation	(float32 _fX, float32 _fY, float32 _fZ) { m_mValue[3][0] += _fX; m_mValue[3][1] += _fY; m_mValue[3][2] += _fZ; }

		inline void			SetScale			(const Vector3& _vScale) { SetScale(_vScale.GetX(), _vScale.GetY(), _vScale.GetZ()); }
		inline void			SetScale			(float32 _fScale) { SetScale(_fScale, _fScale, _fScale); }
		inline void			SetScale			(float32 _fX, float32 _fY, float32 _fZ) { m_mValue[0][0] = _fX; m_mValue[1][1] = _fY; m_mValue[2][2] = _fZ; }

		inline void			ApplyScale			(const Vector3& _vScale) { ApplyScale(_vScale.GetX(), _vScale.GetY(), _vScale.GetZ()); }
		inline void			ApplyScale			(float32 _fScale) { ApplyScale(_fScale, _fScale, _fScale); }
		inline void			ApplyScale			(float32 _fX, float32 _fY, float32 _fZ) { m_mValue[0][0] *= _fX; m_mValue[1][1] *= _fY; m_mValue[2][2] *= _fZ; }

		void				SetRotation			(const Quaternion& _qRotation);

		Matrix44			Inverse				();

		void				Transform			(const Vector3& _vPosition, const Quaternion& _qRotation, const Vector3& _vScale);
		void				Transform			(const Matrix44& _mOther);

		inline float32*		operator[]			(uint32 _uiIndex) { return m_mValue[_uiIndex]; }
		inline const float32* operator[]		(uint32 _uiIndex) const { return m_mValue[_uiIndex]; }

		inline Matrix44		operator*			(const Matrix44& _mOther) const { Matrix44 mResult(*this); mResult.Transform(_mOther); return mResult; }
		inline Matrix44&	operator*=			(const Matrix44& _mOther) { Transform(_mOther); return *this; }

	private:
		float32 m_mValue[4][4];
	};
}}

#endif