/******************************************************************************
**  Includes
******************************************************************************/
#include "dictionary_binary_stream.hpp"
#include "../../container/dictionary.hpp"

/******************************************************************************
**  Class Definition
******************************************************************************/
namespace Gorilla
{
    //! @brief      read
    bool DictionaryBinaryStream::read(StreamReader *stream, DictionaryNode *node)
    {
        Dictionary *dictionary = node->m_dictionary;

        uint32_t page_count;
        if (!stream->read(&page_count))
            return false;

        if(page_count == 0)
            return true;

        if (!stream->read(&dictionary->m_root, sizeof(TDictionaryData<DictionaryData::Format::OBJECT>)))
            return false;
        node->m_data = &dictionary->m_root;

        uint32_t page_size;
        if (!stream->read(&page_size))
            return false;

        // allocate page
        dictionary->add_page_internal(page_count);
        if (!stream->read(&dictionary->m_current_offset))
            return false;

        // read page data
        uint8_t *allocation = dictionary->m_allocations[0];
        const uint32_t data_size = (page_count - 1) * page_size + dictionary->m_current_offset;
        if (!stream->read(allocation, data_size))
            return false;

        // read names
        uint32_t character_count;
        if (!stream->read(&character_count))
            return false;

        dictionary->m_names.resize(character_count);
        if (!stream->read(&dictionary->m_names[0], character_count))
            return false;

        // read key associated to names
        uint32_t hash_count;
        if (!stream->read(&hash_count))
            return false;

        for(uint32_t i = 0; i < hash_count; ++i)
        {
            uint32_t key, offset;

            if (!stream->read(&key))
                return false;

            if (!stream->read(&offset))
                return false;

            dictionary->m_hash_offsets.add(key, offset);
        }

        return true;
    }

    //! @brief      write
    bool DictionaryBinaryStream::write(StreamWriter *stream, DictionaryNode *node, uint32_t flags)
    {
        Dictionary *dictionary = node->m_dictionary;

        const uint32_t page_count = dictionary->m_pages.get_size();
        if (!stream->write(page_count))
            return false;

        if(page_count == 0)
            return true;

        if (!stream->write(&dictionary->m_root, sizeof(TDictionaryData<DictionaryData::Format::OBJECT>)))
            return false;

        if (!stream->write(dictionary->m_page_size))
            return false;

        if (!stream->write(dictionary->m_current_offset))
            return false;

        // write complete page
        const uint32_t page_completed_count = page_count - 1;
        for(uint32_t i = 0; i < page_completed_count; ++i)
        {
            if (!stream->write(dictionary->m_pages[i], dictionary->m_page_size))
                return false;
        }
        if (!stream->write(dictionary->m_pages[page_completed_count], dictionary->m_current_offset))
            return false;

        // write names
        const uint32_t character_count = dictionary->m_names.get_size();
        if (!stream->write(character_count))
            return false;

        if (!stream->write(&dictionary->m_names[0], character_count))
            return false;

        // write hash associated to names
        const uint32_t hash_count = dictionary->m_hash_offsets.get_size();
        if (!stream->write(hash_count))
            return false;
        HashMap<uint32_t, uint32_t>::Iterator it = dictionary->m_hash_offsets.get_first();
        HashMap<uint32_t, uint32_t>::Iterator itEnd = dictionary->m_hash_offsets.get_last();
        while(it != itEnd)
        {
            if (!stream->write(it.get_key()) || !stream->write(it.get_value()))
                return false;
            ++it;
        }

        return true;
    }
}
