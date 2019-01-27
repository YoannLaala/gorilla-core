/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/String/StringHelper.hpp>
#include <Core/Log/LogManager.hpp>
#include <string.h>
#include <wchar.h>

/******************************************************************************
**	Define
******************************************************************************/
#define UNICODE_MASK_VALUE				0x3f		// 0011 1111

#define UNICODE_HEADER_MASK_1_BYTE		0x80		// 1000 0000
#define UNICODE_HEADER_MASK_2_BYTES		0xc0		// 1100 0000
#define UNICODE_HEADER_MASK_3_BYTES		0xe0		// 1110 0000
#define UNICODE_HEADER_MASK_4_BYTES		0xf0		// 1111 0000

#define UNICODE_MAX_VALUE_1_BYTE		0x80		// 128
#define UNICODE_MAX_VALUE_2_BYTES		0x800		// 2048
#define UNICODE_MAX_VALUE_3_BYTES		0x10000		// 65536

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla
{
	//!	@brief		IsFloat
	bool StringHelper::IsFloat(const char* _szText, uint32 _uiLength)
	{
		struct EFlag
		{
			enum Type: uint8
			{
				SEPARATOR	= 1 << 0,
				EXPONENT	= 1 << 1,
			};
		};

		uint8 eFlag = 0;
		uint32 uiIndex = 0;
		while(uiIndex < _uiLength)
		{
			char cCaharacter = _szText[uiIndex];
			switch(cCaharacter)
			{
				case '+':
				case '-':
				{
					// negative sign are possible with exponent
					if((eFlag & EFlag::EXPONENT) != 0)
					{
						// previous character must be exponent
						if(_szText[uiIndex-1] != 'e' && _szText[uiIndex-1] != 'E')
							return false;
					}
					else
					{
						if(uiIndex != 0)
							return false;
					}
					
					break;
				}

				case ',':
				case '.':
				{
					if((eFlag & EFlag::SEPARATOR) != 0)
						return false;
					eFlag |= EFlag::SEPARATOR;
					break;
				}

				case 'e':
				case 'E':
				{
					if(uiIndex == 0)
						return false;

					if((eFlag & EFlag::EXPONENT) != 0)
						return false;
					eFlag |= EFlag::EXPONENT;
					break;
				}

				case 'f':
				case 'd':
				{
					if(uiIndex != _uiLength-1)
						return false;
					break;
				}

				default:
				{
					if(cCaharacter < '0' || cCaharacter > '9')
						return false;
					break;
				}
			}
			++uiIndex;
		}
		
		return true;
	}

	//!	@brief		ToString
	//!	@date		2015-04-04
	void StringHelper::ToString(const wchar_t* _szText, String& _sValueOut)
	{	
		ToString(_szText, (uint32)wcslen(_szText), _sValueOut);
	}

	//!	@brief		ToString
	//!	@date		2015-04-04
	void StringHelper::ToString(const wchar_t* _szText, uint32 _uiSize, String& _sValueOut)
	{	
		_sValueOut.Set("");

		// Iterate all character and encore to UTF8
		for(uint32 uiCharacter  = 0; uiCharacter < _uiSize; ++uiCharacter)
		{
			wchar_t cCharacter16 = _szText[uiCharacter];

			// No need to encode because it will be on 1byte
			if (cCharacter16 < UNICODE_MAX_VALUE_1_BYTE) 
			{
				_sValueOut.Append((char)cCharacter16);
			} 
			// Encode on 2bytes 1100 0000
			else if (cCharacter16 < UNICODE_MAX_VALUE_2_BYTES) 
			{
				_sValueOut.Append((char)(UNICODE_HEADER_MASK_2_BYTES | (cCharacter16 >> 6)));
				_sValueOut.Append((char)(UNICODE_HEADER_MASK_1_BYTE | (cCharacter16 & UNICODE_MASK_VALUE)));
			} 
			// Encode on 3bytes 1110 0000
			else if (cCharacter16 < UNICODE_MAX_VALUE_3_BYTES) 
			{
				_sValueOut.Append((char)(UNICODE_HEADER_MASK_3_BYTES | (cCharacter16 >> 12)));
				_sValueOut.Append((char)(UNICODE_HEADER_MASK_1_BYTE | ((cCharacter16 >> 6) & UNICODE_MASK_VALUE)));
				_sValueOut.Append((char)(UNICODE_HEADER_MASK_1_BYTE | (cCharacter16 & UNICODE_MASK_VALUE)));
			}
			// Encode on 4bytes 1110 0000
			else
			{
				ASSERT(false, "32bits character not supported");
			}
		}
	}

	//!	@brief		ToString16
	//!	@date		2015-04-04
	void StringHelper::ToString16(const char* _szText, String16& _sValueOut)
	{
		ToString16(_szText, (uint32)strlen(_szText), _sValueOut);
	}

	//!	@brief		ToString16
	//!	@date		2015-04-04
	void StringHelper::ToString16(const char* _szText, uint32 _uiSize, String16& _sValueOut)
	{
		_uiSize = _uiSize == (uint32)-1 ? (uint32)strlen(_szText) : _uiSize;
		_sValueOut.Resize(_uiSize);

		uint32 uiRealSize = 0;
		for(uint32 uiCharacter  = 0; uiCharacter < _uiSize; ++uiCharacter)
		{
			char cCharacter = _szText[uiCharacter];
			switch((cCharacter & UNICODE_HEADER_MASK_4_BYTES))
			{
				// Encoded on 2bytes 1100 0000
				case UNICODE_HEADER_MASK_2_BYTES:
				{
					wchar_t cCharacter16 = cCharacter & 0x1f;
					cCharacter16 = cCharacter16 << 6 | (_szText[++uiCharacter] & UNICODE_MASK_VALUE);
					_sValueOut[uiRealSize] = cCharacter16;
					break;
				}

				// Encoded on 3bytes 1110 0000
				case UNICODE_HEADER_MASK_3_BYTES:
				{
					wchar_t cCharacter16 = cCharacter & 0x0f;
					cCharacter16 = cCharacter16 << 6 | (_szText[++uiCharacter] & UNICODE_MASK_VALUE);
					cCharacter16 = cCharacter16 << 6 | (_szText[++uiCharacter] & UNICODE_MASK_VALUE);
					_sValueOut[uiRealSize] = cCharacter16;
					
					break;
				}

				// Encoded on 4bytes 1111 0000
				case UNICODE_HEADER_MASK_4_BYTES:
				{
					ASSERT(false, "32bits character not supported");
					break;
				}
				
				// ANSI character
				default:
				{
					_sValueOut[uiRealSize] = cCharacter;
					break;
				};
			}

			++uiRealSize;
		}

		// Finalize with real size
		_sValueOut.Resize(uiRealSize);
	}

	//!	@brief		ToUint32
	//!	@date		2015-04-04
	uint32 StringHelper::ToUint32(const char* _szText, uint32 uiLength)
	{
		uint32 uiValue = 0, uiMultiplier = 1;
		for(int32 iCharacter = uiLength-1; iCharacter >= 0; --iCharacter)
		{
			uiValue += (_szText[iCharacter] - '0') * (uiMultiplier);
			uiMultiplier *= 10;
		}
		
		return uiValue;
	}

	//!	@brief		ToFloat32
	//!	@date		2015-04-04
	float32 StringHelper::ToFloat32(const char* _szText, uint32 _uiLength)
	{
		return static_cast<float32>(ToFloat64(_szText, _uiLength));
	}

	//!	@brief		ToFloat64
	//!	@date		2015-04-04
	float64 StringHelper::ToFloat64(const char* _szText, uint32 _uiLength)
	{
		float64 fFactor = 1.0f;
		uint32 uiIndex = 0, uiSeparatorIndex = (uint32)-1, uiNumber = 0;
		uint32 uiMultiplier = 0, uiDivider = 0;
		int64 iValue = 0;
		while(uiIndex < _uiLength)
		{
			char cCaharacter = _szText[uiIndex];
			switch(cCaharacter)
			{
				// -0.5
				case '-':
				{
					fFactor = -1.0f;
					break;
				}

				// 0,5
				case ',':
				case '.':
				{
					uiSeparatorIndex = uiNumber;
					break;
				}

				// 0.5e1
				case 'e':
				case 'E':
				{
					if(_szText[++uiIndex] == '-')
						uiDivider = (_szText[++uiIndex] - '0');
					else if(_szText[uiIndex] == '+')
						uiMultiplier = (_szText[++uiIndex] - '0');
					else
						uiMultiplier = (_szText[uiIndex] - '0');
					break;
				}

				// 0.5f 0.5d
				case 'f':
				case 'd':
				{
					break;
				}

				default:
				{
					iValue = iValue * 10 + (cCaharacter - '0');
					++uiNumber;
					break;
				}
			}
			++uiIndex;
		}
		
		// apply separator to divider
		if(uiSeparatorIndex != (uint32)-1)
			uiDivider += (uiNumber - uiSeparatorIndex);

		// compute final factor
		if(uiDivider > uiMultiplier)
		{
			for(uint32 i = 0; i < uiDivider-uiMultiplier; ++i)
				fFactor *= 10;
			fFactor = 1.0f / fFactor;
		}
		else if (uiDivider < uiMultiplier)
		{
			for(uint32 i = 0; i < uiMultiplier - uiDivider; ++i)
				fFactor *= 10;
		}
		
		float64 result = iValue * fFactor;

		return result;
	}

	//!	@brief		Split
	//!	@date		2015-04-04
	void StringHelper::Split(const char* _szValue, const char* _szSeparator, Vector<String>& _vResult)
	{
		const uint32 uiSeparatorSize = static_cast<uint32>(strlen(_szSeparator));
		const char* pStart = _szValue;
		const char* pEnd = strstr(pStart, _szSeparator);
		while (pEnd)
		{
			_vResult.Add().Set(pStart, static_cast<uint32>(pEnd - pStart));
			pStart = pEnd + uiSeparatorSize;
			pEnd = strstr(pStart, _szSeparator);
		}
		_vResult.Add().Set(pStart);
	}
}