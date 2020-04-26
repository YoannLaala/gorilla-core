#pragma once

/******************************************************************************
**  Includes
******************************************************************************/
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
    class DictionaryJsonStream
    {
    public:
        enum class Flag
        {
            PRETTY = 1 << 0,
        };

        static bool read(StreamReader *stream, DictionaryNode *node);
        static bool write(StreamWriter *stream, DictionaryNode *node, uint32_t flags);
    };
}
