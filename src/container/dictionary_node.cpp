/******************************************************************************
**  Includes
******************************************************************************/
#include "dictionary_node.hpp"
#include "dictionary.hpp"
#include "../stream/file_reader.hpp"
#include "../stream/file_writer.hpp"

/******************************************************************************
**  Class Definition
******************************************************************************/
namespace Gorilla
{
    const DictionaryId DictionaryId::START = { 0, 0 };
    const DictionaryId DictionaryId::INVALID = { 0xFFFF, 0xFFFF };
    const DictionaryNode DictionaryNode::INVALID = DictionaryNode();

    //! @brief      constructor
    DictionaryNode::DictionaryNode(Dictionary *dictionary, DictionaryData *data)
        : m_dictionary(dictionary)
        , m_data(data)
    {
        // nothing to do
    }

    //! @brief      constructor
    DictionaryNode::DictionaryNode()
        : m_dictionary(nullptr)
        , m_data(nullptr)
    {
        // nothing to do
    }

    //! @brief      destructor
    DictionaryNode::~DictionaryNode()
    {
        // nothing to do
    }

    //! @brief      get_name
    const char*	DictionaryNode::get_name() const
    {
        return m_dictionary->get_data_name_internal(m_data);
    }

    //! @brief      next
    bool DictionaryNode::next()
    {
        DictionaryId next_id = m_data->get_next_id();
        if (next_id == DictionaryId::INVALID)
        {
            m_data = nullptr;
            return false;
        }

        m_data = m_dictionary->get_data_internal(next_id);
        return is_valid();
    }

    //! @brief      get_next
    DictionaryNode DictionaryNode::get_next() const
    {
        DictionaryId next_id = m_data->get_next_id();
        if (next_id == DictionaryId::INVALID)
            return DictionaryNode::INVALID;

        DictionaryData *next_data = m_dictionary->get_data_internal(next_id);
        return DictionaryNode(m_dictionary, next_data);
    }

    //! @brief      get_first_child
    DictionaryNode DictionaryNode::get_first_child() const
    {
        TDictionaryData<DictionaryData::Format::OBJECT> *data =  reinterpret_cast<TDictionaryData<DictionaryData::Format::OBJECT>*>(m_data);
        if (data->FirstChildId == DictionaryId::INVALID)
            return DictionaryNode::INVALID;

        DictionaryData *first_data = m_dictionary->get_data_internal(data->FirstChildId);
        return DictionaryNode(m_dictionary, first_data);
    }

    //! @brief      get_last_child
    DictionaryNode DictionaryNode::get_last_child() const
    {
        TDictionaryData<DictionaryData::Format::OBJECT> *data =  reinterpret_cast<TDictionaryData<DictionaryData::Format::OBJECT>*>(m_data);
        if (data->LastChildId == DictionaryId::INVALID)
            return DictionaryNode::INVALID;

        DictionaryData *first_data = m_dictionary->get_data_internal(data->LastChildId);
        return DictionaryNode(m_dictionary, first_data);
    }

    //! @brief      find
    DictionaryNode DictionaryNode::find(const char *name) const
    {
        return m_dictionary->get_node_internal(m_data, name);
    }

    //! @brief      add
    DictionaryNode DictionaryNode::add(const char *name /*= nullptr*/)
    {
        return m_dictionary->add_node_internal(m_data, name);
    }

    //! @brief      remove
    void DictionaryNode::remove(const char *name)
    {
        m_dictionary->remove_node_internal(m_data, name);
    }

    //! @brief      set_bool
    void DictionaryNode::set_bool(bool value)
    {
        DictionaryData::Format current_format = m_data->get_format();
        if (current_format != DictionaryData::Format::BOOLEAN)
        {
            uint32_t current_size = m_dictionary->get_data_size_internal(m_data);
            if (current_size != sizeof(TDictionaryData<DictionaryData::Format::BOOLEAN>))
                m_data = m_dictionary->reallocate_data_internal(m_data, sizeof(TDictionaryData<DictionaryData::Format::BOOLEAN>));

            m_data->set_format(DictionaryData::Format::BOOLEAN);
        }

        reinterpret_cast<TDictionaryData<DictionaryData::Format::BOOLEAN>*>(m_data)->Value = value;
    }

