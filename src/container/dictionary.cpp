/******************************************************************************
**  Includes
******************************************************************************/
#include "dictionary.hpp"
#include "../log/log.hpp"

/******************************************************************************
**  Class Definition
******************************************************************************/
namespace Gorilla
{
    //! @brief      constructor
    Dictionary::Dictionary(uint8_t flags /*= 0 */, uint32_t page_size /*= 65536*/)
        : DictionaryNode(this, &m_root)
        , m_current_data(nullptr)
        , m_current_offset(page_size)
        , m_page_size(page_size)
        , m_flags(flags)
    {
        m_root.set_format(DictionaryData::Format::OBJECT);
        m_root.FirstChildId = DictionaryId::INVALID;
        m_root.LastChildId = DictionaryId::INVALID;
    }

    //! @brief      destructor
    Dictionary::~Dictionary()
    {
        const uint32_t uiAllocationCount = m_allocations.get_size();
        for(uint32_t uiAllocation = 0; uiAllocation < uiAllocationCount; ++uiAllocation)
        {
            delete m_allocations[uiAllocation];
        }
        m_allocations.clear();
    }

    //! @brief      clear
    void Dictionary::clear()
    {
        m_pages.clear();
        m_names.clear();
        m_hash_offsets.clear();

        // reset root & offset
        m_root.FirstChildId = DictionaryId::INVALID;
        m_root.LastChildId = DictionaryId::INVALID;
        m_current_offset = m_page_size;
    }

    //! @brief      get_parent_id_internal
    DictionaryId Dictionary::get_parent_id_internal(DictionaryData *child, DictionaryId parent_id /*= DictionaryId::INVALID*/) const
    {
        const TDictionaryData<DictionaryData::Format::OBJECT> *parent_data = nullptr;
        if (parent_id == DictionaryId::INVALID)
            parent_data = &m_root;
        else
            parent_data = reinterpret_cast<TDictionaryData<DictionaryData::Format::OBJECT>*>(get_data_internal(parent_id));

        DictionaryId id = parent_data->FirstChildId;
        while (id != DictionaryId::INVALID)
        {
            DictionaryData *data = get_data_internal(id);
            if (data == child)
                return parent_id;

            DictionaryData::Format format = data->get_format();
            if (format == DictionaryData::Format::OBJECT || format == DictionaryData::Format::ARRAY)
            {
                DictionaryId parent_id = get_parent_id_internal(child, id);
                if(parent_id != DictionaryId::INVALID)
                    return parent_id;
            }

            id = data->get_next_id();
        }

        return DictionaryId::INVALID;
    }

    //! @brief      get_id_internal
    DictionaryId Dictionary::get_id_internal(DictionaryData *parent, const char *name) const
    {
        TDictionaryData<DictionaryData::Format::OBJECT> *parent_data = reinterpret_cast<TDictionaryData<DictionaryData::Format::OBJECT>*>(parent);
        if(parent_data->FirstChildId != DictionaryId::INVALID)
        {
            uint32_t name_hash = Hash::generate(name) & DICTIONARY_NODE_DATA_HASH_BITS;

            DictionaryId id = parent_data->FirstChildId;
            while(id != DictionaryId::INVALID)
            {
                DictionaryData *data = get_data_internal(id);
                if(data->get_hash() == name_hash)
                    return id;

                id = data->get_next_id();
            }
        }

        return DictionaryId::INVALID;
    }

    //! @brief      get_node_internal
    DictionaryNode Dictionary::get_node_internal(DictionaryData *parent, const char *name) const
    {
        DictionaryData *data = get_data_internal(parent, name);
        return DictionaryNode(const_cast<Dictionary*>(this), data);
    }

    //! @brief      get_or_create_node_internal
    DictionaryNode Dictionary::get_or_create_node_internal(DictionaryData *parent, const char *name)
    {
        DictionaryData *data = name ? get_data_internal(parent, name) : nullptr;
        if(!data)
            return add_node_internal(parent, name);

        return DictionaryNode(const_cast<Dictionary*>(this), data);
    }

    //! @brief      add_node_internal
    DictionaryNode Dictionary::add_node_internal(DictionaryData *parent, const char *name)
    {
        TDictionaryData<DictionaryData::Format::OBJECT> *data = reinterpret_cast<TDictionaryData<DictionaryData::Format::OBJECT>*>(create_data_internal(parent, DictionaryData::Format::OBJECT, name, 0));
        data->FirstChildId = DictionaryId::INVALID;
        data->LastChildId = DictionaryId::INVALID;

        return DictionaryNode(this, data);
    }

