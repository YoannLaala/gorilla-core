/******************************************************************************
**  Includes
******************************************************************************/
#include "file.hpp"
#include <Windows.h>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla
{
    //! @brief      read
    //! @details    Return the number of byte read
    uint32_t File::read(void *buffer, uint32_t size)
    {
        uint32_t byte_read;
        ::ReadFile((HANDLE)m_handle, buffer, size, (LPDWORD)&byte_read, nullptr);

        return byte_read;
    }

    //! @brief      write
    //! @details    Return the number of byte written
    uint32_t File::write(const void *buffer, uint32_t size)
    {
        uint32_t byte_write;
        ::WriteFile((HANDLE)m_handle, buffer, size, (LPDWORD)&byte_write, nullptr);

        return byte_write;
    }

    //! @brief      seek
    void File::seek(uint32_t position)
    {
        ::SetFilePointer((HANDLE)m_handle, position, 0, FILE_BEGIN);
    }

    //! @brief      get_size
    uint32_t File::get_size() const
    {
        return ::GetFileSize((HANDLE)m_handle, nullptr);
    }

    //! @brief      get_position
    uint32_t File::get_position() const
    {
        return ::SetFilePointer((HANDLE)m_handle, 0, 0, FILE_CURRENT);
    }

    //! @brief      get_attribute
    uint32_t File::get_attribute() const
    {
        LPBY_HANDLE_FILE_INFORMATION file_information = nullptr;
        ::GetFileInformationByHandle((HANDLE)m_handle, file_information);

        return file_information->dwFileAttributes;
    }

    //! @brief      get_last_write
    uint64_t File::get_last_write() const
    {
        uint64_t creation_time, last_access_time, last_write_time;
        ::GetFileTime((HANDLE)m_handle, (FILETIME*)&creation_time, (FILETIME*)&last_access_time, (FILETIME*)&last_write_time);

        return last_write_time;
    }
}