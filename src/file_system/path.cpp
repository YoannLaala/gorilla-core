/******************************************************************************
**  Includes
******************************************************************************/
#include "path.hpp"

/******************************************************************************
**  Class Definition
******************************************************************************/
namespace Gorilla
{
    //! @brief      Constructor
    Path::Path()
    {
    }

    //! @brief      Constructor
    Path::Path(const char* path)
    {
        *this = path;
    }

    //! @brief      Constructor
    Path::Path(const String& path)
    {
        *this = path.get_buffer();
    }

    //! @brief      Destructor
    Path::~Path()
    {
        // nothing to do
    }

    //! @brief      format
    void Path::format(String *path)
    {
        // separators
        uint32_t position = 0;
        uint32_t path_length = path->get_length();
        while(position < path_length)
        {
            uint32_t local_position = position;
            uint32_t separator_count = 0;
            while ((*path)[local_position] == '/' || (*path)[local_position] == '\\')
            {
                ++local_position;
                ++separator_count;
            }

            if (separator_count)
            {
                (*path)[position] = *PATH_SEPARATOR;
                if (separator_count > 1)
                {
                    MEMMOVE(&(*path)[position+1], &(*path)[local_position], path_length - local_position);
                    path_length -= separator_count-1;
                    (*path)[path_length] = '\0';
                }
            }
            ++position;
        }

        if (path_length != path->get_length())
            path->resize(path_length);

        char pattern[] = { *PATH_SEPARATOR, '.', 0 };
        position = path->find(pattern);
        while (position != (uint32_t)-1)
        {
            uint32_t position_start = position;
            uint32_t position_end = position_start+1;
            while ((*path)[position_end] != *PATH_SEPARATOR && position_end < path_length)
                ++position_end;

            // length == 2: .
            // length == 3: ..
            uint32_t length = position_end - position_start;
            if (length == 3)
            {
                --position_start;
                while((*path)[position_start] != *PATH_SEPARATOR)
                    --position_start;
            }
            length = position_end - position_start;
            const uint32_t move_size = path_length - position_end;
            MEMMOVE(&(*path)[position_start], &(*path)[position_end], move_size);
            path_length -= length;
            path->resize(path_length);

            position = path->find(pattern);
        }
    }

    //! @brief      combine
    void Path::combine(String *result)
    {
        format(result);
    }

    //! @brief      operator=
    Path& Path::operator= (const String& path)
    {
        *this = path.get_buffer();
        return *this;
    }

    //! @brief      operator=
    //! @details    Create all part of the path from the path
    Path& Path::operator= (const char* path)
    {
        m_full = path;
        m_directory = "";
        m_filename = "";
        m_extension = "";
        m_filename_with_extension = "";

        // Search different part
        if(m_full.get_length() > 0)
        {
            format(&m_full);

            uint32_t last_index =  m_full.get_length() - 1;
            for(int32_t i = m_full.get_length()-1; i >= 0; --i)
            {
                // Extension
                if(m_full[i] == '.' && m_extension.get_length() == 0)
                {
                    m_extension.set(&m_full[i+1], last_index - i);
                }

                // Directory
                if(m_full[i] == *PATH_SEPARATOR)
                {
                    m_directory.append(&m_full[0], i + 1);

                    break;
                }
            }

            // FileName (with extension)
            m_filename_with_extension.append(&m_full[m_directory.get_length()], m_full.get_length() - m_directory.get_length());

            // Short Filename (without extension)
            uint32_t file_size = m_filename_with_extension.get_length() - (m_extension.get_length() ? m_extension.get_length() + 1 : 0);
            m_filename.append(&m_filename_with_extension[0], file_size);
        }

        return *this;
    }
}
