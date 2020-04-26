#pragma once

/******************************************************************************
**  Includes
******************************************************************************/
#include "../../container/dictionary_data.hpp"

#include <cstdint>

/******************************************************************************
**  Forward Declaration
******************************************************************************/
namespace Gorilla
{
    class StreamReader;
    class StreamWriter;
    class DictionaryNode;
}

/******************************************************************************
**  Class Declaration
******************************************************************************/
namespace Gorilla
{
    class DictionaryHelperStream
    {
    public:
        typedef bool (*WriteCallback)(StreamWriter *stream, const void *data, uint32_t *offset);

        // static bool read(StreamReader *stream, DictionaryNode *node);
        static bool write(StreamWriter *stream, bool value);
        static bool write(StreamWriter *stream, uint8_t value);
        static bool write(StreamWriter *stream, uint16_t value);
        static bool write(StreamWriter *stream, uint32_t value);
        static bool write(StreamWriter *stream, uint64_t value);
        static bool write(StreamWriter *stream, int8_t value);
        static bool write(StreamWriter *stream, int16_t value);
        static bool write(StreamWriter *stream, int32_t value);
        static bool write(StreamWriter *stream, int64_t value);
        static bool write(StreamWriter *stream, float value);
        static bool write(StreamWriter *stream, double value);
        static bool write(StreamWriter *stream, const char *value);

        static WriteCallback get_write_callback(DictionaryData::Format format);
    };
}
