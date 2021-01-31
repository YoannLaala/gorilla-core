/******************************************************************************
**  Includes
******************************************************************************/
#include "string16.hpp"
#include <wchar.h>
#include <cwctype>

/******************************************************************************
**  Class Definition
******************************************************************************/
namespace Gorilla
{
    //! @brief      Constructor
    String16::String16()
    {
        // nothing to do
    }

    //! @brief      Constructor
    String16::String16(const wchar_t* text)
        : String16()
    {
        set(text);
    }

    //! @brief      Constructor
    String16::String16(const String16& text)
        : String16()
    {
        set(text.get_buffer(), text.get_length());
    }

    //! @brief      Constructor
    String16::String16(uint32_t size)
        : String16()
    {
        resize(size);
    }

    //! @brief      Destructor
    String16::~String16()
    {
        // Nothing to do
    }

    //! @brief      equals
    bool String16::equals(const wchar_t* text) const
    {
        return wcscmp(&m_characters[0], text) == 0;
    }

    //! @brief      to_lower
    String16& String16::to_lower()
    {
        const uint32_t length = m_characters.get_size();
        for(uint32_t i = 0; i < length; ++i)
        {
            m_characters[i] = static_cast<char>(towlower(m_characters[i]));
        }

        return *this;
    }

    //! @brief      to_upper
    String16& String16::to_upper()
    {
        const uint32_t length = m_characters.get_size();
        for(uint32_t i = 0; i < length; ++i)
        {
            m_characters[i] = static_cast<char>(towupper(m_characters[i]));
        }

        return *this;
    }

    //! @brief      find
    //! @details    find a pattern a number of times
    //! @param      pattern the pattern wanted
    //! @param      start_index the index to start the search
    //! @param      count the number of pattern to find
    uint32_t String16::find(const wchar_t* pattern, uint32_t start_index /*= 0*/, uint32_t count /*= 1*/) const
    {
        const wchar_t* pCursor = &m_characters[start_index];
        for(uint32_t uiPattern = 0; uiPattern < count; ++uiPattern)
        {
            pCursor = wcsstr(pCursor, pattern);
        }

        return pCursor ? (uint32_t)(pCursor - &m_characters[0]) : (uint32_t)-1;
    }

    //! @brief      find_last
    //! @details    find the last pattern
    //! @param      pattern the pattern wanted
    //! @param      start_index the index to start the search
    uint32_t String16::find_last(const wchar_t* pattern, uint32_t start_index /*= 0*/) const
    {
        uint32_t index = (uint32_t)-1;

        const wchar_t* start = &m_characters[start_index];
        const wchar_t* cursor = wcsstr(start, pattern);
        while(cursor)
        {
            index = (uint32_t)(cursor - start);
            cursor = wcsstr(++cursor, pattern);
        }

        return index;
    }

    //! @brief      contains
    bool String16::contains(const wchar_t *value, uint32_t start_index /*= 0*/) const
    {
        return find(value, start_index) != (uint32_t)-1;
    }

    //! @brief      insert
    String16& String16::insert(const wchar_t* text, uint32_t index, uint32_t length /*= (uint32_t)-1*/)
    {
        if(text)
        {
            // Get the real size for the new subtext
            if(length == (uint32_t)-1)
                length = (uint32_t)wcslen(text);

            if (length)
            {
                uint32_t old_size, new_size;
                expand(length, &old_size, &new_size);

                // Move the right block if necessary
                uint32_t move_size = old_size - index;
                if(move_size)
                {
                    MEMCPY(&m_characters[index+length], &m_characters[index], move_size * sizeof(wchar_t));
                }

                // Copy
                MEMCPY(&m_characters[index], text, length * sizeof(wchar_t));
            }
        }

        return *this;
    }

