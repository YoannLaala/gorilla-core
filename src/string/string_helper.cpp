/******************************************************************************
**	Includes
******************************************************************************/
#include "string_helper.hpp"
#include "../log/log.hpp"

#include <string.h>
#include <wchar.h>
#include <math.h>

/******************************************************************************
**	Define
******************************************************************************/
#define UNICODE_MASK_VALUE              0x3f        // 0011 1111

#define UNICODE_HEADER_MASK_1_BYTE      0x80        // 1000 0000
#define UNICODE_HEADER_MASK_2_BYTES     0xc0        // 1100 0000
#define UNICODE_HEADER_MASK_3_BYTES     0xe0        // 1110 0000
#define UNICODE_HEADER_MASK_4_BYTES     0xf0        // 1111 0000

#define UNICODE_MAX_VALUE_1_BYTE        0x80        // 128
#define UNICODE_MAX_VALUE_2_BYTES       0x800       // 2048
#define UNICODE_MAX_VALUE_3_BYTES       0x10000     // 65536

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla
{
    //! @brief      to_bool
    inline bool to_bool(const char *buffer, uint32_t length, StringValue *value_out)
    {
        switch (length)
        {
            case 4:
            {
                if (buffer[0] == 't'
                    && buffer[1] == 'r'
                    && buffer[2] == 'u'
                    && buffer[3] == 'e')
                {
                    value_out->Boolean = true;
                    value_out->DetectedType = StringValue::Type::BOOLEAN;
                    return true;
                }
                break;
            }

            case 5:
            {
                if (buffer[0] == 'f'
                    && buffer[1] == 'a'
                    && buffer[2] == 'l'
                    && buffer[3] == 's'
                    && buffer[4] == 'e')
                    {
                        value_out->Boolean = false;
                        value_out->DetectedType = StringValue::Type::BOOLEAN;
                        return true;
                    }
                break;
            }
        }

        return false;
    }

    //! @brief      to_number
    inline bool to_number(const char *buffer, uint32_t length, StringValue *value_out)
    {
        enum class Flag
        {
            SEPARATOR       = 1 << 0,
            EXPONENT        = 1 << 1,
        };

        int8_t flags = 0, value_sign = 1, exponent_sign = 1;
        uint32_t index = 0, point_index = (uint32_t)-1, integer_count = 0;
        uint32_t multiplier = 0, divider = 0, exponent = 0;
        int64_t value = 0;
        while(index < length)
        {
            char character = buffer[index];
            switch(character)
            {
                // sign
                case '+':
                case '-':
                {
                    // negative sign are possible with exponent
                    if(flags & (uint8_t)Flag::EXPONENT)
                    {
                        // previous character must be exponent
                        if(buffer[index-1] != 'e' && buffer[index-1] != 'E')
                            return false;

                        if (character == '-')
                            exponent_sign = -1;

                        break;
                    }

                    if(index != 0)
                        return false;

                    if (character == '-')
                        value_sign = -1;

                    break;
                }

                // support , or . floating point notation (0,5 / 0.5)
                case ',':
                case '.':
                {
                    if(flags & (uint8_t)Flag::SEPARATOR)
                        return false;
                    flags |= (uint8_t)Flag::SEPARATOR;
                    point_index = integer_count;
                    break;
                }

                // support exponent notation (1e6 / 1E6)
                case 'e':
                case 'E':
                {
                    if(index == 0)
                        return false;

                    if(flags & (uint8_t)Flag::EXPONENT)
                        return false;
                    flags |= (uint8_t)Flag::EXPONENT;

                    break;
                }

                // support final f or d notation (0.5f / 0.5d)
                case 'f':
                case 'd':
                {
                    if(index != length-1)
                        return false;
                    break;
                }

                default:
                {
                    if(character < '0' || character > '9')
                        return false;

                    if(flags & (uint8_t)Flag::EXPONENT)
                        exponent = exponent * 10 + (character - '0');
                    else
                    {
                        value = value * 10 + (character - '0');
                        ++integer_count;
                    }
                    break;
                }
            }
            ++index;
        }

        // apply floating point
        if(point_index != (uint32_t)-1)
            divider += (integer_count - point_index);

        // apply exponent
        if (exponent_sign > 0)
            multiplier += exponent;
        else
            divider += exponent;

        // integer
        if (multiplier >= divider)
        {
            value_out->Integer = value_sign * value * (int64_t)pow(10, (multiplier - divider));
            value_out->DetectedType = StringValue::Type::INTEGER;
        }
        else
        {
            value_out->Float = value_sign * value * (1.0 / pow(10, (divider - multiplier)));
            value_out->DetectedType = StringValue::Type::FLOAT;
        }

        return true;
    }

    void StringHelper::to_value(const char *buffer, uint32_t length, StringValue *value_out)
    {
        if (length)
        {
            if (to_bool(buffer, length, value_out))
                return;

            if (to_number(buffer, length, value_out))
                return;
        }

        value_out->String.Buffer = buffer;
        value_out->String.Length = length;
        value_out->DetectedType = StringValue::Type::STRING;
    }


    inline uint32_t compute_string_length(uint64_t value)
    {
	    if (value < 10U) return 1;
	    if (value < 100U) return 2;
	    if (value < 1000U) return 3;
	    if (value < 10000U) return 4;
	    if (value < 100000U) return 5;
	    if (value < 1000000U) return 6;
	    if (value < 10000000U) return 7;
	    if (value < 100000000U) return 8;
	    if (value < 1000000000U) return 9;
        if (value < 10000000000U) return 10;
        if (value < 100000000000U) return 11;
        if (value < 1000000000000U) return 12;
        if (value < 10000000000000U) return 13;
        if (value < 100000000000000U) return 14;
        if (value < 1000000000000000U) return 15;
        if (value < 10000000000000000U) return 16;
        if (value < 100000000000000000U) return 17;
        if (value < 1000000000000000000U) return 18;
        if (value < 10000000000000000000U) return 19;
        return 20;
    }

    //! @brief      to_string_internal
    inline bool to_string_internal(uint64_t value, bool is_negative, char *buffer, uint32_t buffer_length)
    {

        char *cursor = buffer + buffer_length;
        if (is_negative)
            *buffer = '-';

        if (value == 0)
        {
            *--cursor = '0';
            return true;
        }

        while (value > (uint32_t)-1)
        {
            uint32_t chunk = (uint32_t)(value % 1000000000);
            value /= 1000000000;

            while (chunk != 0)
            {
                *--cursor = "0123456789"[chunk % 10];
                chunk /= 10;
            }
        }

        while (value != 0)
        {
            *--cursor = "0123456789"[value % 10];
            value /= 10;
        }

        return true;
    }

    //! @brief      to_string
    bool StringHelper::to_string(int64_t value, String *value_out)
    {
        if (value < 0)
        {
            value = (uint64_t)-value;
            uint32_t length = compute_string_length(value);
            value_out->resize(++length);
            return to_string_internal(value, true, (char*)value_out->get_buffer(), length);
        }

        uint32_t length = compute_string_length((uint64_t)value);
        value_out->resize(length);
        return to_string_internal((uint64_t)value, false, (char*)value_out->get_buffer(), length);
    }

    //! @brief      to_string
    bool StringHelper::to_string(uint64_t value, String *value_out)
    {
        uint32_t length = compute_string_length(value);
        value_out->resize(length);
        return to_string_internal(value, false, (char*)value_out->get_buffer(), length);
    }

    //! @brief      to_string
    bool StringHelper::to_string(double value, String *value_out)
    {
        bool is_negative = false;
        if (value < 0)
        {
            is_negative = true;
            value = -value;
        }

        // extract integral, exponent and decimal
        uint64_t integral = (uint64_t)value;
        uint32_t exponent = compute_string_length(integral);
        double decimal = (value + 0.0000005) - integral;

        // find precision needed for decimal (13.3700 is 2)
        uint32_t decimal_as_uint = (uint32_t)(decimal * 1e6);
        uint32_t precision = 1;
        if (decimal_as_uint)
        {
            precision = 6;
            while (decimal_as_uint % 10 == 0)
            {
                --precision;
                decimal_as_uint /= 10;
            }
        }

        if(is_negative)
            ++exponent;

        uint32_t length = exponent + 1 + precision;
        value_out->resize(length);
        char *buffer = (char*)value_out->get_buffer();

        if (!to_string_internal(integral, is_negative, buffer, exponent))
            return false;

        buffer[exponent] = '.';
        buffer += exponent + 1;

        do
        {
            decimal *= 10;
            uint32_t digit = (uint32_t)decimal;
            *buffer++ = "0123456789"[digit % 10];
            decimal -= digit;
            --precision;
        } while (precision > 0);

        return true;
    }

    //! @brief      to_string
    void StringHelper::to_string(const wchar_t *text, String *value_out)
    {
        to_string(text, (uint32_t)wcslen(text), value_out);
    }

    //! @brief      to_string
    void StringHelper::to_string(const wchar_t *text, uint32_t size, String *value_out)
    {
        value_out->set("");

        // Iterate all character and encore to UTF8
        for(uint32_t i  = 0; i < size; ++i)
        {
            wchar_t character16 = text[i];

            // No need to encode because it will be on 1byte
            if (character16 < UNICODE_MAX_VALUE_1_BYTE)
            {
                value_out->append((char)character16);
            }
            // Encode on 2bytes 1100 0000
            else if (character16 < UNICODE_MAX_VALUE_2_BYTES)
            {
                value_out->append((char)(UNICODE_HEADER_MASK_2_BYTES | (character16 >> 6)));
                value_out->append((char)(UNICODE_HEADER_MASK_1_BYTE | (character16 & UNICODE_MASK_VALUE)));
            }
            // Encode on 3bytes 1110 0000
            else if (character16 < UNICODE_MAX_VALUE_3_BYTES)
            {
                value_out->append((char)(UNICODE_HEADER_MASK_3_BYTES | (character16 >> 12)));
                value_out->append((char)(UNICODE_HEADER_MASK_1_BYTE | ((character16 >> 6) & UNICODE_MASK_VALUE)));
                value_out->append((char)(UNICODE_HEADER_MASK_1_BYTE | (character16 & UNICODE_MASK_VALUE)));
            }
            // Encode on 4bytes 1110 0000
            else
            {
                LOG_ASSERT(false, "32bits character not supported");
            }
        }
    }

    //! @brief      to_string16
    void StringHelper::to_string16(const char *text, String16 *value_out)
    {
        to_string16(text, (uint32_t)STRLEN(text), value_out);
    }

    //! @brief      to_string16
    void StringHelper::to_string16(const char *text, uint32_t size, String16 *value_out)
    {
        if (size == (uint32_t)-1)
            size = (uint32_t)STRLEN(text);

        value_out->resize(size);

        uint32_t real_size = 0;
        for(uint32_t i  = 0; i < size; ++i)
        {
            char character = text[i];
            switch((character & UNICODE_HEADER_MASK_4_BYTES))
            {
                // Encoded on 2bytes 1100 0000
                case UNICODE_HEADER_MASK_2_BYTES:
                {
                    wchar_t character16 = character & 0x1f;
                    character16 = character16 << 6 | (text[++i] & UNICODE_MASK_VALUE);
                    (*value_out)[real_size] = character16;
                    break;
                }

                // Encoded on 3bytes 1110 0000
                case UNICODE_HEADER_MASK_3_BYTES:
                {
                    wchar_t character16 = character & 0x0f;
                    character16 = character16 << 6 | (text[++i] & UNICODE_MASK_VALUE);
                    character16 = character16 << 6 | (text[++i] & UNICODE_MASK_VALUE);
                    (*value_out)[real_size] = character16;

                    break;
                }

                // Encoded on 4bytes 1111 0000
                case UNICODE_HEADER_MASK_4_BYTES:
                {
                    LOG_ASSERT(false, "32bits character not supported");
                    break;
                }

                // ANSI character
                default:
                {
                    (*value_out)[real_size] = character;
                    break;
                };
            }

            ++real_size;
        }

        // Finalize with real size
        value_out->resize(real_size);
    }

    //! @brief      split
    void StringHelper::split(const char *value, const char *separator, Vector<String> *result_out)
    {
        const uint32_t separator_size = static_cast<uint32_t>(STRLEN(separator));
        const char* start = value;
        const char* end = STRSTR(start, separator);
        while (end)
        {
            result_out->add().set(start, static_cast<uint32_t>(end - start));
            start = end + separator_size;
            end = STRSTR(start, separator);
        }
        result_out->add().set(start);
    }
}
