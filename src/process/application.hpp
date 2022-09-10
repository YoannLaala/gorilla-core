#pragma once

#include <cstdint>

namespace Gorilla
{
    class Application
    {
    public:
        static int32_t run(int32_t (*callback)());
    };
}
