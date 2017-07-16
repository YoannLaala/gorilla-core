/******************************************************************************
**	Includes
******************************************************************************/
#include "String16.hpp"
#include <wchar.h>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla
{
	//!	@brief		Constructor
	//!	@date		2015-04-04
	String16::String16()
	{	
		m_vCharacter.SetBuffer(m_aStack, 0, STRING16_STACK_CAPACITY, false);
	}

	//!	@brief		Constructor
	//!	@date		2015-04-04
	String16::String16(const wchar_t* _szText)
		: String16()
	{
		Set(_szText);
	}

	//!	@brief		Constructor
	//!	@date		2015-04-04
	String16::String16(const String16& _sText)
		: String16()
	{
		Set(_sText.GetBuffer(), _sText.GetLength());
	}

	//!	@brief		Constructor
	//!	@date		2015-04-04
	String16::String16(uint32 _uiSize)
		: String16()
	{
		Resize(_uiSize);
	}

	//!	@brief		Destructor
	//!	@date		2015-06-21
	String16::~String16()
	{
		// Nothing to do
	}

	//!	@brief		Equals
	//!	@date		2015-06-21
	bool String16::Equals(const wchar_t* _szText) const 
	{ 
		return wcscmp(&m_vCharacter[0], _szText) == 0; 
	}

	//!	@brief		ToLower
	//!	@date		2015-04-04
	String16& String16::ToLower()
	{
		_wcslwr_s(&m_vCharacter[0], m_vCharacter.GetSize());

		return *this;
	}

	//!	@brief		ToUpper
	//!	@date		2015-04-04
	String16& String16::ToUpper()
	{
		_wcsupr_s(&m_vCharacter[0], m_vCharacter.GetSize());

		return *this;
	}

	//!	@brief		Find
	//!	@details	Find a pattern a number of times
	//! @param		_szPattern the pattern wanted
	//!	@param		_uiStartIndex the index to start the search
	//!	@param		_uiCount the number of pattern to find
	//!	@date		2015-04-04
	uint32 String16::Find(const wchar_t* _szPattern, uint32 _uiStartIndex /*= 0*/, uint32 _uiCount /*= 1*/) const
	{
		const wchar_t* pCursor = &m_vCharacter[_uiStartIndex];
		for(uint32 uiPattern = 0; uiPattern < _uiCount; ++uiPattern)
		{
			pCursor = wcsstr(pCursor, _szPattern);
		}

		return pCursor ? (uint32)(pCursor - &m_vCharacter[0]) : (uint32)-1;
	}

	//!	@brief		FindLast
	//!	@details	Find the last pattern
	//! @param		_szPattern the pattern wanted
	//!	@param		_uiStartIndex the index to start the search
	//!	@date		2015-04-04
	uint32 String16::FindLast(const wchar_t* _szPattern, uint32 _uiStartIndex /*= 0*/) const
	{
		uint32 uiIndex = (uint32)-1;

		const wchar_t* pStart = &m_vCharacter[_uiStartIndex];
		const wchar_t* pCursor = wcsstr(pStart, _szPattern);
		while(pCursor)
		{
			uiIndex = (uint32)(pCursor - pStart);
			pCursor = wcsstr(++pCursor, _szPattern);
		}

		return uiIndex;
	}

	//!	@brief		Insert
	//!	@date		2015-04-04
	String16& String16::Insert(const wchar_t* _szText, uint32 _uiIndex, uint32 _uiLength /*= 0*/)
	{
		if(_szText)
		{
			// Get the real size for the new subtext
			if(_uiLength == 0)
			{
				_uiLength = (uint32)wcslen(_szText);
			}
		
			uint32 uiOldSize, uiNewSize;
			Expand(_uiLength, uiOldSize, uiNewSize);
		
			// Move the right block if necessary
			uint32 uiMoveSize = uiOldSize - _uiIndex;
			if(uiMoveSize)
			{
				memcpy_s(&m_vCharacter[_uiIndex+_uiLength], uiNewSize * sizeof(wchar_t), &m_vCharacter[_uiIndex], uiMoveSize * sizeof(wchar_t));
			}

			// Copy
			memcpy_s(&m_vCharacter[_uiIndex], uiNewSize * sizeof(wchar_t), _szText, _uiLength * sizeof(wchar_t));
		}

		return *this;
	}

	//!	@brief		Insert
	//!	@date		2015-04-04
	String16& String16::Insert(uint32 _uiValue, uint8 _uiBase, bool _bIsNegative, uint32 _uiIndex, uint32 _uiLength /*= 0*/)
	{	
		// Search for interger length only if needed with numbers only i
		if(_uiLength == 0)
		{
			_uiLength = _bIsNegative ? 1 : 0;
			for(uint32 uiValueTmp = _uiValue; uiValueTmp != 0; uiValueTmp/=_uiBase)
			{
				++_uiLength;
			}	
		}

		uint32 uiOldSize, uiNewSize;
		Expand(_uiLength, uiOldSize, uiNewSize);

		// Move the right block if necessary
		uint32 uiMoveSize = uiOldSize - _uiIndex;
		if(uiMoveSize)
		{
			memcpy_s(&m_vCharacter[_uiIndex+_uiLength], uiNewSize * sizeof(wchar_t), &m_vCharacter[_uiIndex], uiMoveSize * sizeof(wchar_t));
		}
	
		// Fill with numbers
		_uiIndex += --_uiLength;
		while(_uiValue)
		{
			m_vCharacter[_uiIndex--] = "0123456789abcdef"[_uiValue % _uiBase];
			_uiValue /= _uiBase;
		}	

		// Handle negative sign	
		if(_bIsNegative)
		{
			m_vCharacter[_uiIndex++] = '-';
		}

		return *this;
	}

	//!	@brief		Append
	//!	@date		2015-04-04
	String16& String16::Insert(int _iValue, uint32 _uiIndex, uint32 _uiLength /*= 0*/)
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
	String16& String16::Insert(uint32 _uiValue, uint32 _uiIndex, uint32 _uiLength /*= 0*/)
	{
		return Insert(_uiValue, 10, false, _uiIndex, _uiLength);
	}

	//!	@brief		Set
	//!	@date		2015-04-04
	String16& String16::Set(const wchar_t* _szText, uint32 _uiLength /*= 0*/)
	{
		m_vCharacter.Clear();
		return Insert(_szText, 0, _uiLength);
	}

	//!	@brief		Set
	//!	@date		2015-04-04
	String16& String16::Set(const String16& _sText)
	{
		m_vCharacter.Clear();
		return Insert(_sText.GetBuffer(), 0, _sText.GetLength());
	}

	//!	@brief		Set
	//!	@date		2015-05-10
	String16& String16::Set(wchar_t _cwchar_t)
	{
		m_vCharacter.Clear();
		return Insert(&_cwchar_t, 0, 1);
	}

	//!	@brief		Set
	//!	@date		2015-04-04
	String16& String16::Set(int _iValue, uint32 _uiLength /*= 0*/)
	{
		m_vCharacter.Clear();
		return Insert(_iValue, 0, _uiLength);
	}

	//!	@brief		Set
	//!	@date		2015-04-04
	String16& String16::Set(uint32 _uiValue, uint32 _uiLength /*= 0*/)
	{
		m_vCharacter.Clear();
		return Insert(_uiValue, 0, _uiLength);
	}

	//!	@brief		Append
	//!	@date		2015-04-04
	String16& String16::Append(const wchar_t* _szText, uint32 _uiLength /*= 0*/)
	{
		return Insert(_szText, m_vCharacter.GetSize(), _uiLength);
	}

	//!	@brief		Append
	//!	@date		2015-04-04
	String16& String16::Append(const String16& _sText)
	{
		return Insert(_sText.GetBuffer(), m_vCharacter.GetSize(), _sText.GetLength());
	}

	//!	@brief		Append
	//!	@date		2015-05-10
	String16& String16::Append(wchar_t _cwchar_t)
	{
		return Insert(&_cwchar_t, m_vCharacter.GetSize(), 1);
	}

	//!	@brief		Append
	//!	@date		2015-04-04
	String16& String16::Append(int _iValue, uint32 _uiLength /*= 0*/)
	{
		return Insert(_iValue, m_vCharacter.GetSize(), _uiLength);
	}

	//!	@brief		Append
	//!	@date		2015-04-04
	String16& String16::Append(uint32 _uiValue, uint32 _uiLength /*= 0*/)
	{
		return Insert(_uiValue, m_vCharacter.GetSize(), _uiLength);
	}

	//!	@brief		Prepend
	//!	@date		2015-04-04
	String16& String16::Prepend(const wchar_t* _szText, uint32 _uiLength /*= 0*/)
	{
		return Insert(_szText, 0, _uiLength);
	}

	//!	@brief		Prepend
	//!	@date		2015-04-04
	String16& String16::Prepend(const String16& _sText)
	{
		return Insert(_sText.GetBuffer(), 0, _sText.GetLength());
	}

	//!	@brief		Prepend
	//!	@date		2015-05-10
	String16& String16::Prepend(wchar_t _cwchar_t)
	{
		return Insert(&_cwchar_t, 0, 1);
	}

	//!	@brief		Prepend
	//!	@date		2015-04-04
	String16& String16::Prepend(int _iValue, uint32 _uiLength /*= 0*/)
	{
		return Insert(_iValue, 0, _uiLength);
	}

	//!	@brief		Prepend
	//!	@date		2015-04-04
	String16& String16::Prepend(uint32 _uiValue, uint32 _uiLength /*= 0*/)
	{
		return Insert(_uiValue, 0, _uiLength);
	}

	//!	@brief		Replace
	//!	@date		2015-04-04
	String16& String16::Replace(const wchar_t* _szOldSubText, const wchar_t* _szNewSubText)
	{
		// Store current value
		String16 sOldText(*this);

		// Start looking for the old SubText
		wchar_t* szOldTextStart = (wchar_t*)sOldText.GetBuffer();
		wchar_t* szOldSubTextStart = wcsstr(&szOldTextStart[0], _szOldSubText);
		if(szOldSubTextStart)
		{
			m_vCharacter.Clear();
		
			uint32 uiNewSubTextLength = (uint32)wcslen(_szNewSubText);
			uint32 uiOldSubTextLength = (uint32)wcslen(_szOldSubText);
			while(szOldSubTextStart)
			{
				// Add Left Part and the new SubText to the result
				szOldSubTextStart[0] = '\0';
				Append(szOldTextStart, (uint32)(szOldSubTextStart - szOldTextStart));
				Append(_szNewSubText, uiNewSubTextLength);

				// Search for other occurence
				szOldTextStart = szOldSubTextStart + uiOldSubTextLength;
				szOldSubTextStart = wcsstr(szOldTextStart, _szOldSubText);
			}
			Append(&szOldTextStart[0]);
		}

		return *this;
	}

	//!	@brief		Remove
	//!	@details	Replace the text with no wchar_tacter ("")
	//!	@date		2015-04-04
	String16& String16::Remove(const wchar_t* _szSubText)
	{
		return Replace(_szSubText, L"");
	}

	//!	@brief		Resize
	//! @details	Resize the wchar_t array ghandling the end wchar_tacter
	//!	@date		2015-04-11
	void String16::Resize(uint32 uiSize)
	{
		const uint32 uiNewCapacity = uiSize+1;
		m_vCharacter.Reserve(uiNewCapacity, false);
		m_vCharacter.Resize(uiSize);
		m_vCharacter[uiSize] = 0;	
	}

	//!	@brief		operator=
	//!	@date		2015-04-04
	String16& String16::operator= (const wchar_t* _szText)
	{
		return Set(_szText);
	}

	//!	@brief		operator=
	//!	@date		2015-04-04
	String16& String16::operator= (const String16& _sText)
	{
		return Set(_sText.GetBuffer(), _sText.GetLength());
	}

	//!	@brief		operator=
	//!	@date		2015-04-04
	String16& String16::operator= (uint32 _uiValue)
	{
		return Set(_uiValue);
	}

	//!	@brief		operator+=
	//!	@date		2015-04-04
	String16& String16::operator+= (const wchar_t* _szText)
	{
		return Append(_szText);
	}

	//!	@brief		operator+
	//!	@date		2015-04-04
	String16 String16::operator+ (const wchar_t* _szText) const
	{
		String16 sResult(*this);
		return sResult.Append(_szText);
	}

	//!	@brief		operator+=
	//!	@date		2015-04-04
	String16& String16::operator+= (const String16& _sText)
	{
		return Append(_sText.GetBuffer(), _sText.GetLength());
	}

	//!	@brief		operator+
	//!	@date		2015-04-04
	String16 String16::operator+ (const String16& _sText) const
	{
		String16 sResult(*this);
		return sResult.Append(_sText.GetBuffer(), _sText.GetLength());
	}

	//!	@brief		operator+=
	//!	@date		2015-04-04
	String16& String16::operator+= (wchar_t _cValue)
	{
		return Append(&_cValue, 1);
	}

	//!	@brief		operator+
	//!	@date		2015-04-04
	String16 String16::operator+ (wchar_t _cValue) const
	{
		String16 sResult(*this);
		return sResult.Append(&_cValue, 1);
	}

	//!	@brief		operator+=
	//!	@date		2015-04-04
	String16& String16::operator+= (int _iValue)
	{
		return Append(_iValue);
	}

	//!	@brief		operator+
	//!	@date		2015-04-04
	String16 String16::operator+ (int _iValue) const
	{
		String16 sResult(*this);
		return sResult.Append(_iValue);
	}

	//!	@brief		operator+=
	//!	@date		2015-04-04
	String16& String16::operator+= (uint32 _uiValue)
	{
		return Append(_uiValue);
	}

	//!	@brief		operator+
	//!	@date		2015-04-04
	String16 String16::operator+ (uint32 _uiValue) const
	{
		String16 sResult(*this);
		return sResult.Append(_uiValue);
	}

	//!	@brief		operator==
	//!	@date		2015-04-04
	bool String16::operator== (const String16& _sText) const
	{
		return Equals(_sText);
	}

	//!	@brief		operator==
	//!	@date		2015-04-04
	bool String16::operator== (const wchar_t* _szText) const
	{
		return Equals(_szText);
	}

	//!	@brief		operator!=
	//!	@date		2015-04-04
	bool String16::operator!= (const String16& _sText) const
	{
		return !Equals(_sText);
	}

	//!	@brief		operator==
	//!	@date		2015-04-04
	bool String16::operator!= (const wchar_t* _szText) const
	{
		return !Equals(_szText);
	}

	//!	@brief		Expand
	//! @details	Reserve more memory to add some String16 to the current one
	//!	@date		2015-04-04
	void String16::Expand(uint32 _uiwchar_tacterCount, uint32& uiOldSizeOut, uint32& uiNewSizeOut)
	{
		uiOldSizeOut = m_vCharacter.GetSize();
		uiNewSizeOut = uiOldSizeOut + _uiwchar_tacterCount;
		m_vCharacter.Reserve(uiNewSizeOut+1, true);
		m_vCharacter.Resize(uiNewSizeOut);
		m_vCharacter[uiNewSizeOut] = 0;	
	}

	//!	@brief		operator+
	//!	@detials	Generate a String16 directly from a const wchar_t*
	//!	@date		2015-04-04
	String16 operator+(const wchar_t* _szTextA, const String16& _sTextB)
	{
		String16 sResult(_szTextA);
		sResult += _sTextB;
		return sResult;
	}
}