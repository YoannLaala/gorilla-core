#pragma once

/******************************************************************************
**  Includes
*****************************************************************************/
#include "dictionary_node.hpp"
#include "../container/vector.hpp"
#include "../container/hash_map.hpp"
#include "../stream/file_reader.hpp"
#include "../stream/file_writer.hpp"

/******************************************************************************
**  Forward Declaration
******************************************************************************/
namespace Gorilla
{
    class FileReader;
    class FileWriter;
}

/******************************************************************************
**  Class Declaration
******************************************************************************/
namespace Gorilla
{
    class Dictionary : public DictionaryNode
    {
        friend class DictionaryNode;
        friend class DictionaryBinaryStream;

    public:
        Dictionary(uint8_t flags = 0, uint32_t page_size = 65536);
        ~Dictionary();

        template <typename T>
        inline bool         read(const char *filepath);
        template <typename T>
        inline bool         read(const char *buffer, uint32_t buffer_size);
        template <typename T>
        inline bool         read(StreamReader *stream);
        template <typename T>
        inline bool         write(const char *filepath, uint32_t flags = 0);
        template <typename T>
        inline bool         write(StreamWriter *stream, uint32_t flags = 0);

        void clear();

    private:
        DictionaryId        get_id_internal(DictionaryData *parent, const char *name) const;

        DictionaryNode      get_node_internal(DictionaryData *parent, const char *name) const;
        DictionaryNode      get_or_create_node_internal(DictionaryData *parent, const char *name);
        DictionaryNode      add_node_internal(DictionaryData *parent, const char *name);
        void                remove_node_internal(DictionaryData *parent, const char *name);

        DictionaryData*     get_data_internal(DictionaryData *parent, const char *name) const;
        DictionaryData*     get_data_internal(DictionaryId id) const;
        const char*         get_data_name_internal(DictionaryData *data) const;
        uint32_t            get_data_size_internal(DictionaryData *data) const;
        uint32_t            get_data_size_internal(DictionaryData::Format format) const;

        DictionaryData*     allocate_data_internal(uint32_t size);
        DictionaryData*     reallocate_data_internal(DictionaryData *data, uint32_t size);
        DictionaryData*     create_data_internal(DictionaryData *parent, DictionaryData::Format format, const char *name, uint32_t extra_size);

        void                allocate_page_internal(uint32_t count);
        void                add_page_internal(uint32_t count);

    protected:
        Vector<uint8_t*>    m_allocations;
        Vector<uint8_t*>    m_pages;
        Vector<char>        m_names;
        HashMap<uint32_t, uint32_t> m_hash_offsets;
        TDictionaryData<DictionaryData::Format::OBJECT> m_root;
        uint8_t*            m_current_data;
        DictionaryId        m_current_id;
        uint32_t            m_current_offset;
        uint32_t            m_page_size;
        uint8_t             m_flags;
    };

    //! @brief      read
    template <typename T>
    bool Dictionary::read(const char *filepath)
    {
        FileReader stream(filepath);
        return read<T>(&stream);
    }

    //! @brief      read
    template <typename T>
    bool Dictionary::read(StreamReader *stream)
    {
        if(!stream->can_read())
            return false;

        clear();
        return T::read(stream, this);
    }

    //! @brief      write
    template <typename T>
    bool Dictionary::write(const char *filepath, uint32_t flags /*= 0*/)
    {
        FileWriter stream(filepath);
        return write<T>(&stream, flags);
    }

    //! @brief      write
    template <typename T>
    bool Dictionary::write(StreamWriter *stream, uint32_t flags /*= 0*/)
    {
        if(!stream->can_write())
            return false;

        return T::write(stream, this, flags);
    }
}
