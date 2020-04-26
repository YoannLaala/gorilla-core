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
    class StreamReader
    {
    public:
        virtual bool    can_read() const = 0;
        virtual uint32_t  get_position() const = 0;
        virtual uint32_t  get_size() const = 0;
        virtual bool    seek(uint32_t position) = 0;
        virtual bool    read(void *buffer, uint32_t size) = 0;

        inline bool     read(bool *value);
        inline bool     read(char *value);
        inline bool     read(uint8_t *value);
        inline bool     read(uint16_t *value);
        inline bool     read(uint32_t *value);
        inline bool     read(uint64_t *value);
        inline bool     read(int8_t *value);
        inline bool     read(int16_t *value);
        inline bool     read(int32_t *value);
        inline bool     read(int64_t *value);
        inline bool     read(float *value);
        inline bool     read(double *value);
        inline bool     read(String *value);
    };

    bool StreamReader::read(bool *value)
    {
        return read(value, sizeof(bool));
    }

    bool StreamReader::read(char *value)
    {
        return read(value, sizeof(char));
    }

    bool StreamReader::read(uint8_t *value)
    {
        return read(value, sizeof(uint8_t));
    }

    bool StreamReader::read(uint16_t *value)
    {
        return read(value, sizeof(uint16_t));
    }

    bool StreamReader::read(uint32_t *value)
    {
        return read(value, sizeof(uint32_t));
    }

    bool StreamReader::read(uint64_t *value)
    {
        return read(value, sizeof(uint64_t));
    }

    bool StreamReader::read(int8_t *value)
    {
        return read(value, sizeof(int8_t));
    }

    bool StreamReader::read(int16_t *value)
    {
        return read(value, sizeof(int16_t));
    }

    bool StreamReader::read(int32_t *value)
    {
        return read(value, sizeof(int32_t));
    }

    bool StreamReader::read(int64_t *value)
    {
        return read(value, sizeof(int64_t));
    }

    bool StreamReader::read(float *value)
    {
        return read(value, sizeof(float));
    }

    bool StreamReader::read(double *value)
    {
        return read(value, sizeof(double));
    }

    bool StreamReader::read(String *value)
    {
        uint32_t length;
        if (!read(&length))
            return false;

        value->resize(length);
        return read(&(*value)[0], length);
    }
}
