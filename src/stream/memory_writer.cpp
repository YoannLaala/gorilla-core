/******************************************************************************
**  Includes
******************************************************************************/
#include "memory_writer.hpp"

/******************************************************************************
**  Class Declaration
******************************************************************************/
namespace Gorilla
{
    //! @brief      constructor
    MemoryWriter::MemoryWriter()
        : m_position(0)
    {
        // Nothing to do
    }

    //! @brief      destructor
    MemoryWriter::~MemoryWriter()
    {
        // Nothing to do
    }

    //! @brief      can_write
    bool MemoryWriter::can_write() const
    {
        return true;
    }

    //! @brief      get_position
    uint32_t MemoryWriter::get_position() const
    {
        return m_position;
    }

    //! @brief      get_size
    uint32_t MemoryWriter::get_size() const
    {
        return m_buffer.get_size();
    }

    //! @brief      seek
    bool MemoryWriter::seek(uint32_t position)
    {
        m_position = position;
        return true;
    }

    //! @brief      write
    bool MemoryWriter::write(const void *buffer, uint32_t size)
    {
        m_buffer.expand(size);
        MEMCPY(&m_buffer[m_position], buffer, size);
        m_position += size;

        return true;
    }
}
