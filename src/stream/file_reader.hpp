#pragma once

/******************************************************************************
**  Includes
******************************************************************************/
#include "stream_reader.hpp"
#include "../file_system/file.hpp"

/******************************************************************************
**  Define
******************************************************************************/
#define FILE_READER_BUFFER_SIZE 65535

/******************************************************************************
**  Class Declaration
******************************************************************************/
namespace Gorilla
{
    class FileReader : public StreamReader
    {
    public:
        FileReader();
        FileReader(const char *filepath);
        ~FileReader();

        bool open(const char *filepath);
        bool close();

        virtual bool can_read() const override;

        virtual uint32_t get_position() const override;
        virtual uint32_t get_size() const override;
        virtual bool seek(uint32_t position) override;

        using StreamReader::read;
        virtual bool read(void *buffer, uint32_t size) override;

    private:
        File*   m_file;
        char    m_buffer[FILE_READER_BUFFER_SIZE];
        uint32_t  m_buffer_position;
    };
}