    //! @brief      set_uint8_t
    void DictionaryNode::set_uint8(uint8_t value)
    {
        DictionaryData::Format current_format = m_data->get_format();
        if (current_format != DictionaryData::Format::UINTEGER8)
        {
            uint32_t current_size = m_dictionary->get_data_size_internal(m_data);
            if (current_size != sizeof(TDictionaryData<DictionaryData::Format::UINTEGER8>))
                m_data = m_dictionary->reallocate_data_internal(m_data, sizeof(TDictionaryData<DictionaryData::Format::UINTEGER8>));

            m_data->set_format(DictionaryData::Format::UINTEGER8);
        }

        reinterpret_cast<TDictionaryData<DictionaryData::Format::UINTEGER8>*>(m_data)->Value = value;
    }

    //! @brief      set_uint16_t
    void DictionaryNode::set_uint16(uint16_t value)
    {
        DictionaryData::Format current_format = m_data->get_format();
        if (current_format != DictionaryData::Format::UINTEGER16)
        {
            uint32_t current_size = m_dictionary->get_data_size_internal(m_data);
            if (current_size != sizeof(TDictionaryData<DictionaryData::Format::UINTEGER16>))
                m_data = m_dictionary->reallocate_data_internal(m_data, sizeof(TDictionaryData<DictionaryData::Format::UINTEGER16>));

            m_data->set_format(DictionaryData::Format::UINTEGER16);
        }

        reinterpret_cast<TDictionaryData<DictionaryData::Format::UINTEGER16>*>(m_data)->Value = value;
    }

    //! @brief      set_uint32_t
    void DictionaryNode::set_uint32(uint32_t value)
    {
        DictionaryData::Format current_format = m_data->get_format();
        if (current_format != DictionaryData::Format::UINTEGER32)
        {
            uint32_t current_size = m_dictionary->get_data_size_internal(m_data);
            if (current_size != sizeof(TDictionaryData<DictionaryData::Format::UINTEGER32>))
                m_data = m_dictionary->reallocate_data_internal(m_data, sizeof(TDictionaryData<DictionaryData::Format::UINTEGER32>));

            m_data->set_format(DictionaryData::Format::UINTEGER32);
        }

        reinterpret_cast<TDictionaryData<DictionaryData::Format::UINTEGER32>*>(m_data)->Value = value;
    }

    //! @brief      set_uint64_t
    void DictionaryNode::set_uint64(uint64_t value)
    {
        DictionaryData::Format current_format = m_data->get_format();
        if (current_format != DictionaryData::Format::UINTEGER64)
        {
            uint32_t current_size = m_dictionary->get_data_size_internal(m_data);
            if (current_size != sizeof(TDictionaryData<DictionaryData::Format::UINTEGER64>))
                m_data = m_dictionary->reallocate_data_internal(m_data, sizeof(TDictionaryData<DictionaryData::Format::UINTEGER64>));

            m_data->set_format(DictionaryData::Format::UINTEGER64);
        }

        reinterpret_cast<TDictionaryData<DictionaryData::Format::UINTEGER64>*>(m_data)->Value = value;
    }

    //! @brief      set_int8
    void DictionaryNode::set_int8(int8_t value)
    {
        DictionaryData::Format current_format = m_data->get_format();
        if (current_format != DictionaryData::Format::INTEGER8)
        {
            uint32_t current_size = m_dictionary->get_data_size_internal(m_data);
            if (current_size != sizeof(TDictionaryData<DictionaryData::Format::INTEGER8>))
                m_data = m_dictionary->reallocate_data_internal(m_data, sizeof(TDictionaryData<DictionaryData::Format::INTEGER8>));

            m_data->set_format(DictionaryData::Format::INTEGER8);
        }

        reinterpret_cast<TDictionaryData<DictionaryData::Format::INTEGER8>*>(m_data)->Value = value;
    }

    //! @brief      set_int16
    void DictionaryNode::set_int16(int16_t value)
    {
        DictionaryData::Format current_format = m_data->get_format();
        if (current_format != DictionaryData::Format::INTEGER16)
        {
            uint32_t current_size = m_dictionary->get_data_size_internal(m_data);
            if (current_size != sizeof(TDictionaryData<DictionaryData::Format::INTEGER16>))
                m_data = m_dictionary->reallocate_data_internal(m_data, sizeof(TDictionaryData<DictionaryData::Format::INTEGER16>));

            m_data->set_format(DictionaryData::Format::INTEGER16);
        }

        reinterpret_cast<TDictionaryData<DictionaryData::Format::INTEGER16>*>(m_data)->Value = value;
    }

