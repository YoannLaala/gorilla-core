#pragma once

/******************************************************************************
**  Includes
******************************************************************************/
#include "../container/vector.hpp"
#include "../string/string.hpp"

/******************************************************************************
**  Class Declaration
******************************************************************************/
namespace Gorilla
{
    class StreamWriter
    {
    public:
        virtual bool    can_write() const = 0;
        virtual uint32_t  get_position() const = 0;
        virtual uint32_t  get_size() const = 0;
        virtual bool    seek(uint32_t position) = 0;
        virtual bool    write(const void *buffer, uint32_t size) = 0;

        inline bool     write(bool value);
        inline bool     write(char value);
        inline bool     write(uint8_t value);
        inline bool     write(uint16_t value);
        inline bool     write(uint32_t value);
        inline bool     write(uint64_t value);
        inline bool     write(int8_t value);
        inline bool     write(int16_t value);
        inline bool     write(int32_t value);
        inline bool     write(int64_t value);
        inline bool     write(float value);
        inline bool     write(double value);
        inline bool     write(const String *value);
    };

    bool StreamWriter::write(bool value)
    {
        return write(&value, sizeof(bool));
    }

    bool StreamWriter::write(char value)
    {
        return write(&value, sizeof(char));
    }

    bool StreamWriter::write(uint8_t value)
    {
        return write(&value, sizeof(uint8_t));
    }

    bool StreamWriter::write(uint16_t value)
    {
        return write(&value, sizeof(uint16_t));
    }

    bool StreamWriter::write(uint32_t value)
    {
        return write(&value, sizeof(uint32_t));
    }

    bool StreamWriter::write(uint64_t value)
    {
        return write(&value, sizeof(uint64_t));
    }

    bool StreamWriter::write(int8_t value)
    {
        return write(&value, sizeof(int8_t));
    }

    bool StreamWriter::write(int16_t value)
    {
        return write(&value, sizeof(int16_t));
    }

    bool StreamWriter::write(int32_t value)
    {
        return write(&value, sizeof(int32_t));
    }

    bool StreamWriter::write(int64_t value)
    {
        return write(&value, sizeof(int64_t));
    }

    bool StreamWriter::write(float value)
    {
        return write(&value, sizeof(float));
    }

    bool StreamWriter::write(double value)
    {
        return write(&value, sizeof(double));
    }

    bool StreamWriter::write(const String *value)
    {
        const uint32_t length = value->get_length();
        if (!write(length))
            return false;

        return write(&(value)[0], length);
    }
}
