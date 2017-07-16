/******************************************************************************
**	Includes
******************************************************************************/
#include "File.hpp"
#include <Windows.h>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla
{
	//!	@brief		Read
	//!	@details	Return the number of byte read
	//!	@date		2015-11-21
	uint32 File::Read(void* _pBuffer, uint32 _uiSize)
	{
		uint32 uiByteRead;
		::ReadFile((void*)this, _pBuffer, _uiSize, (LPDWORD)&uiByteRead, NULL);

		return uiByteRead;
	}

	//!	@brief		Write
	//!	@details	Return the number of byte written
	//!	@date		2015-11-21
	uint32 File::Write(const void* _pBuffer, uint32 _uiSize)
	{
		uint32 uiByteWrite;
		::WriteFile((void*)this, _pBuffer, _uiSize, (LPDWORD)&uiByteWrite, NULL);

		return uiByteWrite;
	}

	//!	@brief		Seek
	//!	@date		2015-11-21
	void File::Seek(uint32 _uiPosition)
	{
		::SetFilePointer((void*)this, _uiPosition, 0, FILE_BEGIN);
	}

	//!	@brief		GetSize
	//!	@date		2015-11-21
	uint32 File::GetSize() const 
	{ 
		return ::GetFileSize((void*)this, NULL); 
	}

	//!	@brief		GetPosition
	//!	@date		2015-11-23
	uint32 File::GetPosition() const 
	{ 
		return ::SetFilePointer((void*)this, 0, 0, FILE_CURRENT); 
	}

	//!	@brief		GetAttribute
	//!	@date		2015-11-21
	uint32 File::GetAttribute() const
	{
		LPBY_HANDLE_FILE_INFORMATION pFileInformation = NULL;
		::GetFileInformationByHandle((void*)this, pFileInformation);

		return pFileInformation->dwFileAttributes;
	}

	//!	@brief		GetLastWrite
	//!	@date		2015-11-21
	uint64 File::GetLastWrite() const
	{
		uint64 uiCreationTime, uiLastAccessTime, uiLastWriteTime;
		::GetFileTime((void*)this, (FILETIME*)&uiCreationTime, (FILETIME*)&uiLastAccessTime, (FILETIME*)&uiLastWriteTime);

		return uiLastWriteTime;
	}
}