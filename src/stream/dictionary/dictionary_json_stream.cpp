/******************************************************************************
**  Includes
******************************************************************************/
#include "dictionary_json_stream.hpp"
#include "dictionary_helper_stream.hpp"
#include "../../container/dictionary.hpp"
#include "../../string/string.hpp"
#include "../../string/string_helper.hpp"

/******************************************************************************
**  Class Definition
******************************************************************************/
namespace Gorilla
{
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
    bool parse_string(char **cursor, const char *buffer_end, String *value_out);
    bool parse_data(char **cursor, const char *buffer_end, DictionaryNode *node_out);
    bool parse_object(char **cursor, const char *buffer_end, DictionaryNode *node_out);
    bool parse_array(char **cursor, const char *buffer_end, DictionaryNode *node_out);

    typedef bool (*write_callback)(StreamWriter *stream, const void *data);
    bool write_struct(StreamWriter *stream, DictionaryNode *node, uint32_t level, uint32_t flags);

    //! @brief      find_next_operation
    static Operation find_next_operation(char **cursor, const char *buffer_end)
    {
        while (*cursor != buffer_end)
        {
            char value = **cursor;
            ++*cursor;

            switch (value)
            {
                case '{': return Operation::BEGIN_OBJECT;
                case '}': return Operation::END_OBJECT;
                case '[': return Operation::BEGIN_ARRAY;
                case ']': return Operation::END_ARRAY;
                case '"': return Operation::STRING;
                case ':': return Operation::ASSIGN;
                case ',': return Operation::NEXT_MEMBER;
                case ' ':
                case '\r':
                case '\n':
                case '\t':
                    break;
                default: return Operation::VALUE;
            }

        }

        return Operation::END_OF_FILE;
    }

    //! @brief      parse_string
    bool parse_string(char **cursor, const char *buffer_end, String *value_out)
    {
        const char *begin = *cursor;
        while (*cursor != buffer_end && **cursor != '"')
            ++*cursor;

        if (**cursor != '"')
            return false;

        const char *end = *cursor;
        value_out->set(begin, (uint32_t)(end-begin));
        ++*cursor;

        return true;
    }

    //! @brief      parse_string_value
    bool parse_string_value(char **cursor, const char *buffer_end, StringValue *value_out)
    {
        const char *begin = *cursor;
        while (*cursor != buffer_end
            && **cursor != ' '
            && **cursor != '\t'
            && **cursor != '\r'
            && **cursor != '\n'
            && **cursor != ','
            && **cursor != '}'
            && **cursor != ']')
                ++*cursor;
        const char *end = *cursor;

        uint32_t value_length = (uint32_t)(end-begin);
        StringHelper::to_value(begin, value_length, value_out);

        return true;
    }

    //! @brief      parse_data
    bool parse_data(char **cursor, const char *buffer_end, DictionaryNode *node_out)
    {
        Operation operation = find_next_operation(cursor, buffer_end);
        switch (operation)
        {
            case Operation::STRING:
            {
                String value;
                if (!parse_string(cursor, buffer_end, &value))
                    return false;

                node_out->set_string(value.get_buffer(), value.get_length());
                break;
            }

            case Operation::VALUE:
            {
                --*cursor;

                StringValue value;
                if (!parse_string_value(cursor, buffer_end, &value))
                    return false;

                switch (value.DetectedType)
                {
                    case StringValue::Type::BOOLEAN:
                    {
                        node_out->set_bool(value.Boolean);
                        break;
                    }

                    case StringValue::Type::INTEGER:
                    {
                        node_out->set_int64(value.Integer);
                        break;
                    }

                    case StringValue::Type::FLOAT:
                    {
                        node_out->set_float64(value.Float);
                        break;
                    }

                    case StringValue::Type::STRING:
                    {
                        if (value.String.Length == 4
                            && value.String.Buffer[0] == 'n'
                            && value.String.Buffer[1] == 'u'
                            && value.String.Buffer[2] == 'l'
                            && value.String.Buffer[3] == 'l')
                        {
                            node_out->set_null();
                            break;
                        }
                        return false;
                    }
                }

                return true;
            }

            case Operation::BEGIN_OBJECT:
            {
                if (!parse_object(cursor, buffer_end, node_out))
                    return false;
                break;
            }

            case Operation::BEGIN_ARRAY:
            {
                if (!parse_array(cursor, buffer_end, node_out))
                    return false;
                break;
            }

            default:
                return false;
        }

        return true;
    }

