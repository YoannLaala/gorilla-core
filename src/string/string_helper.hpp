#pragma once

/******************************************************************************
**  Includes
******************************************************************************/
#include "string.hpp"
#include "string16.hpp"

/******************************************************************************
**  Class Declaration
******************************************************************************/
namespace Gorilla
{
    struct StringReference
    {
        const char *Buffer;
        uint32_t Length;
    };

    struct StringValue
    {
        enum class Type
        {
            BOOLEAN = 0,
            INTEGER,
            FLOAT,
            STRING
        };

        union
        {
            bool Boolean;
            int64_t Integer;
            double Float;
            StringReference String;
        };
        Type DetectedType;
    };

    class StringHelper
    {
    public:
        static void         to_value         (const char *buffer, uint32_t length, StringValue *value_out);

        inline static bool  to_string        (uint32_t value, String *value_out);
        static bool         to_string        (uint64_t value, String *value_out);
        inline static bool  to_string        (int32_t value, String *value_out);
        static bool         to_string        (int64_t value, String *value_out);
        inline static bool  to_string        (float value, String *value_out);
        static bool         to_string        (double value, String *value_out);

        static void         to_string        (const wchar_t *text, uint32_t size, String *value_out);
        static void         to_string        (const wchar_t *text, String *value_out);
        static void         to_string16      (const char *text, uint32_t size, String16 *value_out);
        static void         to_string16      (const char *text, String16 *value_out);

        static void         split            (const char *text, const char *separator, Vector<String> *result_out);
    };

    //! @brief      to_string
    bool StringHelper::to_string(int32_t value, String *value_out)
    {
        return to_string((int64_t)value, value_out);
    }

    //! @brief      to_string
    bool StringHelper::to_string(uint32_t value, String *value_out)
    {
        return to_string((uint64_t)value, value_out);
    }

    //! @brief      to_string
    bool StringHelper::to_string(float value, String *value_out)
    {
        return to_string((double)value, value_out);
    }
}
