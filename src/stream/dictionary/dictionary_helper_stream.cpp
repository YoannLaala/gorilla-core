/******************************************************************************
**  Includes
******************************************************************************/
#include "dictionary_helper_stream.hpp"
#include "../../container/dictionary.hpp"
#include "../../string/string.hpp"
#include "../../string/string_helper.hpp"

/******************************************************************************
**  Class Definition
******************************************************************************/
namespace Gorilla
{
    //! @brief      write_bool
    inline bool write_bool(StreamWriter *stream, bool value)
    {
        bool result = false;
        if (value)
            result = stream->write("true", 4);
        else
            result = stream->write("false", 5);

        return result;
    }

    //! @brief      write_int64
    inline bool write_int64(StreamWriter *stream, int64_t value)
    {
        String result;
        if (!StringHelper::to_string(value, &result))
            return false;

        if (!stream->write(result.get_buffer(), result.get_length()))
            return false;

        return true;
    }

    //! @brief      write_uint64_t
    inline bool write_uint64(StreamWriter *stream, uint64_t value)
    {
        String result;
        if (!StringHelper::to_string(value, &result))
            return false;

        if (!stream->write(result.get_buffer(), result.get_length()))
            return false;

        return true;
    }

    //! @brief      write_double
    inline bool write_double(StreamWriter *stream, double value)
    {
        String result;
        if (!StringHelper::to_string(value, &result))
            return false;

        if (!stream->write(result.get_buffer(), result.get_length()))
            return false;

        return true;
    }

    //! @brief      write_string
    inline bool write_string(StreamWriter *stream, const char *value, uint32_t length)
    {
        if (!stream->write('"'))
            return false;

        if (!stream->write(value, length))
            return false;

        if (!stream->write('"'))
            return false;

        return true;
    }

    //! @brief      write
    bool DictionaryHelperStream::write(StreamWriter *stream, bool value)
    {
        return write_bool(stream, value);
    }

    //! @brief      write
    bool DictionaryHelperStream::write(StreamWriter *stream, uint8_t value)
    {
        return write_uint64(stream, (uint64_t)value);
    }

    //! @brief      write
    bool DictionaryHelperStream::write(StreamWriter *stream, uint16_t value)
    {
        return write_uint64(stream, (uint64_t)value);
    }

    //! @brief      write
    bool DictionaryHelperStream::write(StreamWriter *stream, uint32_t value)
    {
        return write_uint64(stream, (uint64_t)value);
    }

    //! @brief      write
    bool DictionaryHelperStream::write(StreamWriter *stream, uint64_t value)
    {
        return write_int64(stream, value);
    }

    //! @brief      write
    bool DictionaryHelperStream::write(StreamWriter *stream, int8_t value)
    {
        return write_int64(stream, (int64_t)value);
    }

    //! @brief      write
    bool DictionaryHelperStream::write(StreamWriter *stream, int16_t value)
    {
        return write_int64(stream, (int64_t)value);
    }

    //! @brief      write
    bool DictionaryHelperStream::write(StreamWriter *stream, int32_t value)
    {
        return write_int64(stream, (int64_t)value);
    }

    //! @brief      write
    bool DictionaryHelperStream::write(StreamWriter *stream, int64_t value)
    {
        return write_int64(stream, value);
    }

    //! @brief      write
    bool DictionaryHelperStream::write(StreamWriter *stream, float value)
    {
        return write_double(stream, (double)value);
    }

    //! @brief      write
    bool DictionaryHelperStream::write(StreamWriter *stream, double value)
    {
        return write_double(stream, value);
    }

    //! @brief      write
    bool DictionaryHelperStream::write(StreamWriter *stream, const char *value)
    {
        return write_string(stream, value, (uint32_t)STRLEN(value));
    }

    //! @brief      write_buffer_element_bool
    inline bool write_buffer_element_bool(StreamWriter *stream, const void *value, uint32_t *offset)
    {
        if (!write_bool(stream, *(bool*)((uint8_t*)value + *offset)))
            return false;

        *offset += sizeof(bool);

        return true;
    }

    //! @brief      write_buffer_element_uint8_t
    inline bool write_buffer_element_uint8_t(StreamWriter *stream, const void *value, uint32_t *offset)
    {
        if (!write_uint64(stream, *(uint8_t*)((uint8_t*)value + *offset)))
            return false;

        *offset += sizeof(uint8_t);

        return true;
    }

