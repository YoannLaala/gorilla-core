#pragma once

/******************************************************************************
**  Includes
******************************************************************************/
#include "../string/string.hpp"

/******************************************************************************
**  Defines
******************************************************************************/
#if defined(PLATFORM_WINDOWS)
    #define PATH_SEPARATOR "\\"
#else
    #define PATH_SEPARATOR "/"
#endif

/******************************************************************************
**  Class Declaration
******************************************************************************/
namespace Gorilla
{
    class Path
    {
    public:

        Path();
        Path(const char* path);
        Path(const String& path);
        ~Path();

        Path& operator= (const char* path);
        Path& operator= (const String& path);

        inline const String& get_full                       () const { return m_full; }
        inline const String& get_directory                  () const { return m_directory; }
        inline const String& get_filename                   () const { return m_filename; }
        inline const String& get_extension                  () const { return m_extension; }
        inline const String& get_filename_with_extension    () const { return m_filename_with_extension; }

        template <typename T, typename ...ARGUMENTS>
        static void combine(String *result, const T& path, ARGUMENTS... arguments);

        static void format(String *path);

    private:
        static void combine(String *result);

    private:
        String m_full;
        String m_directory;
        String m_filename;
        String m_extension;
        String m_filename_with_extension;
    };

    template <typename T, typename ...ARGUMENTS>
    void Path::combine(String *result, const T& path, ARGUMENTS... arguments)
    {
        if (result->get_length())
            result->append(PATH_SEPARATOR, 1);
        result->append(path);

        combine(result, arguments...);
    }
}
