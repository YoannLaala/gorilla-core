/******************************************************************************
**  Includes
******************************************************************************/
#include "file_system.hpp"
#include "file.hpp"
#include "path.hpp"
#include "../time/time.hpp"

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ftw.h>
#include <fts.h>
#include <pwd.h>

#if defined(PLATFORM_MAC)
    #include <mach-o/dyld.h>
#endif

/******************************************************************************
**  defines
******************************************************************************/
#ifndef MAX_PATH
    #define MAX_PATH 1024
#endif

/******************************************************************************
**  Class Definition
******************************************************************************/
namespace Gorilla
{
    //! @brief      check_file
    bool FileSystem::check_file(const char *filepath)
    {
        struct stat buffer;
        if (stat(filepath, &buffer) == 0 && S_ISREG(buffer.st_mode))
            return true;
        return false;
    }

    //! @brief      open_file
    File* FileSystem::open_file(const char *filepath, Access access /*= Access::Read*/)
    {
        int32_t flags = 0;
        switch(access)
        {
            case Access::READ:
            {
                flags = O_RDONLY;
                break;
            }

            case Access::WRITE:
            {
                flags = O_WRONLY | O_CREAT | O_TRUNC;
                if(!check_directory(filepath)) {
                    Path path(filepath);
                    create_directory(path.get_directory().get_buffer());
                }

                break;
            }
        }

        // Create handle
        int32_t handle = ::open(filepath, flags, 0644);
        if(handle == -1)
            return nullptr;

        File *file = new File();
        file->m_handle = handle;

        return file;
    }

    //! @brief      close_file
    bool FileSystem::close_file(File *file)
    {
        if (!file)
            return false;

        int32_t result = ::close((int32_t)file->m_handle);
        if (result == -1)
            return false;

        delete file;

        return true;
    }

    //! @brief      delete_file
    bool FileSystem::delete_file(const char *filepath)
    {
        return ::remove(filepath) != -1;
    }

    //! @brief      copy_file
    bool FileSystem::copy_file(const char *source, const char *destination)
    {
        bool result = false;
        uint32_t file_source_size = 0;
        File *file_source = nullptr;
        File *file_destination = nullptr;
        char *buffer = nullptr;

        file_source = open_file(source, Access::READ);
        if (!file_source)
            goto finish;

        file_source_size = file_source->get_size();
        buffer = new char[file_source_size];
        if (file_source->read(buffer, file_source_size) != file_source_size)
            goto finish;

        file_destination = open_file(destination, Access::WRITE);
        if (!file_destination)
            goto finish;

        if (file_destination->write(buffer, file_source_size) != file_source_size)
            goto finish;

        result = true;

    finish:
        if (!file_destination)
            close_file(file_destination);

        if (buffer)
            delete[] buffer;

        if (!file_source)
            close_file(file_source);

        return result;
    }

    //! @brief      check_directory
    bool FileSystem::check_directory(const char *path)
    {
        struct stat buffer;
        if (stat(path, &buffer) == 0 && S_ISDIR(buffer.st_mode))
            return true;
        return false;
    }

    //! @brief      create_directory
    bool FileSystem::create_directory(const char *path)
    {
        String formated_path;
        Path::combine(&formated_path, path, PATH_SEPARATOR);

        char directory_path[MAX_PATH];
        uint32_t length = 0, start_index = 0;
        uint32_t end_index = formated_path.find(PATH_SEPARATOR);
        while(end_index != (uint32_t)-1)
        {
            uint32_t sub_length = end_index - start_index + 1;
            length += sub_length;
            MEMCPY(directory_path + start_index, formated_path.get_buffer() + start_index, sub_length);
            directory_path[length] = '\0';
            if (!check_directory(directory_path)) {
                if (::mkdir(directory_path, 0777) == -1)
                    return false;
            }

            start_index = length;
            end_index = formated_path.find(PATH_SEPARATOR, length);
        }

        return true;
    }

