#pragma once

/******************************************************************************
**  Includes
******************************************************************************/
#include "../container/vector.hpp"
#include "../string/string.hpp"

/******************************************************************************
**  Forward Declaration
******************************************************************************/
namespace Gorilla
{
    class File;
}

/******************************************************************************
**  Class Declaration
******************************************************************************/
namespace Gorilla
{
    class FileSystem
    {
    public:
        enum class Access
        {
            READ = 0,
            WRITE,
        };

        enum class Directory
        {
            EXECUTABLE = 0,
            USER,
            TEMPORARY,
        };

        static bool         check_file          (const char *filepath);
        static File*        open_file           (const char *filepath, Access access = Access::READ);
        static bool         close_file          (File *file);
        static bool         delete_file         (const char *filepath);
        static bool         copy_file           (const char *source, const char *destination);

        static bool         check_directory     (const char *path);
        static bool         create_directory    (const char *path);
        static bool         delete_directory    (const char *path);

        static bool         move                (const char *source, const char *destination);

        static void         get_directory_path  (Directory type, String *output);

        static bool         list(const char *path, Vector<String> *directories, Vector<String> *files, const char *extension = nullptr);
    };
}
