/******************************************************************************
**	Includes
******************************************************************************/
#include "Math.hpp"
#include <math.h>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Math
{
	float32 Cos(float32 _fValue)
	{
		return cosf(_fValue);
	}

	float32 Sin(float32 _fValue)
	{
		return sinf(_fValue);
	}

	float32 Log2(float32 _fValue)
	{
		return log2f(_fValue);
	}
}}