/******************************************************************************
**	Includes
******************************************************************************/
#include "Hash.hpp"

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla
{
	//!	@brief		Generate an identifier
	//!	@param		_pKey the pointer containinf octects to hash
	//!	@date		2015-04-04
	uint32 Hash::Generate(const char* _pKey)
	{
		// FNV-1a alternate algorithm
		// 32 bit offset_basis = 2166136261
		// 32 bit FNV_prime = 224 + 28 + 0x93 = 16777619
		uint32 uiHash = 2166136261;
		while(*_pKey)
		{
			uiHash = (*_pKey++ ^ uiHash) * 16777619;
		}

		return uiHash;
	}

	//!	@brief		Generate an identifier
	//!	@param		_pKey the pointer containinf octects to hash
	//!	@param		_uiLength the length of the data
	//!	@date		2015-04-04
	uint32 Hash::Generate(const void* _pKey, uint32 _uiLength)
	{
		// FNV-1a alternate algorithm
		// 32 bit offset_basis = 2166136261
		// 32 bit FNV_prime = 224 + 28 + 0x93 = 16777619
		uint32 uiHash = 2166136261;
		for(uint32 uiByte = 0; uiByte < _uiLength; ++uiByte)
		{
			uiHash = (((char*)_pKey)[uiByte] ^ uiHash) * 16777619;
		}

		return uiHash;
	}
}