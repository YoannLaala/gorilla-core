/******************************************************************************
**	Includes
******************************************************************************/
#include "String.hpp"
#include <string>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla
{
	//!	@brief		Constructor
	//!	@date		2015-04-04
	String::String()
	{	
		m_aStack[0] = 0;
		m_vCharacter.SetBuffer(m_aStack, 0, STRING_STACK_CAPACITY, false);
	}

	//!	@brief		Constructor
	//!	@date		2015-04-04
	String::String(const char* _szText)
		: String()
	{
		Set(_szText);
	}

	//!	@brief		Constructor
	//!	@date		2015-04-04
	String::String(const char* _szText, uint32 _uiSize)
		: String()
	{
		Set(_szText, _uiSize);
	}

	//!	@brief		Constructor
	//!	@date		2015-04-04
	String::String(const String& _sText)
		: String()
	{
		Set(_sText.GetBuffer(), _sText.GetLength());
	}

	//!	@brief		Destructor
	//!	@date		2015-06-21
	String::~String()
	{
		// Nothing to do
	}

	//!	@brief		ToLower
	//!	@date		2015-04-04
	bool String::Equals(const char* _szText) const 
	{ 
		return _szText ? strcmp(GetBuffer(), _szText) == 0 : IsEmpty(); 
	}

	//!	@brief		ToLower
	//!	@date		2015-04-04
	String& String::ToLower()
	{
		const uint32 uiLength = m_vCharacter.GetSize();
		for(uint32 uiChar = 0; uiChar < uiLength; ++uiChar)
		{
			m_vCharacter[uiChar] = static_cast<char>(tolower(m_vCharacter[uiChar]));
		}

		return *this;
	}

	//!	@brief		ToUpper
	//!	@date		2015-04-04
	String& String::ToUpper()
	{
		const uint32 uiLength = m_vCharacter.GetSize();
		for(uint32 uiChar = 0; uiChar < uiLength; ++uiChar)
		{
			m_vCharacter[uiChar] = static_cast<char>(toupper(m_vCharacter[uiChar]));
		}

		return *this;
	}

	//!	@brief		Find
	//!	@details	Find a pattern a number of times
	//! @param		_szPattern the pattern wanted
	//!	@param		_uiStartIndex the index to start the search
	//!	@param		_uiCount the number of pattern to find
	//!	@date		2015-04-04
	uint32 String::Find(const char* _szPattern, uint32 _uiStartIndex /*= 0*/, uint32 _uiCount /*= 1*/) const
	{
		const char* pStart = &m_vCharacter[_uiStartIndex];
		const char* pCursor = pStart;
		for(uint32 uiPattern = 0; uiPattern < _uiCount; ++uiPattern)
		{
			pCursor = strstr(pCursor, _szPattern);
		}

		return pCursor ? static_cast<uint32>(pCursor - &m_vCharacter[0]) : (uint32)-1;
	}

	//!	@brief		FindLast
	//!	@details	Find the last pattern
	//! @param		_szPattern the pattern wanted
	//!	@param		_uiStartIndex the index to start the search
	//!	@date		2015-04-04
	uint32 String::FindLast(const char* _szPattern, uint32 _uiStartIndex /*= 0*/) const
	{
		uint32 uiIndex = (uint32)-1;

		const char* pStart = &m_vCharacter[_uiStartIndex];
		const char* pCursor = strstr(pStart, _szPattern);
		while(pCursor)
		{
			uiIndex = static_cast<uint32>(pCursor - pStart);
			pCursor = strstr(++pCursor, _szPattern);
		}

		return uiIndex;
	}

	//!	@brief		Insert
	//!	@date		2015-04-04
	String& String::Insert(const char* _szText, uint32 _uiIndex, uint32 _uiLength /*= 0*/)
	{
		if(_szText)
		{
			// Get the real size for the new subtext
			if(_uiLength == 0)
			{
				_uiLength = static_cast<uint32>(strlen(_szText));
			}
		
			uint32 uiOldSize, uiNewSize;
			Expand(_uiLength, uiOldSize, uiNewSize);
			char* pSource = &m_vCharacter[_uiIndex];

			// Move the right block if necessary
			uint32 uiMoveSize = uiOldSize - _uiIndex;
			if(uiMoveSize)
			{
				memcpy_s(pSource + _uiLength, uiNewSize, pSource, uiMoveSize);
			}

			// Copy
			memcpy_s(pSource, uiNewSize, _szText, _uiLength);
		}

		return *this;
	}

	//!	@brief		Insert
	//!	@date		2015-04-04
	String& String::Insert(uint32 _uiValue, uint8 _uiBase, bool _bIsNegative, uint32 _uiIndex, uint32 _uiLength /*= 0*/)
	{	
		// Search for interger length only if needed with numbers only i
		if(_uiLength == 0)
		{
			_uiLength = _bIsNegative ? 1 : 0;
			if(_uiValue > 0)
			{
				for(uint32 uiValueTmp = _uiValue; uiValueTmp != 0; uiValueTmp/=_uiBase)
				{
					++_uiLength;
				}	
			}
			// Handle 0 value
			else
			{
				++_uiLength;
			}			
		}

		uint32 uiOldSize, uiNewSize;
		Expand(_uiLength, uiOldSize, uiNewSize);
		char* pSource = &m_vCharacter[0];

		// Move the right block if necessary
		uint32 uiMoveSize = uiOldSize - _uiIndex;
		if(uiMoveSize)
		{
			memcpy_s(pSource + _uiLength, uiNewSize, pSource, uiMoveSize);
		}
	
		// Fill with numbers
		_uiIndex += --_uiLength;
		do
		{
			pSource[_uiIndex--] = "0123456789abcdef"[_uiValue % _uiBase];
			_uiValue /= _uiBase;
		}
		while(_uiValue);

		// Handle negative sign	
		if(_bIsNegative)
		{
			pSource[_uiIndex++] = '-';
		}

		return *this;
	}

	//!	@brief		Append
	//!	@date		2015-04-04
	String& String::Insert(int _iValue, uint32 _uiIndex, uint32 _uiLength /*= 0*/)
	{
		bool bIsNegative = false;
		if(_iValue < 0)
		{
			_iValue = -_iValue;
			bIsNegative = true;
		}

		return Insert((uint32)_iValue, 10, bIsNegative, _uiIndex, _uiLength);
	}

	//!	@brief		Append
	//!	@date		2015-04-04
	String& String::Insert(uint32 _uiValue, uint32 _uiIndex, uint32 _uiLength /*= 0*/)
	{
		return Insert(_uiValue, 10, false, _uiIndex, _uiLength);
	}

	//!	@brief		Set
	//!	@date		2015-04-04
	String& String::Set(const char* _szText, uint32 _uiLength /*= 0*/)
	{
		m_vCharacter.Clear();
		return Insert(_szText, 0, _uiLength);
	}

	//!	@brief		Set
	//!	@date		2015-04-04
	String& String::Set(const String& _sText)
	{
		m_vCharacter.Clear();
		return Insert(_sText.GetBuffer(), 0, _sText.GetLength());
	}

	//!	@brief		Set
	//!	@date		2015-05-10
	String& String::Set(char _cChar)
	{
		m_vCharacter.Clear();
		return Insert(&_cChar, 0, 1);
	}

	//!	@brief		Set
	//!	@date		2015-04-04
	String& String::Set(int _iValue, uint32 _uiLength /*= 0*/)
	{
		m_vCharacter.Clear();
		return Insert(_iValue, 0, _uiLength);
	}

	//!	@brief		Set
	//!	@date		2015-04-04
	String& String::Set(uint32 _uiValue, uint32 _uiLength /*= 0*/)
	{
		m_vCharacter.Clear();
		return Insert(_uiValue, 0, _uiLength);
	}

	//!	@brief		Append
	//!	@date		2015-04-04
	String& String::Append(const char* _szText, uint32 _uiLength /*= 0*/)
	{
		const uint32 uiLength = m_vCharacter.GetSize();
		return Insert(_szText, uiLength, _uiLength);
	}

	//!	@brief		Append
	//!	@date		2015-04-04
	String& String::Append(const String& _sText)
	{
		const uint32 uiLength = m_vCharacter.GetSize();
		return Insert(_sText.GetBuffer(), uiLength, _sText.GetLength());
	}

	//!	@brief		Append
	//!	@date		2015-05-10
	String& String::Append(char _cChar)
	{
		const uint32 uiLength = m_vCharacter.GetSize();
		return Insert(&_cChar, uiLength, 1);
	}

	//!	@brief		Append
	//!	@date		2015-04-04
	String& String::Append(int _iValue, uint32 _uiLength /*= 0*/)
	{
		const uint32 uiLength = m_vCharacter.GetSize();
		return Insert(_iValue, uiLength, _uiLength);
	}

	//!	@brief		Append
	//!	@date		2015-04-04
	String& String::Append(uint32 _uiValue, uint32 _uiLength /*= 0*/)
	{
		const uint32 uiLength = m_vCharacter.GetSize();
		return Insert(_uiValue, uiLength, _uiLength);
	}

	//!	@brief		Prepend
	//!	@date		2015-04-04
	String& String::Prepend(const char* _szText, uint32 _uiLength /*= 0*/)
	{
		return Insert(_szText, 0, _uiLength);
	}

	//!	@brief		Prepend
	//!	@date		2015-04-04
	String& String::Prepend(const String& _sText)
	{
		return Insert(_sText.GetBuffer(), 0, _sText.GetLength());
	}

	//!	@brief		Prepend
	//!	@date		2015-05-10
	String& String::Prepend(char _cChar)
	{
		return Insert(&_cChar, 0, 1);
	}

	//!	@brief		Prepend
	//!	@date		2015-04-04
	String& String::Prepend(int _iValue, uint32 _uiLength /*= 0*/)
	{
		return Insert(_iValue, 0, _uiLength);
	}

	//!	@brief		Prepend
	//!	@date		2015-04-04
	String& String::Prepend(uint32 _uiValue, uint32 _uiLength /*= 0*/)
	{
		return Insert(_uiValue, 0, _uiLength);
	}

	//!	@brief		Replace
	//!	@date		2015-04-04
	String& String::Replace(const char* _szOldSubText, const char* _szNewSubText)
	{
		// Store current value
		String sOldText(*this);

		// Start looking for the old SubText
		char* szOldTextStart = (char*)sOldText.GetBuffer();
		char* szOldSubTextStart = (strstr(&szOldTextStart[0], _szOldSubText));
		if(szOldSubTextStart)
		{
			m_vCharacter.Clear();
		
			uint32 uiNewSubTextLength = (uint32)strlen(_szNewSubText);
			uint32 uiOldSubTextLength = (uint32)strlen(_szOldSubText);
			while(szOldSubTextStart)
			{
				// Add Left Part and the new SubText to the result
				szOldSubTextStart[0] = '\0';
				Append(szOldTextStart, (uint32)(szOldSubTextStart - szOldTextStart));
				Append(_szNewSubText, uiNewSubTextLength);

				// Search for other occurence
				szOldTextStart = szOldSubTextStart + uiOldSubTextLength;
				szOldSubTextStart = strstr(szOldTextStart, _szOldSubText);
			}
			Append(&szOldTextStart[0]);
		}

		return *this;
	}

	//!	@brief		Remove
	//!	@details	Replace the text with no character ("")
	//!	@date		2015-04-04
	String& String::Remove(const char* _szSubText)
	{
		return Replace(_szSubText, "");
	}

	//!	@brief		Resize
	//!	@date		2015-04-04
	void String::Resize(uint32 _uiSize)
	{
		m_vCharacter.Reserve(_uiSize+1, false);
		m_vCharacter.Resize(_uiSize);
		m_vCharacter[_uiSize] = 0;
	}

	//!	@brief		operator=
	//!	@date		2015-04-04
	String& String::operator= (const char* _szText)
	{
		return Set(_szText);
	}

	//!	@brief		operator=
	//!	@date		2015-04-04
	String& String::operator= (const String& _sText)
	{
		return Set(_sText.GetBuffer(), _sText.GetLength());
	}

	//!	@brief		operator+=
	//!	@date		2015-04-04
	String& String::operator+= (const char* _szText)
	{
		return Append(_szText);
	}

	//!	@brief		operator+
	//!	@date		2015-04-04
	String String::operator+ (const char* _szText) const
	{
		String sResult(*this);
		return sResult.Append(_szText);
	}

	//!	@brief		operator+=
	//!	@date		2015-04-04
	String& String::operator+= (const String& _sText)
	{
		return Append(_sText.GetBuffer(), _sText.GetLength());
	}

	//!	@brief		operator+
	//!	@date		2015-04-04
	String String::operator+ (const String& _sText) const
	{
		String sResult(*this);
		return sResult.Append(_sText.GetBuffer(), _sText.GetLength());
	}

	//!	@brief		operator+=
	//!	@date		2015-04-04
	String& String::operator+= (char _cValue)
	{
		return Append(&_cValue, 1);
	}

	//!	@brief		operator+
	//!	@date		2015-04-04
	String String::operator+ (char _cValue) const
	{
		String sResult(*this);
		return sResult.Append(&_cValue, 1);
	}

	//!	@brief		operator+=
	//!	@date		2015-04-04
	String& String::operator+= (int _iValue)
	{
		return Append(_iValue);
	}

	//!	@brief		operator+
	//!	@date		2015-04-04
	String String::operator+ (int _iValue) const
	{
		String sResult(*this);
		return sResult.Append(_iValue);
	}

	//!	@brief		operator+=
	//!	@date		2015-04-04
	String& String::operator+= (uint32 _uiValue)
	{
		return Append(_uiValue);
	}

	//!	@brief		operator+
	//!	@date		2015-04-04
	String String::operator+ (uint32 _uiValue) const
	{
		String sResult(*this);
		return sResult.Append(_uiValue);
	}

	//!	@brief		operator==
	//!	@date		2015-04-04
	bool String::operator== (const String& _sText) const
	{
		return Equals(_sText);
	}

	//!	@brief		operator==
	//!	@date		2015-04-04
	bool String::operator== (const char* _szText) const
	{
		return Equals(_szText);
	}

	//!	@brief		operator!=
	//!	@date		2015-04-04
	bool String::operator!= (const String& _sText) const
	{
		return !Equals(_sText);
	}

	//!	@brief		operator==
	//!	@date		2015-04-04
	bool String::operator!= (const char* _szText) const
	{
		return !Equals(_szText);
	}

	//!	@brief		Expand
	//! @details	Reserve more memory to add some String to the current one
	//!	@date		2015-04-04
	void String::Expand(uint32 _uiCharacterCount, uint32& uiOldSizeOut, uint32& uiNewSizeOut)
	{
		uiOldSizeOut = m_vCharacter.GetSize();
		uiNewSizeOut = uiOldSizeOut + _uiCharacterCount;
		if(m_vCharacter.GetCapacity() <= uiNewSizeOut)
		{
			m_vCharacter.Reserve(uiNewSizeOut << 1, true);
		}
		m_vCharacter.Resize(uiNewSizeOut);
		m_vCharacter[uiNewSizeOut] = 0;
	}

	//!	@brief		operator+
	//!	@detials	Generate a String directly from a const char*
	//!	@date		2015-04-04
	String operator+(const char* _szTextA, const String& _sTextB)
	{
		String sResult(_szTextA);
		sResult += _sTextB;
		return sResult;
	}
}