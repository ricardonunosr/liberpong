#ifndef PLATFORM_H
#define PLATFORM_H

#ifdef _WIN32
#include <windows.h>
#endif

namespace Pong
{
    struct Window;

    struct Platform
    {
        static Window *CreateWindow(int width, int height, const char *title);
        static void SetPlatformKeyCallback(...);
        static bool InitOpenGL(Window *window, int glMajorVersion, int glMinorVersion, int colorBits = 32, int depthBits = 24);
        static float GetTime();
        static void SwapBuffers(Window *window);
        static void PlatformUpdateWindow(Window *window);
    };
}

#endif // PLATFORM_H