#ifndef _CORE_FILE_READER_HPP_
#define _CORE_FILE_READER_HPP_
 
/******************************************************************************
**	Includes
******************************************************************************/
#include "StreamReader.hpp"
#include <Core/File/File.hpp>

/******************************************************************************
**	Define
******************************************************************************/
#define FILE_READER_BUFFER_SIZE 65535

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla
{
	class FileReader : public StreamReader
	{
	public:
		FileReader();
		FileReader(const char* _szFilePath);
		~FileReader();

		bool Open(const char* _szFilePath);
		void Close();

		using StreamReader::Read;
		void Read(void* _pBuffer, uint32 _uiSize) override;

		inline bool				CanRead			() const { return m_pFile != nullptr; }

		inline virtual void		Seek			(uint32 _uiPosition) { m_uiBufferPosition = _uiPosition; }
		inline virtual uint32	GetPosition		() const { return m_pFile->GetPosition() - (FILE_READER_BUFFER_SIZE - m_uiBufferPosition); }
		inline virtual uint32	GetSize			() const { return m_pFile->GetSize(); }

	private:
		File*	m_pFile;
		char	m_szBuffer[FILE_READER_BUFFER_SIZE];
		uint32	m_uiBufferPosition;
	};
}

#endif