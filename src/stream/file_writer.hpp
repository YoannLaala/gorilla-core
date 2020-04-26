#pragma once

/******************************************************************************
**  Includes
******************************************************************************/
#include "stream_writer.hpp"
#include "../file_system/file.hpp"

/******************************************************************************
**  Define
******************************************************************************/
#define FILE_WRITER_BUFFER_SIZE 65535

/******************************************************************************
**  Class Declaration
******************************************************************************/
namespace Gorilla
{
    class FileWriter : public StreamWriter
    {
    public:
        FileWriter();
        FileWriter(const char *filepath);
        ~FileWriter();

        bool open(const char *filepath);
        bool close();

        virtual bool can_write() const override;
        virtual uint32_t get_position() const override;
        virtual uint32_t get_size() const override;
        virtual bool seek(uint32_t position) override;

        using StreamWriter::write;
        virtual bool write(const void *buffer, uint32_t size) override;
        bool flush();

    private:
        File*   m_file;
        char    m_buffer[FILE_WRITER_BUFFER_SIZE];
        uint32_t  m_buffer_position;
    };
}
