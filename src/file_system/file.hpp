#pragma once

/******************************************************************************
**  Includes
******************************************************************************/
#include <cstdint>

/******************************************************************************
**  Class Declaration
******************************************************************************/
namespace Gorilla
{
    class File
    {
        friend class FileSystem;

    public:
        uint32_t read(void *buffer, uint32_t size);
        uint32_t write(const void *buffer, uint32_t size);
        void seek(uint32_t position);

        uint32_t get_size() const;
        uint32_t get_position() const;
        uint32_t get_attribute() const;
        uint64_t get_last_write() const;

    private:
        uint64_t m_handle;
    };
}
