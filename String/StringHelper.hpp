#ifndef _STRING_HELPER_HPP_
#define _STRING_HELPER_HPP_
 
/******************************************************************************
**	Includes
******************************************************************************/
#include "String.hpp"
#include "String16.hpp"

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla
{
	class StringHelper
	{
	public:
		static void			ToString		(const wchar_t* _szText, uint32 _uiSize, String& _sValueOut);
		static void			ToString		(const wchar_t* _szText, String& _sValueOut);
		static void			ToString16		(const char* _szText, uint32 _uiSize, String16& _sValueOut);
		static void			ToString16		(const char* _szText, String16& _sValueOut);

		static uint32		ToUint32		(const char* _szText, uint32 _uiLength);
		static float32		ToFloat32		(const char* _szText, uint32 _uiLength);
		static float64		ToFloat64		(const char* _szText, uint32 _uiLength);

		static void			Split			(const char* _szValue, const char* _szSeparator, Vector<String>& _vResult);
	};
}

#endif