    //! @brief      parse_object
    bool parse_object(char **cursor, const char *buffer_end, DictionaryNode *node_out)
    {
        Operation operation = find_next_operation(cursor, buffer_end);
        while (operation != Operation::END_OBJECT)
        {
            String name;
            if (!parse_string(cursor, buffer_end, &name))
                return false;

            operation = find_next_operation(cursor, buffer_end);
            if (operation != Operation::ASSIGN)
                return false;

            DictionaryNode member = node_out->add(name.get_buffer());
            if (!parse_data(cursor, buffer_end, &member))
                return false;

            operation = find_next_operation(cursor, buffer_end);
            if (operation == Operation::NEXT_MEMBER)
                operation = find_next_operation(cursor, buffer_end);
        }

        return true;
    }

    //! @brief      parse_array
    bool parse_array(char **cursor, const char *buffer_end, DictionaryNode *node_out)
    {
        Operation operation = find_next_operation(cursor, buffer_end);
        switch (operation)
        {
            case Operation::END_ARRAY:
            {
                node_out->set_array();
                return true;
            }

            // buffer
            case Operation::STRING:
            case Operation::VALUE:
            {
                --*cursor;

                StringValue value;
                if (!parse_string_value(cursor, buffer_end, &value))
                    return false;

                uint32_t index = 1;
                uint32_t capacity = 16;
                switch (value.DetectedType)
                {
                    case StringValue::Type::BOOLEAN:
                    {
                        bool *buffer = node_out->set_buffer_bool(capacity);
                        *buffer = value.Boolean;

                        Operation operation = find_next_operation(cursor, buffer_end);
                        while (operation == Operation::NEXT_MEMBER)
                        {
                            if (index >= capacity)
                            {
                                capacity = capacity << 1;
                                buffer = node_out->set_buffer_bool(capacity);
                            }

                            operation = find_next_operation(cursor, buffer_end);
                            if (operation != Operation::VALUE)
                                return false;

                            const char *begin = *cursor-1;
                            while (*cursor != buffer_end
                                && (**cursor == 't' || **cursor == 'r' || **cursor == 'u' || **cursor == 'e'
                                || **cursor == 'f' || **cursor == 'a' || **cursor == 'l' || **cursor == 's'))
                                ++*cursor;
                            const char *end = *cursor;

                            uint32_t value_length = (uint32_t)(end-begin);
                            StringHelper::to_value(begin, value_length, &value);
                            buffer[index++] = value.Boolean;

                            operation = find_next_operation(cursor, buffer_end);
                        }
                        node_out->set_buffer_bool(index);
                        break;
                    }

                    case StringValue::Type::INTEGER:
                    case StringValue::Type::FLOAT:
                    {
                        int64_t *buffer = node_out->set_buffer_int64(capacity);
                        *buffer = value.Integer;

                        StringValue value;
                        Operation operation = find_next_operation(cursor, buffer_end);
                        while (operation == Operation::NEXT_MEMBER)
                        {
                            if (index >= capacity)
                            {
                                capacity = capacity << 1;
                                buffer = node_out->set_buffer_int64(capacity);
                            }

                            operation = find_next_operation(cursor, buffer_end);
                            if (operation != Operation::VALUE)
                                return false;

                            const char *begin = *cursor-1;
                            while (*cursor != buffer_end
                                && ((**cursor >= '0' && **cursor <= '9')
                                || (**cursor == '.' || **cursor == '-' || **cursor == 'f' || **cursor == 'F' || **cursor == 'e' || **cursor == 'E')))
                                ++*cursor;
                            const char *end = *cursor;

                            uint32_t value_length = (uint32_t)(end-begin);
                            StringHelper::to_value(begin, value_length, &value);
                            buffer[index++] = value.Integer;

                            operation = find_next_operation(cursor, buffer_end);
                        }

                        if (value.DetectedType == StringValue::Type::INTEGER)
                            node_out->set_buffer_int64(index);
                        else
                            node_out->set_buffer_float64(index);
                        break;
                    }

                    case StringValue::Type::STRING:
                    {
                        *cursor -= value.String.Length-1;

                        Vector<String> strings(1);
                        if (!parse_string(cursor, buffer_end, &strings[0]))
                            return false;

                        Operation operation = find_next_operation(cursor, buffer_end);
                        while (operation == Operation::NEXT_MEMBER)
                        {
                            operation = find_next_operation(cursor, buffer_end);
                            if (operation != Operation::STRING)
                                return false;

                            if (!parse_string(cursor, buffer_end, &strings.add()))
                                return false;

                            operation = find_next_operation(cursor, buffer_end);
                        }
                        node_out->set_buffer_string(&strings[0], strings.get_size());
                        break;
                    }

                    default:
                        return false;
                }
                break;
            }

            default:
            {
                --*cursor;

                DictionaryNode child = node_out->add();
                if (!parse_data(cursor, buffer_end, &child))
                    return false;

                Operation operation = find_next_operation(cursor, buffer_end);
                while (operation != Operation::END_ARRAY)
                {
                    child = node_out->add();
                    if (!parse_data(cursor, buffer_end, &child))
                        return false;

                    operation = find_next_operation(cursor, buffer_end);
                }

                break;
            }
        }

        return true;
    }

