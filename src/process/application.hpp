#pragma once

#include <cstdint>

namespace Gorilla
{
    class Application
    {
    public:
        static int32_t run(int32_t (*callback)(void*), void *user_data = nullptr);
        static void quit();
    };
}
