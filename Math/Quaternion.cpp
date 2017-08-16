/******************************************************************************
**	Includes
******************************************************************************/
#include "Quaternion.hpp"
#include <math.h>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Math
{
	const Quaternion Quaternion::Identity = Quaternion(0.0f,0.0f,0.0f,1.0f);

	//!	@brief		Constructor
	//!	@date		2015-11-08
	Quaternion::Quaternion()
		: m_fX (0.0f)
		, m_fY (0.0f)
		, m_fZ (0.0f)
		, m_fW (1.0f)
	{
		// Nothing to do
	}

	//!	@brief		Constructor
	//!	@date		2015-11-08
	Quaternion::Quaternion(float32 _fX, float32 _fY, float32 _fZ, float32 _fW)
		: m_fX (_fX)
		, m_fY (_fY)
		, m_fZ (_fZ)
		, m_fW (_fW)
	{
		// Nothing to do
	}

	//!	@brief		Constructor
	//!	@date		2015-11-08
	Quaternion::Quaternion(const Vector3& _vAxis, float32 _fAngle)
	{
		Rotate(_vAxis, _fAngle);
	}

	//!	@brief		Constructor
	//!	@date		2015-11-08
	Quaternion::Quaternion(const Quaternion& _qOther)
		: m_fX (_qOther.GetX())
		, m_fY (_qOther.GetY())
		, m_fZ (_qOther.GetZ())
		, m_fW (_qOther.GetW())
	{
		// Nothing to do
	}

	//!	@brief		Rotate
	//!	@date		2015-11-08
	void Quaternion::Rotate(const Vector3& _vRotation)
	{
		Rotate(_vRotation.GetX(), _vRotation.GetY(), _vRotation.GetZ());
	}

	//!	@brief		Rotate
	//!	@date		2015-11-08
	void Quaternion::Rotate(float32 _fPitchAngle, float32 _fYawAngle, float32 _fRollAngle)
	{
		//Pitch
		if(_fPitchAngle)
		{
			_fPitchAngle *= 0.5f;
			Quaternion qPitchAxis(sinf(_fPitchAngle), 0, 0, cosf(_fPitchAngle));
			*this *= qPitchAxis;
		}

		// Yaw
		if(_fYawAngle)
		{
			_fYawAngle *= 0.5f;
			Quaternion qYawAxis(0, sinf(_fYawAngle), 0, cosf(_fYawAngle));
			*this *= qYawAxis;
		}

		// Roll
		if(_fRollAngle)
		{
			_fRollAngle *= 0.5f;
			Quaternion qRollAxis(0, 0, sinf(_fRollAngle), cosf(_fRollAngle));
			*this *= qRollAxis;
		}
	}

	//!	@brief		Rotate
	//!	@date		2015-11-08
	void Quaternion::Rotate(const Vector3& _vAxis, float32 _fRadian)
	{
		float32 fHalfRadian = _fRadian * 0.5f;
		float32 fSinAngle = sinf(fHalfRadian);

		m_fX = _vAxis.GetX() * fSinAngle;
		m_fY = _vAxis.GetY() * fSinAngle;
		m_fZ = _vAxis.GetZ() * fSinAngle;
		m_fW = cosf(fHalfRadian);
	}

	//!	@brief		Length
	//!	@date		2015-11-08
	float32	Quaternion::Length() const 
	{ 
		return sqrtf(LengthSquare()); 
	}

	//!	@brief		operator *=
	//!	@date		2015-11-08
	Vector3 Quaternion::operator* (const Vector3& _vOther) const
	{
		Quaternion qVector(_vOther.GetX(), _vOther.GetY(), _vOther.GetZ(), 1.0f);
		Quaternion qThisConjugated(*this);
		qThisConjugated.Conjugate();

		Quaternion qResult = *this * qVector * qThisConjugated; 

		return Vector3(qResult.GetX(), qResult.GetY(), qResult.GetZ());
	}

	//!	@brief		operator *=
	//!	@date		2015-11-08
	Quaternion& Quaternion::operator*= (const Quaternion& _qOther)
	{
		Quaternion qThis(*this);
		m_fX = +(qThis.GetX() * _qOther.GetW()) + (qThis.GetY() * _qOther.GetZ()) - (qThis.GetZ() * _qOther.GetY()) + (qThis.GetW() * _qOther.GetX());
		m_fY = -(qThis.GetX() * _qOther.GetZ()) + (qThis.GetY() * _qOther.GetW()) + (qThis.GetZ() * _qOther.GetX()) + (qThis.GetW() * _qOther.GetY());
		m_fZ = +(qThis.GetX() * _qOther.GetY()) - (qThis.GetY() * _qOther.GetX()) + (qThis.GetZ() * _qOther.GetW()) + (qThis.GetW() * _qOther.GetZ());
		m_fW = -(qThis.GetX() * _qOther.GetX()) - (qThis.GetY() * _qOther.GetY()) - (qThis.GetZ() * _qOther.GetZ()) + (qThis.GetW() * _qOther.GetW());

		return *this;
	}
}}