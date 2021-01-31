/******************************************************************************
**  includes
******************************************************************************/
#include "string.hpp"
#include <cctype>

/******************************************************************************
**  class definition
******************************************************************************/
namespace Gorilla
{
    //! @brief      constructor
    String::String()
    {
        // nothing to do
    }

    //! @brief      constructor
    String::String(const char* text)
        : String()
    {
        set(text);
    }

    //! @brief      constructor
    String::String(const char* text, uint32_t size)
        : String()
    {
        set(text, size);
    }

    //! @brief      constructor
    String::String(const String& text)
        : String()
    {
        set(text.get_buffer(), text.get_length());
    }

    //! @brief      constructor
    String::String(String&& text)
        : m_characters(std::move(text.m_characters))
    {
        // nothing to do
    }

    //! @brief      destructor
    String::~String()
    {
        // nothing to do
    }

    //! @brief      equals
    bool String::equals(const char* text) const
    {
        return text ? STRCMP(get_buffer(), text) == 0 : is_empty();
    }

    //! @brief      to_lower
    String& String::to_lower()
    {
        const uint32_t length = m_characters.get_size();
        for(uint32_t i = 0; i < length; ++i)
        {
            m_characters[i] = static_cast<char>(tolower(m_characters[i]));
        }

        return *this;
    }

    //! @brief      to_upper
    String& String::to_upper()
    {
        const uint32_t length = m_characters.get_size();
        for(uint32_t i = 0; i < length; ++i)
        {
            m_characters[i] = static_cast<char>(toupper(m_characters[i]));
        }

        return *this;
    }

    //! @brief      find
    //! @details    find a pattern a number of times
    //! @param      pattern the pattern wanted
    //! @param      start_index the index to start the search
    //! @param      count the number of pattern to find
    uint32_t String::find(const char* pattern, uint32_t start_index /*= 0*/, uint32_t count /*= 1*/) const
    {
        const char* start = &m_characters[start_index];
        const char* cursor = start;
        for(uint32_t i = 0; i < count; ++i)
        {
            cursor = STRSTR(cursor, pattern);
        }

        return cursor ? static_cast<uint32_t>(cursor - &m_characters[0]) : (uint32_t)-1;
    }

    //! @brief      find_last
    //! @details    find the last pattern
    //! @param      pattern the pattern wanted
    //! @param      start_index the index to start the search
    uint32_t String::find_last(const char* pattern, uint32_t start_index /*= 0*/) const
    {
        uint32_t index = (uint32_t)-1;

        const char* start = &m_characters[start_index];
        const char* cursor = STRSTR(start, pattern);
        while(cursor)
        {
            index = static_cast<uint32_t>(cursor - start);
            cursor = STRSTR(++cursor, pattern);
        }

        return index;
    }

    //! @brief      insert
    String& String::insert(const char* text, uint32_t index, uint32_t length /*= (uint32_t)-1*/)
    {
        if(text)
        {
            // get the real size for the new subtext
            if(length == (uint32_t)-1)
                length = static_cast<uint32_t>(STRLEN(text));
            if (length)
            {
                uint32_t old_size, new_size;
                expand(length, &old_size, &new_size);
                char* source = &m_characters[index];

                // move the right block if necessary
                uint32_t move_size = old_size - index;
                if(move_size)
                {
                    MEMMOVE(source + length, source, move_size);
                }

                // copy
                MEMCPY(source, text, length);
            }
        }

        return *this;
    }

