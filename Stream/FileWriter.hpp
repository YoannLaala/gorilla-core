#ifndef _CORE_FILE_WRITER_HPP_
#define _CORE_FILE_WRITER_HPP_
 
/******************************************************************************
**	Includes
******************************************************************************/
#include "StreamWriter.hpp"
#include <Core/File/File.hpp>

/******************************************************************************
**	Define
******************************************************************************/
#define FILE_WRITER_BUFFER_SIZE 65535

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla
{
	class FileWriter : public StreamWriter
	{
	public:
		FileWriter();
		FileWriter(const char* _szFilePath);
		~FileWriter();

		bool Open(const char* _szFilePath);
		void Close();

		inline virtual bool		CanWrite		() const override { return m_pFile != nullptr; } 

		inline virtual void		Seek			(uint32 _uiPosition) override { Flush(); m_pFile->Seek(_uiPosition); }
		inline virtual uint32	GetPosition		() const override { return m_pFile->GetPosition() + m_uiBufferPosition; }
		inline virtual uint32	GetSize			() const override { return m_pFile->GetSize(); }

		using StreamWriter::Write;
		void Write(const void* _pBuffer, uint32 _uiSize) override;		

	private:
		void Flush();

	private:
		File*	m_pFile;
		char	m_szBuffer[FILE_WRITER_BUFFER_SIZE];
		uint32	m_uiBufferPosition;
	};
}

#endif