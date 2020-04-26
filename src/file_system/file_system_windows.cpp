/******************************************************************************
**  Includes
******************************************************************************/
#include "file_system.hpp"
#include "file.hpp"
#include "path.hpp"
#include "../time/time.hpp"

#include <Windows.h>
#include <Shlobj.h>
#include <Shlobj_core.h>

/******************************************************************************
**  Class Definition
******************************************************************************/
namespace Gorilla
{
    //! @brief      check_file
    bool FileSystem::check_file(const char *filepath)
    {
        uint32_t flags = ::GetFileAttributes(filepath);
        return (flags != INVALID_FILE_ATTRIBUTES) && !(flags & FILE_ATTRIBUTE_DIRECTORY);
    }

    //! @brief      open_file
    File* FileSystem::open_file(const char *filepath, Access access /*= Access::Read*/)
    {
        // prepare specific flags
        uint32_t access_windows = 0, method_windows = 0;
        switch(access)
        {
            case Access::READ:
            {
                access_windows = GENERIC_READ;
                method_windows = OPEN_EXISTING;
                break;
            }

            case Access::WRITE:
            {
                access_windows = GENERIC_WRITE;
                method_windows = CREATE_ALWAYS;
                if(!check_directory(filepath)) {
                    Path path(filepath);
                    create_directory(path.get_directory().get_buffer());
                }

                break;
            }
        }

        // Create handle
        uint64_t handle = (uint64_t)::CreateFile(filepath, access_windows, 0, nullptr, method_windows, FILE_ATTRIBUTE_NORMAL, nullptr);
        if(handle == (uint64_t)INVALID_HANDLE_VALUE)
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

        if (::CloseHandle((HANDLE)file->m_handle) == FALSE)
            return false;

        delete file;

        return true;
    }

    //! @brief      delete_file
    bool FileSystem::delete_file(const char *filepath)
    {
        return ::DeleteFile(filepath) == TRUE;
    }

    //! @brief      copy_file
    bool FileSystem::copy_file(const char *source, const char *destination)
    {
        return ::CopyFile(source, destination, FALSE) == TRUE;
    }

    //! @brief      check_directory
    bool FileSystem::check_directory(const char *path)
    {
        uint32_t flags = ::GetFileAttributes(path);
        return (flags != INVALID_FILE_ATTRIBUTES) && (flags & FILE_ATTRIBUTE_DIRECTORY);
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
                if (::CreateDirectory(directory_path, NULL) == FALSE)
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
        String formated_path;
        Path::combine(&formated_path, path, PATH_SEPARATOR, '*');

        // Find the first file
        WIN32_FIND_DATA file_data;
        HANDLE file = ::FindFirstFile(formated_path.get_buffer(), &file_data);
        if(file == INVALID_HANDLE_VALUE)
            return false;

        // Iterates on each file
        String filepath;
        do
        {
            // Avoid Special Directory
            if (file_data.cFileName[0] == '.')
                continue;

            filepath.set(path).append(file_data.cFileName);
            if ((file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
            {
                if (!delete_directory(filepath.get_buffer()))
                    return false;
            }
            else
            {
                if (!delete_file(filepath.get_buffer()))
                    return false;
            }
        } while (::FindNextFile(file, &file_data));

        // Close and delete last directory
        if(::FindClose(file) == FALSE)
            return false;

        if (::RemoveDirectory(path) == FALSE)
            return false;

        return true;
    }

    //! @brief      move
    bool FileSystem::move(const char *source, const char *destination)
    {
        return ::MoveFile(source, destination) == TRUE;
    }

    //! @brief      get_directory_path
    void FileSystem::get_directory_path(Directory type, String *output)
    {
        output->resize(MAX_PATH);
        switch(type)
        {
            case Directory::EXECUTABLE:
            {
                uint32_t length = ::GetModuleFileName(::GetModuleHandle(NULL), (char*)output->get_buffer(), MAX_PATH);
                output->resize(length);
                uint32_t position = output->find_last(PATH_SEPARATOR);
                output->resize(position + 1);
                break;
            }

            case Directory::USER:
            {
                ::SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, (char*)output->get_buffer());
                uint32_t length = (uint32_t)strlen(output->get_buffer());
                output->resize(length);
                output->append(PATH_SEPARATOR);
                break;
            }

            case Directory::TEMPORARY:
            {
                // Base Temp directory
                uint32_t length = GetTempPath(MAX_PATH, (char*)output->get_buffer());
                output->resize(length);
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

        String formated_path;
        Path::combine(&formated_path, path, PATH_SEPARATOR, '*');

        // Find the first file
        WIN32_FIND_DATA file_data;
        HANDLE file = ::FindFirstFile(formated_path.get_buffer(), &file_data);
        if(file == INVALID_HANDLE_VALUE)
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

        // Iterates on each file
        String extension_name;
        do
        {
            // Avoid Special Directory
            if (file_data.cFileName[0] == '.')
                continue;

            bool is_directory = (file_data.dwFileAttributes != INVALID_FILE_ATTRIBUTES) && (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
            if (is_directory && directories)
                Path::combine(&directories->add(), path, file_data.cFileName, '\\');
            else if (files)
            {
                const uint32_t extension_count = extension_list.get_size();
                if(extension_count) {
                    for (uint32_t i = 0; i < extension_count; ++i) {
                        if(STRSTR(file_data.cFileName, extension_list[i].get_buffer())) {
                            Path::combine(&files->add().set(""), path, file_data.cFileName);
                            break;
                        }
                    }
                }
                else
                    Path::combine(&files->add(), path, file_data.cFileName);
            }
        } while (::FindNextFile(file, &file_data));

        // Close and delete last directory
        if(::FindClose(file) == FALSE)
            return false;

        return true;
    }
}