    //! @brief      set_int32
    void DictionaryNode::set_int32(int32_t value)
    {
        DictionaryData::Format current_format = m_data->get_format();
        if (current_format != DictionaryData::Format::INTEGER32)
        {
            uint32_t current_size = m_dictionary->get_data_size_internal(m_data);
            if (current_size != sizeof(TDictionaryData<DictionaryData::Format::INTEGER32>))
                m_data = m_dictionary->reallocate_data_internal(m_data, sizeof(TDictionaryData<DictionaryData::Format::INTEGER32>));

            m_data->set_format(DictionaryData::Format::INTEGER32);
        }

        reinterpret_cast<TDictionaryData<DictionaryData::Format::INTEGER32>*>(m_data)->Value = value;
    }

    //! @brief      set_int64
    void DictionaryNode::set_int64(int64_t value)
    {
        DictionaryData::Format current_format = m_data->get_format();
        if (current_format != DictionaryData::Format::INTEGER64)
        {
            uint32_t current_size = m_dictionary->get_data_size_internal(m_data);
            if (current_size != sizeof(TDictionaryData<DictionaryData::Format::INTEGER64>))
                m_data = m_dictionary->reallocate_data_internal(m_data, sizeof(TDictionaryData<DictionaryData::Format::INTEGER64>));

            m_data->set_format(DictionaryData::Format::INTEGER64);
        }

        reinterpret_cast<TDictionaryData<DictionaryData::Format::INTEGER64>*>(m_data)->Value = value;
    }

    //! @brief      set_float32
    void DictionaryNode::set_float32(float value)
    {
        DictionaryData::Format current_format = m_data->get_format();
        if (current_format != DictionaryData::Format::FLOAT32)
        {
            uint32_t current_size = m_dictionary->get_data_size_internal(m_data);
            if (current_size != sizeof(TDictionaryData<DictionaryData::Format::FLOAT32>))
                m_data = m_dictionary->reallocate_data_internal(m_data, sizeof(TDictionaryData<DictionaryData::Format::FLOAT32>));

            m_data->set_format(DictionaryData::Format::FLOAT32);
        }

        reinterpret_cast<TDictionaryData<DictionaryData::Format::FLOAT32>*>(m_data)->Value = value;
    }

    //! @brief      set_float64
    void DictionaryNode::set_float64(double value)
    {
        DictionaryData::Format current_format = m_data->get_format();
        if (current_format != DictionaryData::Format::FLOAT64)
        {
            uint32_t current_size = m_dictionary->get_data_size_internal(m_data);
            if (current_size != sizeof(TDictionaryData<DictionaryData::Format::FLOAT64>))
                m_data = m_dictionary->reallocate_data_internal(m_data, sizeof(TDictionaryData<DictionaryData::Format::FLOAT64>));

            m_data->set_format(DictionaryData::Format::FLOAT64);
        }

        reinterpret_cast<TDictionaryData<DictionaryData::Format::FLOAT64>*>(m_data)->Value = value;
    }

    //! @brief      set_string
    void DictionaryNode::set_string(const char *value, uint32_t length)
    {
        uint32_t current_size = 0;
        uint32_t string_length = length + 1;
        uint32_t string_size = sizeof(TDictionaryData<DictionaryData::Format::STRING>) + string_length;
        DictionaryData::Format current_format = m_data->get_format();
        if (current_format != DictionaryData::Format::STRING)
            goto reallocate;

        current_size = m_dictionary->get_data_size_internal(m_data);
        if (current_size != string_size)
            goto reallocate;

        goto modify;

    reallocate:
        m_data = m_dictionary->reallocate_data_internal(m_data, string_size);
        m_data->set_format(DictionaryData::Format::STRING);

    modify:
        TDictionaryData<DictionaryData::Format::STRING> *data = reinterpret_cast<TDictionaryData<DictionaryData::Format::STRING>*>(m_data);
        data->Length = length;
        char *buffer = ((char*)data) + sizeof(TDictionaryData<DictionaryData::Format::STRING>);
        MEMCPY(buffer, value, length);
        buffer[length] = 0;
    }

