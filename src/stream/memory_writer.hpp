#pragma once

/******************************************************************************
**  Includes
******************************************************************************/
#include "stream_writer.hpp"

/******************************************************************************
**  Class Declaration
******************************************************************************/
namespace Gorilla
{
    class MemoryWriter : public StreamWriter
    {
    public:
        MemoryWriter();
        ~MemoryWriter();

        virtual bool can_write() const override;
        virtual uint32_t get_position() const override;
        virtual uint32_t get_size() const override;
        virtual bool seek(uint32_t position) override;

        using StreamWriter::write;
        virtual bool write(const void *buffer, uint32_t size) override;

        inline uint8_t& operator[](uint32_t index);

    private:
        Vector<uint8_t> m_buffer;
        uint32_t m_position;
    };

    //! @brief      operator[]
    uint8_t& MemoryWriter::operator[](uint32_t index)
    {
        return m_buffer[index];
    }
}
