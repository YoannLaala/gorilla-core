/******************************************************************************
**	Includes
******************************************************************************/
#include "Vector3.hpp"
#include <Core/Log/LogManager.hpp>
#include <math.h>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Math
{
	const Vector3 Vector3::Zero		= Vector3(0.0f,0.0f,0.0f);
	const Vector3 Vector3::UnitX	= Vector3(1.0f,0.0f,0.0f);
	const Vector3 Vector3::UnitY	= Vector3(0.0f,1.0f,0.0f);
	const Vector3 Vector3::UnitZ	= Vector3(0.0f,0.0f,1.0f);
	const Vector3 Vector3::One		= Vector3(1.0f,1.0f,1.0f);

	//!	@brief		Constructor
	//!	@date		2015-11-08
	Vector3::Vector3()
		: m_fX (0.0f)
		, m_fY (0.0f)
		, m_fZ (0.0f)
	{
		// Nothing to do
	}

	//!	@brief		Constructor
	//!	@date		2015-11-08
	Vector3::Vector3(float32 _fX, float32 _fY, float32 _fZ)
		: m_fX (_fX)
		, m_fY (_fY)
		, m_fZ (_fZ)
	{
		// Nothing to do
	}

	//!	@brief		Constructor
	//!	@date		2015-11-08
	Vector3::Vector3(const Vector3& _vOther)
		: m_fX (_vOther.GetX())
		, m_fY (_vOther.GetY())
		, m_fZ (_vOther.GetZ())
	{
		// Nothing to do
	}

	//!	@brief		Length
	//!	@date		2015-11-08
	float32	Vector3::Length() const 
	{ 
		return sqrtf(LengthSquare()); 
	}

	//!	@brief		Normalize
	//!	@date		2015-11-08
	void Vector3::Normalize() 
	{ 
		float32 fLength = Length();
		ASSERT(fLength > 0.0f, "Failed to normalilze vector with 0 length"); 
		if(fLength)
		{
			float32 fLengthInverse = 1.0f / fLength;
			Mul(fLengthInverse, fLengthInverse, fLengthInverse); 
		}
	}
}}