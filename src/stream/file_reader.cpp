/******************************************************************************
**	Includes
******************************************************************************/
#include "file_reader.hpp"
#include "../file_system/file_system.hpp"

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla
{
    //! @brief      Constructor
    FileReader::FileReader()
        : m_file(nullptr)
        , m_buffer_position(0)
    {
        // Nothing to do
    }

    //! @brief      Constructor
    FileReader::FileReader(const char*filepath)
        : FileReader()
    {
        open(filepath);
    }

    //! @brief      Destructor
    FileReader::~FileReader()
    {
        close();
    }

    //! @brief      Open
    bool FileReader::open(const char*filepath)
    {
        m_file = FileSystem::open_file(filepath, FileSystem::Access::READ);
        m_buffer_position = 0;

        return can_read();
    }

    //! @brief      Close
    bool FileReader::close()
    {
        if (!FileSystem::close_file(m_file))
            return false;
        m_file = nullptr;

        return true;
    }

    //! @brief      can_read
    bool FileReader::can_read() const
    {
         return m_file != nullptr;
    }

    //! @brief      get_position
    uint32_t FileReader::get_position() const
    {
        if (!can_read())
            return 0;

        uint32_t position = m_file->get_position();
        if (position >= FILE_READER_BUFFER_SIZE)
            return position - FILE_READER_BUFFER_SIZE + m_buffer_position;

        return m_buffer_position;
    }

    //! @brief      get_size
    uint32_t FileReader::get_size() const
    {
        if (!can_read())
            return 0;

        return m_file->get_size();
    }

    //! @brief      seek
    bool FileReader::seek(uint32_t position)
    {
        if (!can_read())
            return false;
        m_buffer_position = position;

        return true;
    }

    //! @brief      Read
    bool FileReader::read(void *buffer, uint32_t size)
    {
        // We read a size bigger enough to exceed the buffer
        // In this case we copy first the remaining data from the buffer
        if(m_buffer_position > 0)
        {
            uint32_t buffer_next_position = m_buffer_position + size;
            if(buffer_next_position > FILE_READER_BUFFER_SIZE)
            {
                // In some case we can be at the limit of the buffer
                uint32_t buffer_size = FILE_READER_BUFFER_SIZE - m_buffer_position;
                MEMCPY(buffer, &m_buffer[m_buffer_position], buffer_size);

                m_buffer_position = 0;
                size -= buffer_size;
                buffer = (uint8_t*)buffer + buffer_size;
            }
        }

        // We read directly from the file in case if the buffer is too small
        if(size >= FILE_READER_BUFFER_SIZE)
        {
            if (m_file->read(buffer, size) != size)
                return false;
        }
        // Normal read using the buffer
        else
        {
            // Update buffer if we consumed it
            if(m_buffer_position == 0)
                m_file->read(m_buffer, FILE_READER_BUFFER_SIZE);

            MEMCPY(buffer, &m_buffer[m_buffer_position], size);
            m_buffer_position += size;
        }

        return true;
    }
}
