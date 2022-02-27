#include "platform/platform.h"

#include "gl.h"
#include <iostream>

#include "pong.h"
#include "paddel.h"
#include "ball.h"

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

#define PRESSED 1
#define RELEASED 0

void key_callback(int key, int action)
{
    // Left Paddel
    if (key == 'W' && action == PRESSED)
        _leftPaddel.MoveUp();
    if (key == 'W' && action == RELEASED)
        _leftPaddel.Stop();
    if (key == 'S' && action == PRESSED)
        _leftPaddel.MoveDown();
    if (key == 'S' && action == RELEASED)
        _leftPaddel.Stop();

    // Right Paddel
    if (key == 'UP' && action == PRESSED)
        _rightPaddel.MoveUp();
    if (key == 'UP' && action == RELEASED)
        _rightPaddel.Stop();
    if (key == 'Down' && action == PRESSED)
        _rightPaddel.MoveDown();
    if (key == 'Down' && action == RELEASED)
        _rightPaddel.Stop();

    // Ball
    if (key == 'Space' && action == PRESSED)
        _ball.StartMovement();
}

static bool running = true;

int pongMain(int argc, char **argv)
{
    Pong::Window *window = Pong::Platform::CreateWindow(800, 600, "Pong");
    // Platform::set_platform_key_callback(key_callback);

    // if (!Platform::InitOpenGL(window, 4, 6))
    //     return 1;

    // glEnable(GL_DEBUG_OUTPUT);
    // glDebugMessageCallback(MessageCallback, 0);

    // ResetGame();

    // float deltaTime, lastFrame = 0;

    // /* Loop until the user closes the window */
    // while (running)
    // {
    //     /* Render here */
    //     glClear(GL_COLOR_BUFFER_BIT);
    //     glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    //     float currentFrame = Platform::GetTime();
    //     deltaTime = currentFrame - lastFrame;
    //     lastFrame = currentFrame;

    //     // Update
    //     _leftPaddel.Update(deltaTime);
    //     _rightPaddel.Update(deltaTime);
    //     _ball.Update(deltaTime);
    //     _ball.CollidedWithPad(_leftPaddel);
    //     _ball.CollidedWithPad(_rightPaddel);
    //     const COLLIDED collided = _ball.CollidedWithTerrain();
    //     if (collided == COLLIDED::LEFT || collided == COLLIDED::RIGHT)
    //         ResetGame();

    //     // Render
    //     _leftPaddel.Draw();
    //     _rightPaddel.Draw();
    //     _ball.Draw();

    //     /* Swap front and back buffers */
    //     Platform::SwapBuffers(window);

    //     /* Poll for and process events */
    //     Platform::platform_update_window(window);
    // }

    // delete window;
}

#ifdef _WIN32
int WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, LPSTR cmdline, int cmdshow)
{
    return pongMain(0, (char **)cmdline);
}

#else
int main(int argc, char **argv)
{
    return pongMain(argc, argv);
}
#endif