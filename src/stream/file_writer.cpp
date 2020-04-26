/******************************************************************************
**  Includes
******************************************************************************/
#include "file_writer.hpp"
#include "../file_system/file_system.hpp"

/******************************************************************************
**  Class Definition
******************************************************************************/
namespace Gorilla
{
    //! @brief      Constructor
    FileWriter::FileWriter()
        : m_file(nullptr)
        , m_buffer_position(0)
    {
        // Nothing to do
    }

    //! @brief      Constructor
    FileWriter::FileWriter(const char *filepath)
        : FileWriter()
    {
        open(filepath);
    }

    //! @brief      Destructor
    FileWriter::~FileWriter()
    {
        close();
    }

    //! @brief      open
    bool FileWriter::open(const char *filepath)
    {
        if (m_file && !close())
            return false;

        m_file = FileSystem::open_file(filepath, FileSystem::Access::WRITE);
        m_buffer_position = 0;

        return can_write();
    }

    //! @brief      close
    bool FileWriter::close()
    {
        if (!flush())
            return false;

        bool result = FileSystem::close_file(m_file);
        m_file = nullptr;

        return result;
    }

    //! @brief      can_write
    bool FileWriter::can_write() const
    {
        return m_file != nullptr;
    }

    //! @brief      get_position
    uint32_t FileWriter::get_position() const
    {
        if (!can_write())
            return 0;

        return m_file->get_position() + m_buffer_position;
    }

    //! @brief      get_size
    uint32_t FileWriter::get_size() const
    {
        if (!can_write())
            return 0;

        return m_file->get_size() + m_buffer_position;
    }

    //! @brief      get_size
    bool FileWriter::seek(uint32_t position)
    {
        if (!can_write())
            return false;

        if (!flush())
            return false;
        m_file->seek(position);

        return true;
    }

    //! @brief      flush
    bool FileWriter::flush()
    {
        if(m_buffer_position > 0)
        {
            if (m_file->write(m_buffer, m_buffer_position) != m_buffer_position)
                return false;
            m_buffer_position = 0;
        }

        return true;
    }

    //! @brief      write
    bool FileWriter::write(const void *buffer, uint32_t size)
    {
        // We write a size bigger enough to exceed the buffer
        // In this case we copy first the remaining data from the buffer
        if(m_buffer_position > 0)
        {
            uint32_t buffer_next_position = m_buffer_position + size;
            if(buffer_next_position > FILE_WRITER_BUFFER_SIZE)
            {
                // In some case we can be at the limit of the buffer
                if (m_file->write(m_buffer, m_buffer_position) != m_buffer_position)
                    return false;
                m_buffer_position = 0;
            }
        }

        // We write directly from the file in case if the buffer is too small
        if(size >= FILE_WRITER_BUFFER_SIZE)
        {
            if (m_file->write(buffer, size) != size)
                return false;
        }
        // Normal write using the buffer
        else
        {
            // Update buffer if we consumed it
            if(m_buffer_position >= FILE_WRITER_BUFFER_SIZE)
            {
                if (m_file->write(m_buffer, FILE_WRITER_BUFFER_SIZE) != FILE_WRITER_BUFFER_SIZE)
                    return false;
                m_buffer_position = 0;
            }
            MEMCPY(&m_buffer[m_buffer_position], buffer, size);
            m_buffer_position += size;
        }

        return true;
    }
}
