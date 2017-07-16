/******************************************************************************
**	Includes
******************************************************************************/
#include "FileReader.hpp"
#include "../File/FileManager.hpp"
#include "../Log/LogManager.hpp"

#include <Windows.h>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla
{
	//!	@brief		Constructor
	//!	@date		2015-09-16
	FileReader::FileReader()
		:	m_pFile(NULL)
		,	m_uiBufferPosition(0)
	{
		// Nothing to do
	}

	//!	@brief		Constructor
	//!	@date		2015-09-16
	FileReader::FileReader(const char* _szFilePath)
		:	FileReader()
	{
		Open(_szFilePath);
	}

	//!	@brief		Destructor
	//!	@date		2015-09-16
	FileReader::~FileReader()
	{
		Close();
	}

	//!	@brief		Open
	//!	@date		2015-09-16
	bool FileReader::Open(const char* _szFilePath)
	{
		ASSERT(m_pFile == nullptr, "[FileManager] A file has to be closed before opening it again");
		m_pFile = FileManager::OpenAFile(_szFilePath, FileManager::EAccess::Read);
		m_uiBufferPosition = 0;

		return CanRead();
	}

	//!	@brief		Close
	//!	@date		2015-09-16
	void FileReader::Close()
	{
		FileManager::CloseAFile(m_pFile);
	}

	//!	@brief		Read
	//!	@date		2015-09-16
	void FileReader::Read(void* _pBuffer, uint32 _uiSize)
	{
		// We read a size bigger enough to exceed the buffer
		// In this case we copy first the remaining data from the buffer
		if(m_uiBufferPosition > 0)
		{
			uint32 uiBufferNextPosition = m_uiBufferPosition + _uiSize;
			if(uiBufferNextPosition > FILE_READER_BUFFER_SIZE) 
			{
				// In some case we can be at the limit of the buffer
				uint32 uiBufferSize = FILE_READER_BUFFER_SIZE - m_uiBufferPosition;
				memcpy_s(_pBuffer, _uiSize, &m_szBuffer[m_uiBufferPosition], uiBufferSize);
			
				m_uiBufferPosition = 0;
				_uiSize -= uiBufferSize;
				_pBuffer = (uint8*)_pBuffer + uiBufferSize;
			}
		}
		
		// We read directly from the file in case if the buffer is too small
		if(_uiSize >= FILE_READER_BUFFER_SIZE)
		{
			m_pFile->Read(_pBuffer, _uiSize);
		}
		// Normal read using the buffer
		else
		{
			// Update buffer if we consumed it
			if(m_uiBufferPosition == 0)
			{
				m_pFile->Read(m_szBuffer, FILE_READER_BUFFER_SIZE);
			}
			memcpy_s(_pBuffer, _uiSize, &m_szBuffer[m_uiBufferPosition], _uiSize);
			m_uiBufferPosition += _uiSize;
		}
	}	
}