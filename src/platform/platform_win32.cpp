#define PLATFORM
#include <windows.h>
#include "gl/glcorearb.h"
#include "gl/wglext.h"
#include "gl.h"
#include <stdio.h>
#include <iostream>

#include "pong.h"
#include "paddel.h"
#include "ball.h"

static bool running = true;

typedef void (*KeyFunc) (int key, int action);

struct Window
{
	HWND handle;
	HDC dc;
	HGLRC rc;
};

static KeyFunc platform_key_callback = nullptr;

#define PRESSED 1
#define RELEASED 0

static void set_platform_key_callback(KeyFunc key_callback)
{
	platform_key_callback = key_callback;
}

LRESULT platform_window_callback(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
        case WM_CLOSE:
		running = false;
		break;
		case WM_KEYDOWN:
		case WM_KEYUP:
		{
			// wParam -> w stands for word (16 bits)
			short keyCode = (short)wParam;
			int isDown =  !(lParam & (1 << 31)); // 0 = pressed 1 = released
			int wasDown = (lParam & (1 << 30)); // 0 = released 1 = pressed
			if(platform_key_callback)
			{
				platform_key_callback(keyCode,isDown);
			}
			printf("%d %s-> %s\n", keyCode, wasDown ? "PRESSED":"RELEASED",isDown ? "PRESSED":"RELEASED");
		}
		break;
	}

	return DefWindowProc(hWnd, Msg, wParam, lParam);
}

void platform_update_window(Window* window)
{
	MSG msg;

	while (PeekMessageA(&msg, window->handle, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}
}

Window* create_window(int width, int height, const char* title)
{
	const char* windowClass = "pong_class";

	HWND windowHandle = {};
	WNDCLASSEXA wc = {};

	HINSTANCE hInst = GetModuleHandleA(0);

	if (!GetClassInfoExA(hInst, windowClass, &wc)) {
		wc.cbSize = sizeof(WNDCLASSEXA);
		wc.style = CS_OWNDC;
		wc.lpfnWndProc = platform_window_callback;
		wc.hInstance = hInst;
		wc.lpszClassName = windowClass;
		wc.hCursor = LoadCursor(hInst, IDC_ARROW);
		wc.hIcon = LoadIconA(hInst,MAKEINTRESOURCE(101));

		if (!RegisterClassExA(&wc)) {
			MessageBoxA(windowHandle, "Failed to register window class", "Error", MB_ICONEXCLAMATION | MB_OK);
			return false;
		}
	}

	windowHandle = CreateWindowEx(
                                  WS_EX_APPWINDOW,
                                  "pong_class",
                                  title,
                                  WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_OVERLAPPED
                                  , 100, 100, width, height, 0, 0, hInst, 0
                                  );

	if (windowHandle == 0)
	{
		MessageBoxA(windowHandle, "Failed to create window ", "Error", MB_ICONEXCLAMATION | MB_OK);
		return false;
	}

	Window* window = new Window;
	window->handle = windowHandle;
	window->dc = GetDC(windowHandle);

	return window;
}

/*
* ChoosePixelFormat   // OpenGL 1.1 Context
* SetPixelFormat ()   // Connect pixel format to Device Context(DC)
* wglCreateContext()  // Create OpenGL 1.1 context
* wglMakeCurrent()    // Connect Rendering context(RC) to Device context(DC)
*
* wglGetProcAddress() // Get Pointer to wglChoosePixelFormatARB() wglCreateContextAttribsARB()
* cat src/gl/glcorearb.h | findstr GLAPI > src/gl.h // Get all functions from header easily
* https://www.khronos.org/registry/OpenGL/index_gl.php
*/

bool InitOpenGL(Window* window, int glMajorVersion, int glMinorVersion, int colorBits, int depthBits)
{
	Window* dummyWindow = create_window(0, 0, "dummy");

	const PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),  //  size of this pfd
        1,                     // version number
        PFD_DRAW_TO_WINDOW |   // support window
            PFD_SUPPORT_OPENGL |   // support OpenGL
            PFD_DOUBLEBUFFER,      // double buffered
        PFD_TYPE_RGBA,         // RGBA type
        (BYTE)depthBits,             // 24-bit color depth
        0, 0, 0, 0, 0, 0,      // color bits ignored
        0,                     // no alpha buffer
        0,                     // shift bit ignored
        0,                     // no accumulation buffer
        0, 0, 0, 0,            // accum bits ignored
        (BYTE)colorBits,             // 32-bit z-buffer
        0,                     // no stencil buffer
        0,                     // no auxiliary buffer
        PFD_MAIN_PLANE,        // main layer
        0,                     // reserved
        0, 0, 0                // layer masks ignored
	};

	int iPixelFormat = ChoosePixelFormat(dummyWindow->dc, &pfd);
	if (!iPixelFormat)
	{
		// TODO(Ricardo): Destroy Window
		return false;
	}
    
	if (!SetPixelFormat(dummyWindow->dc, iPixelFormat, &pfd))
	{
		// TODO(Ricardo): Destroy Window
		return false;
	}
    
	HGLRC rc = wglCreateContext(dummyWindow->dc);
	if (!rc)
	{
		// TODO(Ricardo): Destroy Window
		return false;
	}
    
	dummyWindow->rc = rc;
    
	if (!wglMakeCurrent(dummyWindow->dc, dummyWindow->rc))
	{
		// TODO(Ricardo): Destroy Window
		return false;
	}
    
	PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
    
	wglMakeCurrent(0, 0);
	wglDeleteContext(dummyWindow->rc);
	delete dummyWindow;
    
	unsigned int numPixelFormats = 0;
	const int pixelFormatAttribList[] = {
		WGL_DRAW_TO_WINDOW_ARB,GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB,GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB,GL_TRUE,
		WGL_COLOR_BITS_ARB,colorBits,
		WGL_DEPTH_BITS_ARB,depthBits,
		0
	};
    
	wglChoosePixelFormatARB(window->dc, pixelFormatAttribList, nullptr, 1, &iPixelFormat, &numPixelFormats);
	if (numPixelFormats == 0)
	{
		return false;
	}
    
	if (!SetPixelFormat(window->dc, iPixelFormat, &pfd))
	{
		return false;
	}
    
	const int contextAttribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB,glMajorVersion,
		WGL_CONTEXT_MINOR_VERSION_ARB,glMinorVersion,
		WGL_CONTEXT_FLAGS_ARB,
		WGL_CONTEXT_DEBUG_BIT_ARB | WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
	};
    
	rc = wglCreateContextAttribsARB(window->dc, 0, contextAttribs);
	if (!rc)
	{
		return false;
	}
    
	window->rc = rc;
    
	if (!wglMakeCurrent(window->dc, window->rc))
	{
		return false;
	}
    
	// Get OpenGL Function Pointers
	getOpenGLFunctionPointers();
    
	return true;
}

