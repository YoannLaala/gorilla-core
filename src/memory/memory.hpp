#pragma once

/******************************************************************************
**  Include
******************************************************************************/
#include <cstdint>
#include <string.h>

/******************************************************************************
**  Defines
******************************************************************************/
#if defined(PLATFORM_WINDOWS)
    #define MEMCPY(_destination_, _source_, _size_)     ::memcpy_s(_destination_, _size_, _source_, _size_)
    #define MEMMOVE(_destination_, _source_, _size_)    ::memmove_s(_destination_, _size_, _source_, _size_)
#else
    #define MEMCPY(_destination_, _source_, _size_)     ::memcpy(_destination_, _source_, _size_)
    #define MEMMOVE(_destination_, _source_, _size_)    ::memmove(_destination_, _source_, _size_)
#endif
#define MEMSET(_destination_, _value_, _size_)      ::memset(_destination_, _value_, _size_)
#define MEMCMP(_source_a_, _source_b_, _size_)      ::memcmp(_source_a_, _source_b_, _size_)

/******************************************************************************
**  Class Declaration
******************************************************************************/
namespace Gorilla { namespace Memory
{
    enum class Type
    {
        STACK = 0,
        HEAP,
        COUNT,
    };

    void*    allocate    (uint64_t size);
    void     deallocate  (void *address);

    template <class T>
    T*       allocate_array(const uint64_t element_count);

    template <class T>
    void     deallocate_array(T *address, uint64_t element_count);

    template <typename TYPE>
    void static inline memcpy(TYPE* destination, const TYPE* source, uint32_t count)
    {
        for (uint32_t element = 0; element < count; ++element)
            destination[element] = source[element];
    }
    template <>	inline void memcpy<bool>(bool* destination, const bool* source, uint32_t count)           { MEMCPY(destination, source, count); }
    template <> inline void memcpy<uint8_t>(uint8_t* destination, const uint8_t* source, uint32_t count)        { MEMCPY(destination, source, count); }
    template <> inline void memcpy<uint16_t>(uint16_t* destination, const uint16_t* source, uint32_t count)     { MEMCPY(destination, source, count * sizeof(uint16_t)); }
    template <> inline void memcpy<uint32_t>(uint32_t* destination, const uint32_t* source, uint32_t count)     { MEMCPY(destination, source, count * sizeof(uint32_t)); }
    template <> inline void memcpy<uint64_t>(uint64_t* destination, const uint64_t* source, uint32_t count)     { MEMCPY(destination, source, count * sizeof(uint64_t)); }
    template <> inline void memcpy<int8_t>(int8_t* destination, const int8_t* source, uint32_t count)           { MEMCPY(destination, source, count); }
    template <> inline void memcpy<int16_t>(int16_t* destination, const int16_t* source, uint32_t count)        { MEMCPY(destination, source, count * sizeof(int16_t)); }
    template <> inline void memcpy<int32_t>(int32_t* destination, const int32_t* source, uint32_t count)        { MEMCPY(destination, source, count * sizeof(int32_t)); }
    template <> inline void memcpy<int64_t>(int64_t* destination, const int64_t* source, uint32_t count)        { MEMCPY(destination, source, count * sizeof(int64_t)); }
    template <> inline void memcpy<float>(float* destination, const float* source, uint32_t count)  { MEMCPY(destination, source, count * sizeof(float)); }
    template <> inline void memcpy<double>(double* destination, const double* source, uint32_t count)  { MEMCPY(destination, source, count * sizeof(double)); }

    //! @brief      allocate_array
    //! @details    Allocate an array and call constructor on Element
    template <class T>
    T* allocate_array(const uint64_t element_count)
    {
        char *allocation = (char*)allocate(element_count * sizeof(T));
        for(uint64_t i = 0; i < element_count; ++i)
        {
            new (allocation + i * sizeof(T)) T;
        }

        return (T*)allocation;
    }

    //! @brief      deallocate_array
    //! @details    Deallocate an array and call destructor on Element
    template <class T>
    void deallocate_array(T *address, uint64_t element_count)
    {
        for(uint64_t i = 0; i < element_count; ++i)
        {
            address[i]->~T();
        }
        deallocate(address);
    }
}}
