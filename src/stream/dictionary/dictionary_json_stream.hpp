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

        enum class Operation
        {
            BEGIN_OBJECT = 0,
            END_OBJECT,
            BEGIN_ARRAY,
            END_ARRAY,
            STRING,
            ASSIGN,
            VALUE,
            NEXT_MEMBER,
            INDENT,
            END_OF_FILE,
        };

        static bool read(StreamReader *stream, DictionaryNode *node);
        static bool write(StreamWriter *stream, DictionaryNode *node, uint32_t flags);
    };
}
