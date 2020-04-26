#pragma once

/******************************************************************************
**  Includes
******************************************************************************/
#include <cstdint>

/******************************************************************************
**  Class Declaration
******************************************************************************/
namespace Gorilla
{
    class Hash
    {
    public:
        static uint32_t           generate            (const char *source);
        static uint32_t           generate            (const void *source, uint32_t length);
    };
}