    //! @brief      remove_node_internal
    void Dictionary::remove_node_internal(DictionaryData *parent, const char *name)
    {
        TDictionaryData<DictionaryData::Format::OBJECT>* parent_data = reinterpret_cast<TDictionaryData<DictionaryData::Format::OBJECT>*>(parent);
        if(parent_data->FirstChildId != DictionaryId::INVALID)
        {
            uint32_t name_hash = Hash::generate(name) & DICTIONARY_NODE_DATA_HASH_BITS;

            DictionaryData *previous_data = parent;
            DictionaryId previous_id = DictionaryId::INVALID;
            DictionaryId current_id = parent_data->FirstChildId;
            while (current_id != DictionaryId::INVALID)
            {
                // remove link
                DictionaryData *current_data = get_data_internal(current_id);
                DictionaryId next_id = current_data->get_next_id();
                if(current_data->get_hash() == name_hash)
                {
                    // handle the remove of the first one
                    if(previous_id == DictionaryId::INVALID)
                        parent_data->FirstChildId = next_id;
                    else
                        previous_data->set_next_id(next_id);

                    // handle the remove of the last one
                    if(next_id == DictionaryId::INVALID)
                        parent_data->LastChildId = next_id;
                    else
                        parent_data->LastChildId = previous_id;

                    break;
                }
                previous_data = current_data;
                previous_id = current_id;
                current_id = next_id;
            }
        }
    }

    //! @brief      get_parent_data_internal
    DictionaryData* Dictionary::get_parent_data_internal(DictionaryData *child, DictionaryId parent_id /*= DictionaryId::INVALID*/) const
    {
        DictionaryId id = get_parent_id_internal(child, parent_id);
        if (parent_id == DictionaryId::INVALID)
            return (DictionaryData*)&m_root;

        return get_data_internal(parent_id);
    }

    //! @brief      get_data_internal
    DictionaryData* Dictionary::get_data_internal(DictionaryData *parent, const char *name) const
    {
        DictionaryId id = get_id_internal(parent, name);
        if(id != DictionaryId::INVALID)
            return get_data_internal(id);

        return nullptr;
    }

    //! @brief      get_data_internal
    DictionaryData* Dictionary::get_data_internal(DictionaryId id) const
    {
        return reinterpret_cast<DictionaryData*>(m_pages[id.PageIndex] + id.PageOffset);
    }

    //! @brief      get_data_name_internal
    const char* Dictionary::get_data_name_internal(DictionaryData *data) const
    {
        const uint32_t offset = m_hash_offsets.get(data->get_hash(), (uint32_t)-1);
        if (offset != (uint32_t)-1)
            return &m_names[offset];

        return nullptr;
    }

    //! @brief      get_data_size_internal
    uint32_t Dictionary::get_data_size_internal(DictionaryData *data) const
    {
        DictionaryData::Format current_format = data->get_format();
        switch(current_format)
        {
            case DictionaryData::Format::ZERO:      return sizeof(DictionaryData);
            case DictionaryData::Format::OBJECT:    return sizeof(TDictionaryData<DictionaryData::Format::OBJECT>);
            case DictionaryData::Format::ARRAY:     return sizeof(TDictionaryData<DictionaryData::Format::ARRAY>);
            case DictionaryData::Format::BOOLEAN:   return sizeof(TDictionaryData<DictionaryData::Format::BOOLEAN>);
            case DictionaryData::Format::UINTEGER8: return sizeof(TDictionaryData<DictionaryData::Format::UINTEGER8>);
            case DictionaryData::Format::INTEGER8:  return sizeof(TDictionaryData<DictionaryData::Format::INTEGER8>);
            case DictionaryData::Format::UINTEGER16:return sizeof(TDictionaryData<DictionaryData::Format::UINTEGER16>);
            case DictionaryData::Format::INTEGER16: return sizeof(TDictionaryData<DictionaryData::Format::INTEGER16>);
            case DictionaryData::Format::UINTEGER32:return sizeof(TDictionaryData<DictionaryData::Format::UINTEGER32>);
            case DictionaryData::Format::INTEGER32: return sizeof(TDictionaryData<DictionaryData::Format::INTEGER32>);
            case DictionaryData::Format::FLOAT32:   return sizeof(TDictionaryData<DictionaryData::Format::FLOAT32>);
            case DictionaryData::Format::UINTEGER64:return sizeof(TDictionaryData<DictionaryData::Format::UINTEGER64>);
            case DictionaryData::Format::INTEGER64: return sizeof(TDictionaryData<DictionaryData::Format::INTEGER64>);
            case DictionaryData::Format::FLOAT64:   return sizeof(TDictionaryData<DictionaryData::Format::FLOAT64>);
            case DictionaryData::Format::STRING:
                return sizeof(TDictionaryData<DictionaryData::Format::STRING>) + reinterpret_cast<TDictionaryData<DictionaryData::Format::STRING>*>(data)->Length + 1;
            case DictionaryData::Format::BUFFER:
                return sizeof(TDictionaryData<DictionaryData::Format::BUFFER>) + reinterpret_cast<TDictionaryData<DictionaryData::Format::BUFFER>*>(data)->Capacity;
        }

        return 0;
    }

