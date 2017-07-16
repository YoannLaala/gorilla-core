/******************************************************************************
**	Includes
******************************************************************************/
#include "Path.hpp"
#include <Core/File/FileManager.hpp>
#include <Shlwapi.h>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla
{
	//!	@brief		Constructor
	//!	@date		2015-04-04
	Path::Path()
	{
	}

	//!	@brief		Constructor
	//!	@date		2015-04-04
	Path::Path(const char* _szPath)
	{
		*this = _szPath;
	}

	//!	@brief		Constructor
	//!	@date		2015-04-04
	Path::Path(const String& _sPath)
	{
		*this = _sPath.GetBuffer();
	}

	//!	@brief		Destructor
	//!	@date		2015-04-04
	Path::~Path()
	{
	}

	//!	@brief		Format
	//!	@date		2015-04-04
	void Path::Format(String& _sPath)
	{
		_sPath.Replace("/", REPERTORY_SEPARATOR);
		if(IsAbsolute(_sPath.GetBuffer()))
		{
			uint32 uiPositionEnd = _sPath.Find("\\..");
			while(uiPositionEnd != (uint32)-1)
			{
				uint32 uiPositionStart = uiPositionEnd-1;
				uiPositionEnd += 3; 
				while(_sPath[uiPositionStart] != '\\') --uiPositionStart;
				
				const uint32 uiLengthSubString = _sPath.GetLength() - uiPositionEnd;
				memcpy_s(&_sPath[uiPositionStart], uiLengthSubString, &_sPath[uiPositionEnd], uiLengthSubString);
				_sPath.Resize(uiPositionStart + uiLengthSubString);
				uiPositionEnd = _sPath.Find("\\..", uiPositionStart);
			}
		}
	}

	//!	@brief		IsAbsolute
	//!	@date		2015-04-04
	bool Path::IsAbsolute(const char* _szPath)
	{
		return !::PathIsRelative(_szPath);
	}

	//!	@brief		operator=
	//!	@date		2015-04-04
	Path& Path::operator= (const String& _sPath)
	{
		*this = _sPath.GetBuffer();

		return *this;
	}

	//!	@brief		operator=
	//!	@details	Create all part of the path from the _szPath
	//!	@date		2015-04-04
	Path& Path::operator= (const char* _szPath)
	{
		m_sFull						= _szPath;
		m_sDirectory				= "";
		m_sFileName					= "";
		m_sExtension				= "";
		m_sFileNameWithExtension	= "";

		// Search different part
		if(m_sFull.GetLength() > 0)
		{
			Format(m_sFull);

			uint32 uiLastIndex =  m_sFull.GetLength() - 1;
			for(uint32 uiChar = m_sFull.GetLength()-1; uiChar != (uint32)-1; uiChar--)
			{
				// Extension
				if(m_sFull[uiChar] == '.' && m_sExtension.GetLength() == 0)
				{
					m_sExtension.Append(&m_sFull[uiChar+1], uiLastIndex - uiChar);
				}

				// Directory
				if(m_sFull[uiChar] == *REPERTORY_SEPARATOR)
				{
					m_sDirectory.Append(&m_sFull[0], uiChar + 1);

					break;
				}
			}

			// FileName (with extension)
			m_sFileNameWithExtension.Append(&m_sFull[m_sDirectory.GetLength()], m_sFull.GetLength() - m_sDirectory.GetLength());

			// Short Filename (without extension)
			uint32 uiFileSize = m_sFileNameWithExtension.GetLength() - (m_sExtension.GetLength() ? m_sExtension.GetLength() + 1 : 0);
			m_sFileName.Append(&m_sFileNameWithExtension[0], uiFileSize);
		}

		return *this;
	}
}