#ifndef _MATH_HPP_
#define _MATH_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Quaternion.hpp"
#include "Matrix33.hpp"
#include "Matrix44.hpp"

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Math
{
	const float32	K_PI			= 3.141592654f;
	const float32	K_2PI			= 6.283185307f;
	const float32	K_1DIVPI		= 0.318309886f;
	const float32	K_1DIV2PI		= 0.159154943f;
	const float32	K_PIDIV2		= 1.570796327f;
	const float32	K_PIDIV4		= 0.785398163f;

	template <typename T> 
	inline int32 Sign(T& _a) 
	{
		return (T(0) < _val) - (_val < T(0));
	}

	template <typename T> 
	inline T Min(const T& _a,const T& _b)	
	{ 
		return(_a < _b ? _a : _b); 
	}

	template <typename T> 
	inline T Max(const T& _a,const T& _b)	
	{ 
		return(_a > _b ? _a : _b); 
	}

	template<typename T> 
	inline T Clamp(const T& _a, const T& _min, const T& _max)
	{
		return Min<T>(_max, Max<T>(_min, _a));
	}

	template <typename T> 
	inline T Abs(const T& _a)	
	{ 
		return(_a < 0 ? -_a : _a); 
	}

	inline float32 ToRadian(float32 _fDegree)
	{
		return _fDegree * (K_PI / 180.0f); 
	}

	inline float32 ToDegree(float32 _fRadian)
	{
		return _fRadian * (180.0f / K_PI); 
	}

	float32 Cos(float32 _fValue);
	float32 Sin(float32 _fValue);
	float32 Log2(float32 _fValue);
}}

#endif