void MessageCallback(GLenum source,
				GLenum type,
				GLuint id,
				GLenum severity,
				GLsizei length,
				const GLchar *message,
				const void *userParam)
{
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
			(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
			type, severity, message);
}

void key_callback(int key,int action)
{
	//Left Paddel
	if (key == 'W' && action == PRESSED)
		_leftPaddel.MoveUp();
	if (key == 'W' && action == RELEASED)
		_leftPaddel.Stop();
	if (key == 'S' && action == PRESSED)
		_leftPaddel.MoveDown();
	if (key == 'S' && action == RELEASED)
		_leftPaddel.Stop();

	//Right Paddel
	if (key == VK_UP && action == PRESSED)
		_rightPaddel.MoveUp();
	if (key == VK_UP && action == RELEASED)
		_rightPaddel.Stop();
	if (key == VK_DOWN && action == PRESSED)
		_rightPaddel.MoveDown();
	if (key == VK_DOWN && action == RELEASED)
		_rightPaddel.Stop();

	//Ball
	if (key == VK_SPACE && action == PRESSED)
		_ball.StartMovement();
}

int pongMain(int argc, char** argv)
{
	Window* window = create_window(800, 600, "Pong");
    set_platform_key_callback(key_callback);

	if (!InitOpenGL(window, 4, 6, 32, 24))
		return 1;
    
	ShowWindow(window->handle, SW_SHOW);

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);

	ResetGame();

	float deltaTime, lastFrame = 0;
	LARGE_INTEGER frequency, startCounter;
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&startCounter);
	/* Loop until the user closes the window */
	while (running)
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		LARGE_INTEGER endCounter;
		QueryPerformanceCounter(&endCounter);

		float time = (float)(endCounter.QuadPart-startCounter.QuadPart) / (float)frequency.QuadPart;
		deltaTime = time - lastFrame;
		lastFrame = time;

		//Update
		_leftPaddel.Update(deltaTime);
		_rightPaddel.Update(deltaTime);
		_ball.Update(deltaTime);
		_ball.CollidedWithPad(_leftPaddel);
		_ball.CollidedWithPad(_rightPaddel);
		const COLLIDED collided = _ball.CollidedWithTerrain();
		if (collided == COLLIDED::LEFT || collided == COLLIDED::RIGHT)
			ResetGame();

		//Render
		_leftPaddel.Draw();
		_rightPaddel.Draw();
		_ball.Draw();

		/* Swap front and back buffers */
		SwapBuffers(window->dc);

		/* Poll for and process events */
		platform_update_window(window);
	}

	delete window;
}

#ifdef _DEBUG
int main(int argc, char** argv)
{
	return pongMain(argc,argv);
}
#else
int WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, LPSTR cmdline, int cmdshow)
{
	return pongMain(0,(char**)cmdline);
}
#endif