    //! @brief      read
    bool DictionaryJsonStream::read(StreamReader *stream, DictionaryNode *node)
    {
        const uint32_t buffer_size = stream->get_size();
        Vector<char> buffer_array(buffer_size);
        char *buffer = (char*)&buffer_array[0];
        if (!stream->read(buffer, buffer_size))
            return false;

        const char *buffer_end = buffer + buffer_size;
        char *cursor = (char*)buffer;
        Operation operation = find_next_operation(&cursor, buffer_end);
        switch (operation)
        {
            case Operation::BEGIN_OBJECT:
            {
                if (!parse_object(&cursor, buffer_end, node))
                    return false;
                break;
            }

            case Operation::BEGIN_ARRAY:
            {
                if (!parse_array(&cursor, buffer_end, node))
                    return false;
                break;
            }

            default:
                return false;
        }

        return true;
    }

    //! @brief      write_string
    bool write_operation(StreamWriter *stream, Operation operation, uint32_t level, uint32_t flags)
    {
        if (flags & (uint32_t)DictionaryJsonStream::Flag::PRETTY)
        {
            switch (operation)
            {
                case Operation::BEGIN_OBJECT:
                {
                    if (!stream->write("{\n", 2))
                        return false;
                    break;
                }

                case Operation::END_OBJECT:
                {
                    if (!stream->write('\n'))
                        return false;

                    for (int32_t i = 0; i < (int32_t)level; ++i)
                    {
                        if (!stream->write("    ", 4))
                            return false;
                    }

                    if (!stream->write('}'))
                        return false;
                    break;
                }

                case Operation::BEGIN_ARRAY:
                {
                    if (!stream->write("[\n", 2))
                        return false;
                    break;
                }

                case Operation::END_ARRAY:
                {
                    if (!stream->write('\n'))
                        return false;

                    for (int32_t i = 0; i < (int32_t)level; ++i)
                    {
                        if (!stream->write("    ", 4))
                            return false;
                    }

                    if (!stream->write(']'))
                        return false;
                    break;
                }

                case Operation::ASSIGN:
                {
                    if (!stream->write(": ", 2))
                        return false;
                    break;
                }

                case Operation::NEXT_MEMBER:
                {
                    if (!stream->write(",\n", 2))
                        return false;
                    break;
                }

                case Operation::INDENT:
                {
                    for (uint32_t i = 0; i < level; ++i)
                    {
                        if (!stream->write("    ", 4))
                            return false;
                    }
                    break;
                }

                default:
                    break;
            }
        }
        else
        {
            switch (operation)
            {
                case Operation::BEGIN_OBJECT:
                {
                    if (!stream->write('{'))
                        return false;
                    break;
                }

                case Operation::END_OBJECT:
                {
                    if (!stream->write('}'))
                        return false;
                    break;
                }

                case Operation::BEGIN_ARRAY:
                {
                    if (!stream->write('['))
                        return false;
                    break;
                }

                case Operation::END_ARRAY:
                {
                    if (!stream->write(']'))
                        return false;
                    break;
                }

                case Operation::ASSIGN:
                {
                    if (!stream->write(':'))
                        return false;
                    break;
                }

                case Operation::NEXT_MEMBER:
                {
                    if (!stream->write(','))
                        return false;
                    break;
                }

                default:
                    break;
            }
        }

        return true;
    }

    //! @brief      write_buffer
    bool write_buffer(StreamWriter *stream, DictionaryData::Format format, const uint8_t *buffer, uint32_t buffer_count, uint32_t level, uint32_t flags)
    {
        DictionaryHelperStream::WriteCallback write_callback = DictionaryHelperStream::get_write_callback(format);

        if (!write_operation(stream, Operation::INDENT, level, flags))
                return false;

        uint32_t offset = 0;
        if (!write_callback(stream, buffer, &offset))
            return false;

        for (uint32_t i = 1; i < buffer_count; ++i)
        {
            if (!write_operation(stream, Operation::NEXT_MEMBER, level, flags))
                return false;

            if (!write_operation(stream, Operation::INDENT, level, flags))
                return false;

            if (!write_callback(stream, buffer, &offset))
                return false;
        }

        return true;
    }