    //! @brief      insert
    String& String::insert(uint64_t value, uint8_t base, bool is_negative, uint32_t index, uint32_t length /*= (uint32_t)-1*/)
    {
        // search for interger length only if needed with numbers only i
        if(length == (uint32_t)-1)
        {
            length = is_negative ? 1 : 0;
            if(value > 0)
            {
                for(uint64_t i = value; i != 0; i/=base)
                {
                    ++length;
                }
            }
            // handle 0 value
            else
            {
                ++length;
            }
        }

        uint32_t old_size, new_size;
        expand(length, &old_size, &new_size);
        char* source = &m_characters[0];

        // move the right block if necessary
        uint32_t move_size = old_size - index;
        if(move_size)
        {
            MEMMOVE(source + length, source, move_size);
        }

        // fill with numbers
        index += --length;
        do
        {
            source[index--] = "0123456789abcdef"[value % base];
            value /= base;
        }
        while(value);

        // handle negative sign
        if(is_negative)
        {
            source[index++] = '-';
        }

        return *this;
    }

    //! @brief      append
    String& String::insert(int64_t value, uint32_t index, uint32_t length /*= (uint32_t)-1*/)
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
    String& String::insert(uint64_t value, uint32_t index, uint32_t length /*= (uint32_t)-1*/)
    {
        return insert(value, 10, false, index, length);
    }

    //! @brief      set
    String& String::set(const char* text, uint32_t length /*= (uint32_t)-1*/)
    {
        m_characters.clear();
        return insert(text, 0, length);
    }

    //!@brief      set
    String& String::set(const String& text)
    {
        m_characters.clear();
        return insert(text.get_buffer(), 0, text.get_length());
    }

    //! @brief      set
    String& String::set(char value)
    {
        m_characters.clear();
        return insert(&value, 0, 1);
    }

    //! @brief      set
    String& String::set(int64_t value, uint32_t length /*= (uint32_t)-1*/)
    {
        m_characters.clear();
        return insert(value, 0, length);
    }

    //! @brief      set
    String& String::set(uint64_t value, uint32_t length /*= (uint32_t)-1*/)
    {
        m_characters.clear();
        return insert(value, 0, length);
    }

    //! @brief      append
    String& String::append(const char* text, uint32_t length /*= (uint32_t)-1*/)
    {
        const uint32_t current_length = m_characters.get_size();
        return insert(text, current_length, length);
    }

    //! @brief      append
    String& String::append(const String& text)
    {
        const uint32_t length = m_characters.get_size();
        return insert(text.get_buffer(), length, text.get_length());
    }

    //! @brief      append
    String& String::append(char value)
    {
        const uint32_t length = m_characters.get_size();
        return insert(&value, length, 1);
    }

    //! @brief      append
    String& String::append(int64_t value, uint32_t length /*= (uint32_t)-1*/)
    {
        const uint32_t current_length = m_characters.get_size();
        return insert(value, current_length, length);
    }

    //! @brief      append
    String& String::append(uint64_t value, uint32_t length /*= (uint32_t)-1*/)
    {
        const uint32_t current_length = m_characters.get_size();
        return insert(value, current_length, length);
    }

    //! @brief      prepend
    String& String::prepend(const char* text, uint32_t length /*= (uint32_t)-1*/)
    {
        return insert(text, 0, length);
    }

    //! @brief      prepend
    String& String::prepend(const String& text)
    {
        return insert(text.get_buffer(), 0, text.get_length());
    }

    //! @brief      prepend
    String& String::prepend(char value)
    {
        return insert(&value, 0, 1);
    }

    //! @brief      prepend
    String& String::prepend(int64_t value, uint32_t length /*= (uint32_t)-1*/)
    {
        return insert(value, 0, length);
    }

    //! @brief      prepend
    String& String::prepend(uint64_t value, uint32_t length /*= (uint32_t)-1*/)
    {
        return insert(value, 0, length);
    }

