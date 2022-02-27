#include "platform.h"

#include <X11/Xlib.h>

namespace Pong
{

    Window *Platform::CreateWindow(int width, int height, const char *title)
    {
        Display *display;
        int screenNumber;
        XID window;
        XEvent ev;
        int should_quit = 0;
        display = XOpenDisplay(NULL);
        screenNumber = XDefaultScreen(display);
        window = XCreateSimpleWindow(display, XRootWindow(display, screenNumber), 0, 0, 200, 200, 0, XBlackPixel(display, screenNumber), XWhitePixel(display, screenNumber));
        XSelectInput(display, window, ButtonPressMask);
        XMapWindow(display, window);

        return nullptr;
    }

    void Platform::SetPlatformKeyCallback(...)
    {
    }
    bool Platform::InitOpenGL(Window *window, int glMajorVersion, int glMinorVersion, int colorBits, int depthBits)
    {
    }
    float Platform::GetTime()
    {
    }
    void Platform::SwapBuffers(Window *window)
    {
    }
    void Platform::PlatformUpdateWindow(Window *window)
    {
    }

}