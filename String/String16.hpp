#ifndef _STRING16_HPP_
#define _STRING16_HPP_
 
/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/TypeDef.hpp>
#include <Core/Container/Vector.hpp>

/******************************************************************************
**	Defines
******************************************************************************/
#define STRING16_STACK_CAPACITY	64

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla
{
	class String16
	{
	public:
		String16();
		String16(const wchar_t* _szText);
		String16(const String16& _sText);
		String16(uint32 _uiSize);
		~String16();

		inline const wchar_t*	GetBuffer	() const { return &m_vCharacter[0]; }

		inline uint32			GetLength		() const { return m_vCharacter.GetSize(); }
		inline bool				IsEmpty			() const { return m_vCharacter.IsEmpty(); } 
								
		inline bool				Equals			(const String16& _sText) const { return m_vCharacter.GetSize() == _sText.GetLength() && Equals(_sText.GetBuffer()); }
		bool					Equals			(const wchar_t* _szText) const;
		String16&				ToLower			();
		String16&				ToUpper			();
		uint32					Find			(const wchar_t* _szPattern, uint32 _uiStartIndex = 0, uint32 _uiCount = 1) const;
		uint32					FindLast		(const wchar_t* _szPattern, uint32 _uiStartIndex = 0) const;
								
		String16&				Insert			(const wchar_t* _szText,  uint32 _uiIndex, uint32 _uiLength = 0);
		String16&				Insert			(uint32 _uiValue, uint8 _uiBase, bool _bIsNegative, uint32 _uiIndex, uint32 _uiLength = 0);
		String16&				Insert			(int _iValue, uint32 _uiIndex, uint32 _uiLength = 0);
		String16&				Insert			(uint32 _uiValue, uint32 _uiIndex, uint32 _uiLength = 0);
								
		String16&				Set				(const wchar_t* _szText, uint32 _uiLength = 0);
		String16&				Set				(const String16& _sText);
		String16&				Set				(wchar_t _cChar);
		String16&				Set				(int _iValue, uint32 _uiLength = 0);
		String16&				Set				(uint32 _uiValue, uint32 _uiLength = 0);
								
		String16&				Append			(const wchar_t* _szText, uint32 _uiLength = 0);
		String16&				Append			(const String16& _sText);
		String16&				Append			(wchar_t _cChar);
		String16&				Append			(int _iValue, uint32 _uiLength = 0);
		String16&				Append			(uint32 _uiValue, uint32 _uiLength = 0);
								
		String16&				Prepend			(const wchar_t* _szText, uint32 _uiLength = 0);
		String16&				Prepend			(const String16& _sText);
		String16&				Prepend			(wchar_t _cChar);
		String16&				Prepend			(int _iValue, uint32 _uiLength = 0);
		String16&				Prepend			(uint32 _uiValue, uint32 _uiLength = 0);
								
		String16&				Replace			(const wchar_t* _szOldSubText, const wchar_t* _szNewSubText);
		String16&				Remove			(const wchar_t* _szSubText);
		void					Resize			(uint32 uiSize);
								
		String16&				operator=		(const wchar_t* _szText);
		String16&				operator=		(const String16& _sText);
		String16&				operator=		(uint32 _iValue);
								
		String16&				operator+=		(const wchar_t* _szText);
		String16				operator+		(const wchar_t* _szText) const;
		String16&				operator+=		(const String16& _sText);
		String16				operator+		(const String16& _sText) const;
		String16&				operator+=		(wchar_t cValue);
		String16				operator+		(wchar_t cValue) const;
		String16&				operator+=		(int _iValue);
		String16				operator+		(int _iValue) const;
		String16&				operator+=		(uint32 _uiValue);
		String16				operator+		(uint32 _uiValue) const;
								
		bool					operator==		(const String16& _sText) const;
		bool					operator==		(const wchar_t* _szText) const;
		bool					operator!=		(const String16& _sText) const;
		bool					operator!=		(const wchar_t* _szText) const;
								
		inline wchar_t&			operator[]		(uint32 _uiIndex) { return m_vCharacter[_uiIndex]; }
		inline wchar_t			operator[]		(uint32 _uiIndex) const { return m_vCharacter[_uiIndex]; }
								
	private:					
		void					Expand			(uint32 _uiCharacterCount, uint32& uiOldSizeOut, uint32& uiNewSizeOut);

	private:
		wchar_t			m_aStack[STRING16_STACK_CAPACITY];
		Vector<wchar_t> m_vCharacter;
	};
}

#endif