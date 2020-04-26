#pragma once

/******************************************************************************
**  Includes
******************************************************************************/
#include "../string/string.hpp"

/******************************************************************************
**  Defines
******************************************************************************/
#define DICTIONARY_NODE_DATA_HASH_BITS 0xFFFFFFF0
#define DICTIONARY_NODE_DATA_FORMAT_BITS 0x0000000F

/******************************************************************************
**  Class Declaration
******************************************************************************/
namespace Gorilla
{
    struct DictionaryId
    {
        static const DictionaryId START;
        static const DictionaryId INVALID;

        inline bool operator==(DictionaryId id) const;
        inline bool operator!=(DictionaryId id) const;

        uint16_t PageIndex;
        uint16_t PageOffset;
    };

    bool DictionaryId::operator==(DictionaryId id) const
    {
        return PageOffset == id.PageOffset && PageIndex == id.PageIndex;
    }

    bool DictionaryId::operator!=(DictionaryId id) const
    {
        return PageOffset != id.PageOffset || PageIndex != id.PageIndex;
    }

    class DictionaryData
    {
        friend class Dictionary;
        friend class DictionaryNode;
    public:
        enum class Format
        {
            BOOLEAN = 0,
            UINTEGER8,
            UINTEGER16,
            UINTEGER32,
            UINTEGER64,
            INTEGER8,
            INTEGER16,
            INTEGER32,
            INTEGER64,
            FLOAT32,
            FLOAT64,
            STRING,
            BUFFER,
            OBJECT,
            ARRAY,
            ZERO,
        };

    private:
        inline DictionaryId get_next_id() const;
        inline void set_next_id(DictionaryId id);
        inline uint32_t get_hash() const;
        inline void set_hash(uint32_t hash);
        inline Format get_format() const;
        inline void set_format(Format format);

        uint8_t m_next_id[4];       // limit padding
        uint32_t m_hash_and_format; // 28bits hash, 4bits formats
    };

    DictionaryId DictionaryData::get_next_id() const
    {
        return {
            *(uint16_t*)&m_next_id[0],
            *(uint16_t*)&m_next_id[2],
        };
    }

    void DictionaryData::set_next_id(DictionaryId id)
    {
        *((uint32_t*)m_next_id) = *(uint32_t*)&id;
    }

    uint32_t DictionaryData::get_hash() const
    {
        return m_hash_and_format & DICTIONARY_NODE_DATA_HASH_BITS;
    }

    void DictionaryData::set_hash(uint32_t hash)
    {
        m_hash_and_format = (hash & DICTIONARY_NODE_DATA_HASH_BITS) | (uint32_t)get_format();
    }

    DictionaryData::Format DictionaryData::get_format() const
    {
        return (Format)(m_hash_and_format & DICTIONARY_NODE_DATA_FORMAT_BITS);
    }

    void DictionaryData::set_format(Format format)
    {
        m_hash_and_format = get_hash() | ((uint32_t)format & DICTIONARY_NODE_DATA_FORMAT_BITS);
    }

    template <DictionaryData::Format T>
    struct TDictionaryData;

    template <>
    struct TDictionaryData<DictionaryData::Format::OBJECT> : public DictionaryData
    {
        DictionaryId FirstChildId;
        DictionaryId LastChildId;
    };

    template <>
    struct TDictionaryData<DictionaryData::Format::ARRAY> : public DictionaryData
    {
        DictionaryId FirstChildId;
        DictionaryId LastChildId;
    };

    template <>
    struct TDictionaryData<DictionaryData::Format::BOOLEAN> : public DictionaryData
    {
        bool Value;
    };

    template <>
    struct TDictionaryData<DictionaryData::Format::UINTEGER8> : public DictionaryData
    {
        uint8_t Value;
    };

    template <>
    struct TDictionaryData<DictionaryData::Format::UINTEGER16> : public DictionaryData
    {
        uint16_t Value;
    };

    template <>
    struct TDictionaryData<DictionaryData::Format::UINTEGER32> : public DictionaryData
    {
        uint32_t Value;
    };

    template <>
    struct TDictionaryData<DictionaryData::Format::UINTEGER64> : public DictionaryData
    {
        uint64_t Value;
    };

    template <>
    struct TDictionaryData<DictionaryData::Format::INTEGER8> : public DictionaryData
    {
        int8_t Value;
    };

    template <>
    struct TDictionaryData<DictionaryData::Format::INTEGER16> : public DictionaryData
    {
        int16_t Value;
    };

    template <>
    struct TDictionaryData<DictionaryData::Format::INTEGER32> : public DictionaryData
    {
        int32_t Value;
    };

    template <>
    struct TDictionaryData<DictionaryData::Format::INTEGER64> : public DictionaryData
    {
        int64_t Value;
    };

    template <>
    struct TDictionaryData<DictionaryData::Format::FLOAT32> : public DictionaryData
    {
        float Value;
    };

    template <>
    struct TDictionaryData<DictionaryData::Format::FLOAT64> : public DictionaryData
    {
        double Value;
    };

    template <>
    struct TDictionaryData<DictionaryData::Format::STRING> : public DictionaryData
    {
        uint32_t Length;
    };

    template <>
    struct TDictionaryData<DictionaryData::Format::BUFFER> : public DictionaryData
    {
        uint32_t Count;
        uint32_t Capacity;
        DictionaryData::Format Format;
    };
}