    //! @brief      delete_directory
    bool FileSystem::delete_directory(const char *path)
    {
        char  *path_array[] { (char*)path, nullptr };
        FTS *file_tree = fts_open(path_array, FTS_PHYSICAL, nullptr);
        if(!file_tree)
            return false;

        FTSENT *file_tree_entry = fts_read(file_tree);
        while(file_tree_entry)
        {
            // entering into a directory
            if(file_tree_entry->fts_info & FTS_DP)
            {
                int32_t error = ::rmdir(file_tree_entry->fts_path);
                if (error)
                    return false;
            }
            else if(file_tree_entry->fts_info & FTS_F)
            {
                int32_t error = ::remove(file_tree_entry->fts_path);
                if (error)
                    return false;
            }

            file_tree_entry = fts_read(file_tree);
        }

        int32_t result = fts_close(file_tree);
        if (result == -1)
            return false;

        return true;
    }

    //! @brief      move
    bool FileSystem::move(const char *source, const char *destination)
    {
        return ::rename(source, destination) != -1;
    }

    //! @brief      get_directory_path
    void FileSystem::get_directory_path(Directory type, String *output)
    {
        output->resize(MAX_PATH);
        switch(type)
        {
            case Directory::EXECUTABLE:
            {
            #if defined(PLATFORM_MAC)
                uint32_t length = MAX_PATH;
                _NSGetExecutablePath((char*)output->get_buffer(), &length);
            #else
                uint32_t length = (uint32_t)::readlink("/proc/self/exe", (char*)output->get_buffer(), MAX_PATH);
            #endif
                output->resize(length);
                uint32_t position = output->find_last(PATH_SEPARATOR);
                output->resize(position + 1);
                break;
            }

            case Directory::USER:
            {
                struct passwd *user_database = getpwuid(getuid());
                *output = user_database->pw_dir;
                output->append(PATH_SEPARATOR);
                break;
            }

            case Directory::TEMPORARY:
            {
                *output = getenv("TMPDIR");
                if (output->is_empty())
                    *output = "/tmp";
                output->append(PATH_SEPARATOR);
                break;
            }
        }
        Path::format(output);
    }

    //! @brief      list
    bool FileSystem::list(const char *path, Vector<String> *directories, Vector<String> *files, const char *extensions /*= nullptr*/)
    {
        if (directories)
            directories->clear();

        if (files)
            files->clear();

        if (!check_directory(path))
            return false;

        // create extension list
        Vector<String> extension_list;
        if(extensions)
        {
            const char* cursor = extensions;
            const char* start = extensions;
            while (*cursor) {
                ++cursor;
                if(*cursor == ';' || *cursor =='\0') {
                    extension_list.add().set(start, (uint32_t)(cursor - start));
                    start = cursor + 1;
                }
            }
        }

        // ensure directory path is well formated
        String directory(path);
        Path::format(&directory);
        if (directory[directory.get_length()-1] == *PATH_SEPARATOR)
            directory.resize(directory.get_length()-1);

        char  *path_array[] { (char*)directory.get_buffer(), nullptr };
        FTS *file_tree = fts_open(path_array, FTS_PHYSICAL, nullptr);
        if(!file_tree)
            return false;

        FTSENT *file_tree_entry = fts_read(file_tree);
        while(file_tree_entry)
        {
            if (file_tree_entry->fts_level == 1)
            {
                // entering into a directory
                if(file_tree_entry->fts_info & FTS_D && directories)
                    directories->add(file_tree_entry->fts_path);
                else if(file_tree_entry->fts_info & FTS_F && files)
                {
                    const uint32_t extension_count = extension_list.get_size();
                    if(extension_count) {
                        for (uint32_t i = 0; i < extension_count; ++i) {
                            if(STRSTR(file_tree_entry->fts_name, extension_list[i].get_buffer())) {
                                files->add(file_tree_entry->fts_path);
                                break;
                            }
                        }
                    }
                    else
                        files->add(file_tree_entry->fts_path);
                }
            }

            file_tree_entry = fts_read(file_tree);
        }

        int32_t result = fts_close(file_tree);
        if (result == -1)
            return false;

        return true;
    }
}
