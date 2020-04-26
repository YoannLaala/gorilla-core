#pragma once

/******************************************************************************
**  Includes
******************************************************************************/
#include "../container/vector.hpp"

#include <cstdint>

/******************************************************************************
**  Defines
******************************************************************************/
#define STRING16_STACK_CAPACITY 64

/******************************************************************************
**  Class Declaration
******************************************************************************/
namespace Gorilla
{
    class String16
    {
    public:
        String16();
        String16(const wchar_t *text);
        String16(const String16& text);
        String16(uint32_t size);
        ~String16();

        inline const wchar_t*   get_buffer       () const;

        inline uint32_t         get_length       () const;
        inline bool             is_empty         () const;

        inline bool             equals          (const String16& text) const;
        bool                    equals          (const wchar_t *text) const;
        String16&               to_lower        ();
        String16&               to_upper        ();
        uint32_t                find            (const wchar_t *pattern, uint32_t start_index = 0, uint32_t count = 1) const;
        uint32_t                find_last       (const wchar_t *pattern, uint32_t start_index = 0) const;
        bool                    contains        (const wchar_t  *value, uint32_t start_index = 0) const;

        String16&               insert          (const wchar_t *text,  uint32_t index, uint32_t length = 0);
        String16&               insert          (uint64_t value, uint8_t base, bool is_negative, uint32_t index, uint32_t length = 0);
        inline String16&        insert          (int32_t value, uint32_t index, uint32_t length = 0);
        inline String16&        insert          (uint32_t value, uint32_t index, uint32_t length = 0);
        String16&               insert          (int64_t value, uint32_t index, uint32_t length = 0);
        String16&               insert          (uint64_t value, uint32_t index, uint32_t length = 0);

        String16&               set             (const wchar_t *text, uint32_t length = 0);
        String16&               set             (const String16& text);
        String16&               set             (wchar_t value);
        inline String16&        set             (int32_t value, uint32_t length = 0);
        inline String16&        set             (uint32_t value, uint32_t length = 0);
        String16&               set             (int64_t value, uint32_t length = 0);
        String16&               set             (uint64_t value, uint32_t length = 0);

        String16&               append          (const wchar_t *text, uint32_t length = 0);
        String16&               append          (const String16& text);
        String16&               append          (wchar_t value);
        inline String16&        append          (int32_t value, uint32_t length = 0);
        inline String16&        append          (uint32_t value, uint32_t length = 0);
        String16&               append          (int64_t value, uint32_t length = 0);
        String16&               append          (uint64_t value, uint32_t length = 0);

        String16&               prepend         (const wchar_t *text, uint32_t length = 0);
        String16&               prepend         (const String16& text);
        String16&               prepend         (wchar_t value);
        inline String16&        prepend         (int32_t value, uint32_t length = 0);
        inline String16&        prepend         (uint32_t value, uint32_t length = 0);
        String16&               prepend         (int64_t value, uint32_t length = 0);
        String16&               prepend         (uint64_t value, uint32_t length = 0);

        String16&               replace         (const wchar_t *old_text, const wchar_t *new_text);
        String16&               remove          (const wchar_t *text);
        void                    resize          (uint32_t size);

        String16&               operator=       (const wchar_t *text);
        String16&               operator=       (const String16& text);
        inline String16&        operator=       (int32_t value);
        inline String16&        operator=       (uint32_t value);
        String16&               operator=       (int64_t value);
        String16&               operator=       (uint64_t value);

        String16&               operator+=      (const wchar_t *text);
        String16                operator+       (const wchar_t *text) const;
        String16&               operator+=      (const String16& text);
        String16                operator+       (const String16& text) const;
        String16&               operator+=      (wchar_t value);
        String16                operator+       (wchar_t value) const;
        inline String16&        operator+=      (int32_t value);
        inline String16         operator+       (int32_t value) const;
        inline String16&        operator+=      (uint32_t value);
        inline String16         operator+       (uint32_t value) const;
        String16&               operator+=      (int64_t value);
        String16                operator+       (int64_t value) const;
        String16&               operator+=      (uint64_t value);
        String16                operator+       (uint64_t value) const;

        bool                    operator==      (const String16& text) const;
        bool                    operator==      (const wchar_t *text) const;
        bool                    operator!=      (const String16& text) const;
        bool                    operator!=      (const wchar_t *text) const;

        inline wchar_t&         operator[]      (uint32_t index);
        inline wchar_t          operator[]      (uint32_t index) const;

    private:
        void                    expand          (uint32_t character_count, uint32_t *old_size_out, uint32_t *new_size_out);

    private:
        wchar_t m_stack[STRING16_STACK_CAPACITY];
        Vector<wchar_t> m_characters;
    };

    const wchar_t* String16::get_buffer() const
    {
        return &m_characters[0];
    }

    uint32_t String16::get_length() const
    {
        return m_characters.get_size();
    }

    bool String16::is_empty() const
    {
        return m_characters.is_empty();
    }

    bool String16::equals(const String16& text) const
    {
        return m_characters.get_size() == text.get_length() && equals(text.get_buffer());
    }

    //! @brief      insert
    String16& String16::insert(int32_t value, uint32_t index, uint32_t length /*= 0*/)
    {
        return insert((int64_t)value, index, length);
    }

    //! @brief      insert
    String16& String16::insert(uint32_t value, uint32_t index, uint32_t length /*= 0*/)
    {
        return insert((uint64_t)value, index, length);
    }

    //! @brief      set
    String16& String16::set(int32_t value, uint32_t length /*= 0*/)
    {
        return set((int64_t)value, length);
    }

    //! @brief      set
    String16& String16::set(uint32_t value, uint32_t length /*= 0*/)
    {
        return set((uint64_t)value, length);
    }

    //! @brief      append
    String16& String16::append(int32_t value, uint32_t length /*= 0*/)
    {
        return append((int64_t)value, length);
    }

    //! @brief      append
    String16& String16::append(uint32_t value, uint32_t length /*= 0*/)
    {
        return append((uint64_t)value, length);
    }

    //! @brief      prepend
    String16& String16::prepend(int32_t value, uint32_t length /*= 0*/)
    {
        return prepend((int64_t)value, length);
    }

    //! @brief      prepend
    String16& String16::prepend(uint32_t value, uint32_t length /*= 0*/)
    {
        return prepend((uint32_t)value, length);
    }

    //! @brief      operator=
    String16& String16::operator=(int32_t value)
    {
        operator=((int64_t)value);
    }

    //! @brief      operator=
    String16& String16::operator=(uint32_t value)
    {
        operator=((uint64_t)value);
    }

    //! @brief      operator+=
    String16& String16::operator+=(int32_t value)
    {
        operator+=((int64_t)value);
    }

    //! @brief      operator+
    String16 String16::operator+(int32_t value) const
    {
        operator+((int64_t)value);
    }

    //! @brief      operator+=
    String16& String16::operator+=(uint32_t value)
    {
        operator+=((uint64_t)value);
    }

    //! @brief      operator+
    String16 String16::operator+(uint32_t value) const
    {
        operator+((uint64_t)value);
    }

    wchar_t& String16::operator[](uint32_t index)
    {
        return m_characters[index];
    }

    wchar_t String16::operator[](uint32_t index) const
    {
        return m_characters[index];
    }
}
