/******************************************************************************
**  Includes
******************************************************************************/
#include "dictionary_csv_stream.hpp"
#include "dictionary_helper_stream.hpp"
#include "../../container/dictionary.hpp"
#include "../../string/string.hpp"
#include "../../string/string_helper.hpp"

/******************************************************************************
**  Class Definition
******************************************************************************/
namespace Gorilla
{
    struct Column
    {
        String Name;
        StringValue::Type DetectedType;
        Vector<uint8_t> Buffer;
    };

    enum class Operation
    {
        SEPARATOR = 0,
        STRING,
        VALUE,
        END_OF_LINE,
        END_OF_FILE,
    };

    //! @brief      find_next_operation
    static Operation find_next_operation(char **cursor, const char *buffer_end, bool skip_value)
    {
        while (*cursor != buffer_end)
        {
            char value = **cursor;
            switch (value)
            {
                case '"':
                    return Operation::STRING;
                case ';':
                    return Operation::SEPARATOR;
                case '\n':
                    return Operation::END_OF_LINE;
                case ' ':
                case '\t':
                case '\r':
                    break;
                default:
                {
                    if (skip_value)
                        break;
                    return Operation::VALUE;
                }
            }
            ++*cursor;
        }

        --*cursor;
        return Operation::END_OF_FILE;
    }

    //! @brief      read_line
    bool read_line(char **cursor, const char *buffer_end, StringValue *values_out)
    {
        uint32_t quote_count = 0;
        uint32_t column_index = 0;
        char *cursor_start = *cursor;
        Operation operation = Operation::SEPARATOR;
        while (operation != Operation::END_OF_LINE)
        {
            operation = find_next_operation(cursor, buffer_end, true);
            switch (operation)
            {
                case Operation::END_OF_FILE:
                {
                    // no break
                    operation = Operation::END_OF_LINE;
                }
                case Operation::END_OF_LINE:
                {
                    // handle just endline without finishing column
                    if (quote_count != 0)
                    {
                        ++*cursor;
                        operation = find_next_operation(cursor, buffer_end, true);
                    }
                }
                case Operation::SEPARATOR:
                {
                    // trim
                    find_next_operation(&cursor_start, buffer_end, false);

                    char *value_start = cursor_start;
                    uint32_t value_length = (uint32_t)(*cursor-cursor_start);
                    if (quote_count)
                    {
                        if (quote_count % 2 != 0)
                        {
                            ++*cursor;
                            break;
                        }

                        quote_count = 0;
                        cursor_start = ++*cursor;
                        StringHelper::to_value(++value_start, value_length-2, &values_out[column_index++]);
                        break;
                    }

                    cursor_start = ++*cursor;
                    StringHelper::to_value(value_start, value_length, &values_out[column_index++]);

                    break;
                }

                case Operation::STRING:
                {
                    ++quote_count;
                    ++*cursor;
                    break;
                }

                default:
                    return false;
            }
        }

        return true;
    }

