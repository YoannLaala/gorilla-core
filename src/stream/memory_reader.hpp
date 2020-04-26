#pragma once

/******************************************************************************
**  Includes
******************************************************************************/
#include "stream_reader.hpp"

/******************************************************************************
**  Class Declaration
******************************************************************************/
namespace Gorilla
{
    class MemoryReader : public StreamReader
    {
    public:
        MemoryReader();
        MemoryReader(const void *buffer, uint32_t size);
        ~MemoryReader();

        virtual bool can_read() const override;
        virtual uint32_t get_position() const override;
        virtual uint32_t get_size() const override;
        virtual bool seek(uint32_t position) override;

        using StreamReader::read;
        virtual bool read(void *buffer, uint32_t size) override;

        inline uint8_t& operator[](uint32_t index);

        void set_buffer(const void *buffer, uint32_t size);

    private:
        uint8_t *m_buffer;
        uint32_t m_position;
        uint32_t m_size;
    };

    //! @brief      operator[]
    uint8_t& MemoryReader::operator[](uint32_t index)
    {
        return m_buffer[index];
    }
}