    //! @brief      set_object
    void DictionaryNode::set_object()
    {
        DictionaryData::Format current_format = m_data->get_format();
        if (current_format != DictionaryData::Format::OBJECT)
        {
            uint32_t current_size = m_dictionary->get_data_size_internal(m_data);
            if (current_size != sizeof(TDictionaryData<DictionaryData::Format::OBJECT>))
                m_data = m_dictionary->reallocate_data_internal(m_data, sizeof(TDictionaryData<DictionaryData::Format::OBJECT>));

            TDictionaryData<DictionaryData::Format::OBJECT> *data = reinterpret_cast<TDictionaryData<DictionaryData::Format::OBJECT>*>(m_data);
            data->set_format(DictionaryData::Format::OBJECT);
            data->FirstChildId = DictionaryId::INVALID;
            data->LastChildId = DictionaryId::INVALID;
        }
    }

    //! @brief      set_array
    void DictionaryNode::set_array()
    {
        DictionaryData::Format current_format = m_data->get_format();
        if (current_format != DictionaryData::Format::ARRAY)
        {
            uint32_t current_size = m_dictionary->get_data_size_internal(m_data);
            if (current_size != sizeof(TDictionaryData<DictionaryData::Format::ARRAY>))
                m_data = m_dictionary->reallocate_data_internal(m_data, sizeof(TDictionaryData<DictionaryData::Format::ARRAY>));

            TDictionaryData<DictionaryData::Format::ARRAY> *data = reinterpret_cast<TDictionaryData<DictionaryData::Format::ARRAY>*>(m_data);
            data->set_format(DictionaryData::Format::ARRAY);
            data->FirstChildId = DictionaryId::INVALID;
            data->LastChildId = DictionaryId::INVALID;
        }
    }

    //! @brief      set_null
    void DictionaryNode::set_null()
    {
        DictionaryData::Format current_format = m_data->get_format();
        if (current_format != DictionaryData::Format::ZERO)
        {
            uint32_t current_size = m_dictionary->get_data_size_internal(m_data);
            if (current_size != sizeof(DictionaryData))
                m_data = m_dictionary->reallocate_data_internal(m_data, sizeof(DictionaryData));

            m_data->set_format(DictionaryData::Format::ZERO);
        }
    }

    //! @brief      get_buffer_bool
    void DictionaryNode::get_buffer_bool(const bool **buffer, uint32_t *count) const
    {
        TDictionaryData<DictionaryData::Format::BUFFER> *data = reinterpret_cast<TDictionaryData<DictionaryData::Format::BUFFER>*>(m_data);
        *buffer = (const bool*)((uint8_t*)data + sizeof(TDictionaryData<DictionaryData::Format::BUFFER>));
        *count = data->Count;
    }

    //! @brief      get_buffer_uint8_t
    void DictionaryNode::get_buffer_uint8(const uint8_t **buffer, uint32_t *count) const
    {
        TDictionaryData<DictionaryData::Format::BUFFER> *data = reinterpret_cast<TDictionaryData<DictionaryData::Format::BUFFER>*>(m_data);
        *buffer = (const uint8_t*)((uint8_t*)data + sizeof(TDictionaryData<DictionaryData::Format::BUFFER>));
        *count = data->Count;
    }

    //! @brief      get_buffer_uint16_t
    void DictionaryNode::get_buffer_uint16(const uint16_t **buffer, uint32_t *count) const
    {
        TDictionaryData<DictionaryData::Format::BUFFER> *data = reinterpret_cast<TDictionaryData<DictionaryData::Format::BUFFER>*>(m_data);
        *buffer = (const uint16_t*)((uint8_t*)data + sizeof(TDictionaryData<DictionaryData::Format::BUFFER>));
        *count = data->Count;
    }

    //! @brief      get_buffer_uint32_t
    void DictionaryNode::get_buffer_uint32(const uint32_t **buffer, uint32_t *count) const
    {
        TDictionaryData<DictionaryData::Format::BUFFER> *data = reinterpret_cast<TDictionaryData<DictionaryData::Format::BUFFER>*>(m_data);
        *buffer = (const uint32_t*)((uint8_t*)data + sizeof(TDictionaryData<DictionaryData::Format::BUFFER>));
        *count = data->Count;
    }