    //! @brief      insert
    String16& String16::insert(uint64_t value, uint8_t base, bool is_negative, uint32_t index, uint32_t length /*= (uint32_t)-1*/)
    {
        // Search for interger length only if needed with numbers only i
        if(length == (uint32_t)-1)
        {
            length = is_negative ? 1 : 0;
            for(uint64_t i = value; i != 0; i/=base)
            {
                ++length;
            }
        }

        uint32_t old_size, new_size;
        expand(length, &old_size, &new_size);

        // Move the right block if necessary
        uint32_t move_size = old_size - index;
        if(move_size)
        {
            MEMCPY(&m_characters[index+length], &m_characters[index], move_size * sizeof(wchar_t));
        }

        // Fill with numbers
        index += --length;
        while(value)
        {
            m_characters[index--] = "0123456789abcdef"[value % base];
            value /= base;
        }

        // Handle negative sign
        if(is_negative)
        {
            m_characters[index++] = '-';
        }

        return *this;
    }

    //! @brief      append
    String16& String16::insert(int64_t value, uint32_t index, uint32_t length /*= (uint32_t)-1*/)
    {
        bool is_negative = false;
        if(value < 0)
        {
            value = -value;
            is_negative = true;
        }

        return insert((uint64_t)value, 10, is_negative, index, length);
    }

    //! @brief      append
    String16& String16::insert(uint64_t value, uint32_t index, uint32_t length /*= (uint32_t)-1*/)
    {
        return insert(value, 10, false, index, length);
    }

    //! @brief      set
    String16& String16::set(const wchar_t* text, uint32_t length /*= (uint32_t)-1*/)
    {
        m_characters.clear();
        return insert(text, 0, length);
    }

    //! @brief      set
    String16& String16::set(const String16& text)
    {
        m_characters.clear();
        return insert(text.get_buffer(), 0, text.get_length());
    }

    //! @brief      set
    String16& String16::set(wchar_t value)
    {
        m_characters.clear();
        return insert(&value, 0, 1);
    }

    //! @brief      set
    String16& String16::set(int64_t value, uint32_t length /*= (uint32_t)-1*/)
    {
        m_characters.clear();
        return insert(value, 0, length);
    }

    //! @brief      set
    String16& String16::set(uint64_t value, uint32_t length /*= (uint32_t)-1*/)
    {
        m_characters.clear();
        return insert(value, 0, length);
    }

    //! @brief      append
    String16& String16::append(const wchar_t* text, uint32_t length /*= (uint32_t)-1*/)
    {
        return insert(text, m_characters.get_size(), length);
    }

    //! @brief      append
    String16& String16::append(const String16& text)
    {
        return insert(text.get_buffer(), m_characters.get_size(), text.get_length());
    }

    //! @brief      append
    String16& String16::append(wchar_t value)
    {
        return insert(&value, m_characters.get_size(), 1);
    }

    //! @brief      append
    String16& String16::append(int64_t value, uint32_t length /*= (uint32_t)-1*/)
    {
        return insert(value, m_characters.get_size(), length);
    }

    //! @brief      append
    String16& String16::append(uint64_t value, uint32_t length /*= (uint32_t)-1*/)
    {
        return insert(value, m_characters.get_size(), length);
    }

    //! @brief      prepend
    String16& String16::prepend(const wchar_t* text, uint32_t length /*= (uint32_t)-1*/)
    {
        return insert(text, 0, length);
    }

    //! @brief      prepend
    String16& String16::prepend(const String16& text)
    {
        return insert(text.get_buffer(), 0, text.get_length());
    }

    //! @brief      prepend
    String16& String16::prepend(wchar_t value)
    {
        return insert(&value, 0, 1);
    }

    //! @brief      prepend
    String16& String16::prepend(int64_t value, uint32_t length /*= (uint32_t)-1*/)
    {
        return insert(value, 0, length);
    }

    //! @brief      prepend
    String16& String16::prepend(uint64_t value, uint32_t length /*= (uint32_t)-1*/)
    {
        return insert(value, 0, length);
    }

