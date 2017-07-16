#ifndef _CORE_HELPER_COMPRESSION_HPP_
#define _CORE_HELPER_COMPRESSION_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/TypeDef.hpp>

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Helper
{
	void Compress		(uint8* _pBufferDecompressed, uint32 _uiSizeDecompressed, uint8*& _pBufferCompressed, uint32& _uiSizeCompressed);
	void Decompress		(uint8* _pBufferCompressed, uint32 _uiSizeCompressed, uint8*& _pBufferDecompressed, uint32& _uiSizeDecompressed);
}}
#endif