    //! @brief      get_buffer_uint64_t
    void DictionaryNode::get_buffer_uint64(const uint64_t **buffer, uint32_t *count) const
    {
        TDictionaryData<DictionaryData::Format::BUFFER> *data = reinterpret_cast<TDictionaryData<DictionaryData::Format::BUFFER>*>(m_data);
        *buffer = (const uint64_t*)((uint8_t*)data + sizeof(TDictionaryData<DictionaryData::Format::BUFFER>));
        *count = data->Count;
    }

    //! @brief      get_buffer_int8
    void DictionaryNode::get_buffer_int8(const int8_t **buffer, uint32_t *count) const
    {
        TDictionaryData<DictionaryData::Format::BUFFER> *data = reinterpret_cast<TDictionaryData<DictionaryData::Format::BUFFER>*>(m_data);
        *buffer = (const int8_t*)((uint8_t*)data + sizeof(TDictionaryData<DictionaryData::Format::BUFFER>));
        *count = data->Count;
    }

    //! @brief      get_buffer_int16
    void DictionaryNode::get_buffer_int16(const int16_t **buffer, uint32_t *count) const
    {
        TDictionaryData<DictionaryData::Format::BUFFER> *data = reinterpret_cast<TDictionaryData<DictionaryData::Format::BUFFER>*>(m_data);
        *buffer = (const int16_t*)((uint8_t*)data + sizeof(TDictionaryData<DictionaryData::Format::BUFFER>));
        *count = data->Count;
    }

    //! @brief      get_buffer_int32
    void DictionaryNode::get_buffer_int32(const int32_t **buffer, uint32_t *count) const
    {
        TDictionaryData<DictionaryData::Format::BUFFER> *data = reinterpret_cast<TDictionaryData<DictionaryData::Format::BUFFER>*>(m_data);
        *buffer = (const int32_t*)((uint8_t*)data + sizeof(TDictionaryData<DictionaryData::Format::BUFFER>));
        *count = data->Count;
    }

    //! @brief      get_buffer_int64
    void DictionaryNode::get_buffer_int64(const int64_t **buffer, uint32_t *count) const
    {
        TDictionaryData<DictionaryData::Format::BUFFER> *data = reinterpret_cast<TDictionaryData<DictionaryData::Format::BUFFER>*>(m_data);
        *buffer = (const int64_t*)((uint8_t*)data + sizeof(TDictionaryData<DictionaryData::Format::BUFFER>));
        *count = data->Count;
    }

    //! @brief      get_buffer_float32
    void DictionaryNode::get_buffer_float32(const float **buffer, uint32_t *count) const
    {
        TDictionaryData<DictionaryData::Format::BUFFER> *data = reinterpret_cast<TDictionaryData<DictionaryData::Format::BUFFER>*>(m_data);
        *buffer = (const float*)((uint8_t*)data + sizeof(TDictionaryData<DictionaryData::Format::BUFFER>));
        *count = data->Count;
    }

    //! @brief      get_buffer_float64
    void DictionaryNode::get_buffer_float64(const double **buffer, uint32_t *count) const
    {
        TDictionaryData<DictionaryData::Format::BUFFER> *data = reinterpret_cast<TDictionaryData<DictionaryData::Format::BUFFER>*>(m_data);
        *buffer = (const double*)((uint8_t*)data + sizeof(TDictionaryData<DictionaryData::Format::BUFFER>));
        *count = data->Count;
    }

    //! @brief      get_buffer_string
    void DictionaryNode::get_buffer_string(Vector<const char*> *buffer) const
    {
        TDictionaryData<DictionaryData::Format::BUFFER> *data = reinterpret_cast<TDictionaryData<DictionaryData::Format::BUFFER>*>(m_data);
        buffer->clear();
        if (data->Count == 0)
            return;
        buffer->resize(data->Count);

        uint32_t index = 0;
        const char *buffer_raw = (const char*)((uint8_t*)data + sizeof(TDictionaryData<DictionaryData::Format::BUFFER>));
        while (index < data->Count)
        {
            uint32_t length = *(uint32_t*)buffer_raw;
            (*buffer)[index] = buffer_raw + sizeof(uint32_t);
            buffer_raw += sizeof(uint32_t) + length + 1;
            ++index;
        }
    }