    //! @brief      replace
    String& String::replace(const char* old_text, const char* new_text)
    {
        // store current value
        String soldtext(*this);

        // start looking for the old subtext
        char* szoldtextstart = (char*)soldtext.get_buffer();
        char* szoldsubtextstart = (strstr(&szoldtextstart[0], old_text));
        if(szoldsubtextstart)
        {
            m_characters.clear();

            uint32_t uinewsubtextlength = (uint32_t)strlen(new_text);
            uint32_t uioldsubtextlength = (uint32_t)strlen(old_text);
            while(szoldsubtextstart)
            {
                // add left part and the new subtext to the result
                szoldsubtextstart[0] = '\0';
                append(szoldtextstart, (uint32_t)(szoldsubtextstart - szoldtextstart));
                append(new_text, uinewsubtextlength);

                // search for other occurence
                szoldtextstart = szoldsubtextstart + uioldsubtextlength;
                szoldsubtextstart = strstr(szoldtextstart, old_text);
            }
            append(&szoldtextstart[0]);
        }

        return *this;
    }

    //! @brief  remove
    //! @details    replace the text with no character ("")
    String& String::remove(const char* value)
    {
        return replace(value, "");
    }

    //! @brief      resize
    //! @details    allocate size+1bytes to auto add it
    void String::resize(uint32_t size)
    {
        m_characters.reserve(size+1, false);
        m_characters.resize(size);
        m_characters[size] = 0;
    }

    //! @brief      operator=
    String& String::operator= (const char* text)
    {
        return set(text);
    }

    //! @brief      operator=
    String& String::operator= (const String& text)
    {
        return set(text.get_buffer(), text.get_length());
    }

    //! @brief      operator=
    String& String::operator= (String&& text)
    {
        m_characters = std::move(text.m_characters);
        return *this;
    }

    //! @brief      operator=
    String& String::operator= (int64_t value)
    {
        return set(value);
    }

    //! @brief      operator=
    String& String::operator= (uint64_t value)
    {
        return set(value);
    }

    //! @brief      operator+=
    String& String::operator+= (const char* text)
    {
        return append(text);
    }

    //! @brief      operator+
    String String::operator+ (const char* text) const
    {
        String result(*this);
        return result.append(text);
    }

    //!	@brief		operator+=
    //!	@date		2015-04-04
    String& String::operator+= (const String& text)
    {
        return append(text.get_buffer(), text.get_length());
    }

    //! @brief      operator+
    String String::operator+ (const String& text) const
    {
        String result(*this);
        return result.append(text.get_buffer(), text.get_length());
    }

    //! @brief      operator+=
    String& String::operator+= (char value)
    {
        return append(&value, 1);
    }

    //! @brief      operator+
    String String::operator+ (char value) const
    {
        String result(*this);
        return result.append(&value, 1);
    }

    //! @brief      operator+=
    String& String::operator+= (int64_t value)
    {
        return append(value);
    }

    //! @brief      operator+
    String String::operator+ (int64_t value) const
    {
        String result(*this);
        return result.append(value);
    }

    //! @brief      operator+=
    String& String::operator+= (uint64_t value)
    {
        return append(value);
    }

    //! @brief      operator+
    String String::operator+ (uint64_t value) const
    {
        String result(*this);
        return result.append(value);
    }

    //! @brief      operator==
    bool String::operator== (const String& text) const
    {
        return equals(text);
    }

    //! @brief      operator==
    bool String::operator== (const char* text) const
    {
        return equals(text);
    }

    //! @brief      operator!=
    bool String::operator!= (const String& text) const
    {
        return !equals(text);
    }

    //! @brief      operator==
    bool String::operator!= (const char* text) const
    {
        return !equals(text);
    }

    //! @brief      expand
    //! @details    reserve more memory to add some String to the current one
    void String::expand(uint32_t character_count, uint32_t *old_size_out, uint32_t *new_size_out)
    {
        *old_size_out = m_characters.get_size();
        *new_size_out = *old_size_out + character_count;
        if(m_characters.get_capacity() <= *new_size_out)
        {
            m_characters.reserve(*new_size_out << 1, true);
        }
        m_characters.resize(*new_size_out);
        m_characters[*new_size_out] = 0;
    }

    //! @brief      operator+
    String operator+(const char* texta, const String& textb)
    {
        String result(texta);
        result += textb;
        return result;
    }
}
