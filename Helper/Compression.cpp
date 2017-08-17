/******************************************************************************
**	Includes
******************************************************************************/
#include "Compression.hpp"
#include "lz4.h"

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Helper
{
	//!	@brief		Compress
	//!	@date		2015-04-08
	void Compress(uint8* _pBufferDecompressed, uint32 _uiSizeDecompressed, uint8*& _pBufferCompressed, uint32& _uiSizeCompressed)
	{
		// Write the decompressed size in the buffer to allocate exactly the right size during the decompression
		uint32 uiBufferSize = LZ4_COMPRESSBOUND(_uiSizeDecompressed);
		_pBufferCompressed = new uint8[uiBufferSize];
		*reinterpret_cast<uint32*>(_pBufferCompressed) = _uiSizeDecompressed;

		// Compress
		_uiSizeCompressed = LZ4_compress_default((char*)_pBufferDecompressed, (char*)_pBufferCompressed + sizeof(uint32), _uiSizeDecompressed, uiBufferSize) + sizeof(uint32);
	}

	//!	@brief		Decompress
	//!	@date		2015-04-08
	void Decompress(uint8* _pBufferCompressed, uint32 _uiSizeCompressed, uint8*& _pBufferDecompressed, uint32& _uiSizeDecompressed)
	{
		// Retrieve decompressed size
		_uiSizeDecompressed = *reinterpret_cast<uint32*>(_pBufferCompressed);
		
		// Decompress
		_pBufferDecompressed = new uint8[_uiSizeDecompressed];
		LZ4_decompress_safe((char*)_pBufferCompressed + sizeof(uint32), (char*)_pBufferDecompressed, _uiSizeCompressed - sizeof(uint32), _uiSizeDecompressed);
	}
}}