    //! @brief      write_children
    bool write_children(StreamWriter *stream, DictionaryNode *node, uint32_t level, uint32_t flags)
    {
        DictionaryNode child = node->get_first_child();
        if (child.is_valid())
        {
            if (!write_struct(stream, &child, level, flags))
                return false;

            ++child;
            while(child.is_valid())
            {
                if (!write_operation(stream, Operation::NEXT_MEMBER, level, flags))
                    return false;

                if (!write_struct(stream, &child, level, flags))
                    return false;
                ++child;
            }
        }

        return true;
    }

    //! @brief      write_struct
    bool write_struct(StreamWriter *stream, DictionaryNode *node, uint32_t level, uint32_t flags)
    {
        if (!write_operation(stream, Operation::INDENT, level, flags))
            return false;

        const char *name = node->get_name();
        if (name)
        {
            if (!DictionaryHelperStream::write(stream, name))
                return false;

            if (!write_operation(stream, Operation::ASSIGN, level, flags))
                return false;
        }

        switch (node->get_format())
        {
            case DictionaryData::Format::ZERO:
            {
                if (!stream->write("null", 4))
                    return false;
                break;
            }

            case DictionaryData::Format::BOOLEAN:
            {
                if (!DictionaryHelperStream::write(stream, node->get_bool()))
                    return false;
                break;
            }

            case DictionaryData::Format::UINTEGER8:
            {
                if (!DictionaryHelperStream::write(stream, node->get_uint8()))
                    return false;
                break;
            }

            case DictionaryData::Format::UINTEGER16:
            {
                if (!DictionaryHelperStream::write(stream, node->get_uint16()))
                    return false;
                break;
            }

            case DictionaryData::Format::UINTEGER32:
            {
                if (!DictionaryHelperStream::write(stream, node->get_uint32()))
                    return false;
                break;
            }

            case DictionaryData::Format::UINTEGER64:
            {
                if (!DictionaryHelperStream::write(stream, node->get_uint64()))
                    return false;
                break;
            }

            case DictionaryData::Format::INTEGER8:
            {
                if (!DictionaryHelperStream::write(stream, node->get_int8()))
                    return false;
                break;
            }

            case DictionaryData::Format::INTEGER16:
            {
                if (!DictionaryHelperStream::write(stream, node->get_int16()))
                    return false;
                break;
            }

            case DictionaryData::Format::INTEGER32:
            {
                if (!DictionaryHelperStream::write(stream, node->get_int32()))
                    return false;
                break;
            }

            case DictionaryData::Format::INTEGER64:
            {
                if (!DictionaryHelperStream::write(stream, node->get_int64()))
                    return false;
                break;
            }

            case DictionaryData::Format::FLOAT32:
            {
                if (!DictionaryHelperStream::write(stream, node->get_float32()))
                    return false;
                break;
            }

            case DictionaryData::Format::FLOAT64:
            {
                if (!DictionaryHelperStream::write(stream, node->get_float64()))
                    return false;
                break;
            }

            case DictionaryData::Format::STRING:
            {
                if (!DictionaryHelperStream::write(stream, node->get_string()))
                    return false;
                break;
            }

            case DictionaryData::Format::OBJECT:
            {
                if (node->is_empty())
                {
                    if (!stream->write("{}",2))
                        return false;
                    break;
                }

                if (!write_operation(stream, Operation::BEGIN_OBJECT, level, flags))
                    return false;

                if (!write_children(stream, node, level+1, flags))
                    return false;

                if (!write_operation(stream, Operation::END_OBJECT, level, flags))
                    return false;
                break;
            }

            case DictionaryData::Format::ARRAY:
            {
                if (node->is_empty())
                {
                    if (!stream->write("[]",2))
                        return false;

                    return true;
                }

                if (!write_operation(stream, Operation::BEGIN_ARRAY, level, flags))
                    return false;

                if (!write_children(stream, node, level+1, flags))
                    return false;

                if (!write_operation(stream, Operation::END_ARRAY, level, flags))
                    return false;
                break;
            }

            case DictionaryData::Format::BUFFER:
            {
                uint32_t count;
                const uint8_t *buffer = nullptr;
                node->get_buffer_uint8(&buffer, &count);

                if (count == 0)
                {
                    if (!stream->write("[]",2))
                        return false;

                    break;
                }

                if (!write_operation(stream, Operation::BEGIN_ARRAY, level, flags))
                    return false;

                if (!write_buffer(stream, node->get_buffer_format(), buffer, count, level+1, flags))
                    return false;

                if (!write_operation(stream, Operation::END_ARRAY, level, flags))
                    return false;

                break;
            }
        }

        return true;
    }


    //! @brief      write
    bool DictionaryJsonStream::write(StreamWriter *stream, DictionaryNode *node, uint32_t flags)
    {
        return write_struct(stream, node, 0, flags);
    }
}
