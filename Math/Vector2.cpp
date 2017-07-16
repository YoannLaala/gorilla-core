/******************************************************************************
**	Includes
******************************************************************************/
#include "Vector2.hpp"
#include <math.h>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Math
{
	const Vector2 Vector2::Zero		= Vector2(0.0f,0.0f);
	const Vector2 Vector2::UnitX	= Vector2(1.0f,0.0f);
	const Vector2 Vector2::UnitY	= Vector2(0.0f,1.0f);
	const Vector2 Vector2::One		= Vector2(1.0f,1.0f);

	//!	@brief		Constructor
	//!	@date		2015-11-08
	Vector2::Vector2()
		: m_fX (0.0f)
		, m_fY (0.0f)
	{
		// Nothing to do
	}

	//!	@brief		Constructor
	//!	@date		2015-11-08
	Vector2::Vector2(float32 _fX, float32 _fY)
		: m_fX (_fX)
		, m_fY (_fY)
	{
		// Nothing to do
	}

	//!	@brief		Constructor
	//!	@date		2015-11-08
	Vector2::Vector2(const Vector2& _vOther)
		: m_fX (_vOther.GetX())
		, m_fY (_vOther.GetY())
	{
		// Nothing to do
	}

	//!	@brief		Length
	//!	@date		2015-11-08
	float32	Vector2::Length() const 
	{ 
		return sqrtf(LengthSquare()); 
	}
}}