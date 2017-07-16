#ifndef _TYPE_DEF_HPP_
#define _TYPE_DEF_HPP_

/******************************************************************************
**	Include
******************************************************************************/
#include <Core/PigeonHolePrinciple.hpp>

/******************************************************************************
**	Define
******************************************************************************/
#define UNUSED(...) PP_MACRO_CALL(UNUSED, 0, __VA_ARGS__)
#define UNUSED_1(_v1_) _v1_;
#define UNUSED_2(_v1_, _v2_) _v1_; _v2_;
#define UNUSED_3(_v1_, _v2_, _v3_) _v1_; _v2_; _v3_;
#define UNUSED_4(_v1_, _v2_, _v3_, _v4_) _v1_; _v2_; _v3_; _v4_;
#define UNUSED_5(_v1_, _v2_, _v3_, _v4_, _v5_) _v1_; _v2_; _v3_; _v5_;

/******************************************************************************
**	TypeDef
******************************************************************************/
namespace Gorilla
{
	typedef unsigned char			uint8;
	typedef unsigned short int		uint16;
	typedef unsigned int			uint32;
	typedef unsigned __int64		uint64;

	typedef char					int8;
	typedef short int				int16;
	typedef int						int32;
	typedef __int64					int64;

	typedef float					float32;
	typedef double					float64;
}

#endif