    //! @brief      write_buffer_element_uint16_t
    inline bool write_buffer_element_uint16_t(StreamWriter *stream, const void *value, uint32_t *offset)
    {
        if (!write_uint64(stream, *(uint16_t*)((uint8_t*)value + *offset)))
            return false;

        *offset += sizeof(uint16_t);

        return true;
    }

    //! @brief      write_buffer_element_uint32_t
    inline bool write_buffer_element_uint32_t(StreamWriter *stream, const void *value, uint32_t *offset)
    {
        if (!write_uint64(stream, *(uint32_t*)((uint8_t*)value + *offset)))
            return false;

        *offset += sizeof(uint32_t);

        return true;
    }

    //! @brief      write_buffer_element_uint64_t
    inline bool write_buffer_element_uint64_t(StreamWriter *stream, const void *value, uint32_t *offset)
    {
        if (!write_uint64(stream, *(uint64_t*)((uint8_t*)value + *offset)))
            return false;

        *offset += sizeof(uint64_t);

        return true;
    }

    //! @brief      write_buffer_element_int8
    inline bool write_buffer_element_int8(StreamWriter *stream, const void *value, uint32_t *offset)
    {
        if (!write_int64(stream, *(int8_t*)((uint8_t*)value + *offset)))
            return false;

        *offset += sizeof(int8_t);

        return true;
    }

    //! @brief      write_buffer_element_int16
    inline bool write_buffer_element_int16(StreamWriter *stream, const void *value, uint32_t *offset)
    {
        if (!write_int64(stream, *(int16_t*)((uint8_t*)value + *offset)))
            return false;

        *offset += sizeof(int16_t);

        return true;
    }

    //! @brief      write_buffer_element_int32
    inline bool write_buffer_element_int32(StreamWriter *stream, const void *value, uint32_t *offset)
    {
        if (!write_int64(stream, *(int32_t*)((uint8_t*)value + *offset)))
            return false;

        *offset += sizeof(int32_t);

        return true;
    }

    //! @brief      write_buffer_element_int64
    inline bool write_buffer_element_int64(StreamWriter *stream, const void *value, uint32_t *offset)
    {
        if (!write_int64(stream, *(int64_t*)((uint8_t*)value + *offset)))
            return false;

        *offset += sizeof(int64_t);

        return true;
    }

    //! @brief      write_buffer_element_float32
    inline bool write_buffer_element_float32(StreamWriter *stream, const void *value, uint32_t *offset)
    {
        if (!write_double(stream, *(float*)((uint8_t*)value + *offset)))
            return false;

        *offset += sizeof(float);

        return true;
    }

    //! @brief      write_buffer_element_float64
    inline bool write_buffer_element_float64(StreamWriter *stream, const void *value, uint32_t *offset)
    {
        if (!write_double(stream, *(double*)((uint8_t*)value + *offset)))
            return false;

        *offset += sizeof(double);

        return true;
    }

    //! @brief      write_buffer_element_string
    inline bool write_buffer_element_string(StreamWriter *stream, const void *value, uint32_t *offset)
    {
        char *allocated_string = (char*)value + *offset;
        uint32_t length = *(uint32_t*)allocated_string;
        allocated_string += sizeof(uint32_t);
        if (!write_string(stream, allocated_string, length))
            return false;

        *offset += sizeof(uint32_t) + length + 1;

        return true;
    }

    //! @brief      get_format_description
    DictionaryHelperStream::WriteCallback DictionaryHelperStream::get_write_callback(DictionaryData::Format format)
    {
        static const WriteCallback descriptions[] = {
            { write_buffer_element_bool },
            { write_buffer_element_uint8_t },
            { write_buffer_element_uint16_t },
            { write_buffer_element_uint32_t },
            { write_buffer_element_uint64_t },
            { write_buffer_element_int8 },
            { write_buffer_element_int16 },
            { write_buffer_element_int32 },
            { write_buffer_element_int64 },
            { write_buffer_element_float32 },
            { write_buffer_element_float64 },
            { write_buffer_element_string },
        };
        return descriptions[(uint32_t)format];
    }
}