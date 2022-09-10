#include "application.hpp"
#include <Windows.h>

namespace Gorilla
{
    int32_t Application::run(int32_t (*callback)())
    {
        MSG msg = {0};
        while(WM_QUIT != msg.message)
        {
            if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
                continue;
            }

            int32_t result = callback();
            if (result != 0)
                return result;
        }

        return 0;
    }
}
