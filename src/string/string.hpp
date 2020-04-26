#pragma once

/******************************************************************************
**  Includes
******************************************************************************/
#include "../container/vector.hpp"

#include <cstdint>
#include <string.h>

/******************************************************************************
**  Defines
******************************************************************************/
#if defined(PLATFORM_WINDOWS)
    #define STRCPY(_destination_, _source_, _size_)     ::strcpy_s(_destination_, _size_, _source_)
#else
    #define STRCPY(_destination_, _source_, _size_)     ::strcpy(_destination_, _source_, _size_)
#endif
#define STRCMP(_text_a_, _text_b_)                      ::strcmp(_text_a_, _text_b_)
#define STRLEN(_text_)                                  ::strlen(_text_)
#define STRSTR(_text_, _sub_text_)                      ::strstr(_text_, _sub_text_)

#define STRING_STACK_CAPACITY   64

/******************************************************************************
**  Class Declaration
******************************************************************************/
namespace Gorilla
{
    class String
    {
    public:
        String();
        String(const char *value);
        String(const char *value, uint32_t length);
        String(const String& value);
        ~String();

        inline const char*  get_buffer      () const;

        inline uint32_t     get_length      () const;
        inline bool         is_empty        () const;

        inline bool         equals          (const String& value) const;
        bool                equals          (const char *value) const;
        String&             to_lower        ();
        String&             to_upper        ();
        uint32_t            find            (const char *value, uint32_t start_index = 0, uint32_t count = 1) const;
        uint32_t            find_last       (const char *value, uint32_t start_index = 0) const;
        inline bool         contains        (const char  *value, uint32_t start_index = 0) const;

        String&             insert          (const char *value,  uint32_t index, uint32_t length = 0);
        String&             insert          (uint64_t value, uint8_t base, bool is_negative, uint32_t index, uint32_t length = 0);
        inline String&      insert          (int32_t value, uint32_t index, uint32_t length = 0);
        inline String&      insert          (uint32_t value, uint32_t index, uint32_t length = 0);
        String&             insert          (int64_t value, uint32_t index, uint32_t length = 0);
        String&             insert          (uint64_t value, uint32_t index, uint32_t length = 0);

        String&             set             (const char *value, uint32_t length = 0);
        String&             set             (const String& value);
        String&             set             (char value);
        inline String&      set             (int32_t value, uint32_t length = 0);
        inline String&      set             (uint32_t value, uint32_t length = 0);
        String&             set             (int64_t value, uint32_t length = 0);
        String&             set             (uint64_t value, uint32_t length = 0);

        String&             append          (const char *value, uint32_t length = 0);
        String&             append          (const String& value);
        String&             append          (char value);
        inline String&      append          (int32_t value, uint32_t length = 0);
        inline String&      append          (uint32_t value, uint32_t length = 0);
        String&             append          (int64_t value, uint32_t length = 0);
        String&             append          (uint64_t value, uint32_t length = 0);

        String&             prepend         (const char *value, uint32_t length = 0);
        String&             prepend         (const String& value);
        String&             prepend         (char value);
        inline String&      prepend         (int32_t value, uint32_t length = 0);
        inline String&      prepend         (uint32_t value, uint32_t length = 0);
        String&             prepend         (int64_t value, uint32_t length = 0);
        String&             prepend         (uint64_t value, uint32_t length = 0);

        String&             replace         (const char *old_text, const char *new_text);
        String&             remove          (const char *value);
        void                resize          (uint32_t length);

        String&             operator=       (const char *value);
        String&             operator=       (const String& value);
        inline String&      operator=       (int32_t value);
        inline String&      operator=       (uint32_t value);
        String&             operator=       (int64_t value);
        String&             operator=       (uint64_t value);

        String&             operator+=      (const char *value);
        String              operator+       (const char *value) const;
        String&             operator+=      (const String& value);
        String              operator+       (const String& value) const;
        String&             operator+=      (char value);
        String              operator+       (char value) const;
        inline String&      operator+=      (int32_t value);
        inline String       operator+       (int32_t value) const;
        inline String&      operator+=      (uint32_t value);
        inline String       operator+       (uint32_t value) const;
        String&             operator+=      (int64_t value);
        String              operator+       (int64_t value) const;
        String&             operator+=      (uint64_t value);
        String              operator+       (uint64_t value) const;

        bool                operator==      (const String& value) const;
        bool                operator==      (const char *value) const;
        bool                operator!=      (const String& value) const;
        bool                operator!=      (const char *value) const;

        inline char         operator[]      (uint32_t index) const;
        inline char&        operator[]      (uint32_t index);

    private:
        void                expand          (uint32_t character_count, uint32_t *old_size_out, uint32_t *new_size_out);

    private:
        Vector<char>        m_characters;
        char                m_stack[STRING_STACK_CAPACITY];
    };

    //! @brief      get_buffer
    const char* String::get_buffer() const
    {
        return &m_characters[0];
    }

    //! @brief      get_length
    uint32_t String::get_length() const
    {
        return m_characters.get_size();
    }

    //! @brief      is_empty
    bool String::is_empty() const
    {
        return m_characters.is_empty();
    }

    //! @brief      equals
    bool String::equals(const String &value) const
    {
        return m_characters.get_size() == value.get_length() && equals(value.get_buffer());
    }

    //! @brief      contains
    bool String::contains(const char *value, uint32_t start_index /*= 0*/) const
    {
        return find(value, start_index) != (uint32_t)-1;
    }

    //! @brief      insert
    String& String::insert(int32_t value, uint32_t index, uint32_t length /*= 0*/)
    {
        return insert((int64_t)value, index, length);
    }

    //! @brief      insert
    String& String::insert(uint32_t value, uint32_t index, uint32_t length /*= 0*/)
    {
        return insert((uint64_t)value, index, length);
    }

    //! @brief      set
    String& String::set(int32_t value, uint32_t length /*= 0*/)
    {
        return set((int64_t)value, length);
    }

    //! @brief      set
    String& String::set(uint32_t value, uint32_t length /*= 0*/)
    {
        return set((uint64_t)value, length);
    }

    //! @brief      append
    String& String::append(int32_t value, uint32_t length /*= 0*/)
    {
        return append((int64_t)value, length);
    }

    //! @brief      append
    String& String::append(uint32_t value, uint32_t length /*= 0*/)
    {
        return append((uint64_t)value, length);
    }

    //! @brief      prepend
    String& String::prepend(int32_t value, uint32_t length /*= 0*/)
    {
        return prepend((int64_t)value, length);
    }

    //! @brief      prepend
    String& String::prepend(uint32_t value, uint32_t length /*= 0*/)
    {
        return prepend((uint32_t)value, length);
    }

    //! @brief      operator=
    String& String::operator=(int32_t value)
    {
        operator=((int64_t)value);
    }

    //! @brief      operator=
    String& String::operator=(uint32_t value)
    {
        operator=((uint64_t)value);
    }

    //! @brief      operator+=
    String& String::operator+=(int32_t value)
    {
        operator+=((int64_t)value);
    }

    //! @brief      operator+
    String String::operator+(int32_t value) const
    {
        operator+((int64_t)value);
    }

    //! @brief      operator+=
    String& String::operator+=(uint32_t value)
    {
        operator+=((uint64_t)value);
    }

    //! @brief      operator+
    String String::operator+(uint32_t value) const
    {
        operator+((uint64_t)value);
    }

    //! @brief      operator[]
    char String::operator[](uint32_t index) const
    {
        return m_characters[index];
    }

    //! @brief      operator[]
    char& String::operator[](uint32_t index)
    {
        return m_characters[index];
    }
}