    //! @brief      get_data_size_internal
    uint32_t Dictionary::get_data_size_internal(DictionaryData::Format format) const
    {
        switch(format)
        {
            case DictionaryData::Format::ZERO:      return sizeof(DictionaryData);
            case DictionaryData::Format::OBJECT:    return sizeof(TDictionaryData<DictionaryData::Format::OBJECT>);
            case DictionaryData::Format::ARRAY:     return sizeof(TDictionaryData<DictionaryData::Format::ARRAY>);
            case DictionaryData::Format::BOOLEAN:   return sizeof(TDictionaryData<DictionaryData::Format::BOOLEAN>);
            case DictionaryData::Format::UINTEGER8: return sizeof(TDictionaryData<DictionaryData::Format::UINTEGER8>);
            case DictionaryData::Format::INTEGER8:  return sizeof(TDictionaryData<DictionaryData::Format::INTEGER8>);
            case DictionaryData::Format::UINTEGER16:return sizeof(TDictionaryData<DictionaryData::Format::UINTEGER16>);
            case DictionaryData::Format::INTEGER16: return sizeof(TDictionaryData<DictionaryData::Format::INTEGER16>);
            case DictionaryData::Format::UINTEGER32:return sizeof(TDictionaryData<DictionaryData::Format::UINTEGER32>);
            case DictionaryData::Format::INTEGER32: return sizeof(TDictionaryData<DictionaryData::Format::INTEGER32>);
            case DictionaryData::Format::FLOAT32:   return sizeof(TDictionaryData<DictionaryData::Format::FLOAT32>);
            case DictionaryData::Format::UINTEGER64:return sizeof(TDictionaryData<DictionaryData::Format::UINTEGER64>);
            case DictionaryData::Format::INTEGER64: return sizeof(TDictionaryData<DictionaryData::Format::INTEGER64>);
            case DictionaryData::Format::FLOAT64:   return sizeof(TDictionaryData<DictionaryData::Format::FLOAT64>);
            case DictionaryData::Format::STRING:    return sizeof(TDictionaryData<DictionaryData::Format::STRING>);
            case DictionaryData::Format::BUFFER:    return sizeof(TDictionaryData<DictionaryData::Format::BUFFER>);
        }

        return 0;
    }

    //! @brief      allocate_data_internal
    DictionaryData* Dictionary::allocate_data_internal(uint32_t size)
    {
        // create new page if necessary
        uint32_t new_offset = m_current_offset + size;
        if(new_offset >= m_page_size)
        {
            uint32_t page_to_add = (size / m_page_size) + 1;
            add_page_internal(page_to_add);
        }

        DictionaryData *data = reinterpret_cast<DictionaryData*>(m_current_data);
        m_current_data += size;

        return data;
    }

