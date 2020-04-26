/******************************************************************************
**  Includes
******************************************************************************/
#include "hash.hpp"

/******************************************************************************
**  Class Definition
******************************************************************************/
namespace Gorilla
{
    //! @brief      Generate an identifier
    //! @param      source is a final null terminated string
    //! @return     hash using FNV-1a
    uint32_t Hash::generate(const char *source)
    {
        // FNV-1a alternate algorithm
        // 32 bit offset_basis = 2166136261
        // 32 bit FNV_prime = 224 + 28 + 0x93 = 16777619
        uint32_t hash = 2166136261;
        while (*source)
        {
            hash = (*source++ ^ hash) * 16777619;
        }

        return hash;
    }

    //! @brief      Generate an identifier
    //! @param      source is a pointer on value to hash
    //! @param      length is the size of the source
    //! @return     hash using FNV-1a
    uint32_t Hash::generate(const void *source, uint32_t length)
    {
        // FNV-1a alternate algorithm
        // 32 bit offset_basis = 2166136261
        // 32 bit FNV_prime = 224 + 28 + 0x93 = 16777619
        uint32_t hash = 2166136261;
        uint32_t index = 0;
        while (index < length)
        {
            hash = (((char*)source)[index] ^ hash) * 16777619;
            ++index;
        }

        return hash;
    }
}
