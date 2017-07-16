#ifndef _CORE_HPP_
#define _CORE_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Memory/MemoryManager.hpp>
#include <Core/Math/Math.hpp>
#include <Core/Container/Vector.hpp>
#include <Core/Container/Pair.hpp>
#include <Core/Container/Queue.hpp>
#include <Core/Container/HashMap.hpp>
#include <Core/Log/LogManager.hpp>

#define GET_ENUM_VALUE(_type_, _string_, _value_out_) for(uint32 uiElement = 0; uiElement < _type_::Count; ++uiElement) { if(strcmp(_string_, _type_::Name[uiElement]) == 0) { _value_out_ = (_type_::Type)uiElement; break; } }

// File Error
/*// Common Function
namespace Common
{
	inline EVisualStudio::Type GetVisualStudioVersion()
	{
		EVisualStudio::Type eVisual;
		switch(_MSC_VER)
		{
			//VS2005
			case 1400:	
			{
				eVisual = EVisualStudio::VS2005;
				break;
			}

			//VS2008
			case 1500:	
			{
				eVisual = EVisualStudio::VS2008;
				break;
			}

			//VS2010
			case 1600:	
			{
				eVisual = EVisualStudio::VS2010;
				break;
			}

			//VS2012
			case 1700:	
			{
				eVisual = EVisualStudio::VS2012;
				break;
			}

			//VS2013
			case 1800:	
			{
				eVisual = EVisualStudio::VS2013;
				break;
			}

			// Unsupported Compiler
			default:
			{
				LOG_WARNING("Unsupported Compiler!"); 
				break;
			}
		}

		return eVisual;
	}

	inline void GetVisualStudioPath(char* _szPath, uint32 _uiSize)
	{
		// Get information about the Key registry
		HKEY hKey;
		LONG lKeyValue = RegOpenKeyExA(
					  HKEY_LOCAL_MACHINE,			//__in        HKEY hKey,
					  VISUAL_STUDIO_REGISTRY,		//__in_opt    LPCTSTR lpSubKey,
					  0,							//__reserved  DWORD ulOptions,
					  KEY_READ | KEY_WOW64_32KEY,	//__in        REGSAM samDesired,
					  &hKey							//__out       PHKEY phkResult
					);

		// Try for each Visual STudio version known
		for(uint32 uiVersion = EVisualStudio::Number-1 ; uiVersion >= 0; --uiVersion)
		{
			LONG lQueryValue = RegQueryValueExA(
							hKey,							//__in         HKEY hKey,
							EVisualStudio::Name[uiVersion],	//__in_opt     LPCTSTR lpValueName,
							NULL,							//__reserved   LPDWORD lpReserved,
							NULL ,							//__out_opt    LPDWORD lpType,
							(LPBYTE)_szPath,					//__out_opt    LPBYTE lpData,
							(DWORD*)&_uiSize							//__inout_opt  LPDWORD lpcbData
						);

			// Path found
			if(lQueryValue == ERROR_SUCCESS)
			{
				break;
			}
		}

		RegCloseKey(hKey);
	}
}*/

#endif