    //! @brief      reallocate_data_internal
    DictionaryData* Dictionary::reallocate_data_internal(DictionaryData *data, uint32_t size)
    {
        // last data of the page
        uint32_t current_size = get_data_size_internal(data);
        LOG_ASSERT(m_current_data - current_size == (uint8_t*)data, "data reallocation only allowed for last element");

        // not enough space in current page
        uint32_t new_offset = m_current_offset + size;
        if (new_offset > m_page_size)
        {
            DictionaryData *data_reallocated = allocate_data_internal(size);
            data_reallocated->set_hash(data->get_hash());

            // patch parent and siblings
            TDictionaryData<DictionaryData::Format::OBJECT> *parent_data = reinterpret_cast<TDictionaryData<DictionaryData::Format::OBJECT>*>(get_parent_data_internal(data));

            DictionaryData *previous_data = nullptr;
            DictionaryId id = parent_data->FirstChildId;
            while (id != DictionaryId::INVALID)
            {
                DictionaryData *child_data = get_data_internal(id);
                if (child_data == data)
                {
                    if (previous_data)
                        previous_data->set_next_id(m_current_id);

                    if (id == parent_data->FirstChildId)
                        parent_data->FirstChildId = m_current_id;

                    if (id == parent_data->LastChildId)
                        parent_data->LastChildId = m_current_id;
                }
                previous_data = child_data;
                id = child_data->get_next_id();
            }
            data_reallocated->set_next_id(data->get_next_id());
            m_current_offset = m_page_size;
            m_current_id.PageOffset = m_page_size;

            return data_reallocated;
        }

        m_current_data = (uint8_t*)data + size;
        m_current_offset = (uint32_t)((uint8_t*)m_current_data - m_pages[m_current_id.PageIndex]);
        m_current_id.PageOffset = m_current_offset;

        return data;
    }

    //! @brief      create_data_internal
    DictionaryData* Dictionary::create_data_internal(DictionaryData *parent, DictionaryData::Format format, const char *name, uint32_t extra_size)
    {
        uint32_t data_size = get_data_size_internal(format) + extra_size;
        DictionaryData *data = allocate_data_internal(data_size);
        data->set_format(format);
        data->set_next_id(DictionaryId::INVALID);

        // update
        TDictionaryData<DictionaryData::Format::OBJECT>* parent_data = reinterpret_cast<TDictionaryData<DictionaryData::Format::OBJECT>*>(parent);
        if(parent_data->FirstChildId == DictionaryId::INVALID)
            parent_data->FirstChildId = m_current_id;
        else
        {
            DictionaryData *child_data = get_data_internal(parent_data->LastChildId);
            child_data->set_next_id(m_current_id);
        }
        parent_data->LastChildId = m_current_id;

        // handle Name
        if(name)
        {
            uint32_t name_hash = Hash::generate(name) & DICTIONARY_NODE_DATA_HASH_BITS;
            data->set_hash(name_hash);
            uint32_t name_offset = m_hash_offsets.get(name_hash, (uint32_t)-1);
            if(name_offset == (uint32_t)-1)
            {
                //  check name length
                uint32_t name_length = static_cast<uint32_t>(STRLEN(name)) + 1;

                // copy the name & avoid resize each time using expand
                name_offset = m_names.get_size();
                uint32_t new_size = name_offset + name_length;
                if(m_names.get_capacity() <= new_size)
                    m_names.reserve(new_size << 1);

                m_names.expand(name_length);
                MEMCPY(&m_names[0] + name_offset, name, name_length);

                // add the offset for this hash
                m_hash_offsets.add(name_hash, name_offset);
            }
        }
        else
            parent->set_format(DictionaryData::Format::ARRAY);

        m_current_offset += data_size;
        m_current_id.PageOffset = static_cast<uint16_t>(m_current_offset);

        return data;
    }

    //! @brief      allocate_page_internal
    void Dictionary::allocate_page_internal(uint32_t count)
    {
        // Allocate Memory;
        uint8_t *allocation = new uint8_t[count * m_page_size];
        m_allocations.add(allocation);

        // Construct page
        const uint32_t old_size = m_pages.get_size();
        const uint32_t new_size = old_size + count;

        uint8_t *page = allocation;
        m_pages.reserve(new_size);
        for(uint32_t i = old_size; i < new_size; ++i)
        {
            m_pages[i] = page;
            page += m_page_size;
        }
    }

    //! @brief      add_page_internal
    void Dictionary::add_page_internal(uint32_t count)
    {
        const uint32_t old_size = m_pages.get_size();
        const uint32_t new_size = old_size + count;
        LOG_ASSERT(new_size < (1 << 16), "maximum pages reached %u", (1 << 16));

        // reset offset
        m_current_id.PageIndex = static_cast<uint16_t>(old_size);
        m_current_id.PageOffset = 0;
        m_current_offset = 0;

        // Allocate Memory if needed
        if(m_pages.get_capacity() < new_size)
        {
            uint32_t page_to_allocate = (count > 1) ? new_size - old_size : 1;
            allocate_page_internal(page_to_allocate);
        }
        m_pages.resize(new_size);
        m_current_data = m_pages[old_size];
    }
}
