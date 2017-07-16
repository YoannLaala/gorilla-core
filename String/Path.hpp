#ifndef _PATH_HPP_
#define _PATH_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include "String.hpp"

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla
{
	class Path
	{
	public:

		Path();
		Path(const char* _szPath);
		Path(const String& _sText);
		~Path();

		Path& operator= (const char* _szPath);
		Path& operator= (const String& _sPath);

		inline const String& GetFull					() const { return m_sFull; }
		inline const String& GetDirectory				() const { return m_sDirectory; }
		inline const String& GetFileName				() const { return m_sFileName; }
		inline const String& GetExtension				() const { return m_sExtension; }
		inline const String& GetFileNameWithExtension	() const { return m_sFileNameWithExtension; }

		static void Format(String& _sPath);
		static bool IsAbsolute(const char* _szPath);

	private:
		String m_sFull;
		String m_sDirectory;
		String m_sFileName;
		String m_sExtension;
		String m_sFileNameWithExtension;
	};
}

#endif