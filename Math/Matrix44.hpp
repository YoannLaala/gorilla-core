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

		inline void				SetIdentity			() { *this = Identity; }

		inline const Vector3&	GetTranslation		() const { return *reinterpret_cast<const Vector3*>(m_mValue[3]); }
		inline void				SetTranslation		(const Vector3& _vTranslation) { SetTranslation(_vTranslation.GetX(), _vTranslation.GetY(), _vTranslation.GetZ()); }
		inline void				SetTranslation		(float32 _fTranslation) { SetTranslation(_fTranslation, _fTranslation, _fTranslation); }
		inline void				SetTranslation		(float32 _fX, float32 _fY, float32 _fZ) { m_mValue[3][0] = _fX; m_mValue[3][1] = _fY; m_mValue[3][2] = _fZ; }

		inline void				Translate			(const Vector3& _vTranslation) { Translate(_vTranslation.GetX(), _vTranslation.GetY(), _vTranslation.GetZ()); }
		inline void				Translate			(float32 _fTranslation) { Translate(_fTranslation, _fTranslation, _fTranslation); }
		inline void				Translate			(float32 _fX, float32 _fY, float32 _fZ) { m_mValue[3][0] += _fX; m_mValue[3][1] += _fY; m_mValue[3][2] += _fZ; }

		Vector3					GetScale			() const;
		inline void				SetScale			(const Vector3& _vScale) { SetScale(_vScale.GetX(), _vScale.GetY(), _vScale.GetZ()); }
		inline void				SetScale			(float32 _fScale) { SetScale(_fScale, _fScale, _fScale); }
		inline void				SetScale			(float32 _fX, float32 _fY, float32 _fZ) { m_mValue[0][0] = _fX; m_mValue[1][1] = _fY; m_mValue[2][2] = _fZ; }

		inline void				Scale				(const Vector3& _vScale) { Scale(_vScale.GetX(), _vScale.GetY(), _vScale.GetZ()); }
		inline void				Scale				(float32 _fScale) { Scale(_fScale, _fScale, _fScale); }
		inline void				Scale				(float32 _fX, float32 _fY, float32 _fZ) { m_mValue[0][0] *= _fX; m_mValue[1][1] *= _fY; m_mValue[2][2] *= _fZ; }

		Quaternion				GetOrientation		() const;
		inline void				SetOrientation		(const Vector3& _vEuler) { SetOrientation(_vEuler.GetX(), _vEuler.GetY(), _vEuler.GetZ()); }
		inline void				SetOrientation		(float32 _fEulerX, float32 _fEulerY, float32 _fEulerZ) { Quaternion qOrientation; qOrientation.Rotate(_fEulerX, _fEulerY, _fEulerZ); SetOrientation(qOrientation); }
		void					SetOrientation		(const Quaternion& _qOrientation);
		inline void				Rotate				(const Vector3& _vEuler) { Rotate(_vEuler.GetX(), _vEuler.GetY(), _vEuler.GetZ()); }
		inline void				Rotate				(float32 _fEulerX, float32 _fEulerY, float32 _fEulerZ) { Quaternion qRotation; qRotation.Rotate(_fEulerX, _fEulerY, _fEulerZ); Rotate(qRotation); }
		void					Rotate				(const Quaternion& _qRotation);

		Matrix44				Inverse				();

		inline float32*			operator[]			(uint32 _uiIndex) { return m_mValue[_uiIndex]; }
		inline const float32*	operator[]			(uint32 _uiIndex) const { return m_mValue[_uiIndex]; }

		inline Matrix44			operator*			(const Matrix44& _mOther) const { Matrix44 mResult(*this); mResult *= _mOther; return mResult; }
		Matrix44&				operator*=			(const Matrix44& _mOther);

	private:
		float32 m_mValue[4][4];
	};
}}

#endif