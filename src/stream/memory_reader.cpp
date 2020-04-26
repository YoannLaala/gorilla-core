/******************************************************************************
**  Includes
******************************************************************************/
#include "memory_reader.hpp"

/******************************************************************************
**  Class Definition
******************************************************************************/
namespace Gorilla
{
    //! @brief      Constructor
    MemoryReader::MemoryReader()
        : m_buffer(0)
        , m_position(0)
        , m_size(0)
    {
        // Nothing to do
    }

    //! @brief      Constructor
    MemoryReader::MemoryReader(const void *buffer, uint32_t size)
        : m_buffer(static_cast<uint8_t*>(const_cast<void*>(buffer)))
        , m_position(0)
        , m_size(size)
    {
        // Nothing to do
    }

    //! @brief      Destructor
    MemoryReader::~MemoryReader()
    {
        // Nothing to do
    }

    //! @brief      can_read
    bool MemoryReader::can_read() const
    {
        return m_buffer != nullptr;
    }

    //! @brief      get_position
    uint32_t MemoryReader::get_position() const
    {
        return m_position;
    }

    //! @brief      get_size
    uint32_t MemoryReader::get_size() const
    {
        return m_size;
    }

    //! @brief      seek
    bool MemoryReader::seek(uint32_t position)
    {
        m_position = position;
        return true;
    }

    //! @brief      read
    bool MemoryReader::read(void *buffer, uint32_t size)
    {
        MEMCPY(buffer, &m_buffer[m_position], size);
        m_position += size;

        return true;
    }

    //! @brief      set_buffer
    void MemoryReader::set_buffer(const void *buffer, uint32_t size)
    {
        m_buffer = static_cast<uint8_t*>(const_cast<void*>(buffer));
        m_size = size;
        m_position = 0;
    }
}
