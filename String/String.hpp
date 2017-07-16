#ifndef _STRING_HPP_
#define _STRING_HPP_
 
/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/TypeDef.hpp>
#include <Core/Container/Vector.hpp>

/******************************************************************************
**	Defines
******************************************************************************/
#define STRING_STACK_CAPACITY	64

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla
{
	class String
	{
	public:
		String();
		String(const char* _szText);
		String(const char* _szText, uint32 _uiSize);
		String(const String& _sText);
		~String();

		inline const char*	GetBuffer		() const { return &m_vCharacter[0]; }

		inline uint32		GetLength		() const { return m_vCharacter.GetSize(); }
		inline bool			IsEmpty			() const { return m_vCharacter.IsEmpty(); } 

		inline bool			Equals			(const String& _sText) const { return m_vCharacter.GetSize() == _sText.GetLength() && Equals(_sText.GetBuffer()); }
		bool				Equals			(const char* _szText) const;
		String&				ToLower			();
		String&				ToUpper			();
		uint32				Find			(const char* _szPattern, uint32 _uiStartIndex = 0, uint32 _uiCount = 1) const;
		uint32				FindLast		(const char* _szPattern, uint32 _uiStartIndex = 0) const;
		inline bool			Contains		(const char * _szPattern, uint32 _uiStartIndex = 0) const { return Find(_szPattern, _uiStartIndex) != (uint32)-1; }

		String&				Insert			(const char* _szText,  uint32 _uiIndex, uint32 _uiLength = 0);
		String&				Insert			(uint32 _uiValue, uint8 _uiBase, bool _bIsNegative, uint32 _uiIndex, uint32 _uiLength = 0);
		String&				Insert			(int _iValue, uint32 _uiIndex, uint32 _uiLength = 0);
		String&				Insert			(uint32 _uiValue, uint32 _uiIndex, uint32 _uiLength = 0);

		String&				Set				(const char* _szText, uint32 _uiLength = 0);
		String&				Set				(const String& _sText);
		String&				Set				(char _cChar);
		String&				Set				(int _iValue, uint32 _uiLength = 0);
		String&				Set				(uint32 _uiValue, uint32 _uiLength = 0);

		String&				Append			(const char* _szText, uint32 _uiLength = 0);
		String&				Append			(const String& _sText);
		String&				Append			(char _cChar);
		String&				Append			(int _iValue, uint32 _uiLength = 0);
		String&				Append			(uint32 _uiValue, uint32 _uiLength = 0);

		String&				Prepend			(const char* _szText, uint32 _uiLength = 0);
		String&				Prepend			(const String& _sText);
		String&				Prepend			(char _cChar);
		String&				Prepend			(int _iValue, uint32 _uiLength = 0);
		String&				Prepend			(uint32 _uiValue, uint32 _uiLength = 0);

		String&				Replace			(const char* _szOldSubText, const char* _szNewSubText);
		String&				Remove			(const char* _szSubText);
		void				Resize			(uint32 _uiSize);

		String&				operator=		(const char* _szText);
		String&				operator=		(const String& _sText);

		String&				operator+=		(const char* _szText);
		String				operator+		(const char* _szText) const;
		String&				operator+=		(const String& _sText);
		String				operator+		(const String& _sText) const;
		String&				operator+=		(char cValue);
		String				operator+		(char cValue) const;
		String&				operator+=		(int _iValue);
		String				operator+		(int _iValue) const;
		String&				operator+=		(uint32 _uiValue);
		String				operator+		(uint32 _uiValue) const;

		bool				operator==		(const String& _sText) const;
		bool				operator==		(const char* _szText) const;
		bool				operator!=		(const String& _sText) const;
		bool				operator!=		(const char* _szText) const;

		inline char&		operator[]		(uint32 _uiIndex) { return m_vCharacter[_uiIndex]; }
		inline const char&	operator[]		(uint32 _uiIndex) const { return m_vCharacter[_uiIndex]; }		

	private:
		void				Expand			(uint32 _uiCharacterCount, uint32& uiOldSizeOut, uint32& uiNewSizeOut);

	private:
		Vector<char>	m_vCharacter;
		char			m_aStack[STRING_STACK_CAPACITY];
	};
}

#endif