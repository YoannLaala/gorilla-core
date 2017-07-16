/******************************************************************************
**	Includes
******************************************************************************/
#include "Vector4.hpp"
#include <math.h>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Math
{
	const Vector4 Vector4::Zero		= Vector4(0.0f,0.0f,0.0f,0.0f);
	const Vector4 Vector4::UnitX	= Vector4(1.0f,0.0f,0.0f,0.0f);
	const Vector4 Vector4::UnitY	= Vector4(0.0f,1.0f,0.0f,0.0f);
	const Vector4 Vector4::UnitZ	= Vector4(0.0f,0.0f,1.0f,0.0f);
	const Vector4 Vector4::UnitW	= Vector4(0.0f,0.0f,0.0f,1.0f);
	const Vector4 Vector4::One		= Vector4(1.0f,1.0f,1.0f,1.0f);

	//!	@brief		Constructor
	//!	@date		2015-11-08
	Vector4::Vector4()
		: m_fX (0.0f)
		, m_fY (0.0f)
		, m_fZ (0.0f)
		, m_fW (0.0f)
	{
		// Nothing to do
	}

	//!	@brief		Constructor
	//!	@date		2015-11-08
	Vector4::Vector4(float32 _fX, float32 _fY, float32 _fZ, float32 _fW) 
		: m_fX (_fX)
		, m_fY (_fY)
		, m_fZ (_fZ)
		, m_fW (_fW)
	{
		// Nothing to do
	}

	//!	@brief		Constructor
	//!	@date		2015-11-08
	Vector4::Vector4(const Vector4& _vOther)
		: m_fX (_vOther.GetX())
		, m_fY (_vOther.GetY())
		, m_fZ (_vOther.GetZ())
		, m_fW (_vOther.GetW())
	{
		// Nothing to do
	}

	//!	@brief		Length
	//!	@date		2015-11-08
	float32	Vector4::Length() const 
	{ 
		return sqrtf(LengthSquare()); 
	}
}}