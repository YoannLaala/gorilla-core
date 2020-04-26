/******************************************************************************
**  Includes
******************************************************************************/
#include "file.hpp"
#include <sys/stat.h>
#include <unistd.h>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla
{
    //! @brief      read
    //! @details    Return the number of byte read
    uint32_t File::read(void *buffer, uint32_t size)
    {
        int32_t byte_count = ::read((int32_t)m_handle, buffer, size);
        if (byte_count == -1)
            return 0;
        return (uint32_t)byte_count;
    }

    //! @brief      write
    //! @details    Return the number of byte written
    uint32_t File::write(const void *buffer, uint32_t size)
    {
        int32_t byte_count = ::write((int32_t)m_handle, buffer, size);
        if (byte_count == -1)
            return 0;
        return (uint32_t)byte_count;
    }

    //! @brief      seek
    void File::seek(uint32_t position)
    {
        ::lseek((int32_t)m_handle, position, SEEK_SET);
    }

    //! @brief      get_size
    uint32_t File::get_size() const
    {
        struct stat buffer;
        int32_t result = ::fstat((int32_t)m_handle, &buffer);
        if (result == -1)
            return (uint32_t)-1;

        return buffer.st_size;
    }

    //! @brief      get_position
    uint32_t File::get_position() const
    {
        return (uint32_t)::lseek((int32_t)m_handle, 0, SEEK_CUR);
    }

    //! @brief      get_attribute
    uint32_t File::get_attribute() const
    {
        return 0;
    }

    //! @brief      get_last_write
    uint64_t File::get_last_write() const
    {
        struct stat buffer;
        int32_t result = ::fstat((int32_t)m_handle, &buffer);
        if (result == -1)
            return (uint32_t)-1;

        return buffer.st_mtime;
    }
}