    //! @brief      set_buffer_string
    void DictionaryNode::set_buffer_string(const char **buffer, uint32_t count)
    {
        uint32_t offset = 0;
        uint32_t index = 0;
        while (index < count)
        {
            uint32_t string_length = (uint32_t)STRLEN(buffer[index]);
            set_buffer_string_internal(buffer[index], string_length, &offset);
            ++index;
        }

        TDictionaryData<DictionaryData::Format::BUFFER> *data = reinterpret_cast<TDictionaryData<DictionaryData::Format::BUFFER>*>(m_data);
        data->Count = count;
        data->Format = DictionaryData::Format::STRING;
    }

    //! @brief      set_buffer_string
    void DictionaryNode::set_buffer_string(const String *buffer, uint32_t count)
    {
        uint32_t offset = 0;
        uint32_t index = 0;
        while (index < count)
        {
            const String& string = buffer[index];
            set_buffer_string_internal(string.get_buffer(), string.get_length(), &offset);
            ++index;
        }

        TDictionaryData<DictionaryData::Format::BUFFER> *data = reinterpret_cast<TDictionaryData<DictionaryData::Format::BUFFER>*>(m_data);
        data->Count = count;
        data->Format = DictionaryData::Format::STRING;
    }

    //! @brief      set_buffer_string
    void DictionaryNode::set_buffer_string_internal(const char *buffer, uint32_t length, uint32_t *offset)
    {
        // add uint32_t size to keep length
        uint32_t string_size = sizeof(uint32_t) + length + 1;
        char *allocated_buffer = (char*)set_buffer_internal(DictionaryData::Format::UINTEGER8, nullptr, *offset + string_size, sizeof(char));
        char *allocated_string = allocated_buffer + *offset;
        *(uint32_t*)allocated_string = length;
        allocated_string += sizeof(uint32_t);
        MEMCPY(allocated_string, buffer, length);
        allocated_string[length] = 0;
        *offset += string_size;
    }

    //! @brief      set_buffer_internal
    void* DictionaryNode::set_buffer_internal(DictionaryData::Format format, const void *buffer, uint32_t element_count, uint32_t element_size)
    {
        TDictionaryData<DictionaryData::Format::BUFFER> *data = reinterpret_cast<TDictionaryData<DictionaryData::Format::BUFFER>*>(m_data);

        uint32_t current_size = 0;
        uint32_t buffer_size = element_count * element_size;
        uint32_t data_size = sizeof(TDictionaryData<DictionaryData::Format::BUFFER>) + buffer_size;
        DictionaryData::Format current_format = m_data->get_format();
        if (current_format != DictionaryData::Format::BUFFER)
            goto reallocate;

        current_size = m_dictionary->get_data_size_internal(m_data);
        if (current_size != data_size)
            goto reallocate;

        goto modify;

    reallocate:
    {
        DictionaryData *new_data = m_dictionary->reallocate_data_internal(m_data, data_size);
        if (!buffer && current_size && new_data != m_data)
        {
            uint8_t *data_destination = ((uint8_t*)new_data) + sizeof(TDictionaryData<DictionaryData::Format::BUFFER>);
            uint8_t *data_source = ((uint8_t*)m_data) + sizeof(TDictionaryData<DictionaryData::Format::BUFFER>);
            MEMCPY(data_destination, data_source, current_size - sizeof(TDictionaryData<DictionaryData::Format::BUFFER>));
        }
        m_data = new_data;
        data = reinterpret_cast<TDictionaryData<DictionaryData::Format::BUFFER>*>(m_data);
        data->set_format(DictionaryData::Format::BUFFER);
        data->Capacity = buffer_size;
    }

    modify:
        data->Format = format;
        data->Count = element_count;
        void *buffer_array = ((uint8_t*)data) + sizeof(TDictionaryData<DictionaryData::Format::BUFFER>);
        if (buffer)
            MEMCPY(buffer_array, buffer, buffer_size);

        return buffer_array;
    }

    //! @brief      operator[]
    DictionaryNode DictionaryNode::operator[](const char *name)
    {
        return m_dictionary->get_or_create_node_internal(m_data, name);
    }
}