    //! @brief      read
    bool DictionaryCsvStream::read(StreamReader *stream, DictionaryNode *node)
    {
        const uint32_t buffer_size = stream->get_size();
        Vector<char> buffer_array(buffer_size);
        char *buffer = (char*)&buffer_array[0];
        if (!stream->read(buffer, buffer_size))
            return false;

        const char *buffer_end = buffer + buffer_size;
        char *cursor_start = (char*)buffer;
        char *cursor = (char*)buffer;

        Vector<Column> columns;
        Operation operation = find_next_operation(&cursor, buffer_end, false);
        while (operation != Operation::END_OF_LINE && operation != Operation::END_OF_FILE)
        {
            cursor_start = cursor;
            operation = find_next_operation(&cursor, buffer_end, true);
            switch (operation)
            {
                case Operation::END_OF_FILE:
                case Operation::END_OF_LINE:
                case Operation::SEPARATOR:
                {
                    uint32_t value_length = (uint32_t)(cursor-cursor_start);
                    ++cursor;

                    Column& column = columns.add();
                    column.Name.set(cursor_start, value_length);
                    break;
                }

                default:
                    return false;
            }
        }

        const uint32_t column_count = columns.get_size();
        Vector<StringValue> values(column_count);
        if (!read_line(&cursor, buffer_end, &values[0]))
            return false;

        for (uint32_t i = 0; i < column_count; ++i)
            columns[i].DetectedType = values[i].DetectedType;

        uint32_t row_index = 0;
        operation = Operation::END_OF_LINE;
        while (operation != Operation::END_OF_FILE)
        {
            if (row_index > 0)
            {
                if (!read_line(&cursor, buffer_end, &values[0]))
                    return false;
            }

            for (uint32_t i = 0; i < column_count; ++i)
            {
                Vector<uint8_t>& buffer = columns[i].Buffer;
                StringValue& value = values[i];
                switch (value.DetectedType)
                {
                    case StringValue::Type::BOOLEAN:
                    {
                        buffer.add((uint8_t)value.Boolean);
                        break;
                    }

                    case StringValue::Type::FLOAT:
                    case StringValue::Type::INTEGER:
                    {
                        uint32_t offset = buffer.get_size();
                        buffer.expand(sizeof(int64_t));
                        *((int64_t*)&buffer[offset]) = value.Integer;
                        break;
                    }

                    case StringValue::Type::STRING:
                    {
                        uint32_t offset = buffer.get_size();
                        buffer.expand(sizeof(StringReference));
                        *((StringReference*)&buffer[offset]) = value.String;
                        break;
                    }
                }
            }

            ++row_index;
            operation = find_next_operation(&cursor, buffer_end, false);
        }

        for (uint32_t i = 0; i < column_count; ++i)
        {
            Column& column = columns[i];
            switch (column.DetectedType)
            {
                case StringValue::Type::BOOLEAN:
                {
                    node->add(column.Name.get_buffer()).set_buffer_bool((const bool*)&column.Buffer[0], column.Buffer.get_size());
                    break;
                }

                case StringValue::Type::INTEGER:
                {
                    node->add(column.Name.get_buffer()).set_buffer_int64((const int64_t*)&column.Buffer[0], column.Buffer.get_size() / sizeof(int64_t));
                    break;
                }

                case StringValue::Type::FLOAT:
                {
                    node->add(column.Name.get_buffer()).set_buffer_float64((const double*)&column.Buffer[0], column.Buffer.get_size() / sizeof(double));
                    break;
                }

                case StringValue::Type::STRING:
                {
                    const StringReference *string_references = (const StringReference*)&column.Buffer[0];
                    const uint32_t element_count = column.Buffer.get_size() / sizeof(StringReference);
                    Vector<String> strings(element_count);
                    for (uint32_t i = 0; i < element_count; ++i)
                    {
                        const StringReference *string_reference = &string_references[i];
                        if (string_reference->Length)
                            strings[i].set(string_reference->Buffer, string_reference->Length);
                    }

                    node->add(column.Name.get_buffer()).set_buffer_string((const String*)&strings[0], element_count);
                    break;
                }
            }
        }

        return true;
    }

    //! @brief      write_data
    bool write_data(StreamWriter *stream, DictionaryNode *node, uint32_t flags)
    {
        switch(node->get_format())
        {
            case DictionaryData::Format::ZERO:
            {
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

            default:
                return false;
        }

        return true;
    }

    //! @brief      write
    bool DictionaryCsvStream::write(StreamWriter *stream, DictionaryNode *node, uint32_t flags)
    {
        if (!node->is_object())
            return false;

        DictionaryNode child = node->get_first_child();
        if (!child.is_valid())
            return false;

        // column names
        uint32_t column_count = 0;
        while (child.is_valid())
        {
            const char *name = child.get_name();
            if (!stream->write(name, (uint32_t)STRLEN(name)))
                return false;

            if (!stream->write(';'))
                return false;

            ++column_count;
            ++child;
        }
        if (!stream->seek(stream->get_position()-1))
            return false;

        if (!stream->write('\n'))
            return false;

        uint32_t row_index = 0, column_index = 0;
        uint32_t column_finished_count = 0;

        Vector<uint32_t> buffer_offsets;
        buffer_offsets.resize(column_count, 0);
        Vector<DictionaryNode> ancestors(column_count);
        while(column_finished_count != column_count)
        {
            column_finished_count = 0;
            child = node->get_first_child();
            while (child.is_valid())
            {
                switch(child.get_format())
                {
                    case DictionaryData::Format::ARRAY:
                    {
                        DictionaryNode *it = &ancestors[column_index];
                        if (!it->is_valid())
                            *it = child.get_first_child();
                        else
                            it->next();

                        if (!write_data(stream, it, flags))
                            return false;

                        if (!it->get_next().is_valid())
                            ++column_finished_count;

                        break;
                    }

                    case DictionaryData::Format::BUFFER:
                    {
                        uint32_t count;
                        const uint8_t *buffer = nullptr;
                        child.get_buffer_uint8(&buffer, &count);

                        DictionaryHelperStream::WriteCallback write_callback = DictionaryHelperStream::get_write_callback(child.get_buffer_format());
                        if (!write_callback(stream, buffer, &buffer_offsets[column_index]))
                            return false;

                        if (row_index >= count-1)
                        {
                            ++column_finished_count;
                            break;
                        }

                        break;
                    }

                    default:
                    {
                        if (!write_data(stream, &child, flags))
                            return false;

                        ++column_finished_count;
                        break;
                    }
                }

                ++column_index;
                ++child;
                if (column_index != column_count && !stream->write(';'))
                    return false;
            }
            column_index = 0;
            ++row_index;

            if (!stream->write('\n'))
                return false;
        }

        return true;
    }
}