    //! @brief      replace
    String16& String16::replace(const wchar_t* old_text, const wchar_t* new_text)
    {
        // Store current value
        String16 self(*this);

        // Start looking for the old SubText
        wchar_t* old_text_start = (wchar_t*)self.get_buffer();
        wchar_t* old_text_cusor = wcsstr(&old_text_start[0], old_text);
        if(old_text_cusor)
        {
            m_characters.clear();

            uint32_t new_text_length = (uint32_t)wcslen(new_text);
            uint32_t old_text_length = (uint32_t)wcslen(old_text);
            while(old_text_cusor)
            {
                // Add Left Part and the new SubText to the result
                old_text_cusor[0] = '\0';
                append(old_text_start, (uint32_t)(old_text_cusor - old_text_start));
                append(new_text, new_text_length);

                // Search for other occurence
                old_text_start = old_text_cusor + old_text_length;
                old_text_cusor = wcsstr(old_text_start, old_text);
            }
            append(&old_text_start[0]);
        }

        return *this;
    }

    //! @brief      remove
    //! @details    replace the text with no wchar_tacter ("")
    String16& String16::remove(const wchar_t* text)
    {
        return replace(text, L"");
    }

    //! @brief      resize
    //! @details    resize the wchar_t array handling the end character
    void String16::resize(uint32_t size)
    {
        const uint32_t new_capacity = size+1;
        m_characters.reserve(new_capacity, false);
        m_characters.resize(size);
        m_characters[size] = 0;
    }

    //! @brief      operator=
    String16& String16::operator= (const wchar_t* text)
    {
        return set(text);
    }

    //! @brief      operator=
    String16& String16::operator= (const String16& text)
    {
        return set(text.get_buffer(), text.get_length());
    }

    //! @brief      operator=
    String16& String16::operator= (int64_t value)
    {
        return set(value);
    }

    //! @brief      operator=
    String16& String16::operator= (uint64_t value)
    {
        return set(value);
    }

    //! @brief      operator+=
    String16& String16::operator+= (const wchar_t* text)
    {
        return append(text);
    }

    //! @brief      operator+
    String16 String16::operator+ (const wchar_t* text) const
    {
        String16 result(*this);
        return result.append(text);
    }

    //! @brief      operator+=
    String16& String16::operator+= (const String16& text)
    {
        return append(text.get_buffer(), text.get_length());
    }

    //! @brief      operator+
    String16 String16::operator+ (const String16& text) const
    {
        String16 result(*this);
        return result.append(text.get_buffer(), text.get_length());
    }

    //! @brief      operator+=
    String16& String16::operator+= (wchar_t value)
    {
        return append(&value, 1);
    }

    //! @brief      operator+
    String16 String16::operator+ (wchar_t value) const
    {
        String16 result(*this);
        return result.append(&value, 1);
    }

    //! @brief      operator+=
    String16& String16::operator+= (int64_t value)
    {
        return append(value);
    }

    //! @brief      operator+
    String16 String16::operator+ (int64_t value) const
    {
        String16 result(*this);
        return result.append(value);
    }

    //! @brief      operator+=
    String16& String16::operator+= (uint64_t value)
    {
        return append(value);
    }

    //! @brief      operator+
    String16 String16::operator+ (uint64_t value) const
    {
        String16 result(*this);
        return result.append(value);
    }

    //! @brief      operator==
    bool String16::operator== (const String16& text) const
    {
        return equals(text);
    }

    //! @brief      operator==
    bool String16::operator== (const wchar_t* text) const
    {
        return equals(text);
    }

    //! @brief      operator!=
    bool String16::operator!= (const String16& text) const
    {
        return !equals(text);
    }

    //! @brief      operator==
    bool String16::operator!= (const wchar_t* text) const
    {
        return !equals(text);
    }

    //! @brief      expand
    //! @details    reserve more memory to add some String16 to the current one
    void String16::expand(uint32_t character_count, uint32_t *old_size_out, uint32_t *new_size_out)
    {
        *old_size_out = m_characters.get_size();
        *new_size_out = *old_size_out + character_count;
        m_characters.reserve(*new_size_out+1, true);
        m_characters.resize(*new_size_out);
        m_characters[*new_size_out] = 0;
    }
}
