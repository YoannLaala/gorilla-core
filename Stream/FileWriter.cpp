/******************************************************************************
**	Includes
******************************************************************************/
#include "FileWriter.hpp"
#include <Core/File/FileManager.hpp>
#include <Core/File/File.hpp>
#include <Core/Log/LogManager.hpp>
#include <string.h>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla
{
	//!	@brief		Constructor
	//!	@date		2015-09-16
	FileWriter::FileWriter()
		:	m_pFile(nullptr)
		,	m_uiBufferPosition(0)
	{
		// Nothing to do
	}

	//!	@brief		Constructor
	//!	@date		2015-09-16
	FileWriter::FileWriter(const char* _szFilePath)
		: FileWriter()
	{
		Open(_szFilePath);
	}

	//!	@brief		Destructor
	//!	@date		2015-09-16
	FileWriter::~FileWriter()
	{
		Close();
	}

	//!	@brief		Open
	//!	@date		2015-09-16
	bool FileWriter::Open(const char* _szFilePath)
	{
		ASSERT(m_pFile == nullptr, "[FileManager] A file has to be closed before opening it again");
		m_pFile = FileManager::OpenAFile(_szFilePath, FileManager::EAccess::Write);
		ASSERT(m_pFile != nullptr, "[FileManager] Failled to write file '%s'", _szFilePath);
		m_uiBufferPosition = 0;

		return CanWrite();
	}

	//!	@brief		Close
	//!	@date		2015-09-16
	void FileWriter::Close()
	{
		Flush();
		FileManager::CloseAFile(m_pFile);
	}

	//!	@brief		Flush
	//!	@date		2015-09-16
	void FileWriter::Flush()
	{
		if(m_uiBufferPosition > 0)
		{
			m_pFile->Write(m_szBuffer, m_uiBufferPosition);
			m_uiBufferPosition = 0;
		}
	}

	//!	@brief		Write
	//!	@date		2015-09-16
	void FileWriter::Write(const void* _pBuffer, uint32 _uiSize)
	{
		// We write a size bigger enough to exceed the buffer
		// In this case we copy first the remaining data from the buffer
		if(m_uiBufferPosition > 0)
		{
			uint32 uiBufferNextPosition = m_uiBufferPosition + _uiSize;
			if(uiBufferNextPosition > FILE_WRITER_BUFFER_SIZE) 
			{
				// In some case we can be at the limit of the buffer
				m_pFile->Write(m_szBuffer, m_uiBufferPosition);
				m_uiBufferPosition = 0;
			}
		}
		
		// We write directly from the file in case if the buffer is too small
		if(_uiSize >= FILE_WRITER_BUFFER_SIZE)
		{
			m_pFile->Write(_pBuffer, _uiSize);
		}
		// Normal write using the buffer
		else
		{
			// Update buffer if we consumed it
			if(m_uiBufferPosition >= FILE_WRITER_BUFFER_SIZE)
			{
				m_pFile->Write(m_szBuffer, FILE_WRITER_BUFFER_SIZE);
				m_uiBufferPosition = 0;
			}
			memcpy_s(&m_szBuffer[m_uiBufferPosition], FILE_WRITER_BUFFER_SIZE, _pBuffer, _uiSize);
			m_uiBufferPosition += _uiSize;
		}
	}
}